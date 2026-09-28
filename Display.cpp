#include "Display.h"

Display::Display(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t backlight,
                 uint16_t width, uint16_t height)
    : tft(cs, dc, rst), backlightPin(backlight),
      screenWidth(width), screenHeight(height)
{
}

void Display::begin()
{
    pinMode(backlightPin, OUTPUT);
    digitalWrite(backlightPin, HIGH);

    tft.init(screenHeight, screenWidth);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);

    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    // tft.setCursor(10, 10);
}

void Display::showText(const String &message)
{
    tft.fillRect(0, 0, screenWidth, 20, ST77XX_BLACK);
    tft.setCursor(10, 10);
    tft.println(message);
}

void Display::showImage(const uint16_t *image, int width, int height, int x, int y)
{
}

void Display::showButtonsToChoose()
{
    tft.fillCircle(150, 100, 15, ST77XX_RED);
    tft.fillCircle(200, 100, 15, ST77XX_GREEN);
    tft.fillCircle(250, 100, 15, ST77XX_BLUE);
}

void Display::clear()
{
    tft.fillScreen(ST77XX_BLACK);
}