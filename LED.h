#pragma once

#include <Arduino.h>

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
    void update();

private:
    uint8_t redPin;
    uint8_t greenPin;
    uint8_t bluePin;

    uint8_t red;
    uint8_t green;
    uint8_t blue;

    bool isOn;
    bool blinking;

    unsigned long blinkInterval;
    unsigned long lastBlink;
};