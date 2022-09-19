#pragma once
#include <Arduino.h>

namespace JabaraMIDI {

struct Config {
  int midiChannel = 0; // MIDIチャンネル(0-based)
  int midiControlNumber = 7; // 送信するCC(コントロールチェンジ)のControl Number
  int minValue = 64; // CCの値の最小値(蛇腹静止時の値)
  int maxValue = 127; // CCの値の最大値(蛇腹最大動作時の値)

  float positiveScale = 500; // 閉じ方向の蛇腹最大動作時の気圧差(気圧差がこの値のときにmaxValueが出る)
  float negativeScale = 500; // 開き方向の蛇腹最大動作時の気圧差(同上)
  float deadZoneRange = 10; // 安定とみなす気圧差(デッドゾーン)
  float curveFactor = 0.7; // 気圧差をCCの値に変換する際のカーブ(1が線形、小さくするとcompress、大きくするとexpandになる)

  bool mute = false;
};

}  // namespace JabaraMIDI