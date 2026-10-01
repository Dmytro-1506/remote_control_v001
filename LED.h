#pragma once

#include <Arduino.h>


/**
 * @brief Steuert eine RGB-LED.
 *
 * Ermöglicht das Setzen von RGB-Farben sowie das Ein- und Ausschalten
 * der LED. Unterstützt außerdem Blink- und Pulseffekte.
 */
class LED
{
public:
    LED(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);

    void begin();

    void setLED(uint8_t red, uint8_t green, uint8_t blue);
    void off();
    void on();

    void startBlink(unsigned long interval = 500);

    void stopBlink();

    void startPulse(
    uint8_t r1, uint8_t g1, uint8_t b1,
    uint8_t r2, uint8_t g2, uint8_t b2);

    void stopPulse();

    void updateEffects();

private:
    uint8_t redPin;
    uint8_t greenPin;
    uint8_t bluePin;

    uint8_t red;
    uint8_t green;
    uint8_t blue;

    bool isOn;
    bool blinking;
    bool pulsing;

    // Farb-Puls
    float colorPhase;
    unsigned long lastColor;

    // Helligkeits-Puls
    float brightnessPhase;
    unsigned long lastBrightness;

    unsigned long blinkInterval;
    unsigned long lastBlink;

    // Erste Farbe
    uint8_t pulseR1;
    uint8_t pulseG1;
    uint8_t pulseB1;

    // Zweite Farbe
    uint8_t pulseR2;
    uint8_t pulseG2;
    uint8_t pulseB2;
};