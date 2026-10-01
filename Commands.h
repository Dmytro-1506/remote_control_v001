#pragma once

#include "LED.h"
#include "Button.h"
#include "Sound.h"
#include "Display.h"

#include <Arduino.h>
#include <WiFi.h>

extern Button buttonRed;
extern Button buttonGreen;
extern Button buttonBlue;
extern LED led;
extern Sound buzzer;
extern Display display;

extern bool waitForApproval;
extern bool isPictureBeingChecked;
extern bool materialsNeedToBeRemoved;

/**
 * @brief Initialisiert alle Komponenten und startet WLAN und TCP-Server.
 */
void startProgram();

/**
 * @brief Verarbeitet einen eingehenden Befehl vom TCP-Client.
 *
 * @param client Aktive TCP-Verbindung zum Client.
 */
void handleCommand(WiFiClient &client);

/**
 * @brief Verarbeitet eine Betätigung einer Taste.
 *
 * @param color Farbe der betätigten Taste.
 * @param client Aktive TCP-Verbindung zum Client.
 */
void handleButton(const String &color, WiFiClient &client);