#pragma once

class VibroXY {
public:
  VibroXY(byte pin, uint32_t prd = 50) {
    _pin = pin;
    _prd1 = prd;
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
  }
  void on(uint32_t prd = 500, byte num = 1) {
    if (!_flg) return;
    _prd = prd;
    _num = num;
    digitalWrite(_pin, HIGH);
    _tmr = millis();
  }
  void off() {
    digitalWrite(_pin, LOW);
    _tmr = 0;
    _prd = 0;
    _num = 0;
  }
  void flag(bool flg = true) {
    _flg = flg;
    _flg1 = !flg;
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
      digitalWrite(_pin, HIGH);
    }
  }
private:
  byte _pin;
  uint32_t _tmr;
  uint32_t _prd;
  uint32_t _tmr1;
  uint32_t _prd1;
  byte _num;
  bool _flg;
  bool _flg1;
};