#include "Display.h"
#include "Button.h"
#include "Sound.h"
#include "LED.h"
#include "Wifi_server.h"
#include "Commands.h"

namespace
{
    // LED pins
    const int PIN_RED = 14;
    const int PIN_GREEN = 22;
    const int PIN_BLUE = 21;

    // button pins
    const int BUTTON_RED = 12;
    const int BUTTON_GREEN = 27;
    const int BUTTON_BLUE = 26;

    // sound pin
    const int PIEZO_PIN = 13;

    // display pins
    const int TFT_CS = 15;
    const int TFT_DC = 2;
    const int TFT_RST = 4;
    const int TFT_BL = 32;

    const int SCREEN_HEIGHT = 170;
    const int SCREEN_WIDTH = 320;
}

// create sound object
Sound sound(PIEZO_PIN);

// create button objects
Button buttonRed(BUTTON_RED, ButtonColor::RED);
Button buttonGreen(BUTTON_GREEN, ButtonColor::GREEN);
Button buttonBlue(BUTTON_BLUE, ButtonColor::BLUE);

// create display object
Display display(TFT_CS, TFT_DC, TFT_RST, TFT_BL, SCREEN_WIDTH, SCREEN_HEIGHT);

// create LED object
LED led(PIN_RED, PIN_GREEN, PIN_BLUE);

void setup()
{

    Serial.begin(115200);

    sound.begin();

    buttonRed.begin();
    buttonGreen.begin();
    buttonBlue.begin();

    display.begin();

    led.begin();
    led.setLED(255, 0, 0);
    delay(500);

    led.setLED(0, 255, 0);
    delay(500);

    led.setLED(0, 0, 255);
    delay(500);

    led.off();

    sound.playStart();
    display.showText("Status: OK");
    display.showButtonsToChoose();

    startWLAN();

    startServer();
}

void loop()
{

    led.update();

    if (buttonRed.wasPressed())
    {
        sound.playButton();
        display.showText("RED button pressed");
        Serial.println("RED");
        led.setLED(255, 0, 0);
    }

    if (buttonGreen.wasPressed())
    {
        sound.playButton();
        display.showText("GREEN button pressed");
        Serial.println("GREEN");
        led.setLED(0, 255, 0);
    }

    if (buttonBlue.wasPressed())
    {
        sound.playButton();
        display.showText("BLUE button pressed");
        Serial.println("BLUE");
        led.setLED(0, 0, 255);
    }

    handleServer();
}
