#pragma once

#include <Arduino.h>


/**
 * @brief Steuert die Tonausgabe über einen Buzzer.
 *
 * Bietet Funktionen für verschiedene Systemereignisse,
 * darunter Start, Bestätigung, Fehler, Tastenbetätigung
 * sowie Verbindungsaufbau und -trennung.
 */
class Sound
{
public:
    /**
    * @brief Erstellt ein Sound-Objekt.
    *
    * @param pin GPIO-Pin des Buzzers.
    */
    Sound(uint8_t pin);


    /**
    * @brief Initialisiert den GPIO-Pin für die Tonausgabe.
    */
    void begin();


    /**
    * @brief Spielt eine Folge von Tönen ab.
    *
    * @param frequencies Array mit den Frequenzen der Töne in Hertz.
    * @param durations Array mit den Dauerwerten der Töne.
    * @param length Anzahl der abzuspielenden Töne.
    */
    void playTone(
        const unsigned int frequencies[],
        const unsigned int durations[],
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
