#include "Display.h"
#include "Button.h"
#include "Sound.h"
#include "LED.h"
#include "Wifi_server.h"
#include "Commands.h"

#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    startProgram();
    display.showImageTest();
}

void loop()
{
    //handleServer();
    handleClient();
    led.updateEffects();

    if (waitForApproval)
    {
        if (isPictureBeingChecked)
        {
            if (buttonGreen.wasPressed())
            {
                waitForApproval = false;
                isPictureBeingChecked = false;
                buzzer.playButton();
                led.stopPulse();
                display.showWaitingForNextMessage();

                // Antwort an Client senden
                handleButton("green", activeClient);
                Serial.println("Green button was pressed");
            }

            if (buttonRed.wasPressed())
            {
                waitForApproval = false;
                isPictureBeingChecked = false;
                buzzer.playButton();
                led.stopPulse();
                display.showWaitingForNextMessage();

                // andere Antwort senden
                handleButton("red", activeClient);
                Serial.println("Red button was pressed");
            }
        }
        if (materialsNeedToBeRemoved)
        {
            if (buttonBlue.wasPressed())
            {
                waitForApproval = false;
                materialsNeedToBeRemoved = false;
                buzzer.playButton();
                led.stopPulse();
                display.showWaitingForNextMessage();

                // andere Antwort senden
                handleButton("blue", activeClient);
                Serial.println("Blue button was pressed");
            }
        }
    }
}
