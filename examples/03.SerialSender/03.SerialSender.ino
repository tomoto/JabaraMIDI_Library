// Send out the MIDI control change parameters (not the actual MIDI messages) to serial

#include <JabaraMIDI_Basics.h>
#include <JabaraMIDI_Sensor_BME280.h>

const int SDA_PIN = 10;
const int SCL_PIN = 11;
const int SAMPLES_PER_SECOND = 100;
const int MEASURE_INTERVAL = 1000 / SAMPLES_PER_SECOND;

JabaraMIDI::Sensor_BME280 inside(0x77);
JabaraMIDI::Sensor_BME280 outside(0x76);
JabaraMIDI::Sensors<decltype(inside), decltype(outside)> sensors(inside, outside);

JabaraMIDI::Calibrator<SAMPLES_PER_SECOND> calibrator;

JabaraMIDI::Config config;
JabaraMIDI::Processor processor(config, [](JabaraMIDI::Config& config, int value) {
  Serial.printf("c:%d,n:%d,v:%d\n", config.midiChannel, config.midiControlNumber, value);
});

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
  processor.process(d - b);

  delay(MEASURE_INTERVAL - millis() % MEASURE_INTERVAL);
}