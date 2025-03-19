void gestTick() {
  if (g.gest == "") return;

  if (!db[kk::state] and g.gest == "Up-Down") {

    db[kk::state] = true;
    Serial.println("Active");
    vibro.on(500);
    cursorGraph();

  } else if (db[kk::state] and gData.name == 0 and gData.scene == 0 and g.gest == "Up-Down") {
    db[kk::state] = false;
    Serial.println("Inactive");
    oled.clear();
  }

  if (!db[kk::state]) return;

  if (gData.scene == 0) {

    if (g.gest == "Right-Left" or g.gest == "Left-Right") {

      int8_t move = g.gest == "Right-Left" ? 1 : -1;
      if (gData.name + move < 0) {
        gData.name = getNamesCount() - 1;
      } else {
        gData.name = (gData.name + move) % getNamesCount();
      }
      cursorGraph();

    } else if (gData.name == 0) {

      if (g.gest == "Right" or g.gest == "Left") {

        int8_t move = g.gest == "Right" ? 1 : -1;
        if (gData.scene + move < 0) {
          gData.scene = getScenesCount() - 1;
        } else {
          gData.scene = (gData.scene + move) % getScenesCount();
        }
        Serial.println(getScene());
        cursorGraph();

      } else if (g.gest == "Up") {

        db[kk::brightness] = (int(db[kk::brightness]) + 32) % 256;
        cursorGraph();
        oled.setContrast(db[kk::brightness]);

      } else if (g.gest == "Down") {

        db[kk::brightness] -= 32;
        if (db[kk::brightness] < 0) db[kk::brightness] = 255;
        cursorGraph();
        oled.setContrast(db[kk::brightness]);

      } else if (g.gest == "Down-Up") {

        db[kk::vibration] = !db[kk::vibration];
        cursorGraph();
      }
    } else {

      cursorGraph();
      oled.print(g.gest);
      sendGest();
    }
  } else if (g.gest == "Right" or g.gest == "Left") {

    int8_t move = g.gest == "Right" ? 1 : -1;
    if (gData.scene + move < 0) {
      gData.scene = getScenesCount() - 1;
    } else {
      gData.scene = (gData.scene + move) % getScenesCount();
    }
    Serial.println(getScene());
    cursorGraph();

  } else if (g.gest == "Up") {

    Serial.println(getScene());
    cursorGraph();
    sendScene();
  }
  gData.timer = millis();
}
