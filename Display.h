#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

class Display
{
public:
    Display(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t backlight,
            uint16_t width, uint16_t height);

    void begin();

    void showText(const String &message);
    void showImage(
        const uint16_t *image,
        int width,
        int height,
        int x,
        int y);

    void showButtonsToChoose();

    void clear();

private:
    Adafruit_ST7789 tft;
    uint8_t backlightPin;
    uint16_t screenWidth;
    uint16_t screenHeight;
};
