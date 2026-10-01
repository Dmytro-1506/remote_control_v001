#include "Sound.h"

/**
 * @brief Erstellt ein Sound-Objekt.
 *
 * @param pin GPIO-Pin des Buzzers.
 */
Sound::Sound(uint8_t pin)
{
    this->pin = pin;
}


/**
 * @brief Initialisiert den GPIO-Pin für die Tonausgabe.
 */
void Sound::begin()
{
    pinMode(pin, OUTPUT);
}


/**
 * @brief Spielt eine Folge von Tönen ab.
 *
 * @param frequencies Array mit den Frequenzen der Töne in Hertz.
 * @param durations Array mit den Dauerwerten der Töne in Millisekunden.
 * @param length Anzahl der abzuspielenden Töne.
 */
void Sound::playTone(const unsigned int frequencies[], const unsigned int durations[], size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        tone(pin, frequencies[i], durations[i]);
        delay(durations[i]);

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
        100, 100, 300};

    playTone(notes, durations, 3);
}

void Sound::playOK()
{
    const unsigned int notes[] = {
        784, 1047, 784, 1047};

    const unsigned int durations[] = {
        100, 150, 100, 280};

    playTone(notes, durations, 4);
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
        700, 900, 700, 900};

    const unsigned int durations[] = {
        100, 150, 100, 200};

    playTone(notes, durations, 4);
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