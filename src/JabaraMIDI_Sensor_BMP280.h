#pragma once
#include <Arduino.h>
#include <Adafruit_BMP280.h>

namespace JabaraMIDI {

// Note: not tested
class Sensor_BMP280 {
private:
  Adafruit_BMP280 bmp280;
  int addr;

public:
  Sensor_BMP280(int addr, TwoWire* wire = &Wire) : bmp280(wire), addr(addr) {}

  bool begin() {
    if (!bmp280.begin(addr)) {
      return false;
    }

    bmp280.setSampling(
      Adafruit_BMP280::MODE_NORMAL, // run measurement background
      Adafruit_BMP280::SAMPLING_X1, // temp is necessary for pressure correction
      Adafruit_BMP280::SAMPLING_X1, // pressure is an absolute need
      Adafruit_BMP280::FILTER_X4, // a little bit of smoothing
      Adafruit_BMP280::STANDBY_MS_1 // quick measurement
      );

    return true;
  }

  float readPressure() {
    return bmp280.readPressure();
  }
};

}  // namespace JabaraMIDI