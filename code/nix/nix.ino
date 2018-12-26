#include "NixieModule_IN_12.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <Wire.h>
#define ADDR0 0x4F
#define ADDR1 0x4E
#define ADDR2 0x4D
#define ADDR3 0x4C

#define PIN 5

NixieModule_IN_12 nix3 = NixieModule_IN_12(ADDR0);
NixieModule_IN_12 nix2 = NixieModule_IN_12(ADDR1);
NixieModule_IN_12 nix1 = NixieModule_IN_12(ADDR2);
NixieModule_IN_12 nix0 = NixieModule_IN_12(ADDR3);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);


void setup() { 
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Start");
  
  nix0.init();
  nix1.init();
  nix2.init();
  nix3.init();
  
  nix0.setTransitionMode(TMODE_FADE);
  nix1.setTransitionMode(TMODE_FADE);
  nix2.setTransitionMode(TMODE_FADE);
  nix3.setTransitionMode(TMODE_FADE);
  
  nix0.setNumber(0, 0, 1);
  nix1.setNumber(1, 0, 1);
  nix2.setNumber(2, 0, 1);
  nix3.setNumber(3, 0, 1);
  
  nix0.update();
  nix1.update();
  nix2.update();
  nix3.update();
  delay(3000);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

int i = 0;
int step = 20;
void loop() {
  setTransitionMode(TMODE_NORMAL);
  demo_clock();
  delay(1000);
  
  setTransitionMode(TMODE_SHUFFLE);
  demo_clock();
  delay(1000);

  setTransitionMode(TMODE_FADE);
  demo_clock();
  delay(1000);

  setTransitionMode(TMODE_SHUFFLE);
  for(int i=0; i<5; i++){
    demo_slot();
    delay(1000);
  }

  delay(3000);
}

void setTransitionMode(uint8_t m){
  nix0.setTransitionMode(m);
  nix1.setTransitionMode(m);
  nix2.setTransitionMode(m);
  nix3.setTransitionMode(m);
}

void demo_clock(){
  uint8_t h, m;
  h = 12;
  m = 34;
  step = 20;

  nix0.setNumber(0, 0, 1);
  nix1.setNumber(5, 0, 1);
  nix2.setNumber(2, 0, 1);
  nix3.setNumber(1, 0, 1);
  nix0.update();
  nix1.update();
  nix2.update();
  nix3.update();

  for(int i=5; i<10; i++){
    nix0.setNumber(i, 0, step);
    
    for(int i=0; i<step; i++){
      nix0.update();
      delay(10);
    }

    delay(1000);
  }
  
  nix0.setNumber(0, 0, step);
  nix1.setNumber(0, 0, step);
  nix2.setNumber(3, 0, step);

  for(int i=0; i<step; i++){
    nix2.update();
    nix1.update();
    nix0.update();
    delay(10);
  }
  
  delay(1000);

  for(int i=1; i<10; i++){
    nix0.setNumber(i, 0, step);
    
    for(int i=0; i<step; i++){
      nix0.update();
      delay(10);
    }

    delay(1000);
  }
  
  rainbow(20);

}

void demo_slot(){
  uint8_t r0 = random(0, 9);
  uint8_t r1 = random(0, 9);
  uint8_t r2 = random(0, 9);
  uint8_t r3 = random(0, 9);
  
  nix0.setNumber(0, 0, 1);
  nix1.setNumber(0, 0, 1);
  nix2.setNumber(0, 0, 1);
  nix3.setNumber(0, 0, 1);
  nix0.update();
  nix1.update();
  nix2.update();
  nix3.update();
  
  step = 300;


  nix0.setNumber(r0, 0, step);
  nix1.setNumber(r1, 0, step - 150);
  nix2.setNumber(r2, 0, step - 100);
  nix3.setNumber(r3, 0, step - 50);

  for(int i=0; i<step; i++){
    nix0.update();  
    nix1.update();  
    nix2.update();  
    nix3.update();  
    delay(8);
  }
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

