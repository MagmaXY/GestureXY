![preview](https://github.com/user-attachments/assets/b0beff92-4d65-4c91-98f8-278185b91c6d)

# 🤟 GestureXY
Носимое устройство - контроллер жестами и mqtt запросами на esp8266.

## 🗒 Описание проекта
Данный проект позволяет в индивидуальном порядке дистанционно, удобно, быстро и интуитивно управлять жестами устройствами с интеграцией с mqtt сервером.

### 📑 Комплектующие:
* ESP8266 Wemos d1 mini
* OLED SSD1306_128x32
* paj7620
* Li-ion аккумулятор
* Модуль заряда Power Bank для Li-ion аккумуляторов (улучшенный TP4056) c преобразователем 5V (MT3608) и выходом USB-A TPC-5V-2A
* Модульный вибропривод
* Кнопка
### 📑 [Корпус:](https://github.com/MagmaXY/GestureXY/tree/main/3d%20%20models)
* Пластик
* Макетная паяльная плата
* Штырьки
* Провода
* Резинки
* Термоклей
### 📑 [Фунционал:](https://github.com/MagmaXY/GestureXY/tree/main/firmware)
* 15 линейно-осевых жестов
* Настройка яркости дисплея и вибрации
* Вкл/Выкл управления жестами
* Переключение устройств и отправка им жестов
* Переключение сценариев и их отправка всем устройствам
* Управление кнопкой: зажать для следующей сети при подключении; кликнуть для отображения локальной сети; отжать для перезагрузки устройства
* Автоотключение через заданное время
* Веб приложение:

![webface](https://github.com/user-attachments/assets/f8ae2ce2-1946-45fa-9480-74e9c1cea22d)

### 🛐 Зависимости:
* [PubSubClient](https://github.com/knolleary/pubsubclient) 
* [Settings](https://github.com/GyverLibs/Settings) 
* [GyverDB](https://github.com/GyverLibs/GyverDB) 
* [LittleFS](https://github.com/littlefs-project/littlefs) 
* [GyverOLED](https://github.com/GyverLibs/GyverOLED) 
* [EncButton](https://github.com/GyverLibs/EncButton) 
* [paj7620](https://github.com/Seeed-Studio/Grove_Gesture) 

