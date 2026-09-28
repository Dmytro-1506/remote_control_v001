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
      blinkInterval(500),
      lastBlink(0)
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

void LED::update()
{
    if (!blinking)
    {
        return;
    }

    if (millis() - lastBlink >= blinkInterval)
    {
        lastBlink = millis();

        if (isOn)
        {
            off();
        }
        else
        {
            on();
        }
    }
}