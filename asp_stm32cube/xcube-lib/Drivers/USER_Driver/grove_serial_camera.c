/*
 ******************************************************************************
 * Created on: 2018/05/28
 * @file    grove_serial_camera.c
 * @author  Swestgyan s-hosoai
 * @brief   Grove Serial Camera Driver
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "stm32l4xx_hal.h"
#include "grove_serial_camera.h"
//#include "msg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PIC_FMT_VGA    7
#define PIC_FMT_CIF    5
#define PIC_FMT_OCIF   3
#define CAM_ADDR       0
#define CAM_SERIAL     Serial

#define PIC_FMT        PIC_FMT_VGA

typedef uint8_t BOOL;
#define TRUE		((BOOL) 1)
#define FALSE		((BOOL) 0)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

const uint8_t CAMERA_ADDR = (CAM_ADDR << 5);
static UART_HandleTypeDef *m_uart;

/* Private function prototypes -----------------------------------------------*/
static void _clearRxBuf(void);
static SERIAL_CAM_Error _sendCommand(uint8_t cmdId, uint8_t param1, uint8_t param2, uint8_t param3, uint8_t param4, uint8_t timeout);
static BOOL _waitCmdAck(uint8_t cmdId);
static BOOL _waitSync();
static unsigned long _getPictLength();
static SERIAL_CAM_Error _cmd_initial(SERIAL_CAM_Color colorSetting, uint8_t previewResolution, uint8_t jpegResolution);
static SERIAL_CAM_Error _cmd_getPicture(uint8_t getSetting);
static SERIAL_CAM_Error _cmd_snapshot(uint8_t snapshotSetting, uint8_t skipFrameL, uint8_t skipFrameH);
static SERIAL_CAM_Error _cmd_setPackageSize(uint8_t packageSizeL, uint8_t packageSizeH);
static SERIAL_CAM_Error _cmd_setBaudRate(uint8_t firstDriver, uint8_t secondDriver);
static SERIAL_CAM_Error _cmd_reset(uint8_t resetSetting);
static SERIAL_CAM_Error _cmd_powerDown(void);
static SERIAL_CAM_Error _cmd_data(uint8_t dataType, uint8_t lengthByte1, uint8_t lengthByte2, uint8_t lengthByte3);
static SERIAL_CAM_Error _cmd_sync(void);
static SERIAL_CAM_Error _cmd_ack(uint8_t commandId, uint8_t ackCounter, uint8_t packageIdByte0, uint8_t packageIdByte1);
static SERIAL_CAM_Error _cmd_nak(uint8_t nckCounter, uint8_t errorNumber);

/* Exported functions --------------------------------------------------------*/

/* Functions Body ------------------------------------------------------------*/

SERIAL_CAM_Error SerialCam_Initialize(UART_HandleTypeDef *Uart) {
	m_uart = Uart;
	uint8_t retryCnt = 0;

	// send "sync" until receive "ack" and "sync" from cam;
	while (1) {
		if(_cmd_sync()!=SERIAL_CAM_OK){
//			msg_error("sync error\r\n");
		}
		if (_waitCmdAck(0x0e)) {
			if (_waitSync()) {
				break;
			}
		}
		if(retryCnt>60){
//			msg_error("retry overflow\r\n");
			return SERIAL_CAM_ERROR_INITIALIZE;
		}
		retryCnt++;
	}
	_cmd_ack(0x0D, 0x00, 0x00, 0x00);
	return SERIAL_CAM_OK;
}

/*********************************************************************/
SERIAL_CAM_Error SerialCam_PreCapture() {
	uint8_t retryCnt = 0;
	while (1) {
		_clearRxBuf();
		_cmd_initial(0x07, 0x00, PIC_FMT); // color=jpeg, preview=80x60, jpegRes=640x480
		if(_waitCmdAck(0x01)){
			break;
		}
		if(retryCnt>60){
			return SERIAL_CAM_ERROR_INITIALIZE;
		}
		retryCnt++;
	}
	return SERIAL_CAM_OK;
}

/*********************************************************************/
SERIAL_CAM_Error SerialCam_Capture(uint16_t *resultLength) {
	uint8_t retryCnt = 0;
	while (1) {
		_clearRxBuf();
		_cmd_setPackageSize(PIC_PKT_LEN & 0xff, (PIC_PKT_LEN>>8) & 0xff);
		if (_waitCmdAck(0x06)){
			break;
		}
		if(retryCnt>60){
			return SERIAL_CAM_ERROR_INITIALIZE; // TODO
		}
		retryCnt++;
	}

	retryCnt=0;
	while (1) {
		_clearRxBuf();
		_cmd_snapshot(0x00, 0x00, 0x00); // snapshot=compressed pict, skipFrameL=0, skipFrameH=0
		if (_waitCmdAck(0x05)){
			break;
		}
		if(retryCnt>60){
			return SERIAL_CAM_ERROR_INITIALIZE; // TODO
		}
		retryCnt++;
	}

	retryCnt=0;
	while (1) {
		_clearRxBuf();
		_cmd_getPicture(0x01); // getSetting=snapshot
		if (_waitCmdAck(0x04)) {
			*resultLength = _getPictLength();
			if((*resultLength)!=-1){
				return SERIAL_CAM_OK;
			}
		}
		if(retryCnt>60){
			return SERIAL_CAM_ERROR_INITIALIZE; // TODO
		}
		retryCnt++;
	}
//	return SERIAL_CAM_ERROR_INITIALIZE; // TODO
}

/*********************************************************************/
SERIAL_CAM_Error SerialCam_GetData(uint8_t pktBuf[PIC_PKT_LEN], uint8_t length, uint16_t pktCnt) {
	_clearRxBuf();
	_cmd_ack(0x00,0x00, pktCnt&0xff, ((pktCnt>>8)&0xff));
	if (HAL_UART_Receive(m_uart, pktBuf, length, 100) != HAL_OK) {
		return SERIAL_CAM_ERROR_RECEIVE;
	}
	return SERIAL_CAM_OK;

//	unsigned int pktCnt = (picLength) / (PIC_PKT_LEN - 6);
//	if ((picLength % (PIC_PKT_LEN - 6)) != 0){
//		pktCnt += 1;
//	}

//	uint8_t cmd[] = { 0xaa, 0x0e | CAMERA_ADDR, 0x00, 0x00, 0x00, 0x00 };
//	unsigned char pkt[PIC_PKT_LEN];

//    char picName[] = "pic00.jpg";
//    picName[3] = picNameNum/10 + '0';
//    picName[4] = picNameNum%10 + '0';
//
//    if (SD.exists(picName))
//    {
//        SD.remove(picName);
//    }

//    myFile = SD.open(picName, FILE_WRITE);
//    if(!myFile){
//        Serial.println("myFile open fail...");
//    }
//    else{
//        Serial.setTimeout(1000);
//	uint16_t cnt = PIC_PKT_LEN;
//
//	for (unsigned int i = 0; i < pktCnt; i++) {
//		cmd[4] = i & 0xff;
//		cmd[5] = (i >> 8) & 0xff;
//
//		int retry_cnt = 0;
//		retry:
////            delay(10);
//		HAL_UART_Transmit(m_uart, cmd, 6, UartTimeOut);
//		if (HAL_UART_Receive(m_uart, pkt, PIC_PKT_LEN, 100) != HAL_OK) {
////            	continue;
//		}
//
//		unsigned char sum = 0;
//		for (int y = 0; y < cnt - 2; y++) {
//			sum += pkt[y];
//		}
//		if (sum != pkt[cnt - 2]) {
//			if (++retry_cnt < 100)
//				goto retry;
//			else
//				break;
//		}

//            myFile.write((const uint8_t *)&pkt[4], cnt-6);
		//if (cnt != PIC_PKT_LEN) break;
//	}
//	cmd[4] = 0xf0;
//	cmd[5] = 0xf0;
//	HAL_UART_Transmit(m_uart, cmd, 6, UartTimeOut);
//    }
//    myFile.close();
//    picNameNum ++;
//	return SERIAL_CAM_OK;
}
SERIAL_CAM_Error SerialCam_CloseData(){
	return _cmd_ack(0x00, 0x00,0xF0,0xF0);
}


/* Low layer static functions */
/*********************************************************************/
static void _clearRxBuf() {
	uint8_t data; // temporal buffer
	while (HAL_UART_Receive(m_uart, &data, 1, HAL_MAX_DELAY) == HAL_OK) {
		// skip available data
	}
}

static SERIAL_CAM_Error _sendCommand(uint8_t cmdId, uint8_t param1, uint8_t param2, uint8_t param3,
		uint8_t param4, uint8_t timeout) {
	uint8_t cmd[] = {0xAA,0x0d,0x00,0x00,0x00,0x00};
	cmd[0] = 0xAA;
	cmd[1] = cmdId;
	cmd[2] = param1;
	cmd[3] = param2;
	cmd[4] = param3;
	cmd[5] = param4;
	if (HAL_UART_Transmit(m_uart, cmd, 6, 1000) != HAL_OK) {
		return SERIAL_CAM_ERROR_INITIALIZE;
	}
	return SERIAL_CAM_OK;
}

static BOOL _waitCmdAck(uint8_t cmdId){
	uint8_t resp[6] = {0x00, 0x00,0x00,0x00,0x00,0x00};
	HAL_StatusTypeDef status;
	if ((status=HAL_UART_Receive(m_uart, resp, sizeof(resp), HAL_MAX_DELAY)) != HAL_OK) {
//        msg_error("init receive error : %d\r\n", status);
		return FALSE;
	}
//        msg_info("%d, %d, %d, %d, %d, %d", resp[0], resp[1], resp[2], resp[3], resp[4], resp[5]);


	// check valid ack. resp[3] don't care
	if(resp[0]==0xAA && resp[1] == (0x0E | CAMERA_ADDR) && resp[2] == cmdId && resp[4]==0x00 && resp[5]==0x00){
		return TRUE;
	}
	return FALSE;
}

static BOOL _waitSync(){
	uint8_t resp[6];
	if (HAL_UART_Receive(m_uart, resp, 6, 1000) != HAL_OK) {
		return FALSE;
	}

	// check valid sync
	if(resp[0]==0xAA && resp[1] == (0x0D | CAMERA_ADDR) && resp[2] == 0x00 && resp[4] == 0x00 && resp[4]==0x00 && resp[5]==0x00){
		return TRUE;
	}
	return FALSE;
}

static unsigned long _getPictLength(){
	uint8_t resp[6];
	if (HAL_UART_Receive(m_uart, resp, 6, 100) != HAL_OK) {
		return -1;
	}
	if (resp[0] == 0xaa && resp[1] == (0x0a | CAMERA_ADDR)
			&& resp[2] == 0x01) {
		return (unsigned long) (resp[3]) | (resp[4] << 8) | (resp[5] << 16);
	}
	return -1;
}

/* @see datasheet : https://yadom.fr/downloadable/download/sample/sample_id/31/ */
/*
 * colorSetting
 *   2-bit Gray-Scale   01h
 *   4-bit Gray- Scale 02h
 *   8-bit Gray- Scale 03h
 *   2-bit Color     05h
 *   16-bit  Color      06h
 *   JPEG      07h
 * previewResolution
 *   80*60      01h
 *   160*120      03h
 * jpegResolution
 *   80*64      01h
 *   160*128    03h
 *   320*240    05h
 *   640*480    07h
 */
static SERIAL_CAM_Error _cmd_initial(SERIAL_CAM_Color colorSetting, uint8_t previewResolution,
		uint8_t jpegResolution) {
	return _sendCommand(0x01, 0x00, colorSetting, previewResolution, jpegResolution, 100);
}

/*
 * getSetting
 *   Snapshot      01h
 *   Preview Picture    02h
 *   JPEG Preview Picture    03h
 */
static SERIAL_CAM_Error _cmd_getPicture(uint8_t getSetting) {
	return _sendCommand(0x04, getSetting, 0x00, 0x00, 0x00, 100);
}

/*
 * snapshotSetting
 *   Compressed Picture      00h
 *   Uncompressed Picture    01h
 * skipFrame: the number of dropped frames can be defined before compression occurs. “0” keeps the current frame, “1” captures the next frame ect.
 */
static SERIAL_CAM_Error _cmd_snapshot(uint8_t snapshotSetting, uint8_t skipFrameL,
		uint8_t skipFrameH) {
	return _sendCommand(0x05, snapshotSetting, skipFrameL, skipFrameH, 0x00, 100);
}

/* packageSizeH | packageSizeL
 *  ID      (2 byte)
 *  Date Size   (2  byte)
 *  Image  Data      (Package Size - 6byte)
 *  Verify Code   (2  byte)
 * ID: Package  ID,  stars  from  zero  for  an  image
 * Data Size: Size of image data in this package
 * Verify Code: Error  detection  code,  equals  to  the  lower  byte  of  sum  of  the  whole  package  data  except  the  verify   code   field.   The   higher   byte   of   this   code   is   always   zero.   i.e.   verify   code=low   byte(sum(byte【0】to byte【N-2】)        Note:  As  the  transmission  of  uncompressed  image  is  not  the  package  mode,  it  is  not  necessary  to  set  the  package size for the uncompressed image.
 */
static SERIAL_CAM_Error _cmd_setPackageSize(uint8_t packageSizeL, uint8_t packageSizeH) {
	return _sendCommand(0x06, 0x08, packageSizeL, packageSizeH, 0x00, 100);
}

/**
 * Baud  Rate=14.7456MHz/2*(2nd divider +1)/2*(1st    divider+1)
 *   Baud Rate 	1st Divider 2nd Divider
 *   7200bps	FFh      	01h
 *   28800bps	3Fh      	01h
 *   9600bps	BFh      	01h
 *   38400bps	2Fh      	01h
 *   14400bps	7Fh      	01h
 *   57600bps	1Fh      	01h
 *   19200bps	5Fh      	01h
 *   115200bps	0h      	01h
 */
static SERIAL_CAM_Error _cmd_setBaudRate(uint8_t firstDriver, uint8_t secondDriver) {
	return _sendCommand(0x07, firstDriver, secondDriver, 0x00, 0x00, 100);
}

/**
 * Reset  Setting
 *   00h : Reset  the  whole  system,  Camera  will  reboot  and  reset  all  registers  and  state  machine
 *   01h : Reset  state  machine  only
 */
static SERIAL_CAM_Error _cmd_reset(uint8_t resetSetting) {
	return _sendCommand(0x08, resetSetting, 0x00, 0x00, 0x00, 100);
}

/**
 * Camera  will  go  into  sleep  mode  after  receiving  this  command  and  get  resumed  after  sending  out  an  ACK  against SYNC command issued by host.
 */
static SERIAL_CAM_Error _cmd_powerDown() {
	return _sendCommand(0x09, 0x00, 0x00, 0x00, 0x00, 100);
}

/**
 * Data Type
 *   Snapshot  Picture  01h
 *   Preview  Picture   02h
 *   JPEG  Picture      05h
 * Data  Length:  these  3  bytes  represent  the  length  of  Snapshot  Picture,  Preview  Picture  and  JPEG  Preview  Picture.
 */
static SERIAL_CAM_Error _cmd_data(uint8_t dataType, uint8_t lengthByte1, uint8_t lengthByte2,
		uint8_t lengthByte3) {
	return _sendCommand(0x0A, dataType, lengthByte1, lengthByte2, lengthByte3, 100);
}

/**
 * Either the host or the camera can issue this command to make connection. An ACK command must be sent out after receiving this command.
 */
static SERIAL_CAM_Error _cmd_sync() {
	return _sendCommand(0x0D, 0x00, 0x00, 0x00, 0x00, 1000);
}

/**
 * This  command  indicates  the  success  of  the  last  operation.  After  receiving  any  valid  command,  ACK  command  must  be  sent  out  except  when  getting  preview  data.      The  host  can  issue  this  command  request  image  data  package  with  desired  package  ID  after  receiving  Data  command  from  camera,  and  send  this  command with package ID 0F0Fh when finishing with receiving data package.     Note: “Command ID” is 00h when host requesting image data package.
 *   Command ID: The command with that ID is acknowledged by this command.
 *   ACK Counter: Not in use
 *   Package  ID:  For  acknowledging  Data  Command,  these  two  bytes  represent  the  requested  package  ID.  While for acknowledging other commands, these two bytes are set to 00h.
 */
static SERIAL_CAM_Error _cmd_ack(uint8_t commandId, uint8_t ackCounter, uint8_t packageIdByte0,
		uint8_t packageIdByte1) {
	return _sendCommand(0x0E, commandId, ackCounter, packageIdByte0, packageIdByte1, 100);
}

/**
 * This command shows corrupted transmission or unsupported features.
 *   NAK Counter: Not in use
 *   Error Number: (Please read the following chart)
 *   Error Messages				NAK code
 *     Picture Type Error      	01h
 *     Picture Up Scale        	02h
 *     Picture Scale Error      03h
 *     Unexpected  Respond      04h
 *     Send  Picture  Timeout   05h
 *     Unexpected  Command    	06h
 *     ASRAM  JPEG  Type Error  07h
 *     ASRAM  JPEG  Size  Error 08h
 *     Picture  Format  Error   09h
 *     Picture  Size  Error     0Ah
 *     Parameter  Error      	0Bh
 *     Send  Register  Timeout  0Ch
 *     Command  ID  Error      	0Dh
 *     Picture  Not  Ready      0Fh
 *     Transfer  Package  Number  Error  10h
 *     Set Transfer  Package  Size  Wrong  11h
 *     Command  Header  Error      F0h
 *     Command  Length  Error   F1h
 *     Send  Picture  Error     F5h
 *     Send  Command  Error     FFh
 */
static SERIAL_CAM_Error _cmd_nak(uint8_t nckCounter, uint8_t errorNumber) {
	return _sendCommand(0x0F, 0x00, nckCounter, errorNumber, 0x00, 100);
}
