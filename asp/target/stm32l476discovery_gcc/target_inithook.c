/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2016-2016 by 3rd Designing Center
 *              Imageing System Development Division RICOH COMPANY, LTD.
 *  Copyright (C) 2017-2017 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: target_inithook.c 698 2017-08-06 15:31:29Z roi $
 */

#include <sil.h>
#include "stm32l4xx.h"

/*
 *  ������ץ�����stm32l476-discovery�ѡ�
 */

#define sil_modw_mem(addr, mask, val)	sil_wrw_mem((addr), ((sil_rew_mem(addr) & ~(mask)) | (val)))
#define sil_andw_mem(addr, mask)		sil_wrw_mem((addr), (sil_rew_mem(addr) & ~(mask)))
#define sil_orw_mem(addr, val)			sil_wrw_mem((addr), (sil_rew_mem(addr) | (val)))


#define HSI_TIMEOUT_VALUE          (2000U)      /* 2 ms (minimum Tick + 1U) */
#define PLL_TIMEOUT_VALUE          (2000U)      /* 2 ms (minimum Tick + 1U) */
#define MSI_TIMEOUT_VALUE          (2000U)      /* 2 ms (minimum Tick + 1U) */
#define LSE_TIMEOUT_VALUE          (5000*1000)
#define DBP_TIMEOUT_VALUE          (100*1000)
#define PWR_FLAG_TIMEOUT_VALUE     (50)			/* 50micro sec */
#define CLOCKSWITCH_TIMEOUT_VALUE  (5000000U)	/* 5 s    */

#define RCC_MSIRANGE               RCC_CR_MSIRANGE_11
#define MAX_MSIRANGE               12

#if !defined(MSI_RANGE)
#define MSI_RANGE                  0
#endif

#if !defined(HSE_VALUE)
#define HSE_VALUE                  8000000  /* Value of the External oscillator in Hz */
#endif /* HSE_VALUE */
#if !defined(HSI_VALUE)
#define HSI_VALUE                  16000000	/* Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M      6
#define PLL_N      40

/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      7

/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      4

/* I2SCLK =  PLLVCO / PLLR */                          
#define PLL_R      4	/* Possible value between 2 and 7 */

#define RCC_MSICALIBRATION_DEFAULT     0		/* Default MSI calibration trimming value */

#define RCC_USBCLKSOURCE_MSI           RCC_CCIPR_CLK48SEL


/*
 *  VOLTAGE SCALE�����
 */
#define PWR_REGULATOR_VOLTAGE_SCALE1   PWR_CR1_VOS_0     /* Voltage scaling range 1 */
#define PWR_REGULATOR_VOLTAGE_SCALE2   PWR_CR1_VOS_1     /* Voltage scaling range 2 */


/*
 *  MSIRANGE�б�����å��ơ��֥�
 */
static const uint32_t MSIRangeTable[MAX_MSIRANGE] = {100000, 200000, 400000, 800000, 1000000, 2000000, \
                                      4000000, 8000000, 16000000, 24000000, 32000000, 48000000};

uint32_t SystemFrequency;


/*
 *  VOLTAGE SCALE����
 *  param    VoltageScaling
 *           This parameter can be one of the following values:
 *            @arg @ref PWR_REGULATOR_VOLTAGE_SCALE1 Regulator voltage output range 1 mode,
 *                                                typical output voltage at 1.2 V,
 *                                                system frequency up to 80 MHz.
 *            @arg @ref PWR_REGULATOR_VOLTAGE_SCALE2 Regulator voltage output range 2 mode,
 *                                                typical output voltage at 1.0 V,
 *                                                system frequency up to 26 MHz.
 *  return  ���ｪλ�ʤ�E_OK���֤�
 */
static ER
PWRE_ControlVoltageScaling(uint32_t VoltageScaling)
{
	uint32_t tick = 0;
	uint32_t tmp;

	if(VoltageScaling != PWR_REGULATOR_VOLTAGE_SCALE1 && VoltageScaling != PWR_REGULATOR_VOLTAGE_SCALE2)
		return E_PAR;

	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR1), RCC_APB1ENR1_PWREN);
	tmp = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR1));
	(void)(tmp);

	if(VoltageScaling == PWR_REGULATOR_VOLTAGE_SCALE1){	/* SCALE1���� */
		if((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1)) & PWR_CR1_VOS) != PWR_REGULATOR_VOLTAGE_SCALE1){
			sil_modw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1), PWR_CR1_VOS, PWR_REGULATOR_VOLTAGE_SCALE1);

			/*
			 *  VOSF�����ꥢ�����ޤ��Ԥ�
			 */
			while((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_SR2)) & PWR_SR2_VOSF) != 0){
				if(tick > PWR_FLAG_TIMEOUT_VALUE)
					return E_TMOUT;
				tick++;
				sil_dly_nse(1000);
			}
		}

		/*
		 * LOW POWER�⡼�ɤ�ꥻ�å�
		 */
		sil_andw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1), PWR_CR1_LPR);

		/*
		 *  REGLPF�����ꥢ�����ޤ��Ԥ�
		 */
		tick = 0;
		while((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_SR2)) & PWR_SR2_REGLPF) != 0){
			if(tick > PWR_FLAG_TIMEOUT_VALUE)
				return E_TMOUT;
			tick++;
			sil_dly_nse(1000);
		}
	}
	else{		/* SCALE2���� */
		if((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1)) & PWR_CR1_VOS) != PWR_REGULATOR_VOLTAGE_SCALE2){
			sil_modw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1), PWR_CR1_VOS, PWR_REGULATOR_VOLTAGE_SCALE1);
		}

		/*
		 * LOW POWER�⡼�ɤ򥻥å�
		 */
		sil_orw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1), PWR_CR1_LPR);
	}
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR1), RCC_APB1ENR1_PWREN);
	return E_OK;
}

/*
 *  MSIRANGE�˹�碌��Ŭ�ڤ�FLASH LATENCY�����ꤹ��
 *  param   msirange MSIRANG��
 *  return  ���ｪλ�ʤ�E_OK���֤�
 */
static ER
RCC_SetFlashLatencyFromMSIRange(unsigned int msirange)
{
	uint32_t vos = 0;
	uint32_t latency = FLASH_ACR_LATENCY_0WS;	/* default value 0WS */
	uint32_t sysclk  = MSIRangeTable[(msirange >> 4)];

	if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR1)) & RCC_APB1ENR1_PWREN) != 0){
		vos = sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1)) & PWR_CR1_VOS;
	}
	else{
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR1), RCC_APB1ENR1_PWREN);
		vos = sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1)) & PWR_CR1_VOS;
		sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR1), RCC_APB1ENR1_PWREN);
	}

	if(vos == PWR_CR1_VOS_0){		/* VOLTAGE_SCALE1 */
	    if(sysclk > 16000000){		/* 16MHz����� */
			if(sysclk > 32000000)	/* 32MHz����� */
				latency = FLASH_ACR_LATENCY_2WS; /* 2WS */
			else					/* 24MHz�ޤ��� 32MHz */
				latency = FLASH_ACR_LATENCY_1WS; /* 1WS */
		}
		/* ����¾�� 0WS */
	}
	else{							/* VOLTAGE_SCALE2 */
		if(sysclk > 16000000)		/* 16MHz����� */
			latency = FLASH_ACR_LATENCY_3WS; /* 3WS */
		else{
			if(sysclk == 16000000)	/* 16MHz */
				latency = FLASH_ACR_LATENCY_2WS; /* 2WS */
			else if(sysclk == 8000000)	/* 8MHz */
				latency = FLASH_ACR_LATENCY_1WS; /* 1WS */
			/* ����¾�� 0WS */
		}
	}
	/*
	 *  FLASH LATENCY����
	 */
	sil_modw_mem((uint32_t *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR), FLASH_ACR_LATENCY, latency);

	/*
	 *  FLASH LATENCY�����ǧ
	 */
	if((sil_rew_mem((uint32_t *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR)) & FLASH_ACR_LATENCY) != latency)
		return E_SYS;
	return E_OK;
}

/*
 *  ���祷���ƥ९��å�����
 *  �����ͤϰʲ����̤�
 *      System Clock source            = PLL (MSI)
 *      SYSCLK(Hz)                     = 80000000
 *      HCLK(Hz)                       = 80000000
 *      AHB Prescaler                  = 1
 *      APB1 Prescaler                 = 1
 *      APB2 Prescaler                 = 1
 *      MSI Frequency(Hz)              = 4000000
 *      PLL_M                          = 6
 *      PLL_N                          = 40
 *      PLL_R                          = 2
 *      PLL_P                          = 7
 *      PLL_Q                          = 4
 *      Flash Latency(WS)              = 4
 *  return  ���ｪλ�ʤ�E_OK���֤�
 */
static ER
SystemClock_Config(void)
{
	ER       ercd;
	uint32_t msi_range = 0;
	bool_t   pwrclkchanged = false;
	uint32_t tick = 0;

	if((ercd = PWRE_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)) != E_OK)
		return ercd;


	if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR1)) & RCC_APB1ENR1_PWREN) == 0){
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR1), RCC_APB1ENR1_PWREN);
		pwrclkchanged = 1;
	}

	/*
	 *  LSE ENABLE
	 */
	if((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1)) & PWR_CR1_DBP) == 0){
		sil_orw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1), PWR_CR1_DBP);
		while((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR1)) & PWR_CR1_DBP) == 0){
			if(tick > DBP_TIMEOUT_VALUE)
				return E_TMOUT;
			tick++;
			sil_dly_nse(1000);
		}
	}

	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR), RCC_BDCR_LSEON);
	tick = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR)) & RCC_BDCR_LSERDY) == 0){
		if(tick > LSE_TIMEOUT_VALUE)
			return E_TMOUT;
		tick++;
		sil_dly_nse(1000);
    }

	if(pwrclkchanged)
		sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR1), RCC_APB1ENR1_PWREN);

	/*
	 * Disable the LSE Clock Security System.
	 */
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_BDCR), RCC_BDCR_LSECSSON);
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CIER), RCC_CIFR_LSECSSF);

	/*
	 *  MSI����
	 */
    if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) == RCC_CFGR_SWS_MSI){
		/*
		 *  MSI�򥷥��ƥ९��å��Ȥ��ƻ��ѻ���MSI�Υ�󥸤��Ф�RCC_MSIRANGE�����ꤷ�ʤ���
		 */
		if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_MSIRGSEL) != 0)
			msi_range = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_MSIRANGE;
		else
			msi_range = (sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR)) & RCC_CSR_MSISRANGE) >> 4;
		if(RCC_MSIRANGE > msi_range){
			/*
			 *  �������⤤��硢FLASH LATENCY�������
			 */
			if(RCC_SetFlashLatencyFromMSIRange(RCC_MSIRANGE) != E_OK){
				return E_TMOUT;
			}

			/*
			 *  RCC_MSIRANGE�˺�����
			 */
			sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIRGSEL);
			sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIRANGE, RCC_MSIRANGE);
			sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_ICSCR), RCC_ICSCR_MSITRIM, RCC_MSICALIBRATION_DEFAULT << 8);
		}
		else{
			/*
			 *  ��������㤤��硢���RCC_MSIRANGE�˺�����
			 */
			sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIRGSEL);
			sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIRANGE, RCC_MSIRANGE);
			sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_ICSCR), RCC_ICSCR_MSITRIM, RCC_MSICALIBRATION_DEFAULT << 8);

			/*
			 *  FLASH LATENCY�������
			 */
			if(RCC_SetFlashLatencyFromMSIRange(RCC_MSIRANGE) != E_OK){
				return E_TMOUT;
			}
		}
    }
    else{
		/*
		 *  MSI��SYSCLK�Ǥʤ���硢��ư
		 */
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSION);

        /*
		 *  MSI READY�Ԥ�
		 */
		tick = 0;
        while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_MSIRDY) == 0){
			if(tick > MSI_TIMEOUT_VALUE){
				return E_TMOUT;
			}
			tick++;
			sil_dly_nse(1000);
		}

		/*
		 *  RCC_MSIRANGE����
		 */
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIRGSEL);
		sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIRANGE, RCC_MSIRANGE);
		sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_ICSCR), RCC_ICSCR_MSITRIM, RCC_MSICALIBRATION_DEFAULT << 8);
	}

	/*
	 *  Enable MSI Auto-calibration through LSE
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIPLLEN);

	/*
	 *  Select MSI output as USB clock source
	 */
	sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CCIPR), RCC_CCIPR_CLK48SEL, RCC_USBCLKSOURCE_MSI);

    /*
	 *  PLL����
	 */
    if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){
		/*
		 *  PLL��SYSCLK�Ǥʤ���硢PLL���
		 */
		sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_PLLON);

		/*
		 *  PLL����Ԥ�
		 */
		tick = 0;
		while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) != 0){
			if(tick > PLL_TIMEOUT_VALUE){
				return E_TMOUT;
			}
			tick++;
			sil_dly_nse(1000);
 		}

		/*
		 *  PLL����å�����
		 */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), ((PLL_M - 1) << 4) | (PLL_N << 8) | ((PLL_P >> 4) << 17) | \
                   RCC_PLLCFGR_PLLSRC_MSI | (((PLL_Q >> 1) - 1) << 21) | (((PLL_R >> 1) - 1) << 25));

		/*
		 *  PLL�Ƶ�ư
		 */
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_PLLON);

		/*
		 *  PLL �����ƥ९��å�����
		 */
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), RCC_PLLCFGR_PLLREN);

		/*
		 *  PLL READY�Ԥ�
		 */
        tick = 0;
		while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) == 0){
			if(tick > PLL_TIMEOUT_VALUE){
				return E_TMOUT;
			}
			tick++;
			sil_dly_nse(1000);
		}
	}
	else
		return E_SYS;

	/*
	 *  �׵��FLASH LATENCY���������⤤��硢���FLASH��LATENCY�����Ԥ�
	 */
	if(FLASH_ACR_LATENCY_4WS > (sil_rew_mem((uint32_t *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR)) & FLASH_ACR_LATENCY)){
		/*
		 *  LATENCY�����ꤷ�������ǧ
		 */
		sil_modw_mem((uint32_t *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR), FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS);
		if((sil_rew_mem((uint32_t *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR)) & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_4WS){
			return E_SYS;
		}
	}

	/*
	 *  HCLK��ʬ��������
	 */
	sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);

	/*
	 *  SYSCLK��PLL���ѹ��塢�����ǧ
	 */
	sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_SW, RCC_CFGR_SW_PLL);

	tick = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){
		if(tick > CLOCKSWITCH_TIMEOUT_VALUE){
			return E_TMOUT;
		}
		tick++;
		sil_dly_nse(1000);
	}

	/*
	 *  �׵��FLASH LATENCY����������㤤��硢���FLASH��LATENCY�����Ԥ�
	 */
	if(FLASH_ACR_LATENCY_4WS < (sil_rew_mem((uint32_t *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR)) & FLASH_ACR_LATENCY)){
		/*
		 *  LATENCY�����ꤷ�������ǧ
		 */
		sil_modw_mem((uint32_t *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR), FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS);
		if((sil_rew_mem((uint32_t *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR)) & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_4WS){
			return E_SYS;
		}
	}
	SystemFrequency = 80000000;

	/*
	 *  PCLK1��ʬ���ͤ�����
	 */
	sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV1);

	/*
	 *  PCLK2��ʬ���ͤ�����
	 */
	sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1);
	return E_OK;
}

/*
 *  MSI����ˤ�륯��å�����
 *  �����ͤϰʲ����̤�
 *      System Clock source            = MSI
 *      SYSCLK(Hz)                     = msirange�Υ���å���
 *      HCLK(Hz)                       = msirange�Υ���å���
 *      AHB Prescaler                  = 1(���)
 *      APB1 Prescaler                 = 1(���)
 *      APB2 Prescaler                 = 1(���)
 *      MSI Frequency(Hz)              = msirange�Υ���å���
 *      PLL_M                          = 1
 *      PLL_N                          = 80
 *      PLL_R                          = 2
 *      PLL_P                          = 7
 *      PLL_Q                          = 4
 *      Flash Latency(WS)              = msirange�˽��ä�LATENCY
 *  param   msirange
 *  return  ���ｪλ�ʤ�E_OK���֤�
 */
static ER
SystemLowClock_Config(uint8_t msirange)
{
	uint32_t tick = 0;
	ER ercd = E_OK;

	if(msirange == 0 || msirange >= MAX_MSIRANGE)
		return E_PAR;

	if(MSIRangeTable[msirange] == SystemFrequency)
		return E_OK;

	if((ercd = PWRE_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)) != E_OK)
		return ercd;

	/*
	 *  MSI����
	 */
    if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) == RCC_CFGR_SWS_MSI){
		uint32_t msi_range;
		/*
		 *  MSI�򥷥��ƥ९��å��Ȥ��ƻ��ѻ���MSI�Υ�󥸤��Ф�msirange�����ꤷ�ʤ���
		 */
		if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_MSIRGSEL) != 0)
			msi_range = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_MSIRANGE;
		else
			msi_range = (sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CSR)) & RCC_CSR_MSISRANGE) >> 4;
		if((msirange<<4) > msi_range){
			/*
			 *  �������⤤��硢FLASH LATENCY�������
			 */
			if(RCC_SetFlashLatencyFromMSIRange((msirange<<4)) != E_OK){
				return E_TMOUT;
			}

			/*
			 *  msirange�˺�����
			 */
			sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIRGSEL);
			sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIRANGE, (msirange<<4));
			sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_ICSCR), RCC_ICSCR_MSITRIM, RCC_MSICALIBRATION_DEFAULT << 8);
		}
		else{
			/*
			 *  ��������㤤��硢���msirange�˺�����
			 */
			sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIRGSEL);
			sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIRANGE, (msirange<<4));
			sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_ICSCR), RCC_ICSCR_MSITRIM, RCC_MSICALIBRATION_DEFAULT << 8);

			/*
			 *  FLASH LATENCY�������
			 */
			if(RCC_SetFlashLatencyFromMSIRange((msirange<<4)) != E_OK){
				return E_TMOUT;
			}
		}
    }
    else{
		/*
		 *  MSI��SYSCLK�Ǥʤ���硢��ư
		 */
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSION);

        /*
		 *  MSI READY�Ԥ�
		 */
        while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_MSIRDY) == 0){
			if(tick > MSI_TIMEOUT_VALUE){
				return E_TMOUT;
			}
			tick++;
			sil_dly_nse(1000);
		}

		/*
		 *  msirange����
		 */
		sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIRGSEL);
		sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSIRANGE, (msirange<<4));
		sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_ICSCR), RCC_ICSCR_MSITRIM, RCC_MSICALIBRATION_DEFAULT << 8);

		sil_modw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_SW, RCC_CFGR_SW_MSI);

		tick = 0;
		while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != RCC_CFGR_SWS_MSI){
			if(tick > CLOCKSWITCH_TIMEOUT_VALUE)
				return E_TMOUT;
			tick++;
			sil_dly_nse(1000);
		}

		/*
		 *  PLL���
		 */
		sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_PLLON);

        /*
		 *  SAI����å��������
		 */
		if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & (RCC_CR_PLLSAI1RDY | RCC_CR_PLLSAI2RDY)) == 0){
			sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), RCC_PLLCFGR_PLLSRC);
		}

		/*
		 *  PLLP/PLLQ/PLLR���
		 */
		sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), (RCC_PLLCFGR_PLLPEN | RCC_PLLCFGR_PLLQEN | RCC_PLLCFGR_PLLREN));

        /*
		 *  PLL����Ԥ�
		 */
		tick = 0;
		while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) != 0){
			if(tick > PLL_TIMEOUT_VALUE)
				return E_TMOUT;
			tick++;
			sil_dly_nse(1000);
		}
	}
	SystemFrequency = MSIRangeTable[msirange];

	if(SystemFrequency <= 20000000){
		ercd = PWRE_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);
	}
	return ercd;
}

/*
 *  ��ư����å��μ��Ф�
 */
uint32_t
sysclock_init_value(void)
{
#if MSI_RANGE != 0
	return MSIRangeTable[MSI_RANGE];
#else
	return 80000000;	/* 80 MHz */
#endif
}

/*
 *  �����ƥ९��å��ѹ�
 */
ER
sysclock_change(uint8_t range)
{
	if(range == 0)
		return SystemClock_Config();
	else
		return SystemLowClock_Config(range);
}

/*
 *  �����ƥ९��å�����ե�����졼�����
 */
ER
sysclock_config(int mode)
{
	return SystemClock_Config();
}

#ifndef TOPPERS_RAM_EXEC
/*
 *  ���٥�Υ������åȰ�¸�ν����
 *
 *  �������ȥ��åץ⥸�塼�����ǡ�����ν���������˸ƤӽФ���롥
 */
void hardware_init_hook(void)
{
	/*
	 * RCC�Υꥻ�åȤȥ���ե�����졼�����
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_MSION);
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), 0x00000000);
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR),
			 (RCC_CR_PLLSAI2ON | RCC_CR_PLLSAI1ON | RCC_CR_PLLON | RCC_CR_HSEBYP | RCC_CR_HSEON));
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), RCC_PLLCFGR_PLLN_4);

	/* Reset HSEBYP bit */
	sil_andw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_HSEBYP);

	/*
	 *  ������߶ػ�
	 */
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CIER), 0x00000000);

	if(sysclock_change(MSI_RANGE) != E_OK){
		while(1){}
	}

	/*
	 *  PCLK����
	 */
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CCIPR), 0x00000000);
}
#endif

