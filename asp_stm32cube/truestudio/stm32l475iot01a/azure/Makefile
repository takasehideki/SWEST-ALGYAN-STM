#
#  ターゲットの指定（Makefile.targetで上書きされるのを防ぐため）
#
all:

#
#  アプリケーションファイル
#
APPLDIR = .
APPLNAME = azure
#USE_CXX = true
APPL_CFG = $(APPLNAME).cfg
USE_TRUESTUDIO = true

DEBUG = true
OMIT_OPTIMIZATION = true

#
#  toolchainのPATHを明示的に指定する場合
#  Windowsの場合はフルパスにする
#
#GCC_PATH_WIN = C:\Program Files (x86)\Atollic\TrueSTUDIO for STM32 9.0.1\ARMTools\bin

#
#  XCUBEライブラリの使用の定義
#  使用しない場合はコメントアウトにする
#
USE_XCUBE_LIB = true

#
#  XCUBEライブラリのディレクトリの定義
#
XCUBE_LIB_DIR = ../../../xcube-lib

#
#  ASPソースファイルのディレクトリの定義
#
SRCDIR = ../../../asp_kernel

#
#  XCUBEライブラリのビルド
#
include $(XCUBE_LIB_DIR)/Drivers/CMSIS/Makefile.cms
include $(XCUBE_LIB_DIR)/Drivers/STM32L4xx_HAL_Driver/Makefile.hal
include $(XCUBE_LIB_DIR)/Drivers/BSP/Makefile.bsp
include $(XCUBE_LIB_DIR)/Projects/B-L475E-IOT01/Applications/Cloud/Azure/Makefile.app
include $(XCUBE_LIB_DIR)/Projects/Common/Makefile.cmm
include $(XCUBE_LIB_DIR)/Middlewares/Third_Party/Makefile.tpt

#
#  XCUBEライブラリ用の割込み定義
#
C_OBJ += irq_handler/stm32l4xx_it.o
INCLUDE_PATHS += -Iirq_handler

#
#  ネットワーク接続情報（WiFi,Azure）の静的設定
#  リセット後にいちいち各情報を入力するのが面倒な時は，次の行のコメントアウトを
#  外して "static_netinfo.h" に各情報を定義すること
#  個人情報になりがちなので.gitignoreしてある
#
CC_SYMBOLS = -DUSE_STATIC_NETINFO
#
#  XCUBEライブラリ
#
include ../Makefile.xcb

#
#  ASPカーネルライブラリ
#
KERNEL_LIB = .
include ../Makefile.asp

$(ALL_OBJ): kernel_cfg.h

