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
  if (data == g.getRUGest(db[kk::pauseGest])) {
    tx.print("MEDIA_PAUSE");
  } else if (data == g.getRUGest(db[kk::muteGest])) {
    tx.print("MEDIA_VOLUME_MUTE");
  } else if (data == g.getRUGest(db[kk::backspaceGest])) {
    tx.print("KEY_BACKSPACE");
  } else if (data == g.getRUGest(db[kk::nextGest])) {
    tx.print("MEDIA_NEXT");
  } else if (data == g.getRUGest(db[kk::previousGest])) {
    tx.print("MEDIA_PREVIOUS");
  } else if (data == g.getRUGest(db[kk::upGest])) {
    tx.print("MEDIA_VOLUME_UP");
  } else if (data == g.getRUGest(db[kk::downGest])) {
    tx.print("MEDIA_VOLUME_DOWN");
  } else if (data == g.getRUGest(db[kk::powerGest])) {
    tx.print("SYSTEM_POWER_DOWN");
  } else if (data == g.getRUGest(db[kk::sleepGest])) {
    tx.print("SYSTEM_SLEEP");
  } else if (data == g.getRUGest(db[kk::wakeGest])) {
    tx.print("SYSTEM_WAKE_UP");
  } else if (data.startsWith("http") or data.startsWith("C:")) {
    tx.print(data);
  } else {
    for (byte i = 0; i < powerCount(); i++) {
      if (data == power(i)) {
        tx.print("SYSTEM_POWER_DOWN");
        break;
      }
    }
    for (byte i = 0; i < sleepCount(); i++) {
      if (data == sleep(i)) {
        tx.print("SYSTEM_SLEEP");
        break;
      }
    }
    for (byte i = 0; i < wakeCount(); i++) {
      if (data == wake(i)) {
        tx.print("SYSTEM_WAKE_UP");
        break;
      }
    }
    for (byte i = 0; i < pauseCount(); i++) {
      if (data == pause(i)) {
        tx.print("MEDIA_PAUSE");
        break;
      }
    }
  }
}

void sendPacket(String msg) {
  String s = db[kk::header];
  s += msg;
  mqtt.publish(db[kk::remote].c_str(), s.c_str());
}

String sleep(byte i) {
  ParsingXY p(db[kk::sep]);
  return p.sub(db[kk::sleepScenes].c_str(), i);
}

byte sleepCount() {
  ParsingXY p(db[kk::sep]);
  return p.amount(db[kk::sleepScenes].c_str());
}

String power(byte i) {
  ParsingXY p(db[kk::sep]);
  return p.sub(db[kk::powerScenes].c_str(), i);
}

byte powerCount() {
  ParsingXY p(db[kk::sep]);
  return p.amount(db[kk::powerScenes].c_str());
}

String wake(byte i) {
  ParsingXY p(db[kk::sep]);
  return p.sub(db[kk::wakeScenes].c_str(), i);
}

byte wakeCount() {
  ParsingXY p(db[kk::sep]);
  return p.amount(db[kk::wakeScenes].c_str());
}

String pause(byte i) {
  ParsingXY p(db[kk::sep]);
  return p.sub(db[kk::pauseScenes].c_str(), i);
}

byte pauseCount() {
  ParsingXY p(db[kk::sep]);
  return p.amount(db[kk::pauseScenes].c_str());
}
