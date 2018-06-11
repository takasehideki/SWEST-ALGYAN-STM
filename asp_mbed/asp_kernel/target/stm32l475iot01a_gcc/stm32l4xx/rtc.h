/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2017 by TOPPERS PROJECT Educational Working Group.
 * 
 *  ��L���쌠�҂́C�ȉ���(1)�`(4)�̏����𖞂����ꍇ�Ɍ���C�{�\�t�g�E�F
 *  �A�i�{�\�t�g�E�F�A�����ς������̂��܂ށD�ȉ������j���g�p�E�����E��
 *  �ρE�Ĕz�z�i�ȉ��C���p�ƌĂԁj���邱�Ƃ𖳏��ŋ�������D
 *  (1) �{�\�t�g�E�F�A���\�[�X�R�[�h�̌`�ŗ��p����ꍇ�ɂ́C��L�̒���
 *      ���\���C���̗��p��������щ��L�̖��ۏ؋K�肪�C���̂܂܂̌`�Ń\�[
 *      �X�R�[�h���Ɋ܂܂�Ă��邱�ƁD
 *  (2) �{�\�t�g�E�F�A���C���C�u�����`���ȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł���`�ōĔz�z����ꍇ�ɂ́C�Ĕz�z�ɔ����h�L�������g�i���p
 *      �҃}�j���A���Ȃǁj�ɁC��L�̒��쌠�\���C���̗��p��������щ��L
 *      �̖��ۏ؋K����f�ڂ��邱�ƁD
 *  (3) �{�\�t�g�E�F�A���C�@��ɑg�ݍ��ނȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł��Ȃ��`�ōĔz�z����ꍇ�ɂ́C���̂����ꂩ�̏����𖞂�����
 *      �ƁD
 *    (a) �Ĕz�z�ɔ����h�L�������g�i���p�҃}�j���A���Ȃǁj�ɁC��L�̒�
 *        �쌠�\���C���̗��p��������щ��L�̖��ۏ؋K����f�ڂ��邱�ƁD
 *    (b) �Ĕz�z�̌`�Ԃ��C�ʂɒ�߂���@�ɂ���āCTOPPERS�v���W�F�N�g��
 *        �񍐂��邱�ƁD
 *  (4) �{�\�t�g�E�F�A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����邢���Ȃ鑹
 *      �Q������C��L���쌠�҂����TOPPERS�v���W�F�N�g��Ɛӂ��邱�ƁD
 *      �܂��C�{�\�t�g�E�F�A�̃��[�U�܂��̓G���h���[�U����̂����Ȃ闝
 *      �R�Ɋ�Â�����������C��L���쌠�҂����TOPPERS�v���W�F�N�g��
 *      �Ɛӂ��邱�ƁD
 * 
 *  �{�\�t�g�E�F�A�́C���ۏ؂Œ񋟂���Ă�����̂ł���D��L���쌠�҂�
 *  ���TOPPERS�v���W�F�N�g�́C�{�\�t�g�E�F�A�Ɋւ��āC����̎g�p�ړI
 *  �ɑ΂���K�������܂߂āC�����Ȃ�ۏ؂��s��Ȃ��D�܂��C�{�\�t�g�E�F
 *  �A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����������Ȃ鑹�Q�Ɋւ��Ă��C��
 *  �̐ӔC�𕉂�Ȃ��D
 * 
 *  @(#) $Id: rtc.h 698 2017-07-27 17:12:38Z roi $
 */
/*
 * 
 * RTC�p�f�o�C�X�h���C�o�p�f�o�C�X����֐��Q�̊O���錾
 *
 */

#ifndef _RTC_H_
#define _RTC_H_

/*
 *  RTC�A���[����`
 */
#define RTC_ALARM_A     RTC_CR_ALRAE
#define RTC_ALARM_B     RTC_CR_ALRBE

/*
 *  RTC���t�A�E�B�[�N�f�B��`
 */
#define ALARMDAYSEL_DATE    0x00000000
#define ALARMDAYSEL_WEEKDAY 0x40000000

/*
 *  RTC�}�X�N��`
 */
#define ALARMMASK_NONE      0x00000000
#define ALARMMASK_DATESEL   RTC_ALRMAR_MSK4
#define ALARMMASK_HOURS     RTC_ALRMAR_MSK3
#define ALARMMASK_MINUTES   RTC_ALRMAR_MSK2
#define ALARMMASK_SECONDS   RTC_ALRMAR_MSK1
#define ALARMMASK_ALL       (RTC_ALRMAR_MSK1 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK4)

/*
 *  RTC�T�u�Z�J���h�A���[���}�b�`��`
 */
#define ALARMSSMASK_ALL     0x00000000	/* All Alarm SS fields are masked. */
#define ALARMSSMASK_SS14_1  0x01000000	/* SS[0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14_2  0x02000000	/* SS[1:0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14_3  0x03000000	/* SS[2-0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14_4  0x04000000	/* SS[3-0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14_5  0x05000000	/* SS[4-0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14_6  0x06000000	/* SS[5-0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14_7  0x07000000	/* SS[6-0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14_8  0x08000000	/* SS[7-0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14_9  0x09000000	/* SS[8-0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14_10 0x0A000000	/* SS[9-0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14_11 0x0B000000	/* SS[10-0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14_12 0x0C000000	/* SS[11-0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14_13 0x0D000000	/* SS[12-0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_SS14    0x0E000000	/* SS[13-0]���}�b�`�ŃA���[�� */
#define ALARMSSMASK_NONE    0x0F000000	/* SS[14-0]���}�b�`�ŃA���[�� */

#define RTC_ERROR_LSI   0x00000001
#define RTC_ERROR_LSE   0x00000002
#define RTC_ERROR_RTC   0x00000004

#define INHNO_ALARM     IRQ_VECTOR_RTC_WKUP
#define INTNO_ALARM     IRQ_VECTOR_RTC_WKUP

#define INTPRI_ALARM     -1		/* �����ݗD��x */
#define INTATR_ALARM     0		/* �����ݑ��� */

/*
 *  �����ݒ�p�̍\���̂��`
 */
struct tm2 {
  int	tm_sec;			/* �b */
  int	tm_min;			/* �� */
  int	tm_hour;		/* �� */
  int	tm_mday;		/* �����̓� */
  int	tm_mon;			/* �� */
  int	tm_year;		/* �N */
  int	tm_wday;		/* �j�� */
  int	tm_yday;		/* �N���̓� */
  int	tm_isdst;
};

/*
 *  RTC�A���[���\����
 */
typedef struct
{
	uint32_t            alarm;				/* �A���[���I�� */
	uint32_t            alarmmask;			/* �A���[���}�X�N�ݒ� */
	uint32_t            subsecondmask;		/* �A���[���T�u�Z�J���h�}�X�N */
	uint32_t            dayselect;			/* �A���[�����t�ݒ� */
	uint32_t            subsecond;			/* �A���[���T�u�Z�J���h */
	void                (*callback)(void);	/* �A���[���R�[���o�b�N */
}RTC_Alarm_t;


extern uint32_t rtcerrorcode;

extern void rtc_init(intptr_t exinf);
extern ER rtc_set_time(struct tm2 *pt);
extern ER rtc_get_time(struct tm2 *pt);
extern ER rtc_setalarm(RTC_Alarm_t *parm, struct tm2 *ptm);
extern ER rtc_stopalarm(uint32_t Alarm);
extern ER rtc_getalarm(RTC_Alarm_t *parm, struct tm2 *ptm, uint32_t Alarm);
extern void rtc_handler(void);
extern void rtc_info_init(intptr_t exinf);

#endif

