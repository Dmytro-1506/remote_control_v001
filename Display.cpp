#include "Display.h"

Display::Display(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t backlight,
                 uint16_t width, uint16_t height)
    : tft(cs, dc, rst), 
    backlightPin(backlight),
    screenWidth(width), 
    screenHeight(height)
{
}

void Display::begin()
{
    pinMode(backlightPin, OUTPUT);
    digitalWrite(backlightPin, HIGH);

    tft.init(screenHeight, screenWidth);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);

    u8g2.begin(tft);

    u8g2.setFont(u8g2_font_helvR18_tf);
    u8g2.setFontMode(1);
    u8g2.setFontDirection(0);
    u8g2.setForegroundColor(ST77XX_WHITE);

    u8g2.setCursor(30, 70);
    u8g2.print("Programm gestartet");
}

void Display::setCursor(int x, int y)
{
    u8g2.setCursor(x, y);
}

void Display::showText(const String &message, int textSize)
{
    tft.fillScreen(ST77XX_BLACK);

    switch (textSize)
    {
        case 14:
            u8g2.setFont(u8g2_font_helvR14_tf);
            break;

        case 18:
            u8g2.setFont(u8g2_font_helvR18_tf);
            break;

        case 24:
            u8g2.setFont(u8g2_font_helvR24_tf);
            break;

        default:
            u8g2.setFont(u8g2_font_helvR18_tf);
            break;
    }

    u8g2.print(message);
}

void Display::showImage(const uint16_t *image, int width, int height, int x, int y)
{
}

void Display::showImageTest()
{
    tft.fillScreen(ST77XX_BLACK);
    tft.fillRect(10, 10, 64, 64, ST77XX_GREEN);
}

void Display::showWaitingForNextMessage()
{
    tft.fillScreen(ST77XX_BLACK);

    u8g2.setFont(u8g2_font_helvR18_tf);
    u8g2.setCursor(30, 70);
    u8g2.print("Danke!");
    
    u8g2.setCursor(30, 110);
    u8g2.print("Warte auf neue Meldung.");
}

void Display::showApproveButton(const String &status)
{
    if (status == "OK")
    {
        tft.fillCircle(40, 125, 15, ST77XX_GREEN);

        u8g2.setCursor(75, 135);
        u8g2.setFont(u8g2_font_helvR18_tf);
        u8g2.print("JA");
        return;
    }
    else if (status == "FAIL")
    {
        tft.fillCircle(165, 125, 15, ST77XX_RED);

        u8g2.setCursor(200, 135);
        u8g2.setFont(u8g2_font_helvR18_tf);
        u8g2.print("NEIN");
        return;
    }
    else if (status == "NEW_PICTURE")
    {
        tft.fillCircle(40, 125, 15, ST77XX_BLUE);

        u8g2.setCursor(75, 135);
        u8g2.setFont(u8g2_font_helvR18_tf);
        u8g2.print("WEITER");
        return;
    }
}

void Display::clear()
{
    tft.fillScreen(ST77XX_BLACK);
}