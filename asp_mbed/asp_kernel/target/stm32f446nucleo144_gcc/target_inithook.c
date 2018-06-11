/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2016      by 3rd Designing Center
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
 *  @(#) $Id: target_inithook.c 698 2016-06-14 13:48:08Z roi $
 */

#include <sil.h>
#include "stm32f4xx.h"

/*
 *  ������ץ�����stm32f446-nucleo144�ѡ�
 */

#define HSE_STARTUP_TIMEOUT          0x0500		/* HSE�ѥ����ॢ���������� */
#define PWR_OVERDRIVE_TIMEOUT_VALUE  1000

/*
 *  PLL������
 */
#define PLL_M      8
#define PLL_N      360
#define PLL_P      2
#define PLL_Q      7
#define PLL_R      2

#define PWR_REGULATOR_VOLTAGE_SCALE1   PWR_CR_VOS				/* VOLTAGE��������1:168MHz�������С��ɥ饤�ֻ�180MHz */
#define PWR_REGULATOR_VOLTAGE_SCALE2   PWR_CR_VOS_1				/* VOLTAGE��������2:144MHz�������С��ɥ饤�ֻ�168MHz */
#define PWR_REGULATOR_VOLTAGE_SCALE3   PWR_CR_VOS_0				/* VOLTAGE��������3:120MHz. */

#define RCC_CFGR_SW        (RCC_CFGR_SW_0 | RCC_CFGR_SW_1)
#define RCC_CFGR_SWS       (RCC_CFGR_SWS_0 | RCC_CFGR_SWS_1)
#define RCC_CFGR_HPRE      (RCC_CFGR_HPRE_0 | RCC_CFGR_HPRE_1 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_3)
#define RCC_CFGR_PPRE1     (RCC_CFGR_PPRE1_0 | RCC_CFGR_PPRE1_1 | RCC_CFGR_PPRE1_2)
#define RCC_CFGR_PPRE2     (RCC_CFGR_PPRE2_0 | RCC_CFGR_PPRE2_1 | RCC_CFGR_PPRE2_2)
#define RCC_PLLCFGR_PLLM   (RCC_PLLCFGR_PLLM_0 | RCC_PLLCFGR_PLLM_1 | RCC_PLLCFGR_PLLM_2 \
                           | RCC_PLLCFGR_PLLM_3 | RCC_PLLCFGR_PLLM_4 | RCC_PLLCFGR_PLLM_5)
#define RCC_PLLCFGR_PLLN   (RCC_PLLCFGR_PLLN_0 | RCC_PLLCFGR_PLLN_1 | RCC_PLLCFGR_PLLN_2 | RCC_PLLCFGR_PLLN_3 \
                           | RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_7 | RCC_PLLCFGR_PLLN_8)
#define RCC_PLLCFGR_PLLP   (RCC_PLLCFGR_PLLP_0 | RCC_PLLCFGR_PLLP_1)
#define RCC_PLLCFGR_PLLSRC (RCC_PLLCFGR_PLLSRC_HSE)
#define RCC_CR_HSITRIM     (RCC_CR_HSITRIM_0 | RCC_CR_HSITRIM_1 | RCC_CR_HSITRIM_2 | RCC_CR_HSITRIM_3 | RCC_CR_HSITRIM_4)
#define FLASH_ACR_LATENCY  (FLASH_ACR_LATENCY_0WS | FLASH_ACR_LATENCY_1WS | FLASH_ACR_LATENCY_2WS | FLASH_ACR_LATENCY_3WS \
                           | FLASH_ACR_LATENCY_4WS | FLASH_ACR_LATENCY_5WS | FLASH_ACR_LATENCY_6WS | FLASH_ACR_LATENCY_7WS)

/*
 *  RCC_BitAddress_AliasRegion RCC BitAddress AliasRegion
 *  RCC registers bit address in the alias region
 */
#define RCC_OFFSET               (TADR_RCC_BASE - PERIPH_BASE)
/* Alias word address of HSION bit */
#define RCC_CR_OFFSET            (RCC_OFFSET + 0x00)
#define RCC_HSION_BIT_NUMBER     0x00
#define RCC_CR_HSION_BB          (PERIPH_BB_BASE + (RCC_CR_OFFSET * 32) + (RCC_HSION_BIT_NUMBER * 4))
/* Alias word address of PLLON bit */
#define RCC_PLLON_BIT_NUMBER     0x18
#define RCC_CR_PLLON_BB          (PERIPH_BB_BASE + (RCC_CR_OFFSET * 32) + (RCC_PLLON_BIT_NUMBER * 4))

#define RCC_PLLSAION_BIT_NUMBER  0x1C
#define RCC_CR_PLLSAION_BB       (PERIPH_BB_BASE + (RCC_CR_OFFSET * 32) + (RCC_PLLSAION_BIT_NUMBER * 4))

/*
 *  ACR register byte 0 (Bits[7:0]) base address
 */
#define ACR_BYTE0_ADDRESS        (0x40023C00)
/*
 *  CR register byte 3 (Bits[23:16]) base address
 */
#define RCC_CR_BYTE2_ADDRESS     (0x40023802)

#define RCC_HSE_BYPASS           0x05

#define PWR_OFFSET               (TADR_PWR_BASE - PERIPH_BASE)
#define PWR_CR_OFFSET            0x00
#define PWR_CSR_OFFSET           0x04
#define PWR_CR_OFFSET_BB         (PWR_OFFSET + PWR_CR_OFFSET)
#define PWR_CSR_OFFSET_BB        (PWR_OFFSET + PWR_CSR_OFFSET)

#define ODEN_BIT_NUMBER          16
#define CR_ODEN_BB               (unsigned int)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32) + (ODEN_BIT_NUMBER * 4))

#define ODSWEN_BIT_NUMBER        17
#define CR_ODSWEN_BB             (unsigned int)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32) + (ODSWEN_BIT_NUMBER * 4))

#define RCC_CLK48CLKSOURCE_PLLQ    0x00000000
#define RCC_CLK48CLKSOURCE_PLLSAIP RCC_DCKCFGR2_CK48MSEL

#define PLL_TIMEOUT_VALUE          1000		/* 100 ms */
#define PLLSAI_TIMEOUT_VALUE       20		/* 2ms */
#define CLOCKSWITCH_TIMEOUT_VALUE  50000	/* 5 s */

#define sil_mdw_mem(addr, mask, val)	sil_wrw_mem((addr), ((sil_rew_mem(addr) & ~(mask)) | (val)))
#define sil_anw_mem(addr, mask)			sil_wrw_mem((addr), (sil_rew_mem(addr) & ~(mask)))
#define sil_orw_mem(addr, val)			sil_wrw_mem((addr), (sil_rew_mem(addr) | (val)))

/*
 *  USB CLOCK CONFIGURATION
 */
ER USB_RCC_PeriphCLKConfig(uint32_t pllsaim, uint32_t pllsan, uint32_t pllsaip)
{
	uint32_t tickstart = 0U;
	uint32_t pllsaiq = 0U;

    /*
	 * ����å�48����������
	 */
	sil_mdw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_DCKCFGR2), RCC_DCKCFGR2_CK48MSEL, RCC_CLK48CLKSOURCE_PLLSAIP);

    /*
	 *  PLLSAI����å������
	 */
	sil_wrw_mem((uint32_t *)RCC_CR_PLLSAION_BB, 0);

	tickstart = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLSAIRDY) != 0){
		if(tickstart  > (PLLSAI_TIMEOUT_VALUE*100)){
			return E_TMOUT;
		}
		sil_dly_nse(1000);
		tickstart++;
	}

    /*
	 *  PLLSAI����å�����
	 */
	pllsaiq = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLSAICFGR)) & RCC_PLLSAICFGR_PLLSAIQ;
	/* Configure the PLLSAI division factors */
	/* PLLSAI_VCO = f(VCO clock) = f(PLLSAI clock input) * (PLLI2SN/PLLSAIM) */
	/* 48CLK = f(PLLSAI clock output) = f(VCO clock) / PLLSAIP */
	pllsaiq |= (pllsaim | (pllsan << 6)  | (((pllsaip >> 1) -1) << 16));
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLSAICFGR), pllsaiq);

    /*
	 *  PLLSAI����å���Ƴ�
	 */
	sil_wrw_mem((uint32_t *)RCC_CR_PLLSAION_BB, 1);

	tickstart = 0;
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & (RCC_CR_PLLSAIRDY)) == 0){
		if( tickstart > (PLLSAI_TIMEOUT_VALUE*100)){
			return E_TMOUT;
		}
		sil_dly_nse(1000);
		tickstart++;
	}
	return E_OK;
}

/*
 *  �����ƥ९��å�����ե�����졼�����
 */
ER sysclock_config(int mode)
{
	unsigned int tickstart = 0;
	unsigned int tmp;

 	/*
	 *  �ѥ����å�ͭ����
	 */
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR), RCC_APB1ENR_PWREN);
	tmp = sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_APB1ENR));

	/*
	 *  VOLTAGE����
	 */
	sil_mdw_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR), (PWR_CR_VOS), PWR_REGULATOR_VOLTAGE_SCALE1);
	tmp = sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CR));

	/*
	 *  HSE�������
	 */
	sil_wrb_mem((uint8_t *)RCC_CR_BYTE2_ADDRESS, RCC_HSE_BYPASS);

	/*
	 *  HSE��ǥ��Ԥ�
	 */
	while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_HSERDY) == 0){
		if(tickstart > HSE_STARTUP_TIMEOUT){
			return E_TMOUT;
		}
		tickstart++;
		sil_dly_nse(1000*100);
	}

    /*
	 *  �����å����쥯�Ȥ�PLL�Ǥʤ���硢PLL����ߤ��ƺ�����
	 */
	if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){
        /*
	���� *  PLL���
		 */
		sil_wrw_mem((uint32_t *)(RCC_CR_PLLON_BB), 0);

        /*
		 *  PLL����Ԥ�
		 */
        tickstart = 0;
        while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) != 0){
			tickstart++;
			sil_dly_nse(100*1000);
			if(tickstart > PLL_TIMEOUT_VALUE){
				return E_TMOUT;
			}
        }

        /*
		 *  PLL����
		 */
		sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24) | (PLL_R << 28));
        /*
		 *  PLL�Ƴ�
		 */
		sil_wrw_mem((uint32_t *)(RCC_CR_PLLON_BB), 1);

        /*
		 *  PLL�����Ԥ�
		 */
		tickstart = 0;
        while((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) == 0){
			tickstart++;
			sil_dly_nse(100*1000);
			if( tickstart  > PLL_TIMEOUT_VALUE){
				return E_TMOUT;
			}
		}
	}

	/*
	 *  �����С��ɥ饤���ѥ���å�����
	 */
	sil_wrw_mem((uint32_t *)(CR_ODEN_BB), 1);

	tickstart = 0;
	while((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CSR)) & PWR_CSR_ODRDY) == 0){
		tickstart++;
		sil_dly_nse(100*1000);
		if( tickstart  > PWR_OVERDRIVE_TIMEOUT_VALUE){
			return E_TMOUT;
		}
	}

	/*
	 *  �����С��ɥ饤������
	 */
	sil_wrw_mem((uint32_t *)(CR_ODSWEN_BB), 1);

	tickstart = 0;
	while((sil_rew_mem((uint32_t *)(TADR_PWR_BASE+TOFF_PWR_CSR)) & PWR_CSR_ODSWRDY) == 0){
		tickstart++;
		sil_dly_nse(100*1000);
		if( tickstart > PWR_OVERDRIVE_TIMEOUT_VALUE){
			return E_TMOUT;
		}
	}

	/*
	 *  USB�Υ���å�����
	 */
	USB_RCC_PeriphCLKConfig(8, 384, 8);

	/*
	 *  FLASH�����5WS������
	 */
	sil_wrb_mem((uint8_t *)ACR_BYTE0_ADDRESS, FLASH_ACR_LATENCY_5WS);

	/*
	 *  FLASH���꤬ȿ�Ǥ���ʤ���Х��顼��λ
	 */
    if((sil_rew_mem((uint32_t *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR)) & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_5WS){
      return E_SYS;
    }

    /*
	 *  HCLK�Υ���å�����
	 */
	sil_mdw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), (RCC_CFGR_HPRE), RCC_CFGR_HPRE_DIV1);

	/*
	 *  PLL��READY�ˤʤ�ʤ���Х��顼
	 */
	if((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) == 0){
		return E_SYS;
	}

	sil_mdw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), (RCC_CFGR_SW), RCC_CFGR_SW_PLL);

    /*
	 *  �����å����쥯�Ȥ�PLL�ˤʤ�ޤ��Ԥ�
	 */
	tickstart = 0;
	while ((sil_rew_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){
		tickstart++;
		sil_dly_nse(100*1000);
		if( tickstart > CLOCKSWITCH_TIMEOUT_VALUE){
            return E_TMOUT;
		}
	}

	/*
	 *  PCLK1�Υ���ե�����졼�����
	 */
	sil_mdw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), (RCC_CFGR_PPRE1), RCC_CFGR_PPRE1_DIV4);

	/*
	 *  PCLK2�Υ���ե�����졼�����
	 */
	sil_mdw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), (RCC_CFGR_PPRE2), (RCC_CFGR_PPRE1_DIV2 << 3));
	return E_OK;
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
	sil_orw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_HSION);
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CFGR), 0x00000000);
	sil_anw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), (RCC_CR_PLLON | RCC_CR_CSSON | RCC_CR_HSEON));
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), RCC_CFGR_MCO2PRE_2 | RCC_CFGR_MCO1PRE_2
					 | RCC_CFGR_PPRE2_0 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_HPRE_0);

	/* Reset HSEBYP bit */
	sil_anw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_HSEBYP);

	/*
	 *  ������߶ػ�
	 */
	sil_wrw_mem((uint32_t *)(TADR_RCC_BASE+TOFF_RCC_CIR), 0x00000000);

	sysclock_config(0);
}
#endif

