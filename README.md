# JabaraMIDI_Library

* Common code for JabaraMIDI project
  * V1: https://qiita.com/tomoto335/items/92580af26b5c171dc1c0
  * V2: https://qiita.com/tomoto335/items/d20aa668a62ad49cda36
  * V3: https://qiita.com/tomoto335/items/b7f913a5a9f855a304e6

* BME280/BMP280/BMP3XX sensors are supported.
  You need to manually install the libraries from Adafruit for your sensors. See JabaraMIDI_Sensor_[sensor type].h.

* I intentionally wrote all the code in the header files to see how smartly the modern C++ compilers handle them in this template library era. Currently, I do not see any drawbacks. Actually, the binary gets slightly smaller, which was a nice surprise.
