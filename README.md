# LoRaWANExample
LoRaWAN（ABP）での接続サンプルです。
# 前提
* 次のハードウェアで利用することを前提としています。
    * ノード:Dragino LoRa Mini
    * ゲートウェイ:Dragino LG01、OLG01
* LoRaWANの仕様を完全に実装しているわけではありません。
    * ClassAのUplinkだけを行います。
    * ゲートウェイのパケットのカウントは実装していません。
* 次のライブラリを必要とします。
    * Arduino用のLMiCライブラリ (https://github.com/matthijskooijman/arduino-lmic)
    * Arduino用のLoRaライブラリ (https://github.com/sandeepmistry/arduino-LoRa)
