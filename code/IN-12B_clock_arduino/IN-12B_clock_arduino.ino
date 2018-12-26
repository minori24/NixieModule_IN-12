#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define ADDR 0x10

Adafruit_PWMServoDriver nix = Adafruit_PWMServoDriver(ADDR);


void setup(){
  d0.begin();
  d0.setPWMFreq(1000);
}

void loop(){

}
