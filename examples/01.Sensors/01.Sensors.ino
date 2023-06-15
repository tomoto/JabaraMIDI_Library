// Measure the inside/outside pressures and write the raw values to serial

#include <JabaraMIDI_Basics.h>
#include <JabaraMIDI_Sensor_BME280.h>

const int SDA_PIN = 10;
const int SCL_PIN = 11;
const int SAMPLES_PER_SECOND = 100;
const int MEASURE_INTERVAL = 1000 / SAMPLES_PER_SECOND;

// Create individual sensors -- if you use BMP280 instead of BME280,
// include JabaraMIDI_Sensor_BMP280.h and use Sensor_BMP280 instead
JabaraMIDI::Sensor_BME280 iSensor(0x77);
JabaraMIDI::Sensor_BME280 oSensor(0x76);

// If you have only one sensor inside, create a dummy sensor for outside
// JabaraMIDI::Sensor_None oSensor;

// Pair the sensors
JabaraMIDI::Sensors<decltype(iSensor), decltype(oSensor)> sensors(iSensor, oSensor);

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
  Serial.printf("i:%.6f,o:%.6f\n", i, o);

  delay(MEASURE_INTERVAL - millis() % MEASURE_INTERVAL);
}