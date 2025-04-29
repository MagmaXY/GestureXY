![GestureXY](https://github.com/MagmaXY/GestureXY/blob/main/device.png)

# [🤟 GestureXY](https://drive.google.com/drive/folders/1p2T1dCGA9s0eW435RHl4DeHti2FkGzYV)
Носимое устройство - контроллер жестами и mqtt запросами на базе ESP8266.

## 🗒 [Описание проекта](https://github.com/MagmaXY/GestureXY/blob/main/README.md)
Данный проект позволяет в индивидуальном порядке дистанционно, удобно, быстро и интуитивно управлять жестами устройствами в интеграции с mqtt сервером.

### 📑 [Комплектующие:](https://github.com/MagmaXY/GestureXY/blob/main/scheme.png)
* ESP8266 Wemos d1 mini
* OLED SSD1306_128x32
* paj7620
* Li-ion аккумулятор
* Модуль заряда Power Bank для Li-ion аккумуляторов (улучшенный TP4056) c преобразователем 5V (MT3608) и выходом USB-A TPC-5V-2A
* Модульный вибропривод
* Кнопка
### 📑 [Корпус:](https://github.com/MagmaXY/GestureXY/tree/main/3d%20%20models)
* Пластик
* [Плата](https://oshwlab.com/magmaxy/pcb_smd_gesturexy)
  
 ![pcb](https://github.com/user-attachments/assets/9ee1ceb0-2e2e-4c00-a577-98d96819bde7)

* Штырьки
* Ремень
* Болты
### 📑 [Фунционал:](https://github.com/MagmaXY/GestureXY/tree/main/firmware)
* 15 линейно-осевых жестов (инфракрасное распознавание)
* Настройка яркости дисплея и вибрации
* Вкл/Выкл управления жестами
* Автоотключение управления через заданное время
* Переключение устройств и отправка им жестов
* Переключение сценариев и их отправка всем устройствам
* Управление кнопкой: кликнуть для включения устройства; дважды кликнуть для выключения устройства; зажать для следующей сети при подключении; кликнуть для отображения локальной сети; зажать для перезагрузки устройства
* Веб приложение с сохраняемой базой данных:
  
![Веб приложение](https://github.com/user-attachments/assets/4be93591-d852-42bd-837f-b62f5766fe21)



### 🛐 [Зависимости:](https://www.arduino.cc/en/software)
* [PubSubClient](https://github.com/knolleary/pubsubclient) 
* [Settings](https://github.com/GyverLibs/Settings) 
* [GyverDB](https://github.com/GyverLibs/GyverDB) 
* [LittleFS](https://github.com/littlefs-project/littlefs) 
* [GyverOLED](https://github.com/GyverLibs/GyverOLED) 
* [EncButton](https://github.com/GyverLibs/EncButton) 
* [paj7620](https://github.com/Seeed-Studio/Grove_Gesture) 
