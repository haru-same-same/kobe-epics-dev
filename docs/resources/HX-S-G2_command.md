# HX-S-G2 Commands

## ADDRess

- ADDRess [num(0-50)] -> デバイスアドレスの指定, 0はグローバルアドレス

## ALM

- ALM
  - :CLEar -> アラームリセット
  - :CONTain
    - :CC [bin] -> 
    - :CV [bin] -> 

## OUTPut

- OUTPut
  - :DELay
    - :ON [num(0.0-99.99)] -> 出力ONディレイ時間の設定
    - :OFF [num(0.0-99.99)] -> 出力OFFディレイ時間の設定
  - :EXTernal
    - :MODE [num(0-2)] -> 外部接点による出力制御モードの設定
  - :HOT [num(0-2)] -> Power on時の出力の状態設定
  - :MODE [num(0-7)] -> 出力の立ち上がりモードの設定
  - [:STATe [bool]] -> 出力のON/OFF

## MEASure

- MEASure
  - :CORRection
    - :MODE [num(0-2)] -> 直線性補償機能の動作モードの設定
  - :MVAV [bin] -> 計測値の移動平均を行うかどうかの設定
  - [:SCALar]
    - :CURRent
      - [:DC] query -> 電流計測値の取得
    - :VOLTage
      - [:DC] query -> 電圧計測値の取得
    - :POWer
      - [:DC] query -> 電力計測値の取得

## SOURce

- SOURce
  - :CURRent
    - [:LEVel]
      - [:IMMediate]
        - [:AMPLitude [num]] -> 出力電流の設定
  - :PROTection
    - [:LEVel [num]] -> OCP電流の設定
  - :SLEW
    - :RISing [num] -> CCスルーレートの立ち上がりの設定
    - :FALLing [num] -> CCスルーレートの立ち下がりの設定
  - :MEMory
    - :RECall [str(A-C)] -> パネルメモリーの読み出し
      - :MODE [bin] -> パネルメモリーの読み出し手順を設定
    - :STORe [str(A-C)] -> パネルメモリーへの書き込み
  - :RESistance
    - [:LEVel]
      - [:IMMediate]
        - [:AMPLitude [num]] -> 内部抵抗の設定
  - :VOLTage
    - [:LEVel]
      - [:IMMediate]
        - [:AMPLitude [num]] -> 出力電圧の設定
    - :PROTection
      - [:LEVel [num]] -> OVP電圧の設定
    - :SLEW
      - :RISing [num] -> CVスルーレートの立ち上がりの設定
      - :FALLing [num] -> CVスルーレートの立ち下がりの設定

## STATus

- STATus
  - :MEASure
    - :CONDition query -> 電源ステータスの取得
    - :OUTPut 
      - :MODE [bin] -> ステータス出力機能の選択

## SYSTem

- SYSTem
  - :COMMunicate
    - :SERial
      - [:RECeive]
        - :BAUD [num(0-3)] -> ビットレートの設定
        - :PACE [str(ACK, OFF)] -> アクノリッジレスポンスの設定
        - :PARity
          - [:TYPE [str(ODD, EVEN, NONE)]] -> パリティの設定
      - :UNIT [bin] -> クエリレスポンスの単位付加設定
  - :CONTrol
    - :CURRent
      - :MODE [num(0-4)] -> 電流設定制御方法のモード設定
      - :ISOLate [bin] -> 外部アナログ制御信号定電流(CC)の絶縁の設定
    - :VOLTage
      - :MODE [num(0-4)] -> 電圧設定制御方法モードの設定
      - :ISOLate [bin] -> 外部アナログ制御信号定電圧(CV)の絶縁の設定
  - :ERRor
    - [:NEXT] query -> エラーメッセージの読み取り
  - :KEYLock
    - :MODE [num(0-2)] -> LOCKのモードの設定
  - :MONitor
    - :MODE [bin] -> 出力OFF時の電圧計/電流計の表示設定
  - :PRESet
    - :MODE [bin] -> PRESET内容の確定方法の設定
  - :SERies [bin] -> 直列運転時のマスター、スレーブの設定
  - :TRIP -> TRIP動作の実行
    - :MODE [num(0-2)] -> TRIP実行及び、重故障/軽故障時の出力停止動作の設定
  - :BUZzer
    - :BUTton [bin] -> 前面パネル操作時のボタン音のON/OFFの設定
    - :ALArm [bin] -> アラーム発生時のアラーム音のON/OFFの設定
  - :POWer query -> システム容量(kW)の問い合わせ
  - :STORe query -> FUNCTION設定内容の保存

## SEQUENCE

- SEQUENCE
  - :MODE [num(0-2)] -> シーケンス動作の設定
  - :RCOUnt [num(0-9999)] -> シーケンス動作の繰り返し回数を設定
  - :PATTern 
  - :STOP [bin] -> シーケンス動作終了時の出力を設定
  - :STATus query -> シーケンス動作の状態取得