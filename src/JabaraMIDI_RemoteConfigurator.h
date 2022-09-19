#pragma once
#include <Arduino.h>

#include <JabaraMIDI_Config.h>

namespace JabaraMIDI {

template<class Stream>
class RemoteConfigurator {
private:
  Config& config;
  Stream& stream;

public:
  static constexpr char* SerializedFormat = "{\"t\":\"config\",\"cc\":%d,\"min\":%d,\"max\":%d,\"ps\":%f,\"ns\":%f,\"cf\":%f,\"m\":%d}";
  static const int NumberOfFields = 7;
  static constexpr char* RequestFormat = "{\"t\":\"requestConfig\"}";

  RemoteConfigurator(Config& config, Stream& stream) : config(config), stream(stream) {}

  void listen() {
    if (stream.available()) {
      // not perfect, but works
      std::array<char, 200> buf;
      int bufSize = stream.read(buf.data(), buf.size() - 1);
      buf[bufSize] = '\0';

      int cc, min, max, m;
      float ps, ns, cf;
      int readFields = sscanf(buf.data(), SerializedFormat, &cc, &min, &max, &ps, &ns, &cf, &m);
      if (readFields == NumberOfFields) {
        config.midiControlNumber = cc;
        config.minValue = min;
        config.maxValue = max;
        config.positiveScale = ps;
        config.negativeScale = ns;
        config.curveFactor = cf;
        config.mute = !!m;
        Serial.printf("OK: %d %d %d %f %f %f %d\n", cc, min, max, ps, ns, cf, m);
      } else if (strcmp(buf.data(), RequestFormat) == 0) {
        send();
      } else {
        Serial.printf("ERROR: %s\n", buf.data());
      }
    }
  }

  void send() {
    stream.printf(SerializedFormat,
      config.midiControlNumber,
      config.minValue,
      config.maxValue,
      config.positiveScale,
      config.negativeScale,
      config.curveFactor,
      config.mute);
    stream.println();
  }

  void request() {
    stream.println(RequestFormat);
  }
};

}  // namespace JabaraMIDI