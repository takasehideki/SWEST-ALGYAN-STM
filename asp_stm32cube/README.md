# ASP�J�[�l���{STM32Cube �p�b�P�[�W

## �J����

* Atollic TrueSTUDIO for STM32 v9.0.1
  * http://download.atollic.com/TrueSTUDIO/installers/Atollic_TrueSTUDIO_for_STM32_windows_x86_v9.0.1_20180420-1214.exe

## ASP�J�[�l�� (asp_kernel/)

### ���\�[�X

* �^�[�Q�b�g��ˑ����F1.9.3 (�����[�X��2017-04-29)
  * http://toppers.jp/download.cgi/asp-1.9.3.tar.gz
* �^�[�Q�b�g�ˑ����F1.9.4 (�����[�X��2017-08-28)
  * http://toppers.jp/download.cgi/asp_arch_arm_m4_gcc-1.9.4.tar.gz
* �R���t�B�M�����[�^�F1.9.6 Windows�p�o�C�i�� (�����[�X��2017-03-31)
  * http://toppers.jp/download.cgi/cfg-mingw-static-1_9_6.zip

### ��Ȏ����Ώ�

* target/stm32l475iot01a_gcc
  * copy from target/stm32l476nucleo_gcc

## STM32Cube���C�u���� (xcube-lib/)

### ���\�[�X

* X-CUBE-AZURE (Software Version: 1.1.0)
  * Microsoft Azure software expansion for STM32Cube
  * http://www.st.com/ja/embedded-software/x-cube-azure.html


## TrueSTUDIO�ł̑�����@

### �r���h

* truestudio/stm32l475iot01a �����[�N�X�y�[�X�Ƃ��ĊJ��
* sample1�� �������v���W�F�N�g�Ƃ��ăC���|�[�g
  * �v���W�F�N�g�E�G�N�X�v���[���[ > sample1 > �v���p�e�B -> C/C++ Build > Settings ��  
    Board: B-L475E-IOT01A  
    �R�[�h�z�u�FRAM  
  �ɂȂ��Ă��邱�Ƃ��m�F
* ���ʂɃr���h


### �f�o�b�O

debug_\*.launch�Ƃ����̂�u���Ă����܂��D  
�v���W�F�N�g�E�G�N�X�v���[���[�̉E�N���b�N�Łu�f�o�b�O > debug_\*�v�łł���͂�

* debug_asp.launch �̎�Ȑݒ���e�i���@�j
  * ���j���[����u�f�o�b�O�v��I��
  * ���s�o�C�i���Fasp.elf
  * �u�f�o�b�K�v�^�u�ɂ�
    * �f�o�b�O�v���[�u�FST-LINK
    * �V���A�����C���r���[�A�F�L�� �Ƀ`�F�b�N
  * �X�^�[�g�A�b�v�X�N���v�g > �^�[�Q�b�g�\�t�g�E�F�A�������X�N���v�g
    * tbreak main -> tbreak sta_ker

### �V�����v���W�F�N�g�̍���

* �v���W�F�N�g�E�G�N�X�v���[���[��Ŋ����̃v���W�F�N�g�i��Fsample1/�j���R�s�y
  * �ȍ~�̗�Fblinky/�@# �K�X�ǂݑւ��邱��
  * �v���W�F�N�g���Fblinky
  * �u�f�t�H���g�E���P�[�V�����̎g�p�v�̓`�F�b�N��t�����܂�
  * �u�R�s�[�̖��v�̃G���[���o�邪�������Ă悢�i���ŉ�������j
* asp_kernel/ �̃����N�����
  * �v���W�F�N�g�E�G�N�X�v���[���[���blinky�ŉE�N���b�N > �C���|�[�g
  * �u�t�@�C���E�V�X�e���v��I��
    * �u���̃f�B���N�g������v�F<gitrepo>/asp_kernel
    * ���� asp_kernel/ �Ƀ`�F�b�N
    * �u�g��>>]���N���b�N���C�u���[�N�X�y�[�X�Ƀ����N���쐬�v�Ƀ`�F�b�N
  * �u�t�@�C���E�V�X�e���v��I��
    * �u���̃f�B���N�g������v�F<gitrepo>/xcube-lib
    * ���� xcube-lib/ �Ƀ`�F�b�N
    * �u�g��>>]���N���b�N���C�u���[�N�X�y�[�X�Ƀ����N���쐬�v�Ƀ`�F�b�N
  * �I��
* �e��t�@�C�����̕ύX
  * sample1.[c,h,cfg]��blinky.[c,h,cfg]�Ƀ��l�[��
  * Makefile�� APPLNAME �� blinky �ɂ���
  * blinky.cfg ���u#include blinky.h�v�ɂ���
  * blinky.c �́u"#include "blinky.h"�v��#include�̈�ԉ��ɂ���
  * ���Ƃ͓K�X�\�[�X���C������
* �f�o�b�O�Ώۂ̕ύX
  * debug_sample1.launch �� debug_blinky.launch �Ƀ��l�[��
  * debug_blinky.launch ���E�N���b�N > �f�o�b�O > �f�o�b�O�̍\��
  * �u�g�ݍ���C/C++ �A�v���P�[�V�����v�� debug_blinky ��I������
  * Main�^�u��Project: �� blinky �ɂ���
    * Browse... ���� Project Selection ���Ă��悢


## �J���̎Q�l�ɂ������|�W�g��

* Open-source Software Platform Based on TOPPERS/ASP Kernel, mbed and Arduino Library for Renesas GR-PEACH.
  * https://github.com/ncesnagoya/asp-gr_peach_gcc-mbed


## �Q�l�Fmbed online compiler����G�N�X�|�[�g���ăr���h


### mbed��̐ݒ�

"Export toolchain: Make-GCC-ARM"�ŃG�N�X�|�[�g

### WSL

* �N���X�R���p�C���̓���
```
$ sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded
$ sudo gpg --keyserver ha.pool.sks-keyservers.net --recv-keys 6D1D8367A3421AFB
$ sudo gpg --export --armor 6D1D8367A3421AFB | sudo apt-key add -
$ sudo apt update
$ sudo apt install gcc-arm-none-eabi
```

�G�N�X�|�[�g���Ă���Zip��W�J���Ă�����make�ł����͂�

### TrueSTUDIO

Under construction,,,  
�܂����܂������Ă܂���D

