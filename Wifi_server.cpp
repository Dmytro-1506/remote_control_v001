#include "Wifi_server.h"
#include "Commands.h"
#include "Test_commands.h"
#include "Secrets.h"

#include <WiFi.h>

WiFiServer server(SERVER_PORT);

void startWLAN()
{
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid, password);

    Serial.println("WLAN gestartet!");

    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.softAPIP());
}

void startServer()
{
    server.begin();

    Serial.print("TCP Server started on port: ");
    Serial.println(SERVER_PORT);
}

void handleServer()
{
    WiFiClient client = server.available();

    if (!client)
    {
        return;
    }

    Serial.println("Client connected!");

    while (client.connected())
    {
        if (client.available())
        {
            testCommand(client);
        }

        delay(10);
    }

    client.stop();

    Serial.println("Client disconnected.");
}
