#include <HID-Project.h>
#define GT_STREAM_MODE  // для работы readString и прочих

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
  BootKeyboard.print("https://rutube.ru/video/f3b615db135287a64584737e664e1e4b/");
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
      Consumer.write(MEDIA_PLAY_PAUSE);
    } else if (str == "Down") {
      Consumer.write(MEDIA_VOLUME_MUTE);
    } else if (str == "Up-Down") {
      Consumer.write(MEDIA_PREVIOUS);
    } else if (str == "Down-Up") {
      Consumer.write(MEDIA_NEXT);
    } else if (str == "Clockwise") {
      Consumer.write(MEDIA_VOLUME_UP);
    } else if (str == "Anticlockwise") {
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