#include <HID-Project.h>
#define GT_STREAM_MODE

#include <GyverTransfer.h>
GyverTransfer<3, GT_RX, 5000, 20> rx;

void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(3), isr, CHANGE);
  rx.setTimeout(50);
  BootKeyboard.begin();
  Mouse.begin();
  Consumer.begin();
  System.begin();
}

void isr() {
  rx.tickISR();
}

void loop() {
  if (rx.available()) {
    String str = rx.readString();
    str.replace(" ", "");
    Serial.println(str);
    if (str == "MEDIA_PAUSE") {
      Consumer.write(MEDIA_PAUSE);
    } else if (str == "MEDIA_VOLUME_MUTE") {
      Consumer.write(MEDIA_VOLUME_MUTE);
    } else if (str == "KEY_BACKSPACE") {
      Mouse.click();
    } else if (str == "MEDIA_PREVIOUS") {
      Consumer.write(MEDIA_PREVIOUS);
    } else if (str == "MEDIA_NEXT") {
      Consumer.write(MEDIA_NEXT);
    } else if (str == "MEDIA_VOLUME_UP") {
      Consumer.write(MEDIA_VOLUME_UP);
    } else if (str == "MEDIA_VOLUME_DOWN") {
      Consumer.write(MEDIA_VOLUME_DOWN);
    } else if (str == "SYSTEM_POWER_DOWN") {
      System.write(SYSTEM_POWER_DOWN);
    } else if (str == "SYSTEM_SLEEP") {
      System.write(SYSTEM_SLEEP);
    } else if (str == "SYSTEM_WAKE_UP") {
      System.write(SYSTEM_WAKE_UP);
    } else if (str == "RUEN") {
      BootKeyboard.press(KEY_LEFT_GUI);
      delay(500);
      BootKeyboard.press(KEY_BACKSPACE);
      delay(100);
      BootKeyboard.releaseAll();
    } else if (str.startsWith("http") or str.startsWith("C:")) {
      winR(str);
    }
  }
}

void winR(String str) {
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
