void sendPacket(String msg) {
  String s = db[kk::header];
  s += msg;
  mqtt.publish(db[kk::remote].c_str(), s.c_str());
}

void mqttTick() {
  if (WiFi.status() != WL_CONNECTED) return;
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();
}

void connectMQTT() {
  static uint32_t timer;
  if (millis() - timer >= 1000) {
    timer = millis();
    String id("GestureXY-");
    id += String(random(0xffffff), HEX);
    if (mqtt.connect(id.c_str())) mqtt.subscribe(db[kk::local].c_str());
  }
}

void callback(char* topic, byte* payload, uint16_t len) {
  payload[len] = '\0';
  String str = (char*)payload;
  if (!str.startsWith(db[kk::header])) return;
  Serial.println(str);
  String data = str.substring(db[kk::header].length());
  Serial.println(data);
  if (data == g.getRUGest(db[kk::plusBrightnessGest])) {
    db[kk::brightness] += (byte)db[kk::plusBrightness];
    FastLED.setBrightness(db[kk::brightness]);
    FastLED.show();
    String s = "Яркость лампы: ";
    s += db[kk::brightness].c_str();
    sendPacket(s);
  } else if (data == g.getRUGest(db[kk::minusBrightnessGest])) {
    db[kk::brightness] -= (byte)db[kk::plusBrightness];
    FastLED.setBrightness(db[kk::brightness]);
    FastLED.show();
    String s = "Яркость лампы: ";
    s += db[kk::brightness].c_str();
    sendPacket(s);
  } else if (data == g.getRUGest(db[kk::stateGest])) {
    db[kk::state] = !db[kk::state];
    if (db[kk::state]) light();
    else {
      for (int i = 0; i < NUM; i++) {
        leds[i].setRGB(0, 0, 0);
      }
      FastLED.show();
    }
    sendPacket(db[kk::state] ? "Лампа включилась" : "Лампа выключилась");
  } else if (data == g.getRUGest(db[kk::tempGest])) {
    gData.scene = ++gData.scene % 4;
    if (db[kk::state]) light();
    sendPacket(gData.scenes[gData.scene] + " свет лампы");
  } else if (data == g.getRUGest(db[kk::nextGest])) {
    gData.color = ++gData.color % 7;
    String msg;
    for (byte i = 0; i < 2; i++) {
      msg += String(gData.colors[gData.color][i]);
      msg += ", ";
    }
    msg += String(gData.colors[gData.color][2]);
    for (int i = 0; i < NUM; i++) {
      leds[i].setRGB(gData.colors[gData.color][0], gData.colors[gData.color][1], gData.colors[gData.color][2]);
    }
    FastLED.show();
    sendPacket(msg + " цвет лампы");
  } else if (data == g.getRUGest(db[kk::previousGest])) {
    gData.color--;
    if (gData.color < 0) gData.color = 0;
    String msg;
    for (byte i = 0; i < 2; i++) {
      msg += String(gData.colors[gData.color][i]);
      msg += ", ";
    }
    msg += String(gData.colors[gData.color][2]);
    for (int i = 0; i < NUM; i++) {
      leds[i].setRGB(gData.colors[gData.color][0], gData.colors[gData.color][1], gData.colors[gData.color][2]);
    }
    FastLED.show();
    sendPacket(msg + " цвет лампы");
  } else if (data == "Уход" or data == "Сон") {
    db[kk::state] = false;
    for (int i = 0; i < NUM; i++) {
      leds[i].setRGB(0, 0, 0);
    }
    FastLED.show();
  } else if (data == "Приход") {
    db[kk::state] = true;
    light();
  }
}
