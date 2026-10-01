#include "Wifi_server.h"
#include "Commands.h"
#include "Secrets.h"

#include <WiFi.h>

void connectWiFi()
{
    WiFi.begin(WLAN_SSID, WLAN_PASSWORD);

    // Warten, bis die Verbindung steht
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");

    }

    // Erfolgsmeldung und zugewiesene IP-Adresse ausgeben
    Serial.println();
    Serial.println("WLAN verbunden!");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.localIP());
    Serial.print("TCP-Port: ");
    Serial.println(SERVER_PORT);
}


WiFiClient wlanClient;
WiFiServer server(SERVER_PORT);
WiFiClient activeClient;

/**
 * @brief Startet den WLAN-Access-Point des ESP32.
 *
 * Konfiguriert die IP-Adresse und startet den Access Point
 * mit den in der Konfiguration hinterlegten Zugangsdaten.
 */
void startWLAN()
{
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid, password);

    Serial.println("WLAN gestartet!");

    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.softAPIP());
}


/**
 * @brief Startet den TCP-Server.
 *
 * Der Server lauscht auf dem konfigurierten TCP-Port.
 */
void startServer()
{
    server.begin();

    Serial.print("TCP Server started on port: ");
    Serial.println(SERVER_PORT);
}


/**
 * @brief Überprüft die TCP-Verbindung und verarbeitet eingehende Befehle.
 *
 * Nimmt neue Client-Verbindungen an und leitet empfangene Daten
 * an die Befehlsverarbeitung weiter.
 */
void handleServer()
{
    if (!activeClient || !activeClient.connected())
    {
        activeClient = server.available();

        if (activeClient)
        {
            Serial.println("Client connected!");
        }
    }

    if (activeClient && activeClient.available())
    {
        handleCommand(activeClient);
    }
}


void handleClient()
{
    if (!wlanClient || !wlanClient.connected())
    {
        wlanClient = server.available();
    }

    if (wlanClient && wlanClient.available())
    {
        handleCommand(wlanClient);
    }
}
