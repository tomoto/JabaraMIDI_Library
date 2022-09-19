#pragma once
#include <Arduino.h>
#include <JabaraMIDI_Config.h>

namespace JabaraMIDI {

class Processor {
public:
  typedef std::function<void(Config&, int)> send_t;

private:
  Config& config;
  send_t send;
  int lastValue = -1;

public:
  Processor(Config& config, send_t send): config(config), send(send) {}

  void process(float value) {
    if (!config.mute) {
      float d = std::max<float>(abs(value) - config.deadZoneRange, 0);
      float r = pow(d / (value >= 0 ? config.positiveScale : config.negativeScale), config.curveFactor);
      int v = std::min<int>(config.minValue + r * (config.maxValue - config.minValue), config.maxValue);
      if (lastValue != v) {
        lastValue = v;
        send(config, v);
      }
    }
  }
};

}  // namespace JabaraMIDI