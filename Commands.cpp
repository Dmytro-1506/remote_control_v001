#include "Commands.h"

#include <ArduinoJson.h>
#include <cstring>

// =====================================================
// JSON Response senden
// =====================================================

static void sendResponse(
    WiFiClient &client,
    int httpStatus,
    const char *status,
    const char *message,
    JsonDocument &request)
{
    JsonDocument response;

    response["status"] = status;
    response["message"] = message;

    // Ursprüngliches JSON als Objekt zurückgeben
    response["json"] = request.as<JsonObject>();

    String responseBody;

    serializeJson(response, responseBody);

    // Netzwerk-Ausgabe ist nur noch HIER
    client.print("HTTP/1.1 ");

    if (httpStatus == 200)
    {
        client.println("200 OK");
    }
    else
    {
        client.println("400 Bad Request");
    }

    client.println("Content-Type: application/json");

    client.print("Content-Length: ");
    client.println(responseBody.length());

    client.println("Connection: close");
    client.println();

    client.println(responseBody);
}

// =====================================================
// Command verarbeiten
// =====================================================

void handleCommand(WiFiClient &client)
{
    String message = client.readStringUntil('\n');

    message.trim();

    Serial.println("Empfangen:");
    Serial.println(message);

    // =================================================
    // JSON parsen
    // =================================================

    JsonDocument doc;

    DeserializationError error =
        deserializeJson(doc, message);

    if (error)
    {
        Serial.println("Ungueltiges JSON");

        // Kein gültiges JSON vorhanden,
        // deshalb können wir das ursprüngliche JSON
        // nicht zurückgeben.
        JsonDocument emptyRequest;

        sendResponse(
            client,
            400,
            "error",
            "Invalid JSON",
            emptyRequest);

        return;
    }

    // =================================================
    // JSON-Felder prüfen
    // =================================================

    if (!doc["command"].is<const char *>() ||
        !doc["red"].is<int>() ||
        !doc["green"].is<int>() ||
        !doc["blue"].is<int>())
    {
        Serial.println(
            "Fehlende oder falsche JSON-Felder");

        sendResponse(
            client,
            400,
            "error",
            "Expected command, red, green and blue",
            doc);

        return;
    }

    // =================================================
    // Werte aus JSON lesen
    // =================================================

    const char *command = doc["command"];

    int red = doc["red"];
    int green = doc["green"];
    int blue = doc["blue"];

    // =================================================
    // Command prüfen
    // =================================================

    // =============================================
    // LED ausschalten
    // =============================================

    if (strcmp(command, "LEDoff") == 0)
    {
        Serial.println("LED wird ausgeschaltet");

        // Hier später deine tatsächliche LED-Steuerung:
        // digitalWrite(...)

        sendResponse(
            client,
            200,
            "success",
            "LED ist aus",
            doc);

        return;
    }

    // =============================================
    // setLED
    // =============================================

    if (strcmp(command, "setLED") != 0)
    {
        Serial.println("Unbekannter Befehl");

        sendResponse(
            client,
            400,
            "error",
            "Unknown command",
            doc);

        return;
    }

    // =================================================
    // RGB-Werte prüfen
    // =================================================

    if (red < 0 || red > 255 ||
        green < 0 || green > 255 ||
        blue < 0 || blue > 255)
    {
        Serial.println("Ungueltige RGB-Werte");

        sendResponse(
            client,
            400,
            "error",
            "RGB values must be between 0 and 255",
            doc);

        return;
    }

    // =================================================
    // Befehl erfolgreich
    // =================================================

    const String espMessage =
        String(command) + " " +
        String(red) + " " +
        String(green) + " " +
        String(blue);

    Serial.println(espMessage);

    // =================================================
    // Response
    // =================================================

    sendResponse(
        client,
        200,
        "success",
        "LED ist an",
        doc);
}
