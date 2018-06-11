# ASPカーネル+mbedパッケージ

## 開発環境

* Atollic TrueSTUDIO for STM32 v9.0.1
  * http://download.atollic.com/TrueSTUDIO/installers/Atollic_TrueSTUDIO_for_STM32_windows_x86_v9.0.1_20180420-1214.exe

## ASPカーネル (asp/)

### 元ソース

* ターゲット非依存部：1.9.3 (リリース日2017-04-29)
  * http://toppers.jp/download.cgi/asp-1.9.3.tar.gz
* ターゲット依存部：1.9.4 (リリース日2017-08-28)
  * http://toppers.jp/download.cgi/asp_arch_arm_m4_gcc-1.9.4.tar.gz
* コンフィギュレータ：1.9.6 Windows用バイナリ (リリース日2017-03-31)
  * http://toppers.jp/download.cgi/cfg-mingw-static-1_9_6.zip

### 元ソース

* 参考にしたリポジトリ
  * Open-source Software Platform Based on TOPPERS/ASP Kernel, mbed and Arduino Library for Renesas GR-PEACH.
  * https://github.com/ncesnagoya/asp-gr_peach_gcc-mbed

### 主な実装対象

* target/stm32l475iot01a_gcc
  * copy from target/stm32l476nucleo_gcc

## TrueSTUDIO使い方

### ビルド

* truestudio/stm32l475iot01a をワークスペースとして開く
* sample1等 を既存プロジェクトとしてインポート
  * プロジェクト・エクスプローラー > sample1 > プロパティ  
  -> C/C++ Build > Settings で  
    Board: B-L475E-IOT01A  
    コード配置：RAM  
  になっていることを確認
* 普通にビルド


### デバッグ

debug_asp.launchというのを置いておきます．  
プロジェクト・エクスプローラーの右クリックで「デバッグ > debug_asp」でできるはず

* debug_asp.launch の主な設定内容（方法）
  * メニューから「デバッグ」を選択
  * 実行バイナリ：asp.elf
  * 「デバッガ」タブにて
    * デバッグプローブ：ST-LINK
    * シリアルワイヤビューア：有効 にチェック
  * スタートアップスクリプト > ターゲットソフトウェア初期化スクリプト
    * tbreak main -> tbreak sta_ker


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
まだうまく行っていません．

