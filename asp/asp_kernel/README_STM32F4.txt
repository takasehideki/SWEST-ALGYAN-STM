
	TOPPERS/ASP Kernel��Release 1.9.4��STM32F4/L4
		Toyohashi Open Platform for Embedded Real-Time Systems/
		Advanced Standard Profile Kernel

TOPPERS/ASP STM32F4�ϰʲ��Σ��ĤΥܡ��ɤ�GCC�γ�ȯ�Ķ����б����ޤ���
�ܥѥå������ϸ��̥ѥå������Ǥ��뤿�ᡢTOPPERS/ASP�����ͥ륿�����å�
���¸���ѥå�����asp-1.9.3.tar.gz���Ȥ߹�碌�ƻ��Ѥ��Ƥ���������
(1)STM�� STM32F4 Discovery�ܡ���
   Chip:STM32F407VGT6
(2)Olimex��STM32-E407�ܡ���
   Chip:STM32F407ZGT6
(3)�����ƥ��Υ��Ҥ�stm32f429�ܡ���
   Chip:STM32F429
(4)STM��STM32F401 Nucleo�ܡ���
   Chip:STM32F401RET6
(5)STM��STM32F446 Nucleo-64�ܡ���
   Chip:STM32F446RET6
(6)STM��STM32F446 Nucleo-144�ܡ���
   Chip:STM32F446ZET6
(7)STM��STM32L476 Nucleo-64�ܡ���
   Chip:STM32L476RGT6
(8)STM��STM32L476 Discovery�ܡ���
   Chip:STM32L476VGT6

ASP�μ¹Է��֤ϰʲ��Σ��Ĥ򥵥ݡ��Ȥ��ޤ���
�¹Է��֤ϡ�����ѥ�������ѿ�DBGENV��������ѹ����Ǥ��ޤ���
Makefile��������ѹ����Ǥ��ޤ���

(1)RAM�¹ԡ�ROM��˥�(rommon)�ǵ�ư�����ܡ��ɤˡ�UART���Ѥ���
ASP�μ¹Է���(srec)���������ɤ��Ƽ¹Ԥ������
rommon��FLASH ROM����ߥե������tools/rommon���֤��Ƥ���ޤ���
DBGENV�����ꤵ��ʤ���硢�ޤ���RAM������ξ�硢���η��֤Υӥ�ɤ�Ԥ��ޤ���

(2)ROM�¹ԡ�FLASH ROM�˽񤭹���Ǽ¹Ԥ������
DBGENV��ROM������ξ�硢���η��֤Υӥ�ɤ�Ԥ��ޤ���

STM��STM32F401 Nucleo�ܡ��ɤ˴ؤ��Ƥϡ�TrueSTUDIO�Υץ������ȥե������
tools/TrueSTUDIO���Ѱդ��Ƥ��ޤ���
����ˤ�ꡢľ��TrueSTUDIO���Ѥ��ƥӥ�ɤ���ǽ�Ǥ���
�¹Է��֤�ROM�¹Ԥ˸���Ǥ���

�ڥǥ��쥯�ȥ깽����

arch/arm_m_gcc/common
	cortex-m�Υ������
arch/arm_m_gcc/stm32f4xx
	stm32f4��chip��¸��
arch/gcc
	gcc�δĶ���
target/stm32e407_gcc
	STM32-E407�ܡ��ɤΥ������åȰ�¸��
target/stm32f4discovery_gcc
	STM32F4 Discovery�ܡ��ɤΥ������åȰ�¸��
target/stm32f401nucleo_gcc
	STM32F401 Nucleo�ܡ��ɤΥ������åȰ�¸��
target/stm32f429board_gcc
	stm32f429�ܡ��ɤΥ������åȰ�¸��
target/stm32f446nucleo64_gcc
	stm32f446 Nucleo-64�ܡ��ɤΥ������åȰ�¸��
target/stm32f446nucleo144_gcc
	stm32f446 Nucleo-144�ܡ��ɤΥ������åȰ�¸��
target/stm32l476nucleo64_gcc
	stm32l476 Nucleo-64�ܡ��ɤΥ������åȰ�¸��
target/stm32l476discovery_gcc
	stm32l476 discovery�ܡ��ɤΥ������åȰ�¸��
tools/rommon
	ROM��˥���UART���������ROM��˥��񤭹��ߥХ��ʥ�ե�����
	ROM��˥��Υ�������TOPPERS���饳��ƥ�Ĵ��ã�������
	STM32F401-Nucleo 64�ܡ����Ԥˤ����ۤ��ޤ���
tools/TrueSTUDIO
	STM32F401 Nucleo�ܡ����Ѥ�TrueSTUDIO�Υץ������ȥե�����

