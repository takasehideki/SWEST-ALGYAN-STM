# ASP�J�[�l��+mbed�p�b�P�[�W

## �J����

* Atollic TrueSTUDIO for STM32 v9.0.1
  * http://download.atollic.com/TrueSTUDIO/installers/Atollic_TrueSTUDIO_for_STM32_windows_x86_v9.0.1_20180420-1214.exe

## ASP�J�[�l�� (asp/)

### ���\�[�X

* �^�[�Q�b�g��ˑ����F1.9.3 (�����[�X��2017-04-29)
  * http://toppers.jp/download.cgi/asp-1.9.3.tar.gz
* �^�[�Q�b�g�ˑ����F1.9.4 (�����[�X��2017-08-28)
  * http://toppers.jp/download.cgi/asp_arch_arm_m4_gcc-1.9.4.tar.gz
* �R���t�B�M�����[�^�F1.9.6 Windows�p�o�C�i�� (�����[�X��2017-03-31)
  * http://toppers.jp/download.cgi/cfg-mingw-static-1_9_6.zip

### ���\�[�X

* �Q�l�ɂ������|�W�g��
  * Open-source Software Platform Based on TOPPERS/ASP Kernel, mbed and Arduino Library for Renesas GR-PEACH.
  * https://github.com/ncesnagoya/asp-gr_peach_gcc-mbed

### ��Ȏ����Ώ�

* target/stm32l475iot01a_gcc
  * copy from target/stm32l476nucleo_gcc

## TrueSTUDIO�g����

### �r���h

* truestudio/stm32l475iot01a �����[�N�X�y�[�X�Ƃ��ĊJ��
  * �K�v�Ȃ� sample1�� �������v���W�F�N�g�Ƃ��ăC���|�[�g
* ���ʂɃr���h

### �f�o�b�O

���Ԃ񕁒ʂɂł���͂�

* ���ӓ_
  * �v���W�F�N�g�E�G�N�X�v���[���[ > sample1 > C/C++ Build > Settings  
  �Łu�R�[�h�z�u�FRAM�v�ɂȂ��Ă��邱�Ƃ��m�F


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
�܂����܂��s���Ă��܂���D

