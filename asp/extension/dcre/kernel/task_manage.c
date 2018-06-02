/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2017 by Embedded and Real-Time Systems Laboratory
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
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  $Id: task_manage.c 2802 2017-03-26 06:24:56Z ertl-hiro $
 */

/*
 *		������������ǽ
 */

#include "kernel_impl.h"
#include "check.h"
#include "task.h"
#include "wait.h"

/*
 *  �ȥ졼�������ޥ����Υǥե�������
 */
#ifndef LOG_ACRE_TSK_ENTER
#define LOG_ACRE_TSK_ENTER(pk_ctsk)
#endif /* LOG_ACRE_TSK_ENTER */

#ifndef LOG_ACRE_TSK_LEAVE
#define LOG_ACRE_TSK_LEAVE(ercd)
#endif /* LOG_ACRE_TSK_LEAVE */

#ifndef LOG_DEL_TSK_ENTER
#define LOG_DEL_TSK_ENTER(tskid)
#endif /* LOG_DEL_TSK_ENTER */

#ifndef LOG_DEL_TSK_LEAVE
#define LOG_DEL_TSK_LEAVE(ercd)
#endif /* LOG_DEL_TSK_LEAVE */

#ifndef LOG_ACT_TSK_ENTER
#define LOG_ACT_TSK_ENTER(tskid)
#endif /* LOG_ACT_TSK_ENTER */

#ifndef LOG_ACT_TSK_LEAVE
#define LOG_ACT_TSK_LEAVE(ercd)
#endif /* LOG_ACT_TSK_LEAVE */

#ifndef LOG_IACT_TSK_ENTER
#define LOG_IACT_TSK_ENTER(tskid)
#endif /* LOG_IACT_TSK_ENTER */

#ifndef LOG_IACT_TSK_LEAVE
#define LOG_IACT_TSK_LEAVE(ercd)
#endif /* LOG_IACT_TSK_LEAVE */

#ifndef LOG_CAN_ACT_ENTER
#define LOG_CAN_ACT_ENTER(tskid)
#endif /* LOG_CAN_ACT_ENTER */

#ifndef LOG_CAN_ACT_LEAVE
#define LOG_CAN_ACT_LEAVE(ercd)
#endif /* LOG_CAN_ACT_LEAVE */

#ifndef LOG_EXT_TSK_ENTER
#define LOG_EXT_TSK_ENTER()
#endif /* LOG_EXT_TSK_ENTER */

#ifndef LOG_EXT_TSK_LEAVE
#define LOG_EXT_TSK_LEAVE(ercd)
#endif /* LOG_EXT_TSK_LEAVE */

#ifndef LOG_TER_TSK_ENTER
#define LOG_TER_TSK_ENTER(tskid)
#endif /* LOG_TER_TSK_ENTER */

#ifndef LOG_TER_TSK_LEAVE
#define LOG_TER_TSK_LEAVE(ercd)
#endif /* LOG_TER_TSK_LEAVE */

#ifndef LOG_CHG_PRI_ENTER
#define LOG_CHG_PRI_ENTER(tskid, tskpri)
#endif /* LOG_CHG_PRI_ENTER */

#ifndef LOG_CHG_PRI_LEAVE
#define LOG_CHG_PRI_LEAVE(ercd)
#endif /* LOG_CHG_PRI_LEAVE */

#ifndef LOG_GET_PRI_ENTER
#define LOG_GET_PRI_ENTER(tskid, p_tskpri)
#endif /* LOG_GET_PRI_ENTER */

#ifndef LOG_GET_PRI_LEAVE
#define LOG_GET_PRI_LEAVE(ercd, tskpri)
#endif /* LOG_GET_PRI_LEAVE */

#ifndef LOG_GET_INF_ENTER
#define LOG_GET_INF_ENTER(p_exinf)
#endif /* LOG_GET_INF_ENTER */

#ifndef LOG_GET_INF_LEAVE
#define LOG_GET_INF_LEAVE(ercd, exinf)
#endif /* LOG_GET_INF_LEAVE */

/*
 *  ������������
 */
#ifdef TOPPERS_acre_tsk

#ifndef TARGET_MIN_STKSZ
#define TARGET_MIN_STKSZ	1U		/* ̤����ξ���0�Ǥʤ����Ȥ�����å� */
#endif /* TARGET_MIN_STKSZ */

ER_UINT
acre_tsk(const T_CTSK *pk_ctsk)
{
	TCB		*p_tcb;
	TINIB	*p_tinib;
	ATR		tskatr;
	SIZE	stksz;
	void	*stk;
	ER		ercd;

	LOG_ACRE_TSK_ENTER(pk_ctsk);
	CHECK_TSKCTX_UNL();
	CHECK_RSATR(pk_ctsk->tskatr, TA_ACT|TARGET_TSKATR);
	CHECK_ALIGN_FUNC(pk_ctsk->task);
	CHECK_NONNULL_FUNC(pk_ctsk->task);
	CHECK_TPRI(pk_ctsk->itskpri);
	CHECK_PAR(pk_ctsk->stksz >= TARGET_MIN_STKSZ);
	if (pk_ctsk->stk != NULL) {
		CHECK_ALIGN_STKSZ(pk_ctsk->stksz);
		CHECK_ALIGN_STACK(pk_ctsk->stk);
	}
	tskatr = pk_ctsk->tskatr;
	stksz = pk_ctsk->stksz;
	stk = pk_ctsk->stk;

	t_lock_cpu();
	if (queue_empty(&free_tcb)) {
		ercd = E_NOID;
	}
	else {
		if (stk == NULL) {
			stksz = ROUND_STK_T(stksz);
			stk = kernel_malloc(stksz);
			tskatr |= TA_MEMALLOC;
		}
		if (stk == NULL) {
			ercd = E_NOMEM;
		}
		else {
			p_tcb = ((TCB *) queue_delete_next(&free_tcb));
			p_tinib = (TINIB *)(p_tcb->p_tinib);
			p_tinib->tskatr = tskatr;
			p_tinib->exinf = pk_ctsk->exinf;
			p_tinib->task = pk_ctsk->task;
			p_tinib->ipriority = INT_PRIORITY(pk_ctsk->itskpri);
#ifdef USE_TSKINICTXB
			init_tskinictxb(&(p_tinib->tskinictxb), stk, pk_ctsk);
#else /* USE_TSKINICTXB */
			p_tinib->stksz = stksz;
			p_tinib->stk = stk;
#endif /* USE_TSKINICTXB */
			p_tinib->texatr = TA_NULL;
			p_tinib->texrtn = NULL;

			p_tcb->actque = false;
			make_dormant(p_tcb);
			if ((p_tcb->p_tinib->tskatr & TA_ACT) != 0U) {
				if (make_active(p_tcb)) {
					dispatch();
				}
			}
			ercd = TSKID(p_tcb);
		}
	}
	t_unlock_cpu();

  error_exit:
	LOG_ACRE_TSK_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_acre_tsk */

/*
 *  �������κ��
 */
#ifdef TOPPERS_del_tsk

ER
del_tsk(ID tskid)
{
	TCB		*p_tcb;
	TINIB	*p_tinib;
	ER		ercd;

	LOG_DEL_TSK_ENTER(tskid);
	CHECK_TSKCTX_UNL();
	CHECK_TSKID(tskid);
	p_tcb = get_tcb(tskid);

	t_lock_cpu();
	if (p_tcb->p_tinib->tskatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (TSKID(p_tcb) > tmax_stskid && TSTAT_DORMANT(p_tcb->tstat)) {
		p_tinib = (TINIB *)(p_tcb->p_tinib);
#ifdef USE_TSKINICTXB
		term_tskinictxb(&(p_tinib->tskinictxb));
#else /* USE_TSKINICTXB */
		if ((p_tinib->tskatr & TA_MEMALLOC) != 0U) {
			kernel_free(p_tinib->stk);
		}
#endif /* USE_TSKINICTXB */
		p_tinib->tskatr = TA_NOEXS;
		queue_insert_prev(&free_tcb, &(p_tcb->task_queue));
		ercd = E_OK;
	}
	else {
		ercd = E_OBJ;
	}
	t_unlock_cpu();

  error_exit:
	LOG_DEL_TSK_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_del_tsk */

/*
 *  �������ε�ư
 */
#ifdef TOPPERS_act_tsk

ER
act_tsk(ID tskid)
{
	TCB		*p_tcb;
	ER		ercd;

	LOG_ACT_TSK_ENTER(tskid);
	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	p_tcb = get_tcb_self(tskid);

	t_lock_cpu();
	if (p_tcb->p_tinib->tskatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (TSTAT_DORMANT(p_tcb->tstat)) {
		if (make_active(p_tcb)) {
			dispatch();
		}
		ercd = E_OK;
	}
	else if (!(p_tcb->actque)) {
		p_tcb->actque = true;
		ercd = E_OK;
	}
	else {
		ercd = E_QOVR;
	}
	t_unlock_cpu();

  error_exit:
	LOG_ACT_TSK_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_act_tsk */

/*
 *  �������ε�ư���󥿥�������ƥ������ѡ�
 */
#ifdef TOPPERS_iact_tsk

ER
iact_tsk(ID tskid)
{
	TCB		*p_tcb;
	ER		ercd;

	LOG_IACT_TSK_ENTER(tskid);
	CHECK_INTCTX_UNL();
	CHECK_TSKID(tskid);
	p_tcb = get_tcb(tskid);

	i_lock_cpu();
	if (p_tcb->p_tinib->tskatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (TSTAT_DORMANT(p_tcb->tstat)) {
		if (make_active(p_tcb)) {
			reqflg = true;
		}
		ercd = E_OK;
	}
	else if (!(p_tcb->actque)) {
		p_tcb->actque = true;
		ercd = E_OK;
	}
	else {
		ercd = E_QOVR;
	}
	i_unlock_cpu();

  error_exit:
	LOG_IACT_TSK_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_iact_tsk */

/*
 *  ��������ư�׵�Υ���󥻥�
 */
#ifdef TOPPERS_can_act

ER_UINT
can_act(ID tskid)
{
	TCB		*p_tcb;
	ER_UINT	ercd;

	LOG_CAN_ACT_ENTER(tskid);
	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	p_tcb = get_tcb_self(tskid);

	t_lock_cpu();
	if (p_tcb->p_tinib->tskatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else {
		ercd = p_tcb->actque ? 1 : 0;
		p_tcb->actque = false;
	}
	t_unlock_cpu();

  error_exit:
	LOG_CAN_ACT_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_can_act */

/*
 *  ���������ν�λ
 */
#ifdef TOPPERS_ext_tsk

ER
ext_tsk(void)
{
	ER		ercd;

	LOG_EXT_TSK_ENTER();
	CHECK_TSKCTX();

	if (t_sense_lock()) {
		/*
		 *  CPU���å����֤�ext_tsk���ƤФ줿���ϡ�CPU���å�������
		 *  �Ƥ��饿������λ���롥������ϡ������ӥ���������Ǥ�CPU
		 *  ���å����ά����Ф褤������
		 */
	}
	else {
		t_lock_cpu();
	}
	if (disdsp) {
		/*
		 *  �ǥ����ѥå��ػ߾��֤�ext_tsk���ƤФ줿���ϡ��ǥ����ѥ�
		 *  �����ľ��֤ˤ��Ƥ��饿������λ���롥
		 */
		disdsp = false;
	}
	if (!ipmflg) {
		/*
		 *  �����ͥ���٥ޥ�����IPM�ˤ�TIPM_ENAALL�ʳ��ξ��֤�ext_tsk
		 *  ���ƤФ줿���ϡ�IPM��TIPM_ENAALL�ˤ��Ƥ��饿������λ��
		 *  �롥
		 */
		t_set_ipm(TIPM_ENAALL);
		ipmflg = true;
	}
	dspflg = true;

	(void) make_non_runnable(p_runtsk);
	make_dormant(p_runtsk);
	if (p_runtsk->actque) {
		p_runtsk->actque = false;
		(void) make_active(p_runtsk);
	}
	exit_and_dispatch();
	ercd = E_SYS;

  error_exit:
	LOG_EXT_TSK_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_ext_tsk */

/*
 *  �������ζ�����λ
 */
#ifdef TOPPERS_ter_tsk

ER
ter_tsk(ID tskid)
{
	TCB		*p_tcb;
	ER		ercd;

	LOG_TER_TSK_ENTER(tskid);
	CHECK_TSKCTX_UNL();
	CHECK_TSKID(tskid);
	p_tcb = get_tcb(tskid);
	CHECK_NONSELF(p_tcb);

	t_lock_cpu();
	if (p_tcb->p_tinib->tskatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (TSTAT_DORMANT(p_tcb->tstat)) {
		ercd = E_OBJ;
	}
	else {
		if (TSTAT_RUNNABLE(p_tcb->tstat)) {
			/*
			 *  p_tcb�ϼ��������Ǥʤ����ᡤ�ʥ��󥰥�ץ����å��Ǥϡ˼�
			 *  �Ծ��֤Ǥʤ���make_non_runnable(p_tcb)�ǥ������ǥ����ѥ�
			 *  ����ɬ�פˤʤ뤳�ȤϤʤ���
			 */
			(void) make_non_runnable(p_tcb);
		}
		else if (TSTAT_WAITING(p_tcb->tstat)) {
			wait_dequeue_wobj(p_tcb);
			wait_dequeue_tmevtb(p_tcb);
		}
		make_dormant(p_tcb);
		if (p_tcb->actque) {
			p_tcb->actque = false;
			if (make_active(p_tcb)) {
				dispatch();
			}
		}
		ercd = E_OK;
	}
	t_unlock_cpu();

  error_exit:
	LOG_TER_TSK_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_ter_tsk */

/*
 *  �������Υ١���ͥ���٤��ѹ�
 */
#ifdef TOPPERS_chg_pri

ER
chg_pri(ID tskid, PRI tskpri)
{
	TCB		*p_tcb;
	uint_t	newpri;
	ER		ercd;

	LOG_CHG_PRI_ENTER(tskid, tskpri);
	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	CHECK_TPRI_INI(tskpri);
	p_tcb = get_tcb_self(tskid);

	t_lock_cpu();
	if (p_tcb->p_tinib->tskatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (TSTAT_DORMANT(p_tcb->tstat)) {
		ercd = E_OBJ;
	}
	else {
		newpri = (tskpri == TPRI_INI) ? p_tcb->p_tinib->ipriority
											: INT_PRIORITY(tskpri);
		if (change_priority(p_tcb, newpri)) {
			dispatch();
		}
		ercd = E_OK;
	}
	t_unlock_cpu();

  error_exit:
	LOG_CHG_PRI_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_chg_pri */

/*
 *  ������ͥ���٤λ���
 */
#ifdef TOPPERS_get_pri

ER
get_pri(ID tskid, PRI *p_tskpri)
{
	TCB		*p_tcb;
	ER		ercd;

	LOG_GET_PRI_ENTER(tskid, p_tskpri);
	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	p_tcb = get_tcb_self(tskid);

	t_lock_cpu();
	if (p_tcb->p_tinib->tskatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (TSTAT_DORMANT(p_tcb->tstat)) {
		ercd = E_OBJ;
	}
	else {
		*p_tskpri = EXT_TSKPRI(p_tcb->priority);
		ercd = E_OK;
	}
	t_unlock_cpu();

  error_exit:
	LOG_GET_PRI_LEAVE(ercd, *p_tskpri);
	return(ercd);
}

#endif /* TOPPERS_get_pri */

/*
 *  ���������γ�ĥ����λ���
 */
#ifdef TOPPERS_get_inf

ER
get_inf(intptr_t *p_exinf)
{
	ER		ercd;

	LOG_GET_INF_ENTER(p_exinf);
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	*p_exinf = p_runtsk->p_tinib->exinf;
	ercd = E_OK;
	t_unlock_cpu();

  error_exit:
	LOG_GET_INF_LEAVE(ercd, *p_exinf);
	return(ercd);
}

#endif /* TOPPERS_get_inf */