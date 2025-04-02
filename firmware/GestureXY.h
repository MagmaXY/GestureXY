#pragma once
#include "paj7620.h"

class GestureXY {
public:
  GestureXY() {
    _prd = 100;
    _flg = true;
    _flg1 = false;
  }
  void setReaction(uint32_t react = 400) {
    _react = react;
  }
  void setQuit(uint32_t quit = 800) {
    _quit = quit;
  }
  void setTurn(uint16_t turn = 0) {
    _turn = turn;
  }
  uint8_t init() {
    return uint8_t(paj7620Init());
  }
  void tick() {
    _gest = "";
    gest = "";
    static uint8_t data, data1;
    if (_flg and millis() - _tmr >= _prd) {
      data = 0;
      data1 = 0;
      uint8_t error = paj7620ReadReg(0x43, 1, &data);
      if (!error) {
        switch (data) {
          case GES_RIGHT_FLAG:
            _flg = false;
            _flg1 = true;
            _tmr1 = millis();
            break;
          case GES_LEFT_FLAG:
            _flg = false;
            _flg1 = true;
            _tmr1 = millis();
            break;
          case GES_UP_FLAG:
            _flg = false;
            _flg1 = true;
            _tmr1 = millis();
            break;
          case GES_DOWN_FLAG:
            _flg = false;
            _flg1 = true;
            _tmr1 = millis();
            break;
          case GES_FORWARD_FLAG:
            _flg = false;
            _flg1 = true;
            _tmr1 = millis();
            break;
          case GES_BACKWARD_FLAG:
            _flg = false;
            _flg1 = true;
            _tmr1 = millis();
            break;
          case GES_CLOCKWISE_FLAG:
            _gest = "Clockwise";
            break;
          case GES_COUNT_CLOCKWISE_FLAG:
            _gest = "Anticlockwise";
            break;
          default:
            break;
        }
      }
    }
    if (_flg1 and millis() - _tmr1 >= _react) {
      _flg = true;
      _flg1 = false;
      paj7620ReadReg(0x43, 1, &data1);
      _tmr = millis();
      switch (data) {
        case GES_RIGHT_FLAG:
          if (data1 == GES_LEFT_FLAG) {
            _gest = "Right-Left";
          } else if (data1 == GES_FORWARD_FLAG) {
            _gest = "Forward";
            _tmr += _quit;
          } else if (data1 == GES_BACKWARD_FLAG) {
            _gest = "Backward";
            _tmr += _quit;
          } else {
            _gest = "Right";
          }
          break;
        case GES_LEFT_FLAG:
          if (data1 == GES_RIGHT_FLAG) {
            _gest = "Left-Right";
          } else if (data1 == GES_FORWARD_FLAG) {
            _gest = "Forward";
            _tmr += _quit;
          } else if (data1 == GES_BACKWARD_FLAG) {
            _gest = "Backward";
            _tmr += _quit;
          } else {
            _gest = "Left";
          }
          break;
        case GES_UP_FLAG:
          if (data1 == GES_DOWN_FLAG) {
            _gest = "Up-Down";
          } else if (data1 == GES_FORWARD_FLAG) {
            _gest = "Forward";
            _tmr += _quit;
          } else if (data1 == GES_BACKWARD_FLAG) {
            _gest = "Backward";
            _tmr += _quit;
          } else {
            _gest = "Up";
          }
          break;
        case GES_DOWN_FLAG:
          if (data1 == GES_UP_FLAG) {
            _gest = "Down-Up";
          } else if (data1 == GES_FORWARD_FLAG) {
            _gest = "Forward";
            _tmr += _quit;
          } else if (data1 == GES_BACKWARD_FLAG) {
            _gest = "Backward";
            _tmr += _quit;
          } else {
            _gest = "Down";
          }
          break;
        case GES_FORWARD_FLAG:
          if (data1 == GES_BACKWARD_FLAG) {
            _gest = "Forward-Backward";
            _tmr += _quit;
          } else {
            _gest = "Forward";
            _tmr += _quit;
          }
          break;
        case GES_BACKWARD_FLAG:
          if (data1 == GES_FORWARD_FLAG) {
            _gest = "Backward-Forward";
            _tmr += _quit;
          } else {
            _gest = "Backward";
            _tmr += _quit;
          }
          break;
        case GES_CLOCKWISE_FLAG:
          _gest = "Clockwise";
          break;
        case GES_COUNT_CLOCKWISE_FLAG:
          _gest = "Anticlockwise";
          break;
        default:
          paj7620ReadReg(0x44, 1, &data1);
          if (data1 == GES_WAVE_FLAG) {
            _gest = "Wave";
          }
          break;
      }
      _exchange();
    }
  }
  String getGest(byte ind) {
    String gests[15] = {
      "Up",
      "Down",
      "Up-Down",
      "Down-Up",
      "Right",
      "Left",
      "Right-Left",
      "Left-Right",
      "Forward",
      "Backward",
      "Forward-Backward",
      "Backward-Forward",
      "Clockwise",
      "Anticlockwise",
      "Wave"
    };
    return gests[ind];
  }
  String getRUGest(byte ind) {
    String gests[15] = {
      "Вперёд",
      "Назад",
      "Вперёд-Назад",
      "Назад-Вперёд",
      "Вправо",
      "Влево",
      "Вправо-Влево",
      "Влево-Вправо",
      "Вниз",
      "Вверх",
      "Вниз-Вверх",
      "Вверх-Вниз",
      "По часовой",
      "Против часовой",
      "Волна"
    };
    return gests[ind];
  }
  String getRUGest() {
    String gests[15] = {
      "Up",
      "Down",
      "Up-Down",
      "Down-Up",
      "Right",
      "Left",
      "Right-Left",
      "Left-Right",
      "Forward",
      "Backward",
      "Forward-Backward",
      "Backward-Forward",
      "Clockwise",
      "Anticlockwise",
      "Wave"
    };
    String RUgests[15] = {
      "Вперёд",
      "Назад",
      "Вперёд-Назад",
      "Назад-Вперёд",
      "Вправо",
      "Влево",
      "Вправо-Влево",
      "Влево-Вправо",
      "Вниз",
      "Вверх",
      "Вниз-Вверх",
      "Вверх-Вниз",
      "По часовой",
      "Против часовой",
      "Волна"
    };
    for (byte i = 0; i < 15; i++) {
      if (gest == gests[i]) return RUgests[i];
    }
    return "";
  }
  String gest;
private:
  int16_t _turn;
  uint32_t _tmr;
  uint32_t _tmr1;
  uint32_t _prd;
  bool _flg;
  bool _flg1;
  uint32_t _quit;
  uint32_t _react;

  String _gest;
  void _exchange() {
    if (_gest == "") return;
    switch (_turn) {
      case 0:
        gest = _gest;
        break;
      case 90:
        if (_gest == "Right") {
          gest = "Up";
        } else if (_gest == "Left") {
          gest = "Down";
        } else if (_gest == "Right-Left") {
          gest = "Up-Down";
        } else if (_gest == "Left-Right") {
          gest = "Down-Up";
        } else if (_gest == "Up") {
          gest = "Left";
        } else if (_gest == "Down") {
          gest = "Right";
        } else if (_gest == "Up-Down") {
          gest = "Left-Right";
        } else if (_gest == "Down-Up") {
          gest = "Right-Left";
        } else {
          gest = _gest;
        }
        break;
      case 180:
        if (_gest == "Right") {
          gest = "Left";
        } else if (_gest == "Left") {
          gest = "Right";
        } else if (_gest == "Right-Left") {
          gest = "Left-Right";
        } else if (_gest == "Left-Right") {
          gest = "Right-Left";
        } else if (_gest == "Up") {
          gest = "Down";
        } else if (_gest == "Down") {
          gest = "Up";
        } else if (_gest == "Up-Down") {
          gest = "Down-Up";
        } else if (_gest == "Down-Up") {
          gest = "Up-Down";
        } else {
          gest = _gest;
        }
        break;
      case 270:
        if (_gest == "Right") {
          gest = "Down";
        } else if (_gest == "Left") {
          gest = "Up";
        } else if (_gest == "Right-Left") {
          gest = "Down-Up";
        } else if (_gest == "Left-Right") {
          gest = "Up-Down";
        } else if (_gest == "Up") {
          gest = "Left";
        } else if (_gest == "Down") {
          gest = "Right";
        } else if (_gest == "Up-Down") {
          gest = "Left-Right";
        } else if (_gest == "Down-Up") {
          gest = "Right-Left";
        } else {
          gest = _gest;
        }
        break;
    }
  }
};
