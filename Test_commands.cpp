#include "Test_commands.h"

#include <ArduinoJson.h>
#include <cstring>

// =====================================================
// Result-Typen
// =====================================================

enum ResultType
{
    RESULT_IO,
    RESULT_NIO,
    RESULT_RESET,
    RESULT_UNKNOWN
};

// =====================================================
// Result String -> Enum
// =====================================================

ResultType getResultType(const char *result)
{
    if (strcmp(result, "IO") == 0)
    {
        return RESULT_IO;
    }

    if (strcmp(result, "NIO") == 0)
    {
        return RESULT_NIO;
    }

    if (strcmp(result, "reset") == 0)
    {
        return RESULT_RESET;
    }

    return RESULT_UNKNOWN;
}

// =====================================================
// JSON Response senden
// =====================================================

void sendResponse(
    WiFiClient &client,
    int httpStatus,
    const char *status,
    const char *message,
    JsonDocument &request)
{
    JsonDocument response;

    response["status"] = status;
    response["message"] = message;

    // Ursprüngliches JSON zurückgeben
    response["json"] = request.as<JsonObject>();

    String responseBody;

    serializeJson(response, responseBody);

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
    
    serializeJson(response, responseBody);
    client.println(responseBody);  // JSON-Zeile senden
    client.flush();
    client.stop();                 // nur diese Verbindung schließen
}

// =====================================================
// Command verarbeiten
// =====================================================

void testCommand(WiFiClient &client)
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
    // result aus JSON lesen
    // =================================================

    const char *result = doc["result"];

    if (result == nullptr)
    {
        Serial.println("result fehlt");

        JsonDocument emptyRequest;

        sendResponse(
            client,
            400,
            "error",
            "Missing result",
            emptyRequest);

        return;
    }

    // String -> Enum
    ResultType resultType = getResultType(result);

    // =================================================
    // result bearbeiten
    // =================================================

    switch (resultType)
    {
        // -------------------------------------------------
        // IO
        // -------------------------------------------------

    case RESULT_IO:
    {
        Serial.println("Result: IO");

        const char *data = doc["data"];

        if (data != nullptr)
        {
            Serial.print("Data: ");
            Serial.println(data);
        }

        sendResponse(
            client,
            200,
            "success",
            "Congratulations!",
            doc);

        break;
    }

        // -------------------------------------------------
        // NIO
        // -------------------------------------------------

    case RESULT_NIO:
    {
        Serial.println("Result: NIO");

        const char *data = doc["data"];

        if (data != nullptr)
        {
            Serial.print("Data: ");
            Serial.println(data);
        }

        sendResponse(
            client,
            200,
            "error",
            "ERROR",
            doc);

        break;
    }

        // -------------------------------------------------
        // RESET
        // -------------------------------------------------

    case RESULT_RESET:
    {
        Serial.println("Result: reset");

        const char *resetMessage = doc["message"];

        if (resetMessage != nullptr)
        {
            Serial.print("Message: ");
            Serial.println(resetMessage);
        }

        sendResponse(
            client,
            200,
            "reset",
            "RESET",
            doc);

        break;
    }

        // -------------------------------------------------
        // Unbekannt
        // -------------------------------------------------

    case RESULT_UNKNOWN:
    {
        Serial.println("Unbekanntes result:");

        Serial.println(result);

        sendResponse(
            client,
            400,
            "error",
            "Unknown result",
            doc);

        break;
    }
    }
}