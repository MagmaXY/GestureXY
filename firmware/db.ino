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
  db.init(kk::power, 255);
  db.init(kk::state, false);
  db.init(kk::reaction, 400);
  db.init(kk::quit, 800);
  db.init(kk::connect, false);
  db.init(kk::stateGest, "Up-Down");
  db.init(kk::vibrationGest, "Down-Up");
  db.init(kk::plusBrightnessGest, "Up");
  db.init(kk::minusBrightnessGest, "Down");
  db.init(kk::nextNameGest, "Right-Left");
  db.init(kk::previousNameGest, "Left-Right");
  db.init(kk::nextSceneGest, "Right");
  db.init(kk::previousSceneGest, "Left");
  db.init(kk::sceneGest, "Up");
}

void build(sets::Builder& b) {
  {
    sets::Group g(b, "🤖 Датчик");
    b.Slider(kk::reaction, "⏲ Реакция", 100, 1000, 100);
    b.Slider(kk::quit, "⏲ Ожидание", 100, 1000, 100);
    b.Slider(kk::turn, "🔄 Поворот", 0, 270, 90);
  }
  {
    sets::Group g(b, "🖼️ Дисплей");
    b.Slider(kk::brightness, "🔆 Яркость", 0, 255, 1);
    b.Slider(kk::ons, "🔅 Активное управление", 0, 15000, 1000);
    b.Switch(kk::state, "📴 Управление");
    if (b.Button("fill"_h, "🪣 Залить")) oled.rect(0, 0, 128, 31, OLED_FILL);
  }
  {
    sets::Group g(b, "📳 Вибрация");
    b.Switch(kk::vibration, "🚩 Состояние");
    b.Slider(kk::power, "📲 Сила", 0, 255, 1);
    b.Slider(kk::searchs, "🕑 Поиск", 500, 15000, 500);
    if (b.Button("search"_h, "🔎 Поиск")) vibro.on(db[kk::searchs]);
  }
  {
    sets::Group g(b, "👋 Жесты");
    b.Input(kk::stateGest, "📴 Управление");
    b.Input(kk::vibrationGest, "🚩 Вибрация");
    b.Input(kk::plusBrightnessGest, "🔆 Яркость");
    b.Input(kk::minusBrightnessGest, "🔅 Яркость");
    b.Input(kk::nextNameGest, "🔜 Устройство");
    b.Input(kk::previousNameGest, "🔚 Устройство");
    b.Input(kk::nextSceneGest, "🔜 Сценарий");
    b.Input(kk::previousSceneGest, "🔚 Сценарий");
    b.Input(kk::sceneGest, "📨 Сценарий");
  }
  {
    sets::Group g(b, "👨‍💻 Serial");
    b.Input(kk::serial, "🕓 Бод");
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
    b.Input(kk::ssid, "🌐 Сеть");
    b.Pass(kk::pass, "🔑 Пароль");
    b.Input(kk::ssid1, "🌐 реСеть");
    b.Pass(kk::pass1, "🔑 реПароль");
  }
  {
    if (b.Button("setup"_h, "🔂 Перезагрузить")) {
      ESP.restart();
    }
  }

  {
    b.Link("🔗 GestureXY от MagmaXY", "https://github.com/MagmaXY/GestureXY");
  }


  if (b.build.isAction()) {
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
      case kk::power:
        vibro.setPower(b.build.value);
        break;
    }
    gData.timer = millis();
  }
}
