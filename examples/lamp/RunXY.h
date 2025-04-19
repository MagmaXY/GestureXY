#pragma once

// **************** НАСТРОЙКА МАТРИЦЫ ****************
#if (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y

#elif (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y x

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y (HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (HEIGHT - y - 1)
#define THIS_Y x

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (WIDTH - x - 1)
#define THIS_Y (HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (HEIGHT - y - 1)
#define THIS_Y (WIDTH - x - 1)

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (WIDTH - x - 1)
#define THIS_Y y

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y (WIDTH - x - 1)

#else
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y
#pragma message "Wrong matrix parameters! Set to default"

#endif

class RunXY {
public:

  RunXY() {
    _offset = WIDTH;
  }

  void setColor(CRGB color) {
    _color = color;
  }

  void fill(CRGB* ledS, String text) {
    if (loadingFlag) {
      _offset = WIDTH;  // перемотка в правый край
      loadingFlag = false;
      fullTextFlag = false;
    }

    if (millis() - _scrollTimer >= D_TEXT_SPEED) {
      _scrollTimer = millis();
      FastLED.clear();
      byte i = 0, j = 0;
      while (text[i] != '\0') {
        if ((byte)text[i] > 191) {  // работаем с русскими буквами!
          i++;
        } else {
          _drawLetter(j, text[i], _offset + j * (LET_WIDTH + SPACE), ledS);
          i++;
          j++;
        }
      }
      fullTextFlag = false;

      _offset--;
      if (_offset < -j * (LET_WIDTH + SPACE)) {  // строка убежала
        _offset = WIDTH + 3;
        fullTextFlag = true;
      }
      FastLED.show();
    }
  }

  bool loadingFlag, fullTextFlag;

private:

  int _stringLength(String thisString) {
    byte i = 0, j = 0;
    while (thisString[i] != '\0') {
      if ((byte)thisString[i] > 191) {  // работаем с русскими буквами!
        i++;
      } else {
        i++;
        j++;
      }
    }
    return j;
  }

  void _drawLetter(uint8_t index, uint8_t letter, int16_t _offset, CRGB* ledS) {
    int8_t start_pos = 0, finish_pos = LET_WIDTH;

    if (_offset < -LET_WIDTH || _offset > WIDTH) return;
    if (_offset < 0) start_pos = -_offset;
    if (_offset > (WIDTH - LET_WIDTH)) finish_pos = WIDTH - _offset;

    for (byte i = start_pos; i < finish_pos; i++) {
      int thisByte;
      if (MIRR_V) thisByte = _getFont((byte)letter, LET_WIDTH - 1 - i);
      else thisByte = _getFont((byte)letter, i);

      for (byte j = 0; j < LET_HEIGHT; j++) {
        boolean thisBit;

        if (MIRR_H) thisBit = thisByte & (1 << j);
        else thisBit = thisByte & (1 << (LET_HEIGHT - 1 - j));

        // рисуем столбец (i - горизонтальная позиция, j - вертикальная)
        if (TEXT_DIRECTION) {
          if (thisBit) {
            ledS[_getPixelNumber(_offset + i, TEXT_HEIGHT + j)] = _color;
          } else {
            _drawPixelXY(_offset + i, TEXT_HEIGHT + j, ledS);
          }
        } else {
          if (thisBit) {
            ledS[_getPixelNumber(i, _offset + TEXT_HEIGHT + j)] = _color;
          } else {
            _drawPixelXY(i, _offset + TEXT_HEIGHT + j, ledS);
          }
        }
      }
    }
  }

  uint8_t _getFont(uint8_t font, uint8_t row) {
    font = font - '0' + 16;                                       // перевод код символа из таблицы ASCII в номер согласно нумерации массива
    if (font <= 90) return pgm_read_byte(&(fontHEX[font][row]));  // для английских букв и символов
    else if (font >= 112 && font <= 159) {                        // и пизд*ц ждя русских
      return pgm_read_byte(&(fontHEX[font - 17][row]));
    } else if (font >= 96 && font <= 111) {
      return pgm_read_byte(&(fontHEX[font + 47][row]));
    }
  }

  uint16_t _getPixelNumber(int8_t x, int8_t y) {
    if ((THIS_Y % 2 == 0) || MATRIX_TYPE) {  // если чётная строка
      return (THIS_Y * _WIDTH + THIS_X);
    } else {  // если нечётная строка
      return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1);
    }
  }

  void _drawPixelXY(int8_t x, int8_t y, CRGB* ledS) {
    if (x < 0 || x > WIDTH - 1 || y < 0 || y > HEIGHT - 1) return;
    int thisPixel = _getPixelNumber(x, y) * SEGMENTS;
    for (byte i = 0; i < SEGMENTS; i++) {
      ledS[thisPixel + i].setRGB(0, 0, 0);
    }
  }

  uint32_t _scrollTimer;
  byte _offset;
  CRGB _color;
};
