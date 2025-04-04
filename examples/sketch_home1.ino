#include <HID-Project.h>
#define GT_STREAM_MODE

#include <GyverTransfer.h>
GyverTransfer<3, GT_RX, 5000, 20> rx;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  attachInterrupt(digitalPinToInterrupt(3), isr, CHANGE);
  rx.setTimeout(50);
  Consumer.begin();
  BootKeyboard.begin();
  System.begin();
  BootKeyboard.press(KEY_LEFT_GUI);
  delay(500);
  BootKeyboard.press('r');
  delay(100);
  BootKeyboard.releaseAll();
  BootKeyboard.print("https://github.com/MagmaXY/GestureXY/");
  BootKeyboard.press(KEY_RETURN);
  delay(50);
  BootKeyboard.releaseAll();
}
String str = "";

void isr() {
  rx.tickISR();
}

void loop() {
  if (str != "") {
    Serial.println(str);
    if (str == "Вперёд") {
      Consumer.write(MEDIA_PAUSE);
    } else if (str == "Назад") {
      Consumer.write(MEDIA_VOLUME_MUTE);
    } else if (str == "Вперёд") {
      Consumer.write(MEDIA_PAUSE);
    } else if (str == "Вперёд-Назад") {
      BootKeyboard.press(KEY_BACKSPACE);
      delay(200);
      BootKeyboard.releaseAll();
    } else if (str == "Назад-Вперёд") {
      BootKeyboard.press(KEY_LEFT_GUI);
      delay(500);
      BootKeyboard.press('r');
      delay(100);
      BootKeyboard.releaseAll();
      BootKeyboard.print("https://github.com/MagmaXY/GestureXY/");
      BootKeyboard.press(KEY_RETURN);
      delay(50);
      BootKeyboard.releaseAll();
    } else if (str == "Влево") {
      Consumer.write(MEDIA_PREVIOUS);
    } else if (str == "Вправо") {
      Consumer.write(MEDIA_NEXT);
    } else if (str == "Вправо-Влево") {
      Consumer.write(MEDIA_VOLUME_UP);
    } else if (str == "Влево-Вправо") {
      Consumer.write(MEDIA_VOLUME_DOWN);
    } else if (str == "Вверх") {
      System.write(SYSTEM_POWER_DOWN);
    } else if (str == "Уход") {
      System.write(SYSTEM_SLEEP);
    } else if (str == "Прмход") {
      System.write(SYSTEM_WAKE_UP);
    } else if (str.startsWith("http")) {
      BootKeyboard.press(KEY_LEFT_GUI);
      delay(500);
      BootKeyboard.press('r');
      delay(100);
      BootKeyboard.releaseAll();
      BootKeyboard.print(str);
      BootKeyboard.press(KEY_RETURN);
      delay(50);
      BootKeyboard.releaseAll();
    }
    str = "";
  }
  if (rx.available()) {
    str = rx.readString();
  }
  if (Serial.available()) {
    str = Serial.readString();
  }
}
