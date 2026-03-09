#pragma once

class ParsingXY {
public:
  ParsingXY(char sep = ',') {
    _sep = sep;
  }
  String sub(const char* str, byte i = 0) {
    byte ind1 = 0;
    int8_t cnt = -1, ind0 = -1;
    for (; ind1 < strlen(str); ind1++) {
      if (str[ind1] == _sep and cnt < i) {
        cnt += 1;
        if (cnt == i) break;
        if (cnt < i) ind0 = ind1;
      }
    }
    String s;
    s.reserve(ind1 - ind0 - 1);
    s = str;
    return s.substring(ind0 + 1, ind1);
  }

  byte amount(const char* str) {
    byte ind1 = 0, cnt = 0;
    for (; ind1 < strlen(str); ind1++)
      if (str[ind1] == _sep) cnt += 1;
    return cnt + 1;
  }
private:
  char _sep;
};
