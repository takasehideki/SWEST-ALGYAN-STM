/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015      by 3rd Designing Center
 *              Imageing System Development Division RICOH COMPANY, LTD.
 * 
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  @(#) $Id: target_inithook.c 698 2015-08-29 17:44:08Z roi $
 */

#include <sil.h>
#include "stm32f4xx.h"

/*
 *  ������ץ�����stm32f429-board�ѡ�
 */

#define HSE_STARTUP_TIMEOUT          0x0500		/* HSE�ѥ����ॢ���������� */

/*
 *  PLL������
 */
#define PLL_M      25
#define PLL_N      336
#define PLL_P      2
#define PLL_Q      7

#define PLLI2S_N   192
#define PLLI2S_R   5

#define RCC_CFGR_SW       (RCC_CFGR_SW_0 | RCC_CFGR_SW_1)
#define RCC_CFGR_SWS      (RCC_CFGR_SWS_0 | RCC_CFGR_SWS_1)

/*
 *  ���٥�Υ������åȰ�¸�ν����
 *
 *  �������ȥ��åץ⥸�塼�����ǡ�����ν���������˸ƤӽФ���롥
 */
void hardware_init_hook(void)
{
	unsigned int StartUpCounter = 0, HSEStatus = 0;

	/*
	 * RCC�Υꥻ�åȤȥ���ե�����졼�����
	 */
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) | RCC_CR_HSION);
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), 0x00000000);
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & ~(RCC_CR_PLLON | RCC_CR_CSSON | RCC_CR_HSEON));
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), RCC_CFGR_MCO2PRE_2 | RCC_CFGR_MCO1PRE_2
					 | RCC_CFGR_PPRE2_0 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_HPRE_0);

	/* Reset HSEBYP bit */
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & ~RCC_CR_HSEBYP);

	/*
	 *  ������߶ػ�
	 */
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CIR), 0x00000000);

	/*
	 *  HSEͭ����
	 */
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) | RCC_CR_HSEON);

	/*
	 *  HSE��ǥ��Ԥ�
	 */
	do{
		HSEStatus = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_HSERDY;
		StartUpCounter++;
	}while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_HSERDY) != 0){
		/*
		 *  VOLTAGE��������1����
		 */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR), sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR)) | RCC_APB1ENR_PWREN);
		sil_wrw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR), sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR)) | PWR_CR_VOS);

	    /*
		 *  HCLK�Υ���å�����
		 */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) | RCC_CFGR_HPRE_DIV1);

		/*
		 *  PCLK1�Υ���ե�����졼�����
		 */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) | RCC_CFGR_PPRE2_DIV2);

		/*
		 *  PCLK1�Υ���ե�����졼�����
		 */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) | RCC_CFGR_PPRE1_DIV4);

		/*
		 *  PLL����
		 */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24));

 		/*
		 *  PLLͭ����
		 */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) | RCC_CR_PLLON);

        /*
		 *  PLL�����Ԥ�
		 */
		while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) == 0)
		{
		}

		/*
		 *  180MHz�ؤΥ����С��ɥ饤������
		 */
		sil_wrw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR), sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR)) | PWR_CR_ODEN);
		while((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CSR)) & PWR_CSR_ODRDY) == 0)
		{
		}
		sil_wrw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR), sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR)) | PWR_CR_ODSWEN);
		while((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CSR)) & PWR_CSR_ODSWRDY) == 0)
		{
		}

		/*
		 *  FALSH�ץ�ե��å�����
		 */
		sil_wrw_mem((uint32_t *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR), FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS);

		/*
		 *  PLL�򥷥��ƥॽ�����Ȥ�������
		 */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), (sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL);

		/*
		 *  PLL�������ƥॽ�������ꤵ���ޤ��Ԥ�
		 */
		while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
		{
		}
	}
	else{
		/*
		 * HSE���������
		 */
	}
}

