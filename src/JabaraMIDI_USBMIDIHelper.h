#pragma once
#include <Arduino.h>

// Depending on
// https://github.com/adafruit/Adafruit_TinyUSB_Arduino
// https://github.com/FortySevenEffects/arduino_midi_library
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

namespace JabaraMIDI {

struct USBMIDIHelper {
  struct USBInitializer {
    // According to https://github.com/adafruit/Adafruit_TinyUSB_Arduino,
    // the descriptors must be set before TinyUSB constructor calls
    USBInitializer(const char* deviceName) {
#if defined(ARDUINO_ARCH_ESP32)
      USB.productName(deviceName);
#else
      #error("Not tested");
      TinyUSBDevice.setProductDescriptor(deviceName);
#endif
    }
  } usbInitializer;

  Adafruit_USBD_MIDI usbd;
  midi::SerialMIDI<decltype(usbd)> serial;
  midi::MidiInterface<decltype(serial)> midi;
  typedef decltype(midi) MidiInterface;

  USBMIDIHelper(const char* deviceName) : usbInitializer(deviceName), serial(usbd), midi(serial) {}

  MidiInterface* operator->() { return &midi; }

  void begin(midi::Channel channelToListen = MIDI_CHANNEL_OMNI) {
    midi.begin(channelToListen);
    while (!TinyUSBDevice.mounted()) {
      delay(10);
    }
    Serial.println("USB MIDI initialized");
  }
};

}  // namespace JabaraMIDI