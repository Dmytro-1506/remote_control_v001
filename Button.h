#pragma once

#include <Arduino.h>

enum class ButtonColor
{
    RED,
    GREEN,
    BLUE
};

/**
 * @brief Repräsentiert eine Taste mit Entprellung.
 */
class Button
{
public:
    /**
    * @brief Erstellt ein Button-Objekt.
    *
    * @param pin GPIO-Pin der Taste.
    * @param color Farbe der Taste.
    */
    Button(uint8_t pin, ButtonColor color);

    void begin();

    /**
    * @brief Prüft, ob die Taste gedrückt wurde.
    *
    * @return true, wenn eine neue Betätigung erkannt wurde, sonst false.
    */
    bool wasPressed();

private:
    uint8_t pin;
    ButtonColor color;

    bool rawState;
    bool stableState;

    unsigned long lastChange;
};