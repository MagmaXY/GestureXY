void db_init() {
  LittleFS.begin();
  db.begin();

  db.init(kk::ssid, "53");
  db.init(kk::pass, "Zz11116666");
  db.init(kk::ssid1, "Dima");
  db.init(kk::pass1, "MagmaXY2020");
  db.init(kk::host, "mqtt.eclipseprojects.io");
  db.init(kk::port, 1883);
  db.init(kk::header, "GXY:");
  db.init(kk::local, "GXY_Медиа");
  db.init(kk::remote, "GXY_Контроллер");
  db.init(kk::serial, 115200);
  db.init(kk::pauseGest, 0);
  db.init(kk::muteGest, 1);
  db.init(kk::backspaceGest, 2);
  db.init(kk::sleepGest, 3);
  db.init(kk::previousGest, 4);
  db.init(kk::nextGest, 5);
  db.init(kk::upGest, 6);
  db.init(kk::downGest, 7);
  db.init(kk::wakeGest, 9);
  db.init(kk::powerGest, 11);
  db.init(kk::sep, ',');
  db.init(kk::sleepScenes, "Уход");
  db.init(kk::powerScenes, "Сон");
  db.init(kk::wakeScenes, "Приход");
  db.init(kk::pauseScenes, "Работа");
  db.init(kk::str, "");
}

void build(sets::Builder& b) {
  {
    sets::Group g(b, "🎵 Медиа");
    b.Input(kk::str, "Сообщение");
    if (b.Button("STR"_h, "📨 Отправить")) {
      tx.print(" ");
      tx.print(db[kk::str]);
    }
    if (b.Button("RUEN"_h, "📨 Поменять язык")) {
      tx.print(" ");
      tx.print("RUEN");
    }
  }
  {
    sets::Group g(b, "👋 Жесты");
    String gests = "Вперёд;Назад;Вперёд-Назад;Назад-Вперёд;Вправо;Влево;Вправо-Влево;Влево-Вправо;Вниз;Вверх;Вниз-Вверх;Вверх-Вниз;По часовой;Против часовой;Волна";
    b.Select(kk::pauseGest, "⏯ Пауза", gests);
    b.Select(kk::muteGest, "🔇 Звук", gests);
    b.Select(kk::backspaceGest, " Пробел", gests);
    b.Select(kk::sleepGest, "😴 Сон", gests);
    b.Select(kk::previousGest, "🔚 Музыка", gests);
    b.Select(kk::nextGest, "🔜 Музыка", gests);
    b.Select(kk::upGest, "➕ Громкость", gests);
    b.Select(kk::downGest, "➖ Громкость", gests);
    b.Select(kk::wakeGest, "🥱 Разбудить", gests);
    b.Select(kk::powerGest, "🖥 Выключить", gests);
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
    b.Input(kk::sleepScenes, "📜 Сон");
    b.Input(kk::powerScenes, "📑 Выключение");
    b.Input(kk::wakeScenes, "📜 Пробуждение");
    b.Input(kk::pauseScenes, "📑 Пауза");
  }
  {
    sets::Group g(b, "📶 WiFi");
    b.Input(kk::ssid, "🌐 Сеть");
    b.Pass(kk::pass, "🔑 Пароль");
    b.Input(kk::ssid1, "🌐 реСеть");
    b.Pass(kk::pass1, "🔑 реПароль");
  }
  {
    if (b.Button("zavod"_h, "💾 Сброс до заводских настроек")) {
      db.clear();
      ESP.restart();
    }
    if (b.Button("setup"_h, "🔂 Перезагрузить")) {
      ESP.restart();
    }
  }

  {
    b.Link("🔗 GestureXY от MagmaXY", "https://github.com/MagmaXY/GestureXY");
  }
}
