/**
  ******************************************************************************
  * Created on: 2018/05/28
  * @file    l9110s_motor_driver.h
  * @author  Swestgyan s-hosoai
  * @brief   L9110S Motor Driver
  *
  ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __L9110S_MOTOR_DRIVER_H
#define __L9110S_MOTOR_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/*Color  Setting*/

/* Private macros ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void l9110s_Init(GPIO_TypeDef *leftMotorPort, uint16_t leftMotorForwardPin, uint16_t leftMotorBackPin,
		GPIO_TypeDef *rightMotorPort, uint16_t rightMotorForwardPin, uint16_t rightMotorBackPin);
void l9110s_Drive(int8_t leftMotorSpeed, int8_t rightMotorSpeed);

#ifdef __cplusplus
}
#endif

#endif /* __L9110S_MOTOR_DRIVER_H */
