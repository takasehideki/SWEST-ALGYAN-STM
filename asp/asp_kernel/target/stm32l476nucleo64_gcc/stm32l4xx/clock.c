/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2010 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2012-2015 by GT Development Center RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2015-2016 by TOPPERS PROJECT Educational Working Group.
 * 
 *  �嵭����Ԥϡ��ʲ��� (1)��(4) �ξ�狼��Free Software Foundation 
 *  �ˤ�äƸ�ɽ����Ƥ��� GNU General Public License �� Version 2 �˵�
 *  �Ҥ���Ƥ���������������˸¤ꡤ�ܥ��եȥ��������ܥ��եȥ�����
 *  ����Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ�������ѡ������ۡʰʲ���
 *  ���ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
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
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����Ŭ�Ѳ�ǽ����
 *  �ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ����������Ѥˤ��ľ
 *  ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤������Ǥ�����ʤ���
 * 
 *  @(#) $Id: clock.c,v 1.4 2016/05/07 08:10:07 roi Exp $
 */

#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include <stdio.h>
#define _SYS__STDINT_H
#include <time.h>
#include "kernel_cfg.h"
#include "device.h"
#include "rtc.h"
#include "monitor.h"

#define NUM_RTC_CMD     3

static int_t date_func(int argc, char **argv);
static int_t time_func(int argc, char **argv);
static int_t clock_func(int argc, char **argv);

/*
 *  �ңԣå��ޥ�ɥơ��֥�
 */
static const COMMAND_INFO rtc_command_info[] = {
	{"DATE",	date_func    },	/* ���ˤ����� */
	{"TIME",	time_func    },	/* �������� */
	{"CLOCK",	clock_func   }	/* ���� */
};

static const char rtc_name[] = "RTC";
static const char rtc_help[] =
"  Rtc     DATE  (year #) (month #) (day #)\n"
"          TIME  (hour #) (min   #) (sec #)\n"
"          CLOCK DATE:TIME\n";

static const char monthday[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static COMMAND_LINK rtc_command_link = {
	NULL,
	NUM_RTC_CMD,
	rtc_name,
	NULL,
	rtc_help,
	&rtc_command_info[0]
};

/*
 *  ������atox
 */
static int atoix(char *s)
{
	int c, val = 0;

	while((c = *s++) != 0){
		if(c >= '0' && c <= '9')
			val = val * 10 + (c - '0');
	}
	return val;
}

/*
 *  mktime�ؿ�
 */
time_t mktime(struct tm *ptm)
{
	int day = ptm->tm_year *365;
	int yday, i; 

	day += (ptm->tm_year+2) / 4;
	for(i = 1, yday = 0 ; i < ptm->tm_mon ; i++)
		yday += monthday[i-1];
	if(ptm->tm_mon > 2 && ((ptm->tm_year+2) % 4) == 0)
		yday++;
	yday += ptm->tm_mday - 1;
	day  += yday;
	ptm->tm_wday = (day+4) % 7;
	ptm->tm_yday = yday+1;
	syslog_3(LOG_INFO, "yday[%d] wday[%d] day[%d]", yday+1, ptm->tm_wday, ptm->tm_yday);
	return day * (24*60*60) + ptm->tm_hour * 60*60 + ptm->tm_min * 60 + ptm->tm_sec;
}

/*
 * gmtime_r�ؿ�
 */
struct tm *gmtime_r(const time_t *pt, struct tm *ptm)
{
	int day = *pt / (24*60*60);
	int time, i, yday, mday;

	time = *pt - day * 24 * 60 * 60;
	ptm->tm_wday = (day+4) % 7;
	for(i = 365, ptm->tm_year = 0 ; i < day ; i += 365){
		if(((ptm->tm_year+2) % 4) == 0)
			i++;
		ptm->tm_year++;
	}

	ptm->tm_yday = yday = day - i + 365 + 1;
	for(ptm->tm_mon = 1 ; ptm->tm_mon <= 12 && yday > 0 ; ptm->tm_mon++){
		mday = monthday[ptm->tm_mon-1];
		if(ptm->tm_mon == 2 && ((ptm->tm_year+2) % 4) == 0)
			mday++;
		if(yday < mday){
			ptm->tm_mday = yday;
			break;
		}
		yday -= mday;
	}
	ptm->tm_hour = time / (60*60);
	time -= ptm->tm_hour * 60 * 60;
	ptm->tm_min  = time / 60;
	ptm->tm_sec = time & 60;
	return ptm;
}

/*
 *  RTC���ޥ������ؿ�
 */
void rtc_info_init(intptr_t exinf)
{
	setup_command(&rtc_command_link);
}

/*
 *  �������ꥳ�ޥ�ɴؿ�
 */
static int_t date_func(int argc, char **argv)
{
	struct tm timedate;
	int arg1 = 2016;

	rtc_get_time((struct tm2 *)&timedate);
	if(argc > 1)
		arg1 = atoix(argv[1]);
	if(argc > 2)
		timedate.tm_mon = atoix(argv[2]);
	else
		timedate.tm_mon = 1;
	if(argc > 3)
		timedate.tm_mday = atoix(argv[3]);
	else
		timedate.tm_mday = 1;
	printf("%04d/%02d/%02d\n", arg1, timedate.tm_mon, timedate.tm_mday);
	timedate.tm_year = arg1-1970;
	timedate.tm_isdst = 0;
	mktime((struct tm *)&timedate);
	rtc_set_time((struct tm2 *)&timedate);
	return 0;
}

/*
 *  �������ꥳ�ޥ�ɴؿ�
 */
static int_t time_func(int argc, char **argv)
{
	struct tm2 timedate;

	rtc_get_time(&timedate);
	if(argc > 1)
		timedate.tm_hour = atoix(argv[1]);
	else
		timedate.tm_hour = 0;
	if(argc > 2)
		timedate.tm_min = atoix(argv[2]);
	else
		timedate.tm_min = 0;
	if(argc > 3)
		timedate.tm_sec = atoix(argv[3]);
	else
		timedate.tm_sec = 0;
	printf("%02d:%02d:%02d\n", timedate.tm_hour, timedate.tm_min, timedate.tm_sec);
	timedate.tm_isdst = 0;
	mktime((struct tm *)&timedate);
	rtc_set_time(&timedate);
	return 0;
}


/*
 *  �����Ф����ޥ�ɴؿ�
 */
static int_t clock_func(int argc, char **argv)
{
	struct tm2 timedate;

	rtc_get_time(&timedate);
	mktime(&timedate);
	printf("        %04d/%02d/%02d\n", timedate.tm_year+1970, timedate.tm_mon, timedate.tm_mday);
	printf("y[%d][%d] %02d:%02d:%02d\n", timedate.tm_yday, timedate.tm_wday, timedate.tm_hour, timedate.tm_min, timedate.tm_sec);
	return 0;
}

