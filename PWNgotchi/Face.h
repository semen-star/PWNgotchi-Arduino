#ifndef FACE_H
#define FACE_H
#include <Adafruit_SSD1306.h>

// Эмоции
enum Emotion { NORMAL, HAPPY, HACKING, SLEEP };

void drawEyes(Adafruit_SSD1306 &d, int x, int y, Emotion state) {
  int r = 12;
  d.clearDisplay();
  
  // Рисуем рамку
  d.drawRect(0, 0, 128, 64, WHITE);
  
  switch(state) {
    case NORMAL:
      d.fillCircle(x - 25, y, r, WHITE);
      d.fillCircle(x + 25, y, r, WHITE);
      d.drawLine(x - 10, y + 15, x + 10, y + 15, WHITE); // Рот
      break;
      
    case HAPPY:
      // Глаза дуги ^ ^
      d.drawLine(x - 35, y, x - 25, y - 10, WHITE); d.drawLine(x - 25, y - 10, x - 15, y, WHITE);
      d.drawLine(x + 15, y, x + 25, y - 10, WHITE); d.drawLine(x + 25, y - 10, x + 35, y, WHITE);
      d.setCursor(x - 5, y + 10); d.print("UWU");
      break;
      
    case HACKING:
      // Глаза как терминалы [ ]
      d.drawRect(x - 35, y - 10, 20, 20, WHITE); d.fillRect(x - 30, y - 5, 10, 10, WHITE);
      d.drawRect(x + 15, y - 10, 20, 20, WHITE); d.fillRect(x + 20, y - 5, 10, 10, WHITE);
      d.setCursor(40, 50); d.print("PWNING...");
      break;
      
    case SLEEP:
       d.setCursor(x - 20, y); d.print("Z z Z");
       d.drawLine(x - 10, y + 15, x + 10, y + 15, WHITE);
       break;
  }
  d.display();
}
#endif
