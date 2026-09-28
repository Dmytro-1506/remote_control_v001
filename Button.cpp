#include "Button.h"

namespace
{
    constexpr unsigned long DEBOUNCE_TIME = 50;
}

Button::Button(uint8_t pin, ButtonColor color)
  : pin(pin),
    color(color),
    rawState(HIGH),
    stableState(HIGH),
    lastChange(0)
{
}

void Button::begin()
{
    pinMode(pin, INPUT_PULLUP);
}

bool Button::wasPressed()
{
    bool reading = digitalRead(pin);

    // Rohzustand hat sich geändert
    if (reading != rawState)
    {
        rawState = reading;
        lastChange = millis();
    }

    // Zustand ist lange genug stabil
    if ((millis() - lastChange) >= DEBOUNCE_TIME)
    {
        if (stableState != rawState)
        {
            stableState = rawState;

            // Taste wurde gedrückt
            if (stableState == LOW)
            {
                return true;
            }
        }
    }

    return false;
}