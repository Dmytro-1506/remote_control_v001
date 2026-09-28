#include "Sound.h"

Sound::Sound(uint8_t pin)
{
    this->pin = pin;
}

void Sound::begin()
{
    pinMode(pin, OUTPUT);
}

void Sound::playTone(const unsigned int frequency[], const unsigned int duration[], size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        tone(pin, frequency[i], duration[i]);
        delay(duration[i]);

        // Pause zwischen den Tönen
        noTone(pin);
        delay(15);
    }

    noTone(pin);
}

void Sound::playStart()
{
    const unsigned int notes[] = {
        440, 554, 659};

    const unsigned int durations[] = {
        100, 100, 200};

    playTone(notes, durations, 3);
}

void Sound::playOK()
{
    const unsigned int notes[] = {
        784, 1047};

    const unsigned int durations[] = {
        100, 180};

    playTone(notes, durations, 2);
}

void Sound::playFail()
{
    const unsigned int notes[] = {
        500, 350};

    const unsigned int durations[] = {
        180, 300};

    playTone(notes, durations, 2);
}

void Sound::playRequest()
{
    const unsigned int notes[] = {
        700, 900};

    const unsigned int durations[] = {
        100, 100};

    playTone(notes, durations, 2);
}

void Sound::playReset()
{
    const unsigned int notes[] = {
        800, 600, 400};

    const unsigned int durations[] = {
        100, 100, 200};

    playTone(notes, durations, 3);
}

void Sound::playButton()
{
    const unsigned int notes[] = {
        1200};

    const unsigned int durations[] = {
        50};

    playTone(notes, durations, 1);
}

void Sound::playConnection()
{
    const unsigned int notes[] = {
        523, 659, 784};

    const unsigned int durations[] = {
        100, 100, 200};

    playTone(notes, durations, 3);
}

void Sound::playDisconnect()
{
    const unsigned int notes[] = {
        784, 659, 523};

    const unsigned int durations[] = {
        100, 100, 200};

    playTone(notes, durations, 3);
}

void Sound::playEnd()
{
    const unsigned int notes[] = {
        659, 523, 440};

    const unsigned int durations[] = {
        100, 100, 200};

    playTone(notes, durations, 3);
}

void Sound::stop()
{
    noTone(pin);
}