#pragma once

#include <WiFi.h>


/**
 * @brief Aktive TCP-Verbindung zum verbundenen Client.
 */
extern WiFiClient activeClient;
extern WiFiClient wlanClient;


void connectWiFi();


/**
 * @brief Startet den WLAN-Access-Point des ESP32.
 */
void startWLAN();


/**
 * @brief Startet den TCP-Server.
 */
void startServer();


/**
 * @brief Überprüft die TCP-Verbindung und verarbeitet eingehende Befehle.
 */
void handleServer();


void handleClient();
