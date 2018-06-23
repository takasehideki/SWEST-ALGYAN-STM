/**
 ******************************************************************************
 * Created on: 2018/06/02
 * @file    l9110s_motor_driver.c
 * @author  Swestgyan s-hosoai
 * @brief   L9110S Motor Driver
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "l9110s_motor_driver.h"
#include "msg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static GPIO_TypeDef *_leftMotorPort;
static GPIO_TypeDef *_rightMotorPort;
static uint16_t _leftForwardPin;
static uint16_t _leftBackPin;
static uint16_t _rightForwardPin;
static uint16_t _rightBackPin;

/* Private function prototypes -----------------------------------------------*/
//static void _clearRxBuf(void);
/* Exported functions --------------------------------------------------------*/

/* Functions Body ------------------------------------------------------------*/
void l9110s_Init(GPIO_TypeDef *leftMotorPort, uint16_t leftMotorForwardPin, uint16_t leftMotorBackPin,
		GPIO_TypeDef *rightMotorPort, uint16_t rightMotorForwardPin, uint16_t rightMotorBackPin) {
	_leftMotorPort = leftMotorPort;
	_rightMotorPort = rightMotorPort;
	_leftForwardPin = leftMotorForwardPin;
	_leftBackPin = leftMotorBackPin;
	_rightForwardPin = rightMotorForwardPin;
	_rightBackPin = rightMotorBackPin;

	GPIO_InitTypeDef leftInit;
	leftInit.Pin = leftMotorForwardPin |leftMotorBackPin;
	leftInit.Mode = GPIO_MODE_OUTPUT_PP;
	leftInit.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(leftMotorPort, &leftInit);

	GPIO_InitTypeDef rightInit;
	rightInit.Pin = rightMotorForwardPin | rightMotorBackPin;
	rightInit.Mode = GPIO_MODE_OUTPUT_PP;
	rightInit.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(rightMotorPort, &rightInit);
}

// TODO to pwm
void l9110s_Drive(int8_t leftMotorSpeed, int8_t rightMotorSpeed) {
	// clear all pins -> stop
	HAL_GPIO_WritePin(_leftMotorPort, _leftForwardPin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(_leftMotorPort, _leftBackPin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(_rightMotorPort, _rightForwardPin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(_rightMotorPort, _rightBackPin, GPIO_PIN_SET);

	if (leftMotorSpeed > 0) {
		HAL_GPIO_WritePin(_leftMotorPort, _leftForwardPin, GPIO_PIN_RESET);
	} else if (leftMotorSpeed < 0) {
		HAL_GPIO_WritePin(_leftMotorPort, _leftBackPin, GPIO_PIN_RESET);
	} else {
		// stop, already cleared
	}

	if (rightMotorSpeed > 0) {
		HAL_GPIO_WritePin(_rightMotorPort, _rightForwardPin, GPIO_PIN_RESET);
	} else if (rightMotorSpeed < 0) {
		HAL_GPIO_WritePin(_rightMotorPort, _rightBackPin, GPIO_PIN_RESET);
	} else {
		// stop, already cleared
	}
}

