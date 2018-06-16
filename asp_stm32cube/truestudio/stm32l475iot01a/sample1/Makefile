#
#  ターゲットの指定（Makefile.targetで上書きされるのを防ぐため）
#
all:

#
#  アプリケーションファイル
#
APPLDIR = .
APPLNAME = sample1
USE_CXX = true
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
#  MBEDライブラリのディレクトリの定義
#
#MBED_LIB_DIR = ../../../mbed-lib

#
#  ASPソースファイルのディレクトリの定義
#
SRCDIR = ../../../asp_kernel

#
#  MBEDライブラリのビルド
#
#include $(MBED_LIB_DIR)/mbed-src/Makefile.src
#include ../Makefile.mbd

#
#  ASPカーネルライブラリ
#
KERNEL_LIB = .
include ../Makefile.asp

$(ALL_OBJ): kernel_cfg.h
