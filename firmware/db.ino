void db_init() {
  LittleFS.begin();
  db.begin();

  db.init(kk::ssid, "53");
  db.init(kk::pass, "Zz11116666");
  db.init(kk::ssid1, "Dima");
  db.init(kk::pass1, "MagmaXY2020");
  db.init(kk::host, "test.mosquitto.org");
  db.init(kk::port, 1883);
  db.init(kk::header, "GXY:");
  db.init(kk::names, "GXY_Controller,GXY_Home,GXY_Lamp");
  db.init(kk::scenes, "None,Wake up,Away,Comeback,Cinema,Sleep");
  db.init(kk::sep, ',');
  db.init(kk::serial, 115200);
  db.init(kk::searchs, 2000);
  db.init(kk::ons, 5000);
  db.init(kk::brightness, 127);
  db.init(kk::turn, 90);
  db.init(kk::vibration, true);
  db.init(kk::state, false);
  db.init(kk::reaction, 400);
  db.init(kk::quit, 800);
  db.init(kk::connect, false);
}

void build(sets::Builder& b) {
  {
    sets::Group g(b, "🛠 Устройство");
    b.Slider(kk::reaction, "⏲ Реакция датчика", 100, 1000, 100);
    b.Slider(kk::quit, "⏲ Ожидание датчика", 100, 1000, 100);
    b.Slider(kk::turn, "🔄 Поворот датчика", 0, 270, 90);
    b.Slider(kk::brightness, "🔆 Яркость дисплея", 0, 255, 1);
    b.Slider(kk::ons, "🔅 Активное управление", 0, 15000, 1000);
    b.Switch(kk::vibration, "📳 Вибрация");
    b.Switch(kk::state, "📴 Управление жестами");
  }
  {
    sets::Group g(b, "⚙ MQTT");
    b.Input(kk::host, "🖥 Хост");
    b.Input(kk::port, "🔢 Порт");
    b.LED(kk::connect, "☁ Статус", mqtt.connected());
    b.Input(kk::header, "📨 Заголовок");
    b.Input(kk::sep, "✏️ Символ-разделитель");
    b.Input(kk::names, "📜 Устройства");
    b.Input(kk::scenes, "📑 Сценарии");
  }
  {
    sets::Group g(b, "📶 WiFi");
    b.Input(kk::ssid, "🌐 SSID");
    b.Pass(kk::pass, "🔑 Pass");
    b.Input(kk::ssid1, "🌐 reSSID");
    b.Pass(kk::pass1, "🔑 rePass");
  }
  {
    sets::Group g(b, "👨‍💻 Serial");
    b.Input(kk::serial, "🕓 Бод");
  }
  {
    sets::Group g(b, "🔍 Поиск");
    b.Slider(kk::searchs, "🕑 Долгота", 500, 15000, 500);
    if (b.Button("search"_h, "🔎 Поиск")) vibro.on(db[kk::searchs]);
  }

  {
    if (b.Button("fill"_h, "🪣 Залить дисплей")) oled.rect(0, 0, 128, 31, OLED_FILL);
  }

  {
    if (b.Button("setup"_h, "🔂 Перезагрузить")) {
      ESP.restart();
    }
  }

  {
    b.Link("🔗 GestureXY by MagmaXY", "https://github.com/MagmaXY/GestureXY");
  }


  if (b.build.isAction()) {
    gData.timer = millis();
    switch (b.build.id) {
      case kk::brightness:
        oled.setContrast(b.build.value);
        break;
      case kk::state:
        Serial.println(b.build.value.toInt() ? "Active" : "Inactive");
        oled.clear();
        vibro.on(500);
        if (b.build.value.toInt()) cursorGraph();
        break;
      case kk::names:
        if (db[kk::state] and gData.scene == 0) cursorGraph();
        break;
      case kk::scenes:
        if (!b.build.value.startsWith("None,"))
          db[kk::scenes] = "None," + String(b.build.value);
        if (db[kk::state] and gData.scene != 0) cursorGraph();
        break;
      case kk::vibration:
        if (db[kk::state]) cursorGraph();
        vibro.flag((bool)b.build.value);
        break;
      case kk::turn:
        g.setTurn(b.build.value);
        break;
      case kk::sep:
        if (db[kk::state]) cursorGraph();
        break;
      case kk::quit:
        g.setQuit(b.build.value);
        break;
      case kk::reaction:
        g.setReaction(b.build.value);
        break;
    }
  }
}
