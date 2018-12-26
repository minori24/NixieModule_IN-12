
#include <Wire.h>

#define ADDR 0x4F

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();

  Wire.beginTransmission(ADDR);
  Wire.write(0x00);
  Wire.write(0x80);
  Wire.endTransmission();
  delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
delay(200);
  Wire.beginTransmission(ADDR);
  Wire.write(0x08);
  Wire.write(0x13);
   uint8_t returned = Wire.endTransmission();
  
  Wire.requestFrom(ADDR, 1);
  Serial.print("returned:");
  Serial.print(returned, HEX);
  Serial.println("");
  uint8_t r = Wire.read();
  Serial.println(r, HEX);
  
}
