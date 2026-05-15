void db_init() {
  LittleFS.begin();
  db.begin();

  db.init(kk::ssid, "53");
  db.init(kk::pass, "Zz11116666");
  db.init(kk::ssid1, "Dima");
  db.init(kk::pass1, "MagmaXY2020");
  db.init(kk::host, "broker.hivemq.com");
  db.init(kk::port, 1883);
  db.init(kk::header, "GXY:");
  db.init(kk::local, "GXY_Лампа");
  db.init(kk::remote, "GXY_Контроллер");
  db.init(kk::serial, 115200);
  db.init(kk::brightness, 127);
  db.init(kk::state, false);
  db.init(kk::stateGest, 0);
  db.init(kk::tempGest, 1);
  db.init(kk::plusBrightnessGest, 4);
  db.init(kk::plusBrightness, 32);
  db.init(kk::minusBrightnessGest, 5);
  db.init(kk::previousGest, 2);
  db.init(kk::nextGest, 3);
  db.init(kk::sep, ',');
  db.init(kk::offScenes, "Уход,Сон");
  db.init(kk::onScenes, "Приход");
  db.init(kk::workScenes, "Работа");
  db.init(kk::chillScenes, "Отдых");
}

void build(sets::Builder& b) {
  {
    sets::Group g(b, "🏮 Лампа");
    b.Slider(kk::brightness, "🔆 Яркость", 0, 255, 1);
    b.Switch(kk::state, "📴 Свет");
  }
  {
    sets::Group g(b, "👋 Жесты");
    String gests = "Вперёд;Назад;Вперёд-Назад;Назад-Вперёд;Вправо;Влево;Вправо-Влево;Влево-Вправо;Вниз;Вверх;Вниз-Вверх;Вверх-Вниз;По часовой;Против часовой;Волна";
    b.Select(kk::stateGest, "📴 Свет", gests);
    b.Select(kk::tempGest, "🔜 Температура", gests);
    b.Select(kk::plusBrightnessGest, "🔆 Яркость", gests);
    b.Slider(kk::plusBrightness, "🎇 Изменение яркости", 0, 255, 1);
    b.Select(kk::minusBrightnessGest, "🔅 Яркость", gests);
    b.Select(kk::nextGest, "🔜 Цвет", gests);
    b.Select(kk::previousGest, "🔚 Цвет", gests);
  }
  {
    sets::Group g(b, "👨‍💻 Serial");
    b.Number(kk::serial, "🕓 Бод");
  }
  {
    sets::Group g(b, "⚙ MQTT");
    b.Input(kk::host, "🖥 Хост");
    b.Number(kk::port, "🔢 Порт");
    b.LED("☁ Статус", mqtt.connected());
    b.Input(kk::header, "📨 Заголовок");
    b.Input(kk::local, "🏮 Устройство");
    b.Input(kk::remote, "🤖 Устройство");
  }
  {
    sets::Group g(b, "📨 Сценарии");
    b.Input(kk::sep, "✏️ Символ-разделитель");
    b.Input(kk::onScenes, "📜 Включение");
    b.Input(kk::offScenes, "📑 Выключение");
    b.Input(kk::workScenes, "📜 Нормальный свет");
    b.Input(kk::chillScenes, "📑 Тёплый свет");
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
        FastLED.setBrightness(b.build.value);
        FastLED.show();
        break;
      case kk::state:
        Serial.println(b.build.value.toInt() ? "Активный" : "Неактивный");
        if (b.build.value.toInt()) light();
        else {
          for (int i = 0; i < NUM; i++) {
            leds[i].setRGB(0, 0, 0);
          }
          FastLED.show();
        }
        break;
    }
  }
}
