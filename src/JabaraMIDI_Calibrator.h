#pragma once
#include <Arduino.h>

namespace JabaraMIDI {

template<int SamplesPerSeconds, int WindowSeconds = 5, int Threshold = 10>
class Calibrator {
private:
  std::array<float, SamplesPerSeconds * WindowSeconds> samples;
  int ptr = 0;
  float baseline = 0;

public:
  Calibrator() {
    samples.fill(0);
  }

  float getBaseline() {
    return baseline;
  }

  void process(float value) {
    samples[ptr] = value;
    ptr = (ptr + 1) % samples.size();

    if (ptr % SamplesPerSeconds == 0) {
      // average
      float sum = 0;
      float min = 999999999;
      float max = -min;
      for (float s : samples) {
        sum += s;
        min = std::min(s, min);
        max = std::max(s, max);
      }
      float b = sum / samples.size();

      // // median
      // decltype(samples) sorted;
      // std::partial_sort_copy(begin(samples), end(samples), begin(sorted), end(sorted));
      // float min = sorted.front();
      // float max = sorted.back();
      // float b = sorted[sorted.size() / 2];
      
      if (max - b <= Threshold && b - min <= Threshold) {
        baseline = b ;
      }
    }
  }
};

}  // namespace JabaraMIDI