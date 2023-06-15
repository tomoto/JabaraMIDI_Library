// Measure the difference between inside/outside sensors and
// continuously calibrate the baseline offset

#include <JabaraMIDI_Basics.h>
#include <JabaraMIDI_Sensor_BME280.h>

const int SDA_PIN = 10;
const int SCL_PIN = 11;
const int SAMPLES_PER_SECOND = 100;
const int MEASURE_INTERVAL = 1000 / SAMPLES_PER_SECOND;

JabaraMIDI::Sensor_BME280 iSensor(0x77);
JabaraMIDI::Sensor_BME280 oSensor(0x76);
JabaraMIDI::Sensors<decltype(iSensor), decltype(oSensor)> sensors(iSensor, oSensor);

JabaraMIDI::Calibrator<SAMPLES_PER_SECOND> calibrator;

static void halt() {
  while (true) {
    delay(10);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!sensors.begin()) {
    halt();
  }
}

void loop() {
  float i = sensors.readPressureInside();
  float o = sensors.readPressureOutside();
  
  float d = i - o;
  calibrator.process(d);
  float b = calibrator.getBaseline();

  Serial.printf("d:%.6f,b:%.6f\n", d, b);
  delay(MEASURE_INTERVAL - millis() % MEASURE_INTERVAL);
}