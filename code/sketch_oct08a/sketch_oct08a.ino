#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

#define ADDR 0x4F
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(ADDR);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  pwm.begin();

  Wire.beginTransmission(ADDR);
  Wire.write(0x00);
  Wire.write(0x80);
  Wire.endTransmission();
  delay(10);
  pwm.setPWMFreq(1000);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //setPWM(0, 4095);
  for(uint8_t i=0; i<9; i++){
    for(uint8_t j=0; j<100; j++){
      float t = 4095 - 4095.0 / 100 * j;
      pwm.setPWM(i, 0, (uint16_t)t);
      pwm.setPWM(i+1,0, 4095 - (uint16_t)t);
      delay(10);
    }
    pwm.setPWM(i, 0, 0);
  }

  for(uint8_t j=0; j<100; j++){
    float t = 4095 - 4095.0 / 100 * j;
    pwm.setPWM(9, 0, (uint16_t)t);
    pwm.setPWM(0, 0, 4095 - (uint16_t)t);
    delay(10);
  }
  
  pwm.setPWM(9, 0, 0);
  /*
  Wire.beginTransmission(ADDR);
  Wire.write(0x08);
  Wire.write(0x08);
  Wire.endTransmission();
  
  int8_t r = Wire.requestFrom(ADDR, 1);
  r = Wire.read();
  Serial.println(r, HEX);
  */
}

void setPWM(uint16_t on, uint16_t off){
  Wire.beginTransmission(ADDR);
  Wire.write(0x0A);
  Wire.write(on);
  Wire.write(on << 0x10);
  Wire.write(off);
  Wire.write(off << 0x10);
  Wire.endTransmission();  
}
