#pragma once
#include <Arduino.h>

namespace JabaraMIDI {

class Sensor_None {
public:
  bool begin() { return true; }
  float readPressure() { return 0; }
};

template <class ISensor, class OSensor>
class Sensors {
private:
  ISensor& iSensor;
  OSensor& oSensor;

public:
  Sensors(ISensor& iSensor, OSensor& oSensor) : iSensor(iSensor), oSensor(oSensor) {}

  bool begin() {
    return beginImpl(iSensor, "inside") && beginImpl(oSensor, "outside");
  }

  float readPressureInside() {
    return iSensor.readPressure();
  }

  float readPressureOutside() {
    return oSensor.readPressure();
  }

private:
  template<class Sensor>
  bool beginImpl(Sensor& s, const char* name) {
    if (!s.begin()) {
      Serial.printf("Failed to initialize sensor %s.\n", name);
      return false;
    }
    Serial.printf("BME280 %s initialized.\n", name);
    return true;
  }
};

} // namespace JabaraMIDI