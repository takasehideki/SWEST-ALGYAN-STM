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
 *  $Id: blinky.c takasehideki $
 */

/* 
 *  blinkyアプリの本体
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
#include "blinky.h"

#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"


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
 *  並行実行されるタスクへのメッセージ領域
 */
char	message[3];

void led_task(intptr_t exinf)
{
  /* 内部では何もしていないので書かなくてもよい */
  HAL_Init();
  /* オンボードLED(LD2)を使用できるように初期化 */
  BSP_LED_Init(LED_GREEN);

  while (true) {
    //msg_info("Blinky LED 10s\n");
    syslog(LOG_NOTICE, "Blinky LED 1\n");
    dly_tsk(1000);
    syslog(LOG_NOTICE, "Blinky LED 10s\n");
    Led_Blink(1000, 500, 10);
    syslog(LOG_NOTICE, "Blinky LED 2\n");
    dly_tsk(1000);
  }
}

/*
 *  メインタスク
 */
void main_task(intptr_t exinf)
{
  char	c;
  ER_UINT	ercd;

  SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
  syslog(LOG_NOTICE, "Blinky program starts (exinf = %d).\n", (int_t) exinf);

  ercd = serial_opn_por(TASK_PORTID);
  if (ercd < 0 && MERCD(ercd) != E_OBJ) {
    syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.\n",
        itron_strerror(ercd), SERCD(ercd));
  }
  SVC_PERROR(serial_ctl_por(TASK_PORTID,
        (IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

  /*
   *  タスクの起動
   */
  SVC_PERROR(act_tsk(LED_TASK));

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

  syslog(LOG_NOTICE, "Sample program ends.\n");
  SVC_PERROR(ext_ker());
  assert(0);
}


/**
 * @brief Set LED state
 */
void Led_SetState(bool_t on)
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


