#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Face.h"

#define SCREEN_W 128
#define SCREEN_H 64

TwoWire I2C_1 = TwoWire(0);
TwoWire I2C_2 = TwoWire(1);

Adafruit_SSD1306 displayFace(SCREEN_W, SCREEN_H, &I2C_1, -1);
Adafruit_SSD1306 displayLogs(SCREEN_W, SCREEN_H, &I2C_2, -1);

// Параметры персонажа
float happiness = 100.0;
int expPoints = 0;
int level = 1;
int netsSeenTotal = 0;

String currentSSID = "NONE";
int currentRSSI = 0;
String incomingData = "";

void setup() {
  Serial.begin(115200);
Serial2.begin(115200, SERIAL_8N1, 4, 2); 

  
  I2C_1.begin(21, 22, 400000);
  I2C_2.begin(33, 32, 400000); 

  displayFace.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  displayLogs.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  displayFace.clearDisplay();
  displayLogs.clearDisplay();
}

void loop() {
  // 1. Слушаем "Нос" (NodeMCU)
  while (Serial2.available()) {
    char c = Serial2.read();
    if (c == '\n') {
      parseData(incomingData);
      incomingData = "";
    } else {
      incomingData += c;
    }
  }

  // 2. Жизненные циклы
  static unsigned long lastTick = 0;
  if (millis() - lastTick > 1000) {
    happiness -= 0.1; // Счастье медленно падает (голодает без сетей)
    if (happiness < 0) happiness = 0;
    lastTick = millis();
    updateLogScreen(); // Обновляем инфо-экран каждую секунду
  }

  // 3. Анимация лица
  Emotion currentEmo = NORMAL;
  if (happiness < 30) currentEmo = SLEEP;
  if (currentRSSI > -50) currentEmo = HACKING;
  
  static unsigned long lastFace = 0;
  if (millis() - lastFace > 50) {
    drawEyes(displayFace, 64, 32, currentEmo);
    lastFace = millis();
  }
}

void parseData(String data) {
  if (data.startsWith("NET:")) {
    netsSeenTotal++;
    happiness += 5.0; // Нашел сеть — обрадовался!
    if (happiness > 100) happiness = 100;
    
    expPoints += 10; // Получаем опыт
    if (expPoints >= 100) {
      level++;
      expPoints = 0;
    }

    String params = data.substring(4);
    int firstPipe = params.indexOf('|');
    int secondPipe = params.lastIndexOf('|');
    currentRSSI = params.substring(0, firstPipe).toInt();
    currentSSID = params.substring(firstPipe + 1, secondPipe);
  }
}

void updateLogScreen() {
  displayLogs.clearDisplay();
  displayLogs.setTextSize(1);
  displayLogs.setTextColor(WHITE);
  
  // Верхняя статус-панель
  displayLogs.setCursor(0, 0);
  displayLogs.print("LVL:"); displayLogs.print(level);
  displayLogs.print(" HAPPY:"); displayLogs.print((int)happiness); displayLogs.println("%");
  displayLogs.drawLine(0, 9, 128, 9, WHITE);
  
  // Информация о сети
  displayLogs.setCursor(0, 15);
  displayLogs.print("SCANNING..."); 
  displayLogs.setCursor(0, 25);
  displayLogs.print("> "); displayLogs.println(currentSSID);
  
  // Полоска опыта
  displayLogs.setCursor(0, 45);
  displayLogs.print("EXP:");
  displayLogs.drawRect(30, 45, 90, 7, WHITE);
  displayLogs.fillRect(32, 47, map(expPoints, 0, 100, 0, 86), 3, WHITE);

  // Общее кол-во найденного
  displayLogs.setCursor(0, 56);
  displayLogs.print("TOTAL DISCOVERED: "); displayLogs.print(netsSeenTotal);

  displayLogs.display();
}
