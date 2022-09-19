// Send out the MIDI control change messages as a USB MIDI device
// and provide the remote configuration capability
// (The processor needs to support TinyUSB)

#include <JabaraMIDI_Basics.h>
#include <JabaraMIDI_Sensor_BME280.h>
#include <JabaraMIDI_USBMIDIHelper.h>
#include <JabaraMIDI_RemoteConfigurator.h>

const int SDA_PIN = 10;
const int SCL_PIN = 11;
const int SAMPLES_PER_SECOND = 100;
const int MEASURE_INTERVAL = 10;

JabaraMIDI::Sensor_BME280 inside(0x77);
JabaraMIDI::Sensor_BME280 outside(0x76);
JabaraMIDI::Sensors<decltype(inside), decltype(outside)> sensors(inside, outside);

JabaraMIDI::Calibrator<SAMPLES_PER_SECOND> calibrator;

JabaraMIDI::Config config;
JabaraMIDI::USBMIDIHelper usbMIDI("Jabara MIDI");
JabaraMIDI::Processor processor(config, [](JabaraMIDI::Config& config, int value) {
  usbMIDI->sendControlChange(config.midiControlNumber, value, config.midiChannel + 1);
});

JabaraMIDI::RemoteConfigurator<decltype(Serial)> remoteConfigurator(config, Serial);

static void halt() {
  while (true) {
    delay(10);
  }
}

void setup() {
  Serial.begin();
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!sensors.begin()) {
    halt();
  }

  usbMIDI.begin();
}

void loop() {
  remoteConfigurator.listen();

  float i = sensors.readPressureInside();
  float o = sensors.readPressureOutside();
  float d = i - o;
  calibrator.process(d);
  float b = calibrator.getBaseline();
  processor.process(d - b);

  delay(MEASURE_INTERVAL - millis() % MEASURE_INTERVAL);
}