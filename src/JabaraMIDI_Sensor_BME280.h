#pragma once
#include <Arduino.h>
#include <Adafruit_BME280.h>

namespace JabaraMIDI {

class Sensor_BME280 {
private:
  Adafruit_BME280 bme280;
  int addr;
  TwoWire* wire;

public:
  Sensor_BME280(int addr, TwoWire* wire = &Wire) : addr(addr), wire(wire) {}

  bool begin() {
    if (!bme280.begin(addr, wire)) {
      return false;
    }

    bme280.setSampling(
      Adafruit_BME280::MODE_NORMAL, // run measurement background
      Adafruit_BME280::SAMPLING_X1, // temp is necessary for pressure correction
      Adafruit_BME280::SAMPLING_X1, // pressure is an absolute need
      Adafruit_BME280::SAMPLING_NONE, // humidity is not necessary
      Adafruit_BME280::FILTER_X4, // a little bit of smoothing
      Adafruit_BME280::STANDBY_MS_0_5 // quick measurement
      );

    return true;
  }

  float readPressure() {
    return bme280.readPressure();
  }
};

}  // namespace JabaraMIDI