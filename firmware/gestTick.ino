void gestTick() {

  if (g.gest == "") return;

  if (!db[kk::state] and g.gest == db[kk::stateGest]) {

    db[kk::state] = true;
    Serial.println("Active");
    vibro.on(500);
    cursorGraph();
    gData.timer = millis();
    return;

  } else if (db[kk::state] and gData.name == 0 and gData.scene == 0 and g.gest == db[kk::stateGest]) {
    db[kk::state] = false;
    Serial.println("Inactive");
    oled.clear();
    gData.timer = millis();
    return;
  }

  if (!db[kk::state]) return;

  if (gData.scene == 0) {

    if (g.gest == db[kk::nextNameGest] or g.gest == db[kk::previousNameGest]) {

      int8_t move = g.gest == db[kk::nextNameGest] ? 1 : -1;
      if (gData.name + move < 0) {
        gData.name = getNamesCount() - 1;
      } else {
        gData.name = (gData.name + move) % getNamesCount();
      }
      cursorGraph();

    } else if (gData.name == 0) {

      if (g.gest == db[kk::nextSceneGest] or g.gest == db[kk::previousSceneGest]) {

        int8_t move = g.gest == db[kk::nextSceneGest] ? 1 : -1;
        if (gData.scene + move < 0) {
          gData.scene = getScenesCount() - 1;
        } else {
          gData.scene = (gData.scene + move) % getScenesCount();
        }
        Serial.println(getScene());
        cursorGraph();

      } else if (g.gest == db[kk::plusBrightnessGest]) {

        db[kk::brightness] = (int(db[kk::brightness]) + (int)db[kk::plusBrightness]) % 256;
        cursorGraph();
        oled.setContrast(db[kk::brightness]);

      } else if (g.gest == db[kk::minusBrightnessGest]) {

        db[kk::brightness] -= (int)db[kk::plusBrightness];
        if (db[kk::brightness] < 0) db[kk::brightness] = 255;
        cursorGraph();
        oled.setContrast(db[kk::brightness]);

      } else if (g.gest == db[kk::vibrationGest]) {

        db[kk::vibration] = !db[kk::vibration];
        cursorGraph();
      }
    } else {

      cursorGraph();
      oled.print(g.gest);
      sendGest();
    }
  } else if (g.gest == db[kk::nextSceneGest] or g.gest == db[kk::previousSceneGest]) {

    int8_t move = g.gest == db[kk::nextSceneGest] ? 1 : -1;
    if (gData.scene + move < 0) {
      gData.scene = getScenesCount() - 1;
    } else {
      gData.scene = (gData.scene + move) % getScenesCount();
    }
    Serial.println(getScene());
    cursorGraph();

  } else if (g.gest == db[kk::sceneGest]) {

    Serial.println(getScene());
    cursorGraph();
    sendScene();
  }
  gData.timer = millis();
}
