#include "Button.h"

namespace
{
    constexpr unsigned long DEBOUNCE_TIME = 50;
}

/**
 * @brief Erstellt ein Button-Objekt.
 *
 * @param pin GPIO-Pin der Taste.
 * @param color Farbe der Taste.
 */
Button::Button(uint8_t pin, ButtonColor color)
  : pin(pin),
    color(color),
    rawState(HIGH),
    stableState(HIGH),
    lastChange(0)
{
}

/**
 * @brief Initialisiert den GPIO-Pin mit internem Pull-up-Widerstand.
 */
void Button::begin()
{
    pinMode(pin, INPUT_PULLUP);
}

/**
 * @brief Prüft, ob die Taste neu gedrückt wurde.
 *
 * @return true, wenn eine Betätigung erkannt wurde, sonst false.
 */
bool Button::wasPressed()
{
    bool reading = digitalRead(pin);

    // Rohzustand hat sich geändert und Entprellzeit wird gestartet
    if (reading != rawState)
    {
        rawState = reading;
        lastChange = millis();
    }

    // Zustand ist seit der Entprellzeit stabil
    if ((millis() - lastChange) >= DEBOUNCE_TIME)
    {
        if (stableState != rawState)
        {
            stableState = rawState;

            // Neuer stabiler Zustand: Taste wurde gedrückt
            if (stableState == LOW)
            {
                return true;
            }
        }
    }

    return false;
}