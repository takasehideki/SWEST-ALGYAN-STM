/**
  ******************************************************************************
  * @file    AzureXcubeSample.c
  * @author  MCD Application Team
  * @brief   Azure IoT Hub connection example.
  *          Basic telemetry on sensor-equipped boards.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "iot_flash_config.h"
#include "msg.h"
#include "timingSystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "serializer.h"
#include "iothub_client_ll.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"   /* For Sleep() */
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "iothubtransportmqtt.h"
#include "jsondecoder.h"
//#include "azure_c_shared_utility/macro_utils.h" /* For enum-string translation */

#include "main.h"


/* Private define ------------------------------------------------------------*/
#define MESSAGE_COUNT                     5
#define DOWORK_LOOP_NUM                   3

#define MODEL_MAC_SIZE                    13
#define MODEL_STATUS_SIZE                 32
#define MODEL_DEFAULT_TELEMETRYINTERVAL   5
#define MODEL_DEFAULT_LEDSTATUSON         true

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  char mac[MODEL_MAC_SIZE];
} model_strings_t;

typedef struct EVENT_INSTANCE_TAG
{
  IOTHUB_MESSAGE_HANDLE messageHandle;
  size_t messageTrackingId;  // For tracking the messages within the user callback.
} EVENT_INSTANCE;

/* The Azure model declaration defines many symbols which are unused. Mask the compilation warning. */
#ifdef __ICCARM__             /* IAR */
#pragma diag_suppress=Pe177
#elif defined ( __CC_ARM )    /* Keil / armcc */
#pragma diag_suppress 177
#elif defined ( __GNUC__ )    /*GNU Compiler */
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

BEGIN_NAMESPACE(IotThing);

DECLARE_MODEL(SerializableIotSampleDev_t,
    /* Event data: Temperature, Humidity... */
    WITH_DATA(ascii_char_ptr, mac),
    WITH_DATA(float, temperature),
    WITH_DATA(float, humidity),
    WITH_DATA(float, pressure),
    WITH_DATA(int, proximity),
    WITH_DATA(int, accX),
    WITH_DATA(int, accY),
    WITH_DATA(int, accZ),
    WITH_DATA(float, gyrX),
    WITH_DATA(float, gyrY),
    WITH_DATA(float, gyrZ),
    WITH_DATA(int, magX),
    WITH_DATA(int, magY),
    WITH_DATA(int, magZ),
    WITH_DATA(EDM_DATE_TIME_OFFSET, ts),
    WITH_DATA(int, devContext),
    /* Methods */
    WITH_METHOD(Reboot),
    WITH_METHOD(Hello, ascii_char_ptr, msg),
    /* Commands */
    WITH_ACTION(LedToggle),
    /* Desired Properties */
    WITH_DESIRED_PROPERTY(int, DesiredTelemetryInterval),
    /* Reported Properties */
    WITH_REPORTED_PROPERTY(bool, LedStatusOn),
    WITH_REPORTED_PROPERTY(int, TelemetryInterval)
  );

  END_NAMESPACE(IotThing);

    
  typedef struct {
    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;
    SerializableIotSampleDev_t *serModel;
    model_strings_t strings;
  } IotSampleDev_t;


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static bool g_continueRunning;
static bool g_publishData;
static bool g_reboot;

/* Private function prototypes -----------------------------------------------*/
int device_model_create(IotSampleDev_t **pModel);
int device_model_destroy(IotSampleDev_t *model);
int32_t comp_left_ms(uint32_t init, uint32_t now, uint32_t timeout);
void deviceTwinReportedStateCallback(int status_code, void* userContextCallback);

void iothub_client_XCube_sample_run(void);

/* Exported functions --------------------------------------------------------*/
int cloud_device_enter_credentials(void)
{
  iot_config_t iot_config;
  int ret = 0;

  memset(&iot_config, 0, sizeof(iot_config_t));
    
  printf("\nEnter the Azure connection string of your device: (template: HostName=xxx;DeviceId=xxx;SharedAccessKey=xxxx=) \n");
  getInputString(iot_config.device_name, USER_CONF_DEVICE_NAME_LENGTH);
  msg_info("read: --->\n%s\n<---\n", iot_config.device_name);
    
  if(setIoTDeviceConfig(&iot_config) != 0)
  {
    ret = -1;
    msg_error("Failed programming the IoT device configuration to Flash.\n");
  }
  
  return ret;
}

bool app_needs_device_keypair()
{
  return false;
}


EXECUTE_COMMAND_RESULT LedToggle(SerializableIotSampleDev_t *dev)
{
  IotSampleDev_t * device = (IotSampleDev_t *) dev->devContext;
  unsigned char *buffer;
  size_t bufferSize;

  msg_info("Received LedToggle command.\n");

  dev->LedStatusOn = !dev->LedStatusOn;
  Led_SetState(dev->LedStatusOn);

  /* Report the changed state */
  if (SERIALIZE_REPORTED_PROPERTIES(&buffer, &bufferSize, *dev) != CODEFIRST_OK)
  {
    msg_error("Failed serializing Reported State.\n");
  }
  else
  {
    if (IoTHubClient_LL_SendReportedState(device->iotHubClientHandle, buffer, bufferSize, deviceTwinReportedStateCallback, NULL) != IOTHUB_CLIENT_OK)
    {
      msg_error("Failed sending Reported State.\n");
    }
    free(buffer);
  }

  return EXECUTE_COMMAND_SUCCESS;
}


METHODRETURN_HANDLE Reboot(SerializableIotSampleDev_t *dev)
{
  (void)(dev);
  METHODRETURN_HANDLE result = MethodReturn_Create(1, "{\"Message\":\"Rebooting with Method\"}");
  msg_info("Received Reboot request.\n");
  g_reboot = true;
  return result;
}


METHODRETURN_HANDLE Hello(SerializableIotSampleDev_t *dev, ascii_char_ptr msg)
{
  (void)(dev);
  METHODRETURN_HANDLE result = MethodReturn_Create(1, "{\"Message\":\"Helloed with Method\"}");
  msg_info("Received Hello %s.\n", msg);
  return result;
}


static int DeviceMethodCallback(const char* method_name, const unsigned char* payload, size_t size, unsigned char** response, size_t* resp_size, void* userContextCallback)
{
  int result;
  char* payloadZeroTerminated = (char*)malloc(size + 1);
  if (payloadZeroTerminated == 0)
  {
    msg_error("Allocation failed.\n");
    *resp_size = 0;
    *response = NULL;
    result = -1;
  }
  else
  {
    (void)memcpy(payloadZeroTerminated, payload, size);
    payloadZeroTerminated[size] = '\0';

    /*execute method - userContextCallback is of type deviceModel*/
    METHODRETURN_HANDLE methodResult = EXECUTE_METHOD(userContextCallback, method_name, payloadZeroTerminated);
    free(payloadZeroTerminated);

    if (methodResult == NULL)
    {
      printf("failed to EXECUTE_METHOD\r\n");
      *resp_size = 0;
      *response = NULL;
      result = -1;
    }
    else
    {
      /* get the serializer answer and push it in the networking stack*/
      const METHODRETURN_DATA* data = MethodReturn_GetReturn(methodResult);
      if (data == NULL)
      {
        printf("failed to MethodReturn_GetReturn\r\n");
        *resp_size = 0;
        *response = NULL;
        result = -1;
      }
      else
      {
        result = data->statusCode;
        if (data->jsonValue == NULL)
        {
          char* resp = "{}";
          *resp_size = strlen(resp);
          *response = (unsigned char*)malloc(*resp_size);
          (void)memcpy(*response, resp, *resp_size);
        }
        else
        {
          *resp_size = strlen(data->jsonValue);
          *response = (unsigned char*)malloc(*resp_size);
          (void)memcpy(*response, data->jsonValue, *resp_size);
        }
      }
      MethodReturn_Destroy(methodResult);
    }
  }
  return result;
}


static void DeviceTwinCallback(DEVICE_TWIN_UPDATE_STATE status_code,  const unsigned char* payload, size_t size, __attribute__((unused)) void* userContextCallback)
{
  IotSampleDev_t * device = userContextCallback;

  msg_info("DeviceTwinCallback payload: %.*s\nStatus_code = %d\n", size, (const char*) payload, status_code);

  JSON_DECODER_RESULT JSONDec;
  MULTITREE_HANDLE multiTreeHandle;
  char* temp = malloc(size + 1);
  if (temp == NULL)
  {
    msg_error("Alloc failed.\n");
    return;
  }

  /* We need to add the missing termination char */
  (void)memcpy(temp, payload, size);
  temp[size] = '\0';

  if((JSONDec = JSONDecoder_JSON_To_MultiTree(temp,&multiTreeHandle)) != JSON_DECODER_OK)
  {
    msg_error("Decoding JSON Code=%d\r\n",JSONDec);
    free(temp);
    return;
  }

  MULTITREE_HANDLE childHandle;
  if(MultiTree_GetChildByName(multiTreeHandle, "desired", &childHandle) != MULTITREE_OK)
  {
    /* The payload has no "desired" object. Looking up directly at the property name. */
    childHandle = multiTreeHandle;
  }

  MULTITREE_HANDLE childHandle2;
  if(MultiTree_GetChildByName(childHandle, "DesiredTelemetryInterval", &childHandle2) == MULTITREE_OK)
  {
    void const *value = NULL;
    if (MultiTree_GetValue(childHandle2, &value) != MULTITREE_OK)
    {
      msg_error("Failed parsing the desired TelemetryInterval attribute.\n");
    }
    else
    {
      unsigned char *buffer;
      size_t bufferSize;

      device->serModel->TelemetryInterval = atoi(value);
      msg_info("Setting telemetry interval to %d.\n", device->serModel->TelemetryInterval);

      if (SERIALIZE_REPORTED_PROPERTIES(&buffer, &bufferSize, *(device->serModel)) != CODEFIRST_OK)
      {
        msg_error("Failed serializing Reported State.\n");
      }
      else
      {
        if (IoTHubClient_LL_SendReportedState(device->iotHubClientHandle, buffer, bufferSize, deviceTwinReportedStateCallback, NULL) != IOTHUB_CLIENT_OK)
        {
          msg_error("Failed sending Reported State.\n");
        }
        free(buffer);
      }
    }
  }

  MultiTree_Destroy(multiTreeHandle);
  free(temp);
}


static IOTHUBMESSAGE_DISPOSITION_RESULT MessageCallback(IOTHUB_MESSAGE_HANDLE message, void* userContextCallback)
{
  IOTHUBMESSAGE_DISPOSITION_RESULT result;
  const unsigned char* buffer;
  size_t size;

  if (IoTHubMessage_GetByteArray(message, &buffer, &size) != IOTHUB_MESSAGE_OK)
  {
    msg_error("unable to IoTHubMessage_GetByteArray\n");
    result = IOTHUBMESSAGE_ABANDONED;
  }
  else
  {
    /*buffer is not zero terminated*/
    char* temp = malloc(size + 1);
    if (temp == NULL)
    {
      msg_error("failed to malloc\n");
      result = IOTHUBMESSAGE_ABANDONED;
    }
    else
    {
      (void)memcpy(temp, buffer, size);
      temp[size] = '\0';

      msg_info("Received message: %s\n", temp);

      EXECUTE_COMMAND_RESULT executeCommandResult = EXECUTE_COMMAND(userContextCallback, temp);
      result =
        (executeCommandResult == EXECUTE_COMMAND_ERROR) ? IOTHUBMESSAGE_ABANDONED :
        (executeCommandResult == EXECUTE_COMMAND_SUCCESS) ? IOTHUBMESSAGE_ACCEPTED :
        IOTHUBMESSAGE_REJECTED;
      free(temp);
    }
  }
  return result;
}


void deviceTwinReportedStateCallback(int status_code, void* userContextCallback)
{
  (void)(userContextCallback);
  (void)(status_code); 
  msg_info("IoTHub: reported properties delivered.\n");
}


static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* userContextCallback)
{
  IOTHUB_MESSAGE_HANDLE msgHnd = userContextCallback;
  msg_info("Confirmation received for message with result = %s\n", ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result));
  IoTHubMessage_Destroy(msgHnd); 
}


/** Main loop */
void iothub_client_XCube_sample_run(void)
{
  int ret = 0;
  IotSampleDev_t * device = NULL;
  g_continueRunning = true;
  g_publishData = false;
  g_reboot = false;
  const char * connectionString = NULL;
  const char * ca_cert = NULL;

  ret = platform_init();

  if (ret != 0)
  {
    msg_error("Failed to initialize the platform.\n");
  }
  else
  {
    ret = (SERIALIZER_REGISTER_NAMESPACE(IotThing) == NULL);
  }

  if (ret != 0)
  {
    msg_error("SERIALIZER_REGISTER_NAMESPACE failed.\n");
  }
  else
  {
    ret = device_model_create(&device);
  }

  if (ret != 0)
  {
    msg_error("Failed to create the device model.\n");
  }
  else 
  {
    ret = (getIoTDeviceConfig(&connectionString) != 0);
  }

  if (ret != 0)
  {
    msg_error("Cannot retrieve the connection string from the user configuration storage.\n");
  }
  else
  {
    device->iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol);
    ret = (device->iotHubClientHandle == NULL);
  }

  if (ret != 0)
  {
    msg_error("Failed creating the iotHub client.\n");
  }
  else
  {
    bool traceOn = false;
    IoTHubClient_LL_SetOption(device->iotHubClientHandle, "logtrace", &traceOn);

    ret = getTLSKeys(&ca_cert, NULL, NULL);
  }

  if (ret != 0)
  {
    msg_error("Cannot retrieve the the root CA from the user configuration storage.\n");
  }
  else
  {
    ret = (IoTHubClient_LL_SetOption(device->iotHubClientHandle, "TrustedCerts", ca_cert) != IOTHUB_CLIENT_OK);
  }

  if (ret != 0)
  {
    msg_error("Failed setting the option: \"TrustedCerts\"\n");
  }
  else
  {
    ret = (IoTHubClient_LL_SetDeviceMethodCallback(device->iotHubClientHandle, DeviceMethodCallback, device->serModel) != IOTHUB_CLIENT_OK);
  }

  if (ret != 0)
  {
    msg_error("Failed registering the device method callback.\n");
  }
  else
  {
    ret = (IoTHubClient_LL_SetMessageCallback(device->iotHubClientHandle, MessageCallback, device->serModel) != IOTHUB_CLIENT_OK);
  }

  if (ret != 0)
  {
    msg_error("Failed registering the message callback.\n");
  }
  else
  {
    ret = (IoTHubClient_LL_SetDeviceTwinCallback(device->iotHubClientHandle, DeviceTwinCallback, device) != IOTHUB_CLIENT_OK);
  }

  if (ret != 0)
  {
    msg_error("Failed registering the twin callback.\n");
  }
  else
  {
    msg_info("Callbacks registered successfully.\n");

    /* Report the initial state. */
    unsigned char *buffer;
    size_t bufferSize;
    net_macaddr_t mac = { 0 };
    if (net_get_mac_address(hnet, &mac) == NET_OK)
    {
      snprintf(device->serModel->mac, MODEL_MAC_SIZE - 1, "%02X%02X%02X%02X%02X%02X", mac.mac[0], mac.mac[1], mac.mac[2], mac.mac[3], mac.mac[4], mac.mac[5]);
    }
    else
    {
      msg_warning("Could not retrieve the MAC address to set the device ID.\n");
      snprintf(device->serModel->mac, MODEL_MAC_SIZE - 1, "MyDevice-UnknownMAC");
    }

    device->serModel->TelemetryInterval = MODEL_DEFAULT_TELEMETRYINTERVAL;
    device->serModel->LedStatusOn = MODEL_DEFAULT_LEDSTATUSON;
    Led_SetState(device->serModel->LedStatusOn);

    if (SERIALIZE_REPORTED_PROPERTIES(&buffer, &bufferSize, *device->serModel) != CODEFIRST_OK)
    {
      msg_error("Serializing Reported State.\n");
    }
    else
    {
      if(IoTHubClient_LL_SendReportedState(device->iotHubClientHandle, buffer, bufferSize, deviceTwinReportedStateCallback, NULL)!= IOTHUB_CLIENT_OK)
      {
        msg_error("Failure Sending Reported State.\n");
      } 
      free(buffer);
    }

    for (size_t index = 0; index < DOWORK_LOOP_NUM; index++)
    {
      IoTHubClient_LL_DoWork(device->iotHubClientHandle);
    }

    /* Loop sending telemetry data. */
    uint32_t last_telemetry_time_ms = HAL_GetTick();
    do
    {
      uint8_t command = Button_WaitForMultiPush(500);
      bool b_sample_data = (command == BP_SINGLE_PUSH); /* If short button push, publish once. */
      if (command == BP_MULTIPLE_PUSH)                  /* If long button push, toggle the telemetry publication. */
      {
        g_publishData = !g_publishData;
        msg_info("%s the sensor values publication loop.\n", (g_publishData == true) ? "Enter" : "Exit");
      }

      int32_t left_ms = comp_left_ms(last_telemetry_time_ms, HAL_GetTick(), device->serModel->TelemetryInterval * 1000);

      if ( ((g_publishData == true) && (left_ms <= 0))
          || (b_sample_data == true) )
      {
        SerializableIotSampleDev_t * mdl = device->serModel;
        unsigned char* destination;
        size_t destinationSize;

        last_telemetry_time_ms = HAL_GetTick();

        /* Read the Data from the sensors */
        time_t time = TimingSystemGetSystemTime();
        memset(&mdl->ts.dateTime, 0, sizeof(EDM_DATE_TIME_OFFSET));
        mdl->ts.dateTime = *(gmtime(&time));
        {
#ifdef SENSOR
          int16_t  ACC_Value[3];
          float    GYR_Value[3];
          int16_t  MAG_Value[3];
          mdl->temperature = BSP_TSENSOR_ReadTemp();
          mdl->humidity = BSP_HSENSOR_ReadHumidity();
          mdl->pressure = BSP_PSENSOR_ReadPressure();
          mdl->proximity = VL53L0X_PROXIMITY_GetDistance();
          BSP_ACCELERO_AccGetXYZ(ACC_Value);
          mdl->accX = ACC_Value[0];
          mdl->accY = ACC_Value[1];
          mdl->accZ = ACC_Value[2];
          BSP_GYRO_GetXYZ(GYR_Value);
          mdl->gyrX = GYR_Value[0];
          mdl->gyrY = GYR_Value[1];
          mdl->gyrZ = GYR_Value[2];
          BSP_MAGNETO_GetXYZ(MAG_Value);
          mdl->magX = MAG_Value[0];
          mdl->magY = MAG_Value[1];
          mdl->magZ = MAG_Value[2];

          /* Serialize the device data. */
          if (SERIALIZE(&destination, &destinationSize,
                mdl->mac,
                mdl->temperature, mdl->humidity, mdl->pressure, mdl->proximity,
                mdl->accX , mdl->accY, mdl->accZ,
                mdl->gyrX , mdl->gyrY, mdl->gyrZ,
                mdl->magX , mdl->magY, mdl->magZ,
                mdl->ts) != CODEFIRST_OK)
#else /* SENSOR */
          /* Serialize the device data. */
          if (SERIALIZE(&destination, &destinationSize,
                mdl->mac,
                mdl->ts) != CODEFIRST_OK)
#endif /* SENSOR */
          {
            msg_error("Failed to serialize.\n");
          }
          else
          {
            IOTHUB_MESSAGE_HANDLE msgHnd = NULL;
            /* Create the message. */
            /* Note: The message is destroyed by the confirmation callback. */
            if ((msgHnd = IoTHubMessage_CreateFromByteArray(destination, destinationSize)) == NULL)
            {
              msg_error("Failed allocating an iotHubMessage.\n");
            } 
            else
            {
              /* Send the message. */
              if (IoTHubClient_LL_SendEventAsync(device->iotHubClientHandle, msgHnd, SendConfirmationCallback, msgHnd) != IOTHUB_CLIENT_OK)
              {
                msg_error("IoTHubClient_LL_SendEventAsync failed.\n");
              }

            }
            free(destination);
          }
        }

        /* Visual notification of the telemetry publication: LED blink. */
        Led_Blink(80, 40, 5);
        /* Restore the LED state */
        Led_SetState(device->serModel->LedStatusOn);
      }

      for (size_t index = 0; index < DOWORK_LOOP_NUM; index++)
      {
        IoTHubClient_LL_DoWork(device->iotHubClientHandle);
      }

      ThreadAPI_Sleep(500);
    } while (g_continueRunning && !g_reboot);
    
    msg_info("iothub_client_XCube_sample_run exited, call DoWork %d more time to complete final sending...\n", DOWORK_LOOP_NUM);
    for (size_t index = 0; index < DOWORK_LOOP_NUM; index++)
    {
      IoTHubClient_LL_DoWork(device->iotHubClientHandle);
    }
  }

  if (device->iotHubClientHandle != NULL)
  {
    IoTHubClient_LL_Destroy(device->iotHubClientHandle);
    device->iotHubClientHandle = NULL;
  }

  ret = device_model_destroy(device);
  if (ret != 0) msg_error("Failed destroying the device model.\n");

  serializer_deinit();
  platform_deinit();

  if(g_reboot == true)
  {
    msg_info("Calling HAL_NVIC_SystemReset()\n");
    HAL_NVIC_SystemReset();
  }

}


int device_model_create(IotSampleDev_t ** pDev)
{
  int ret = -1;
  IotSampleDev_t *dev = malloc(sizeof(IotSampleDev_t));
  if (dev == NULL)
  {
    msg_error("Model alloc failed.\n");
  }
  else
  {
    memset(dev, 0, sizeof(IotSampleDev_t));
    dev->serModel = CREATE_MODEL_INSTANCE(IotThing, SerializableIotSampleDev_t);
    if (dev->serModel == NULL)
    {
      msg_error("Ser model alloc failed.\n");
      free(pDev);
    }
    else
    {
      /* Keep a reference to the upper device structure.
       * It is used in the device methods or actions implementation.
       */
      dev->serModel->devContext = (int32_t) dev;

      /* Register the buffers for all the string model attributes */
      dev->serModel->mac = dev->strings.mac;

      *pDev = dev;
      ret = 0;
    }
  }

  return ret;
}

int device_model_destroy(IotSampleDev_t *dev)
{
  free(dev->serModel);
  free(dev);
  return 0;
}

/**
 * @brief   Return the integer difference between 'init + timeout' and 'now'.
 *          The implementation is robust to uint32_t overflows.
 * @param   In:   init      Reference index.
 * @param   In:   now       Current index.
 * @param   In:   timeout   Target index.
 * @retval  Number of units from now to target.
 */
int32_t comp_left_ms(uint32_t init, uint32_t now, uint32_t timeout)
{
  int32_t ret = 0;
  uint32_t wrap_end = 0;

  if (now < init)
  { // Timer wrap-around detected
    // printf("Timer: wrap-around detected from %d to %d\n", init, now);
    wrap_end = UINT32_MAX - init;
  }
  ret = wrap_end - (now - init) + timeout;

  return ret;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
