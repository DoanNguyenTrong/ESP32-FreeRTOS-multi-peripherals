## A comprehensive example of using ESP32 with FreeRTOS to work with multiple peripherals

### Introduction

In this enertaining project, I want to play a little bit with FreeRTOS in creating multiple threads to handle peripherals.
If you are interested in playing with embedded programming and control, why don't 



### Step-by-step instruction

##### Presiquisites
 - Install Arduino IDE and [ESP Arduino package](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/mac.md)
 - I use platformIO and VSCode to work as it provides a lot of great functionalities. If you want to use other ways, feel free to do it.
   - PlatformIO: https://github.com/espressif/arduino-esp32/blob/master/docs/platformio.md

##### Libraries
Install MQTT, RFID, Servo, Stepper, LCD, DHT sensor libraries using `git submodule`
```bash
git submodule add -f https://github.com/adafruit/DHT-sensor-library.git lib/DHT-sensor
## Dependency of DHT-sensor
git submodule add -f  https://github.com/adafruit/Adafruit_Sensor.git lib/Adafruit-sensor
## After this step, you need to copy Adafruit_Sensor.h and Adafruit_Sensor.cpp to DHT-sensor/

git submodule add -f https://github.com/arduino-libraries/LiquidCrystal.git lib/LCD

git submodule add -f https://github.com/knolleary/pubsubclient.git lib/MQTT

git submodule add -f https://github.com/miguelbalboa/rfid.git lib/RFID

git submodule add -f https://github.com/jkb-git/ESP32Servo.git lib/Servo

git submodule add -f https://github.com/pkerspe/ESP-FlexyStepper.git lib/Stepper
```

Later, if you want to use the most up-to-date version of these submodules, run the below commands
```bash
git submodule init
# update your submodule --remote fetches new commits in the submodules
# and updates the working tree to the commit described by the branch
git submodule update --remote
```

Also, you need to modify `monitor_port` and `upload_port` in the platformio.ini file to make sure they point to the actual port that the ESP32 is connected.

