/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2016 by 3rd Designing Center
 *              Imageing System Development Division RICOH COMPANY, LTD.
 *  Copyright (C) 2016-2017 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: target_config.h 698 2017-08-06 15:30:34Z roi $
 */

/*
 *  ���åװ�¸�⥸�塼���stm32l476-discovery�ѡ�
 *
 *  �����ͥ�Υ������åȰ�¸���Υ��󥯥롼�ɥե����롥kernel_impl.h�Υ���
 *  ���åȰ�¸���ΰ����դ��Ȥʤ롥
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *  �������åȰ�¸���Υϡ��ɥ������񸻤����
 */
#include "stm32l4xx.h"

/*
 *  ���������Ԥ��Τ��������������SIL�Υ������åȰ�¸����
 */
#define SIL_DLY_TIM1    50
#ifdef TOPPERS_RAM_EXEC
#define SIL_DLY_TIM2    70
#else
#define SIL_DLY_TIM2    48
#endif

/*
 *  �ȥ졼�����˴ؤ�������
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ��ư�����ƥ९��å��μ���
 */
extern uint32_t sysclock_init_value(void);

/*
 *  �����ƥ९��å����ѹ�
 */
extern ER sysclock_change(uint8_t range);

#endif /* TOPPERS_MACRO_ONLY */


/*
 *  ���åװ�¸�⥸�塼���stm32l4xx�ѡ�
 */
#include "arm_m_gcc/stm32l4xx/chip_config.h"

#endif /* TOPPERS_TARGET_CONFIG_H */
