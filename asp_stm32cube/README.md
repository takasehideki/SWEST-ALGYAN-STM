# ASPカーネル＋STM32Cube パッケージ

## 開発環境

* Atollic TrueSTUDIO for STM32 v9.0.1
  * http://download.atollic.com/TrueSTUDIO/installers/Atollic_TrueSTUDIO_for_STM32_windows_x86_v9.0.1_20180420-1214.exe

## ASPカーネル (asp_kernel/)

### 元ソース

* ターゲット非依存部：1.9.3 (リリース日2017-04-29)
  * http://toppers.jp/download.cgi/asp-1.9.3.tar.gz
* ターゲット依存部：1.9.4 (リリース日2017-08-28)
  * http://toppers.jp/download.cgi/asp_arch_arm_m4_gcc-1.9.4.tar.gz
* コンフィギュレータ：1.9.6 Windows用バイナリ (リリース日2017-03-31)
  * http://toppers.jp/download.cgi/cfg-mingw-static-1_9_6.zip

### 主な実装対象

* target/stm32l475iot01a_gcc
  * copy from target/stm32l476nucleo_gcc

## STM32Cubeライブラリ (xcube-lib/)

### 元ソース

* X-CUBE-AZURE (Software Version: 1.1.0)
  * Microsoft Azure software expansion for STM32Cube
  * http://www.st.com/ja/embedded-software/x-cube-azure.html


## TrueSTUDIOでの操作方法

### ビルド

* truestudio/stm32l475iot01a をワークスペースとして開く
* sample1等 を既存プロジェクトとしてインポート
  * プロジェクト・エクスプローラー > sample1 > プロパティ -> C/C++ Build > Settings で  
    Board: B-L475E-IOT01A  
    コード配置：RAM  
  になっていることを確認
* 普通にビルド


### デバッグ

debug_\*.launchというのを置いておきます．  
プロジェクト・エクスプローラーの右クリックで「デバッグ > debug_\*」でできるはず

* debug_asp.launch の主な設定内容（方法）
  * メニューから「デバッグ」を選択
  * 実行バイナリ：asp.elf
  * 「デバッガ」タブにて
    * デバッグプローブ：ST-LINK
    * シリアルワイヤビューア：有効 にチェック
  * スタートアップスクリプト > ターゲットソフトウェア初期化スクリプト
    * tbreak main -> tbreak sta_ker

### 新しいプロジェクトの作り方

* プロジェクト・エクスプローラー上で既存のプロジェクト（例：sample1/）をコピペ
  * 以降の例：blinky/　# 適宜読み替えること
  * プロジェクト名：blinky
  * 「デフォルト・ロケーションの使用」はチェックを付けたまま
  * 「コピーの問題」のエラーが出るが無視してよい（次で解消する）
* asp_kernel/ のリンクを作る
  * プロジェクト・エクスプローラー上のblinkyで右クリック > インポート
  * 「ファイル・システム」を選択
    * 「次のディレクトリから」：<gitrepo>/asp_kernel
    * 下の asp_kernel/ にチェック
    * 「拡張>>]をクリックし，「ワークスペースにリンクを作成」にチェック
  * 「ファイル・システム」を選択
    * 「次のディレクトリから」：<gitrepo>/xcube-lib
    * 下の xcube-lib/ にチェック
    * 「拡張>>]をクリックし，「ワークスペースにリンクを作成」にチェック
  * 終了
* 各種ファイル名の変更
  * sample1.[c,h,cfg]をblinky.[c,h,cfg]にリネーム
  * Makefileの APPLNAME を blinky にする
  * blinky.cfg を「#include blinky.h」にする
  * blinky.c の「"#include "blinky.h"」を#includeの一番下にする
  * あとは適宜ソースを修正する
* デバッグ対象の変更
  * debug_sample1.launch を debug_blinky.launch にリネーム
  * debug_blinky.launch を右クリック > デバッグ > デバッグの構成
  * 「組み込みC/C++ アプリケーション」の debug_blinky を選択する
  * MainタブのProject: を blinky にする
    * Browse... から Project Selection してもよい


## 開発の参考にしたリポジトリ

* Open-source Software Platform Based on TOPPERS/ASP Kernel, mbed and Arduino Library for Renesas GR-PEACH.
  * https://github.com/ncesnagoya/asp-gr_peach_gcc-mbed


## 参考：mbed online compilerからエクスポートしてビルド


### mbed上の設定

"Export toolchain: Make-GCC-ARM"でエクスポート

### WSL

* クロスコンパイラの入手
```
$ sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded
$ sudo gpg --keyserver ha.pool.sks-keyservers.net --recv-keys 6D1D8367A3421AFB
$ sudo gpg --export --armor 6D1D8367A3421AFB | sudo apt-key add -
$ sudo apt update
$ sudo apt install gcc-arm-none-eabi
```

エクスポートしてきたZipを展開してそこでmakeでいいはず

### TrueSTUDIO

Under construction,,,  
まだうまくいってません．

