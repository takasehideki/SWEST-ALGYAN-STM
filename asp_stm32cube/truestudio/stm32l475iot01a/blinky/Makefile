#
#  ターゲットの指定（Makefile.targetで上書きされるのを防ぐため）
#
all:

#
#  アプリケーションファイル
#
APPLDIR = .
APPLNAME = blinky
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
include $(XCUBE_LIB_DIR)/Drivers/Makefile.cms
include $(XCUBE_LIB_DIR)/Drivers/BSP/Makefile.bsp

#
#  XCUBEライブラリのコンフィギュレーション設定用のヘッダファイル
#
INCLUDE_PATHS += -Ixcube-lib-conf 

#C_OBJ += xcube-lib-src/stm32l4xx_hal_msp.o \


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

