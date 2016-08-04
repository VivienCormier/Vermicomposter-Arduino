# Vermicomposter-Arduino 1.0
Arduino version of the worm composter. To improve the performance and keep the worms healthy.

## Feature
This first version use an humidity sensor to trigger the fan on the top. The result is to reduce to humidity inside the top box.

## Introduction
Before to run the Arduino project, you need to move the libraries to your libraries folder. Please follow the README.

## Schema

## Links
http://playground.arduino.cc/Main/DHT11Lib
https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib
https://github.com/adafruit/DHT-sensor-library

# Bugs
Unfortunately the 100% humidity rate is reached to soon and the fan is triggered all the time. The compost become too dry. We should use a rain sensor to detect the condensation and turn on the fan if the top box is too humid.
