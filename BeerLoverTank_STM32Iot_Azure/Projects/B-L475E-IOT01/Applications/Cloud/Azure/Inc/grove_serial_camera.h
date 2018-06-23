/**
  ******************************************************************************
  * Created on: 2018/05/28
  * @file    grove_serial_camera.h
  * @author  Swestgyan s-hosoai
  * @brief   Grove Serial Camera Driver
  *
  ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GROVE_SERIAL_CAMERA_H
#define __GROVE_SERIAL_CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
typedef int8_t SERIAL_CAM_Error;
typedef int8_t SERIAL_CAM_Color;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define PIC_PKT_LEN    128                  //data length of each read, dont set this too big because ram is limited

/*Color  Setting*/
#define SERIAL_CAM_COLOR_GRAY_2BIT		((SERIAL_CAM_Color) 1)
#define SERIAL_CAM_COLOR_GRAY_4BIT		((SERIAL_CAM_Color) 2)
#define SERIAL_CAM_COLOR_GRAY_8BIT		((SERIAL_CAM_Color) 3)
#define SERIAL_CAM_COLOR_COLOR_2BIT		((SERIAL_CAM_Color) 5)
#define SERIAL_CAM_COLOR_COLOR_16BIT	((SERIAL_CAM_Color) 6)
#define SERIAL_CAM_COLOR_COLOR_JPEG		((SERIAL_CAM_Color) 7)

#define SERIAL_CAM_OK					((SERIAL_CAM_Error)  0)
#define SERIAL_CAM_ERROR_INITIALIZE		((SERIAL_CAM_Error) -1)
#define SERIAL_CAM_ERROR_TRANSMITION	((SERIAL_CAM_Error) -2)
#define SERIAL_CAM_ERROR_RECEIVE		((SERIAL_CAM_Error) -3)
/* Private macros ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
SERIAL_CAM_Error SerialCam_Initialize(UART_HandleTypeDef *Uart);
SERIAL_CAM_Error SerialCam_PreCapture(void);
SERIAL_CAM_Error SerialCam_Capture(uint16_t *resultLength);
SERIAL_CAM_Error SerialCam_GetData(uint8_t pktBuf[PIC_PKT_LEN], uint8_t length, uint16_t pktCnt);
SERIAL_CAM_Error SerialCam_CloseData(void);

#ifdef __cplusplus
}
#endif

#endif /* __GROVE_SERIAL_CAMERA_H */
