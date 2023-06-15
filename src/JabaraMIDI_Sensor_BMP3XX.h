#pragma once
#include <Arduino.h>
#include <Adafruit_BMP3XX.h>

namespace JabaraMIDI {

// Note: not tested
class Sensor_BMP3XX {
private:
  Adafruit_BMP3XX bmp;
  TwoWire* wire;
  int addr;

public:
  Sensor_BMP3XX(int addr = BMP3XX_DEFAULT_ADDRESS, TwoWire* wire = &Wire) : wire(wire), addr(addr) {}

  bool begin() {
    if (!bmp.begin_I2C(addr, wire)) {
      return false;
    }

    // Measure at the maximum speed and power
    bmp.setOutputDataRate(BMP3_ODR_200_HZ);
    // Use 4x oversampling (~12ms = ~80Hz) with a light filtering to balance
    // the accuracy, responsiveness, and the number of data points
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

    return true;
  }

  float readPressure() {
    return bmp.readPressure();
  }
};

}  // namespace JabaraMIDI