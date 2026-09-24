#include "display_functions.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TFT_CS 15
#define TFT_DC 2
#define TFT_RST 4
#define TFT_BL 32

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void initDisplay()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    tft.init(170, 320);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);

    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.setCursor(10, 10);
}

void displayTextMessage(const String &message)
{
    tft.println(message);
}

void displayColorFigure(int r, int g, int b) 
{
    uint16_t color = tft.color565(r, g, b);

    int16_t x = 295;
    int16_t y = 20;
    int16_t radius = 15;

    tft.fillCircle(x, y, radius, color);
}