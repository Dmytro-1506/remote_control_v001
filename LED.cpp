#include "LED.h"

LED::LED(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
    : redPin(redPin),
      greenPin(greenPin),
      bluePin(bluePin),
      red(0),
      green(0),
      blue(0),
      isOn(false),
      blinking(false),
      pulsing(false),
      blinkInterval(500),
      lastBlink(0),
      colorPhase(0.0),
      brightnessPhase(0.0),
      lastColor(0),
      lastBrightness(0)
{
}

void LED::begin()
{
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    off();
}

void LED::setLED(uint8_t red, uint8_t green, uint8_t blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;

    isOn = true;
    blinking = false;
    pulsing = false;

    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
}

void LED::off()
{
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);

    isOn = false;
}

void LED::on()
{
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);

    isOn = true;
}

void LED::startBlink(unsigned long interval)
{
    blinkInterval = interval;
    blinking = true;
    lastBlink = millis();
}

void LED::stopBlink()
{
    blinking = false;
    on();
}

void LED::startPulse(
    uint8_t r1, uint8_t g1, uint8_t b1,
    uint8_t r2, uint8_t g2, uint8_t b2)
{
    pulsing = true;
    blinking = false;

    colorPhase = 0.0;
    brightnessPhase = 0.0;

    lastColor = millis();
    lastBrightness = millis();

    pulseR1 = r1;
    pulseG1 = g1;
    pulseB1 = b1;

    pulseR2 = r2;
    pulseG2 = g2;
    pulseB2 = b2;
}

void LED::stopPulse()
{
    pulsing = false;
    on();
}

void LED::updateEffects()
{
    if (pulsing)
    {
        unsigned long now = millis();

        // ========================================
        // 1. FARBE
        // ========================================

        float dtColor = now - lastColor;
        lastColor = now;

        // Geschwindigkeit der Farbänderung
        colorPhase += dtColor * 0.003;

        if (colorPhase >= TWO_PI)
        {
            colorPhase -= TWO_PI;
        }

        float colorValue = sin(colorPhase);

        uint8_t currentR = 0;
        uint8_t currentG = 0;
        uint8_t currentB = 0;

        if (colorValue >= 0)
        {
            // Farbe 1: Rot
            currentR = pulseR1 * colorValue;
            currentG = pulseG1 * colorValue;
            currentB = pulseB1 * colorValue;
        }
        else
        {
            // Farbe 2: Grün
            float amount = -colorValue;

            currentR = pulseR2 * amount;
            currentG = pulseG2 * amount;
            currentB = pulseB2 * amount;
        }


        // ========================================
        // 2. HELLIGKEIT
        // ========================================

        float dtBrightness = now - lastBrightness;
        lastBrightness = now;

        // Geschwindigkeit der Helligkeitspulsierung
        brightnessPhase += dtBrightness * 0.006;

        if (brightnessPhase >= TWO_PI)
        {
            brightnessPhase -= TWO_PI;
        }

        float brightness = 0.5 * ((sin(brightnessPhase) + 1.0) / 2.0);


        // ========================================
        // 3. LED AUSGEBEN
        // ========================================

        analogWrite(
            redPin,
            currentR * brightness
        );

        analogWrite(
            greenPin,
            currentG * brightness
        );

        analogWrite(
            bluePin,
            currentB * brightness
        );

        return;
    }

    if (!blinking)
    {
        return;
    }

    if (millis() - lastBlink >= blinkInterval)
    {
        lastBlink = millis();

        if (isOn)
            off();
        else
            on();
    }
}