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
    s += gest(db[kk::brightness]);
    sendPacket(s);
  } else if (data == g.getRUGest(db[kk::minusBrightnessGest])) {
    db[kk::brightness] -= (byte)db[kk::plusBrightness];
    FastLED.setBrightness(db[kk::brightness]);
    FastLED.show();
    String s = "Яркость лампы: ";
    s += gest(db[kk::brightness]);
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
    gData.color = ++gData.color % 8;
    String msg;
    for (byte i = 0; i < 2; i++) {
      msg += String(gData.colors[gData.color][i]);
      msg += ", ";
    }
    msg += String(gData.colors[gData.color][2]);
    if (db[kk::state]) {
      for (int i = 0; i < NUM; i++) {
        leds[i].setRGB(gData.colors[gData.color][0], gData.colors[gData.color][1], gData.colors[gData.color][2]);
      }
    }
    FastLED.show();
    sendPacket(msg + " цвет лампы");
  } else if (data == g.getRUGest(db[kk::previousGest])) {
    gData.color--;
    if (gData.color < 0) gData.color = 8;
    String msg;
    for (byte i = 0; i < 2; i++) {
      msg += String(gData.colors[gData.color][i]);
      msg += ", ";
    }
    msg += String(gData.colors[gData.color][2]);
    if (db[kk::state]) {
      for (int i = 0; i < NUM; i++) {
        leds[i].setRGB(gData.colors[gData.color][0], gData.colors[gData.color][1], gData.colors[gData.color][2]);
      }
    }
    FastLED.show();
    sendPacket(msg + " цвет лампы");
  } else {
    for (byte i = 0; i < offCount(); i++) {
      if (data == off(i)) {
        db[kk::state] = false;
        for (int i = 0; i < NUM; i++) {
          leds[i].setRGB(0, 0, 0);
        }
        FastLED.show();
        break;
      }
    }
    for (byte i = 0; i < onCount(); i++) {
      if (data == on(i)) {
        db[kk::state] = true;
        light();
        break;
      }
    }
    for (byte i = 0; i < workCount(); i++) {
      if (data == work(i)) {
        gData.scene = 2;
        db[kk::state] = true;
        light();
        break;
      }
    }
    for (byte i = 0; i < chillCount(); i++) {
      if (data == chill(i)) {
        gData.scene = 3;
        db[kk::state] = true;
        light();
        break;
      }
    }
  }
}

String gest(String g) {
  return g;
}

String on(byte i) {
  ParsingXY p(db[kk::sep]);
  return p.sub(db[kk::onScenes].c_str(), i);
}

byte onCount() {
  ParsingXY p(db[kk::sep]);
  return p.amount(db[kk::onScenes].c_str());
}

String off(byte i) {
  ParsingXY p(db[kk::sep]);
  return p.sub(db[kk::offScenes].c_str(), i);
}

byte offCount() {
  ParsingXY p(db[kk::sep]);
  return p.amount(db[kk::offScenes].c_str());
}

String work(byte i) {
  ParsingXY p(db[kk::sep]);
  return p.sub(db[kk::workScenes].c_str(), i);
}

byte workCount() {
  ParsingXY p(db[kk::sep]);
  return p.amount(db[kk::workScenes].c_str());
}

String chill(byte i) {
  ParsingXY p(db[kk::sep]);
  return p.sub(db[kk::chillScenes].c_str(), i);
}

byte chillCount() {
  ParsingXY p(db[kk::sep]);
  return p.amount(db[kk::chillScenes].c_str());
}
