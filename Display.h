#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <U8g2_for_Adafruit_GFX.h>


/**
 * @brief Steuert die grafische Ausgabe auf dem TFT-Display.
 *
 * Die Klasse übernimmt die Initialisierung des Displays und stellt
 * Funktionen zur Anzeige von Texten, Bildern und Benutzeroberflächen
 * sowie zum Löschen des Display-Inhalts bereit.
 */
class Display
{
public:
    Display(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t backlight,
            uint16_t width, uint16_t height);

    void begin();
    void setCursor(int x, int y);

    void showText(const String &message, int textSize);
    void showImage(
        const uint16_t *image,
        int width,
        int height,
        int x,
        int y);

    void showApproveButton(const String &status);

    void showWaitingForNextMessage();
    void showImageTest();

    void clear();

private:
    Adafruit_ST7789 tft;
    U8G2_FOR_ADAFRUIT_GFX u8g2;

    uint8_t backlightPin;
    uint16_t screenWidth;
    uint16_t screenHeight;
};
