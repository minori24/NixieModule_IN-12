#include <NixieModule_IN_12.h>
#include <Wire.h>

NixieModule_IN_12::NixieModule_IN_12(uint8_t i2caddr){
  _i2caddr = i2caddr;
}

void NixieModule_IN_12::init(void){
  Wire.begin();
  reset();
  setPWMFreq(1000);
  _brightness = 1.0;
  setNumber(0, 0, 0);
  for (uint8_t i = 0; i < 11; i++) {
    setPWM(i, 0, 0);
  }
}

void NixieModule_IN_12::reset(void){
  write8(PCA9685_MODE1, 0x80);
  delay(10);
}

void NixieModule_IN_12::setPWMFreq(float freq){

  float oscClk = 25000000;
  float prescaleval = oscClk / (4096 * freq * 0.9) - 1;

  uint8_t prescale = floor(prescaleval + 0.5);
  uint8_t oldmode = read8(PCA9685_MODE1);
  uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
  write8(PCA9685_MODE1, newmode); // go to sleep
  write8(PCA9685_PRESCALE, prescale); // set the prescaler
  write8(PCA9685_MODE1, oldmode);
  delay(5);
  write8(PCA9685_MODE1, oldmode | 0xa0);  //  This sets the MODE1 register to turn on auto increment.
}

void NixieModule_IN_12::setPWM(uint8_t num, uint16_t on, uint16_t off){
  Wire.beginTransmission(_i2caddr);
  Wire.write(LED0_ON_L + 4*num);
  Wire.write(on);
  Wire.write(on >> 8);
  Wire.write(off);
  Wire.write(off >> 8);
  Wire.endTransmission();
}

//set brightness(0 - 1.0) of all numbers
void NixieModule_IN_12::setBrightness(float b){
  uint16_t off = floor(4095.0 * b);
  setPWM(_num, 0, off);
  _brightness = b;
}

void NixieModule_IN_12::doTransition(uint8_t num, float b){
  uint16_t off = floor(4095.0 * b);
  setPWM(num, 0, off);
}

// Set active number and transition step
void NixieModule_IN_12::setNumber(uint8_t newNum, uint8_t dot, uint16_t step){
  uint8_t n = 11 - newNum % 10;
  if (newNum == 1) n = 0;
  if (newNum == 0) n = 1;
  if (dot) n = 10;

  _step = step;
  _stepCount = 0;
  _num = _newNum;
  _newNum = n;
  _isTran = 1;
}

void NixieModule_IN_12::setTransitionMode(uint8_t tmode){
  _mode = tmode;

  for (uint8_t i = 0; i < 11; i++) {
    setPWM(i, 0, 0);
  }
  _num = 0;
}

void NixieModule_IN_12::update(){
  if(_isTran && _stepCount < _step){
    if(_mode == TMODE_FADE){
      if(_step > 0){
        float b = (float)(_stepCount + 1) / (float)(_step) * _brightness;
        doTransition(_num, 1.0 - b);
        doTransition(_newNum, b);
      }
      else{
        _isTran = 0;
      }
    }
    else if(_mode == TMODE_SHUFFLE){
      if (_stepCount < _step - 1) {
        doTransition((_num + _stepCount) % 10, 0);
        doTransition((_num + _stepCount) % 10 + 1, _brightness);
      }
      else{
        doTransition((_num + _stepCount) % 10, 0);
        doTransition(_newNum, _brightness);
      }
    }
    else if(_mode == TMODE_NORMAL){
      doTransition(_num, 0);
      doTransition(_newNum, _brightness);
      _isTran = 0;
    }
    else{
      doTransition(_num, 0);
      doTransition(_newNum, _brightness);
      _isTran = 0;
    }

    _stepCount++;
  }
  else{
    _isTran = 0;
  }
}

uint8_t NixieModule_IN_12::read8(uint8_t addr){
  Wire.beginTransmission(_i2caddr);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)1);
  return Wire.read();
}

void NixieModule_IN_12::write8(uint8_t addr, uint8_t d){
  Wire.beginTransmission(_i2caddr);
  Wire.write(addr);
  Wire.write(d);
  Wire.endTransmission();
}
