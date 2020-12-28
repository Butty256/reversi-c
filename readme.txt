======================
  リバーシプログラム
======================

ソフト名　　　　　：reversi
開発環境　　　　　：Ubuntu 18.04.1 bionic (4.15.0-39-generic), gcc-7
コンパイル環境　　：C11コンパイル環境
最終更新日　　　　：2018/12/09
同梱ファイル　　　：main.c
　　　　　　　　　　board.h
　　　　　　　　　　board.c
　　　　　　　　　　com.h
　　　　　　　　　　com.c
　　　　　　　　　　evaluator.h
　　　　　　　　　　evaluator.c
　　　　　　　　　　learn.c
　　　　　　　　　　eval.dat
　　　　　　　　　　readme.txt

---------------------
・概要
main.c：
人やコンピュータと戦うことのできるプログラム．
learn.c：
コンピュータの学習用プログラム．

・仕様
自動でパスします．

・ファイル構成
プログラムソースファイル：
main.c，board.c，com.c，evaluator.c，learn.c
評価用ファイル：
eval.dat

・コンパイル（C言語規格C11）
main.c：
main.c，board.c，com.c，evaluator.cを
使うコンパイラのコマンドに従って
一括コンパイルまたは分割コンパイルする．
learn.c：
learn.c，board.c，com.c，evaluator.cを
使うコンパイラのコマンドに従って
一括コンパイルまたは分割コンパイルする．

・動作方法
main：
実行ファイルを実行し，黒と白のプレーヤー選択する．
入力方法は「a 1」のように入力する．
また，「undo」で前の自分の番に戻る．
「quit」で試合終了する．
learn：
実行ファイルを実行し学習回数を入力する．
あとは学習が完了するのを待つだけ．
