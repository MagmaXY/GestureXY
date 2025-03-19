#include <HID-Project.h>
#define GT_STREAM_MODE

#include <GyverTransfer.h>
GyverTransfer<3, GT_RX, 5000, 20> rx;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);
  attachInterrupt(0, isr, CHANGE);
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
String str;

void isr() {
  rx.tickISR();
}

void loop() {
  if (str != "") {
    Serial.println(str);
    if (str == "Up") {
      Consumer.write(MEDIA_PAUSE);
    } else if (str == "Down") {
      Consumer.write(MEDIA_VOLUME_MUTE);
    } else if (str == "Up") {
      Consumer.write(MEDIA_PAUSE);
    } else if (str == "Up-Down") {
      BootKeyboard.press(KEY_RIGHT_ARROW);
      delay(50);
      BootKeyboard.releaseAll();
    } else if (str == "Down-Up") {
      BootKeyboard.press(KEY_LEFT_ARROW);
      delay(50);
      BootKeyboard.releaseAll();
    } else if (str == "Right") {
      Consumer.write(MEDIA_NEXT);
    } else if (str == "Right-Left") {
      Consumer.write(MEDIA_VOLUME_UP);
    } else if (str == "Left-Right") {
      Consumer.write(MEDIA_VOLUME_DOWN);
    } else if (str == "Backward") {
      System.write(SYSTEM_POWER_DOWN);
    } else if (str == "Away") {
      System.write(SYSTEM_SLEEP);
    } else if (str == "Comeback") {
      System.write(SYSTEM_WAKE_UP);
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
