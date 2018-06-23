/**
 ******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @brief   Main program body.
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
#include "main.h"
#include "grove_serial_camera.h"
#include "l9110s_motor_driver.h"

extern void iothub_client_XCube_sample_run(void);

/* Global variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;
RNG_HandleTypeDef hrng;
net_hnd_t hnet; /* Is initialized by cloud_main(). */
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static UART_HandleTypeDef console_uart;
static UART_HandleTypeDef cam_uart;

static volatile uint8_t button_flags = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void SPI_WIFI_ISR(void);

static void Console_UART_Init(void);
static void Cam_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

static void Tank_Init(void);
static void RTC_Init(void);
static void Button_ISR(void);
static void tank_test(void);
static void cam_test(void);
static void cloud_test(void const *arg);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval Status
 */
int main(void) {
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	Periph_Config();

	BSP_LED_Init(LED_GREEN);
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

	/* RNG init function */
	hrng.Instance = RNG;
	if (HAL_RNG_Init(&hrng) != HAL_OK) {
		Error_Handler();
	}

	/* RTC init */
	RTC_Init();
	/* UART console init */
	Console_UART_Init();
	Cam_UART_Init();
//	MX_TIM2_Init();
//	MX_TIM3_Init();

	Tank_Init();

	/* test code */
    cam_test();
	while (1) {
//		tank_test();
//		HAL_GPIO_WritePin(GPIOA, 15, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOA, 2, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOB, 1, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOB, 4, GPIO_PIN_RESET);
	}

#ifdef FIREWALL_MBEDLIB
	firewall_init();
#endif

//	cloud_test(0);
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follows :
 *            System Clock source            = PLL (MSI MSE)
 *            SYSCLK(Hz)                     = 80000000
 *            HCLK(Hz)                       = 80000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 1
 *            APB2 Prescaler                 = 1
 *            MSI Frequency(Hz)              = 48000000
 *            PLL_M                          = 6
 *            PLL_N                          = 20
 *            PLL_R                          = 2
 *            PLL_P                          = 7
 *            PLL_Q                          = 2
 *            Flash Latency(WS)              = 4
 * @param  None
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE
			| RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 6;
	RCC_OscInitStruct.PLL.PLLN = 20;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}

	/* Enable MSI PLL mode */
	HAL_RCCEx_EnableMSIPLLMode();
}

/**
 * @brief Set LED state
 */
void Led_SetState(bool on) {
	if (on == true) {
		BSP_LED_On(LED_GREEN);
	} else {
		BSP_LED_Off(LED_GREEN);
	}
}

/**
 * @brief Blink LED for 'count' cycles of 'period' period and 'duty' ON duration.
 * duty < 0 tells to start with an OFF state.
 */
void Led_Blink(int period, int duty, int count) {
	if ((duty > 0) && (period >= duty)) {
		/*  Shape:   ____
		 on |_off__ */
		do {
			Led_SetState(true);
			HAL_Delay(duty);
			Led_SetState(false);
			HAL_Delay(period - duty);
		} while (count--);
	}
	if ((duty < 0) && (period >= -duty)) {
		/*  Shape:         ____
		 __off_| on   */
		do {
			Led_SetState(false);
			HAL_Delay(period + duty);
			Led_SetState(true);
			HAL_Delay(-duty);
		} while (count--);
	}
}

/**
 * @brief Update button ISR status
 */
static void Button_ISR(void) {
	button_flags++;
}

/**
 * @brief Waiting for button to be pushed
 */
uint8_t Button_WaitForPush(uint32_t delay) {
	uint32_t time_out = HAL_GetTick() + delay;
	do {
		if (button_flags > 1) {
			button_flags = 0;
			return BP_MULTIPLE_PUSH;
		}

		if (button_flags == 1) {
			button_flags = 0;
			return BP_SINGLE_PUSH;
		}
	} while (HAL_GetTick() < time_out);
	return BP_NOT_PUSHED;
}

/**
 * @brief Waiting for button to be pushed
 */
uint8_t Button_WaitForMultiPush(uint32_t delay) {
	HAL_Delay(delay);
	if (button_flags > 1) {
		button_flags = 0;
		return BP_MULTIPLE_PUSH;
	}

	if (button_flags == 1) {
		button_flags = 0;
		return BP_SINGLE_PUSH;
	}
	return BP_NOT_PUSHED;
}

/**
 * @brief UART console init function
 */
static void Console_UART_Init(void) {
	console_uart.Instance = USART1;
	console_uart.Init.BaudRate = 115200;
	console_uart.Init.WordLength = UART_WORDLENGTH_8B;
	console_uart.Init.StopBits = UART_STOPBITS_1;
	console_uart.Init.Parity = UART_PARITY_NONE;
	console_uart.Init.Mode = UART_MODE_TX_RX;
	console_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	console_uart.Init.OverSampling = UART_OVERSAMPLING_16;
#ifdef UART_ONE_BIT_SAMPLE_DISABLE
	console_uart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	console_uart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
#endif
	BSP_COM_Init(COM1, &console_uart);
}

/* UART4 init function  for camera */
static void Cam_UART_Init(void) {
	cam_uart.Instance = UART4;
	cam_uart.Init.BaudRate = 115200;
	cam_uart.Init.WordLength = UART_WORDLENGTH_8B;
	cam_uart.Init.StopBits = UART_STOPBITS_1;
	cam_uart.Init.Parity = UART_PARITY_NONE;
	cam_uart.Init.Mode = UART_MODE_TX_RX;
	cam_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	cam_uart.Init.OverSampling = UART_OVERSAMPLING_16;
	cam_uart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	cam_uart.AdvancedInit.AdvFeatureInit =
	UART_ADVFEATURE_DMADISABLEONERROR_INIT;
	cam_uart.AdvancedInit.DMADisableonRxError =
	UART_ADVFEATURE_DMA_DISABLEONRXERROR;

	  /* Enable GPIO clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	  /* Enable USART clock */
	__HAL_RCC_UART4_CLK_ENABLE();

	/* Configure USART Tx as alternate function */
	GPIO_InitTypeDef gpio_init_structure;
	gpio_init_structure.Pin = GPIO_PIN_0;
	gpio_init_structure.Mode = GPIO_MODE_AF_PP;
	gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init_structure.Pull = GPIO_NOPULL;
	gpio_init_structure.Alternate = GPIO_AF8_UART4;
	HAL_GPIO_Init(GPIOA, &gpio_init_structure);

	/* Configure USART Rx as alternate function */
	gpio_init_structure.Pin = GPIO_PIN_1;
        gpio_init_structure.Mode = GPIO_MODE_AF_PP;
	gpio_init_structure.Alternate = GPIO_AF8_UART4;
	HAL_GPIO_Init(GPIOA, &gpio_init_structure);

	if (HAL_UART_Init(&cam_uart) != HAL_OK) {
		Error_Handler();
	}
}

/* TIM2 init function */
static void MX_TIM2_Init(void) {

	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 0;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4)
			!= HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	HAL_TIM_MspPostInit(&htim2);

}

/* TIM3 init function */
static void MX_TIM3_Init(void) {

	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 0;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_OC_Init(&htim3) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig)
			!= HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sConfigOC.OCMode = TIM_OCMODE_TIMING;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4)
			!= HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	HAL_TIM_MspPostInit(&htim3);

}

static void Tank_Init() {
	/* motor driver init
	 *  Left Motor : GPIOA
	 *   Forward : CN1-PIN2(PA15)
	 *   Back : CN1-PIN3(PA2)
	 *  Right Motor : GPIOB
	 *   Forward : CN3-PIN6(PB1)
	 *   Back : CN3-PIN7(PB4)
	 */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	l9110s_Init(GPIOA, GPIO_PIN_15, GPIO_PIN_2,
	GPIOB, GPIO_PIN_1, GPIO_PIN_4);
}

#if (defined(__GNUC__) && !defined(__CC_ARM))
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE {
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART2 and Loop until the end of transmission */
	while (HAL_OK != HAL_UART_Transmit(&console_uart, (uint8_t *) &ch, 1, 30000)) {
		;
	}
	return ch;
}

/**
 * @brief  Retargets the C library scanf function to the USART.
 * @param  None
 * @retval None
 */
GETCHAR_PROTOTYPE {
	/* Place your implementation of fgetc here */
	/* e.g. read a character on USART and loop until the end of read */
	uint8_t ch = 0;
	while (HAL_OK != HAL_UART_Receive(&console_uart, (uint8_t *) &ch, 1, 30000)) {
		;
	}
	return ch;
}

/**
 * @brief RTC init function
 */
static void RTC_Init(void) {
	/**Initialize RTC Only */
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
#ifdef RTC_OUTPUT_REMAP_NONE
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
#endif
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtc) != HAL_OK) {
		Error_Handler();
	}
}

static void cam_test() {
	if (SerialCam_Initialize(&cam_uart) != SERIAL_CAM_OK) {
		msg_error("initialize error");
	}
	uint8_t pktBuf[PIC_PKT_LEN];
	for (int i = 0; i < 10; i++) {
		msg_info("Take Picture! %d\n", i);
		if (SerialCam_PreCapture() != SERIAL_CAM_OK) {
			msg_error("precapture error");
		}
		uint16_t picLength;
		if (SerialCam_Capture(&picLength) != SERIAL_CAM_OK) {
			msg_error("initialize error");
		}
		msg_info("total size : %d", picLength);
		uint8_t totalFrame = (picLength) / (PIC_PKT_LEN - 6);
		if ((picLength % (PIC_PKT_LEN - 6)) != 0) {
			totalFrame += 1;
		}

		for (int frameCnt = 0; frameCnt < totalFrame; frameCnt++) {
			SerialCam_GetData(pktBuf, PIC_PKT_LEN, frameCnt);
			HAL_UART_Receive(&console_uart, &pktBuf[4], PIC_PKT_LEN - 6, 100); // strip head 4 byte and tail 2byte
		}
		SerialCam_CloseData();
		HAL_Delay(5000);
	}
}

static void tank_test() {
	l9110s_Drive(1, 1);
	msg_info("forward");
	HAL_Delay(1000);
	l9110s_Drive(-1, -1);
	msg_info("back");
	HAL_Delay(1000);
	l9110s_Drive(1, -1);
	msg_info("left s");
	HAL_Delay(1000);
	l9110s_Drive(-1, 1);
	msg_info("right s");
	HAL_Delay(1000);
	l9110s_Drive(1, 0);
	msg_info("left");
	HAL_Delay(1000);
	l9110s_Drive(0, 1);
	msg_info("right");
	HAL_Delay(1000);
	l9110s_Drive(0, 0);
	msg_info("stop");
	HAL_Delay(1000);
}

static void cloud_test(void const *arg) {
	iothub_client_XCube_sample_run();
}

/**
 * @brief  EXTI line detection callback.
 * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
	case (GPIO_PIN_13): {
		Button_ISR();
		break;
	}

	case (GPIO_PIN_1): {
		SPI_WIFI_ISR();
		break;
	}

	default: {
		break;
	}
	}
}

void SPI3_IRQHandler(void) {
	HAL_SPI_IRQHandler(&hspi);
}
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void) {
	while (1) {
		BSP_LED_Toggle(LED_GREEN);
		HAL_Delay(200);
	}
}

void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
