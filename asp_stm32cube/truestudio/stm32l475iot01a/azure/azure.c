/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2012 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  $Id: azure.c takasehideki $
 */

/* 
 *  azureアプリの本体
 *
 *  プログラムの概要: 
 *
 *  TODO (そのうちちゃんと書く)
 *
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "azure.h"

/*
 *  サービスコールのエラーのログ出力
 */
  Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
  if (ercd < 0) {
    t_perror(LOG_ERROR, file, line, expr, ercd);
  }
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))

/*
 *  メインタスク
 */
void main_task(intptr_t exinf)
{
  char	c;
  ER_UINT	ercd;

  SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
  syslog(LOG_NOTICE, "BlinkyButtonISR program starts (exinf = %d).", (int_t) exinf);

  /* ここでシリアルオープンしたら後々に死ぬ可能性あり */
  ercd = serial_opn_por(TASK_PORTID);
  if (ercd < 0 && MERCD(ercd) != E_OBJ) {
    syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
        itron_strerror(ercd), SERCD(ercd));
  }
  SVC_PERROR(serial_ctl_por(TASK_PORTID,
        (IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

  /*
   *  タスクの起動
   */
  SVC_PERROR(act_tsk(LED_TASK));
  //SVC_PERROR(act_tsk(AZURE_TASK));

  /*
   *  メインループ
   */
  do {
    SVC_PERROR(serial_rea_dat(TASK_PORTID, &c, 1));
    switch (c) {
      default:
        break;
    }
  } while (c != '\003' && c != 'Q');

  syslog(LOG_NOTICE, "Sample program ends.");
  SVC_PERROR(ext_ker());
  assert(0);
}


/*
 *  ここからAzureアプリの記述
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"


extern void iothub_client_XCube_sample_run(void);

/* Global variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;
RNG_HandleTypeDef hrng;
net_hnd_t         hnet; /* Is initialized by cloud_main(). */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* TOPPERS側でUSART1を使えるようにしてるから要らないはず */
//static UART_HandleTypeDef console_uart;
static volatile uint8_t button_flags = 0;

/* Private function prototypes -----------------------------------------------*/
/* startupで初期化できているはずなので要らないはず */
//static void SystemClock_Config(void);
void SPI_WIFI_ISR(void);

/* TOPPERS側でUSART1を使えるようにしてるから要らないはず */
//static void Console_UART_Init(void);
static void RTC_Init(void);
static void Button_ISR(void);
static void cloud_test(void const *arg);


void led_task(intptr_t exinf)
{
  /* 内部では何もしていないので書かなくてもよい */
  HAL_Init();
  /* オンボードLED(LD2)を使用できるように初期化 */
  BSP_LED_Init(LED_GREEN);
  /* USER Push-SWを割込みモードで使用できるように初期化 */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  while (true) {
    syslog(LOG_NOTICE, "Wait for interrupt upto 1s");
    if (Button_WaitForPush(1000) != BP_NOT_PUSHED) {
      //msg_info("Blinky LED 10s\n");
      syslog(LOG_NOTICE, "Blinky LED 10s");
      Led_Blink(1000, 500, 10);
    }
  }
}

void azure_task(intptr_t exinf)
{
  syslog(LOG_NOTICE, "Azure program starts (exinf = %d).", (int_t) exinf);

  /* 内部では何もしていないので書かなくてもよい */
  HAL_Init();
  /* オンボードLED(LD2)を使用できるように初期化 */
  BSP_LED_Init(LED_GREEN);
  /* USER Push-SWを割込みモードで使用できるように初期化 */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* RNG init function */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }

  /* RTC init */
  RTC_Init();
  /* UART console init */
  //Console_UART_Init();  

#ifdef FIREWALL_MBEDLIB
  firewall_init();
#endif

  msg_info("Hello\n");

  cloud_test(0);
}



/**
 * @brief Set LED state
 */
void Led_SetState(bool on)
{
  if (on == true)
  {
    BSP_LED_On(LED_GREEN);
  }
  else
  {
    BSP_LED_Off(LED_GREEN);
  }
}

/**
 * @brief Blink LED for 'count' cycles of 'period' period and 'duty' ON duration.
 * duty < 0 tells to start with an OFF state.
 */ 
void Led_Blink(int period, int duty, int count)
{
  if ( (duty > 0) && (period >= duty) )
  {
    /*  Shape:   ____
        on |_off__ */
    do
    {
      Led_SetState(true);
      HAL_Delay(duty);
      Led_SetState(false);
      HAL_Delay(period - duty);
    } while (count--);
  }
  if ( (duty < 0) && (period >= -duty) )
  {
    /*  Shape:         ____
        __off_| on   */
    do
    {
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
static void Button_ISR(void)
{
  button_flags++;
}

/**
 * @brief Waiting for button to be pushed
 */
uint8_t Button_WaitForPush(uint32_t delay)
{
  uint32_t time_out = HAL_GetTick()+delay;
  do
  {
    if (button_flags > 1)
    {   
      button_flags = 0;
      return BP_MULTIPLE_PUSH;
    }

    if (button_flags == 1)
    {
      button_flags = 0;
      return BP_SINGLE_PUSH;
    }
  }
  while( HAL_GetTick() < time_out);
  return BP_NOT_PUSHED;
}

/**
 * @brief Waiting for button to be pushed
 */
uint8_t Button_WaitForMultiPush(uint32_t delay)
{
  HAL_Delay(delay);
  if (button_flags > 1)
  {
    button_flags = 0;
    return BP_MULTIPLE_PUSH;
  }

  if (button_flags == 1)
  {
    button_flags = 0;
    return BP_SINGLE_PUSH;
  }
  return BP_NOT_PUSHED;
}

/**
 * @brief RTC init function
 */
static void RTC_Init(void)
{
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
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
}

static void cloud_test(void const *arg)
{
  iothub_client_XCube_sample_run();
}


/**
 * @brief  EXTI line detection callback.
 * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case (GPIO_PIN_13):
      {
        Button_ISR();
        break;
      }

    case (GPIO_PIN_1):
      {
        SPI_WIFI_ISR();
        break;
      }

    default:
      {
        break;
      }
  }
}

void SPI3_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&hspi);
}
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */

void Error_Handler(void)
{
  while(1)
  {
    BSP_LED_Toggle(LED_GREEN);
    HAL_Delay(200);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

