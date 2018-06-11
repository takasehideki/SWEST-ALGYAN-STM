/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2015 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
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
 *  @(#) $Id$
 */

/*
 *		����߽�����ǥ��ARMv6-M�ѡ�
 *
 *  ���Υ��󥯥롼�ɥե�����ϡ�core_config.h�ʤޤ��ϡ��������饤��
 *  �롼�ɤ����ե�����ˤΤߤ��饤�󥯥롼�ɤ���롥¾�Υե����뤫��
 *  ľ�ܥ��󥯥롼�ɤ��ƤϤʤ�ʤ���
 */

#ifndef TOPPERS_CORE_INTMODEL_V6M_H
#define TOPPERS_CORE_INTMODEL_V6M_H

/*
 *  �������åȰ�¸�Υ��֥�������°��
 */
#define TARGET_INHATR  TA_NONKERNEL /* �������å�����γ���ߥϥ�ɥ�°�� */

/*
 *  �����ͥ���٥ޥ����γ���ɽ��������ɽ�����Ѵ�
 *
 *  ������֥����Υ������ե����뤫�饤�󥯥롼�ɤ�����Τ���ˡ�
 *  CAST�����
 *   ����ɽ�� : TMIN_INTPRI  ��       0  
 *   ����ɽ�� :      0       ��  -TMIN_INTPRI
 */
#define EXT_IPM(iipm)   (CAST(PRI,iipm + TMIN_INTPRI))     /* ����ɽ������ɽ���� */
#define INT_IPM(ipm)    (CAST(uint8_t, ipm - TMIN_INTPRI)) /* ����ɽ��������ɽ���� */

/*
 *  �����ͥ���٥ޥ�����NVIC��ͥ���٤��Ѵ�
 */
#define INT_NVIC_PRI(ipm)    (((1 << TBITW_IPRI) - CAST(uint8_t, -(ipm)))  << (8 - TBITW_IPRI))

/*
 *  TIPM_ENAALL�ʳ����ͥ���٥ޥ���������ˤ�����ɽ��
 *
 */
#define IIPM_ENAALL  (-TMIN_INTPRI)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ������׵�ػߥե饰�μ¸��Τ�����ѿ�
 */
extern uint32_t ief;			/* IRQ�γ�����׵���ĥե饰�ξ��� */
extern uint8_t  ief_systick;	/* SysTick�γ�����׵���ĥե饰�ξ��� */

/*
 *  �����ͥ���٥ޥ����¸��Τ�����ѿ�
 */
extern uint8_t iipm;		/* ���ߤγ����ͥ���٥ޥ������� */ 

/*
 *  �����ͥ���٥ޥ����¸��Τ�����ѿ���kernel_cfg.c��
 */
extern const uint32_t iipm_enable_irq_tbl[];
extern const uint8_t iipm_enable_systic_tbl[];

/*
 *  CPU��å����֤ؤΰܹ�
 *
 */
Inline void
x_lock_cpu(void)
{
	set_primask();
	/* ����ƥ����륻������������ǥ��꤬�񤭴�����ǽ�������� */
	ARM_MEMORY_CHANGED;    
}

#define t_lock_cpu()    x_lock_cpu()
#define i_lock_cpu()    x_lock_cpu()

/*
 *  CPU��å����֤β��
 *
 */
Inline void
x_unlock_cpu(void)
{
	/* ����ƥ����륻������������ǥ��꤬�񤭴�����ǽ�������� */
	ARM_MEMORY_CHANGED;
	clear_primask();
}

#define t_unlock_cpu()    x_unlock_cpu()
#define i_unlock_cpu()    x_unlock_cpu()

/*
 *  CPU��å����֤λ���
 */
Inline bool_t
x_sense_lock(void)
{
	return(read_primask() == 0x1u);
}

#define t_sense_lock()    x_sense_lock()
#define i_sense_lock()    x_sense_lock()

/*
 *  chg_ipm��ͭ���ʳ����ͥ���٤��ϰϤ�Ƚ��
 *
 *  TMIN_INTPRI���ͤˤ�餺��chg_ipm�Ǥϡ�-(1 << TBITW_IPRI)��TIPM_ENAALL�ʡ�0��
 *  ���ϰϤ�����Ǥ��뤳�ȤȤ���ʥ������å�����γ�ĥ�ˡ�
 *  �����ͥ���٤Υӥå���(TBITW_IPRI)�� 2 �ξ��ϡ�-4 �� 0 �������ǽ�Ǥ��롥
 *   
 */
#define VALID_INTPRI_CHGIPM(intpri) \
				((-((1 << TBITW_IPRI) - 1) <= (intpri) && (intpri) <= TIPM_ENAALL))

/*
 * �ʥ�ǥ��Ρ˳����ͥ���٥ޥ���������
 *
 */
Inline void
x_set_ipm(PRI intpri)
{
	uint32_t tmp;
	iipm = INT_IPM(intpri);

	tmp = sil_rew_mem((void *)SYSTIC_CONTROL_STATUS);
	if ((iipm_enable_systic_tbl[iipm] & ief_systick) ==  0x01) {
		tmp |= SYSTIC_TICINT;
	}else{
		tmp &= ~SYSTIC_TICINT;
	}
	sil_wrw_mem((void *)SYSTIC_CONTROL_STATUS, tmp);

	/* ��ö������߶ػ� */
	sil_wrw_mem((void *)NVIC_CLRENA0, 0xffffffff);
	sil_wrw_mem((void *)NVIC_SETENA0, (iipm_enable_systic_tbl[iipm] & ief));
}

#define t_set_ipm(intpri)    x_set_ipm(intpri)
#define i_set_ipm(intpri)    x_set_ipm(intpri)

/*
 * �ʥ�ǥ��Ρ˳����ͥ���٥ޥ����λ���
 *
 */
Inline PRI
x_get_ipm(void)
{
	return EXT_IPM(iipm);
}

#define t_get_ipm()    x_get_ipm()
#define i_get_ipm()    x_get_ipm()

/*
 *  ������׵�ػߥե饰
 */

/*
 *  �����°�������ꤵ��Ƥ��뤫��Ƚ�̤��뤿����ѿ���kernel_cfg.c��
 */
extern const uint32_t	bitpat_cfgint[];

/*
 *  ������׵�ػߥե饰�Υ��å�
 *
 *  �����°�������ꤵ��Ƥ��ʤ�������׵�饤����Ф��Ƴ�����׵�ػ�
 *  �ե饰�򥯥ꥢ���褦�Ȥ������ˤϡ�false���֤���  
 */
Inline bool_t
x_disable_int(INTNO intno)
{
	uint32_t tmp;

	/*
	 *  �����°�������ꤵ��Ƥ��ʤ����
	 */
	if ((bitpat_cfgint[intno >> 5] & (1 << (intno & 0x1f))) == 0x00) {
		return(false);
	}

	if (intno == IRQNO_SYSTICK) {
		tmp = sil_rew_mem((void *)SYSTIC_CONTROL_STATUS);
		tmp &= ~SYSTIC_TICINT;
		sil_wrw_mem((void *)SYSTIC_CONTROL_STATUS, tmp);
		ief_systick &= ~0x01;
	}else {
		tmp = intno - 16;
		sil_wrw_mem((void *)(NVIC_CLRENA0), (1 << (tmp & 0x1f)));
		ief &= ~(1 << (tmp & 0x1f));
	}

	return(true);
}

#define t_disable_int(intno) x_disable_int(intno)
#define i_disable_int(intno) x_disable_int(intno)

/*
 *  ������׵�ػߥե饰�β��
 *
 *  �����°�������ꤵ��Ƥ��ʤ�������׵�饤����Ф��Ƴ�����׵�ػ�
 *  �ե饰�򥯥ꥢ���褦�Ȥ������ˤϡ�false���֤���
 */
Inline bool_t
x_enable_int(INTNO intno)
{
	uint32_t tmp;

	/*
	 *  �����°�������ꤵ��Ƥ��ʤ����
	 */
	if ((bitpat_cfgint[intno >> 5] & (1 << (intno & 0x1f))) == 0x00) {
		return(false);
	}

	if (intno == IRQNO_SYSTICK) {
		ief_systick |= 0x01;
		if ((iipm_enable_systic_tbl[iipm] & ief_systick) ==  0x01) {
			tmp = sil_rew_mem((void *)SYSTIC_CONTROL_STATUS);
			tmp |= SYSTIC_TICINT;;
			sil_wrw_mem((void *)SYSTIC_CONTROL_STATUS, tmp);
		}
	}else {
		tmp = intno - 16;
		ief |= (1 << (tmp & 0x1f));
		if ((iipm_enable_irq_tbl[iipm] & (1 << (tmp & 0x1f))) != 0) {
			sil_wrw_mem((void *)(NVIC_SETENA0), (1 << (tmp & 0x1f)));
		}
	}

	return(true);
}

#define t_enable_int(intno) x_enable_int(intno)
#define i_enable_int(intno) x_enable_int(intno)

/*
 *  PendSVC�ϥ�ɥ��core_support.S��
 */
extern void pendsvc_handler(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_CORE_INTMODEL_V6M_H */
