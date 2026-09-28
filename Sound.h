#pragma once

#include <Arduino.h>

class Sound
{
public:
    Sound(uint8_t pin);

    void begin();

    void playTone(
        const unsigned int frequency[],
        const unsigned int duration[],
        size_t length);

    void playStart();
    void playOK();
    void playFail();
    void playRequest();
    void playReset();
    void playButton();
    void playConnection();
    void playDisconnect();

    void playEnd();
    void stop();

private:
    uint8_t pin;
};
