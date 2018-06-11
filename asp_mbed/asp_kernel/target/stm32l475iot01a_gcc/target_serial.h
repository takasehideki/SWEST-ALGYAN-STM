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
 *  �嵭����Ԥϡ��ʲ���(1)���(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
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
 *  @(#) $Id: target_serial.h 698 2017-07-27 08:03:59Z roi $
 */

/*
 *    ���ꥢ��I/O�ǥХ�����SIO�˥ɥ饤��
 */

#ifndef TOPPERS_TARGET_SERIAL_H
#define TOPPERS_TARGET_SERIAL_H

/*
 *  ���åװ�¸�⥸�塼���stm32l476-nucleo64�ѡ�
 */

/*
 *  SIO��GPIO������
 */
#define TADR_U1_GPIO_BASE  TADR_GPIOA_BASE
#define TOFF_U1_APBNER     TOFF_RCC_APB1ENR1
#define ENABLE_U1_PORT     RCC_APB1ENR1_USART2EN
#define TOFF_U1_APBRSTR    TOFF_RCC_APB1RSTR1
#define RESET_U1_BIT       RCC_APB1RSTR1_USART2RST
#define ENABLE_U1_GPIO     RCC_AHB2ENR_GPIOAEN
#define TADR_U2_GPIO_BASE  TADR_GPIOA_BASE
#define TOFF_U2_APBNER     TOFF_RCC_APB2ENR
#define ENABLE_U2_PORT     RCC_APB2ENR_USART1EN
#define TOFF_U2_APBRSTR    TOFF_RCC_APB2RSTR
#define RESET_U2_BIT       RCC_APB2RSTR_USART1RST
#define ENABLE_U2_GPIO     RCC_AHB2ENR_GPIOAEN
#define TX1_PINPOS         2
#define RX1_PINPOS         3
#define TX2_PINPOS         9
#define RX2_PINPOS         10

#define GPIO_U1_AF         0x07U		/* AF7: USART2 Alternate Function mapping */
#define GPIO_U2_AF         0x07U		/* AF7: USART1 Alternate Function mapping */
#define U1_GPIOSPEED       GPIO_OSPEEDER_OSPEEDR2
#define U2_GPIOSPEED       GPIO_OSPEEDER_OSPEEDR2
#define U1_SRCINDEX        2
#define U2_SRCINDEX        0

/*
 *  SIO�Υ١������ɥ쥹
 */
#define USART1_BASE        TADR_USART2_BASE
#define USART2_BASE        TADR_USART1_BASE

/*
 *  ���ꥢ��I/O�ݡ��ȿ������
 */
#define TNUM_SIOP       2			/* ���ݡ��Ȥ��륷�ꥢ��I/O�ݡ��Ȥο� */

/*
 *  SIO�γ���ߥϥ�ɥ�Υ٥����ֹ�
 */
#define INHNO_SIO1      IRQ_VECTOR_USART2
#define INTNO_SIO1      IRQ_VECTOR_USART2
#define INHNO_SIO2      IRQ_VECTOR_USART1
#define INTNO_SIO2      IRQ_VECTOR_USART1

#define INTPRI_SIO       -3        /* �����ͥ���� */
#define INTATR_SIO       0         /* �����°�� */


#include "arm_m_gcc/stm32l4xx/chip_serial.h"

#endif /* TOPPERS_TARGET_SERIAL_H */
