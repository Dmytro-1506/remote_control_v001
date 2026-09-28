#pragma once

#include <Arduino.h>

enum class ButtonColor
{
    RED,
    GREEN,
    BLUE
};

class Button
{
public:
    Button(uint8_t pin, ButtonColor color);

    void begin();

    bool wasPressed();

private:
    uint8_t pin;
    ButtonColor color;

    bool rawState;
    bool stableState;

    unsigned long lastChange;
};