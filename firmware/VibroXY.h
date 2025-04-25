#pragma once

class VibroXY {
public:
  VibroXY(byte pin) {
    _pin = pin;
    _flg0 = true;
    _prd1 = 50;
    _pwm = 255;
    pinMode(_pin, OUTPUT);
    off();
  }
  void setPeriod(uint32_t prd = 50) {
    _prd1 = prd;
  }
  void setPower(byte pwm = 255) {
    _pwm = pwm;
  }
  void on(uint32_t prd = 500, byte num = 1) {
    if (!_flg0) return;
    _prd = prd;
    _num = num;
    _flg = true;
    _flg1 = false;
    _tmr = millis();
    analogWrite(_pin, _pwm);
  }
  void off() {
    digitalWrite(_pin, LOW);
    _flg1 = false;
    _flg1 = false;
    _prd = 0;
    _num = 0;
  }
  void flag(bool flg = true) {
    _flg0 = flg;
    if (!flg) off();
  }
  void tick() {
    if (_flg and _num > 0 and millis() - _tmr >= _prd) {
      _num--;
      digitalWrite(_pin, LOW);
      if (_num > 0) {
        _flg1 = true;
        _tmr1 = millis();
      }
      _tmr = millis();
    }
    if (_flg1 and millis() - _tmr1 >= _prd1) {
      _flg1 = false;
      analogWrite(_pin, _pwm);
      _tmr = millis();
    }
  }
private:
  byte _pin;
  uint32_t _tmr;
  uint32_t _prd;
  uint32_t _tmr1;
  uint32_t _prd1;
  byte _num;
  byte _pwm;
  bool _flg0;
  bool _flg;
  bool _flg1;
};
