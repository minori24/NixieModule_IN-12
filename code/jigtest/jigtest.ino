#include "NixieModule_IN_12.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <Wire.h>
#define ADDR0 0x4F


#define PIN 5

NixieModule_IN_12 nix0 = NixieModule_IN_12(ADDR0);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_GRB + NEO_KHZ800);


void setup() { 
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Start");
  
  nix0.init();
  nix0.setTransitionMode(TMODE_FADE);
  nix0.setNumber(0, 1);
  nix0.update();
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

int i = 0;
int step = 20;
void loop() {
  nix0.setTransitionMode(TMODE_NORMAL);

  for(int i=0; i<10; i++){
    nix0.setNumber(i, 1);
    nix0.update();
    delay(200);
  }

  nix0.dot(1);
  delay(200);
  nix0.dot(0);
  
  rainbow(5);
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

