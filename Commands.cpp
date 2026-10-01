#include "Wifi_server.h"
#include "Commands.h"
#include "Display.h"
#include "Button.h"
#include "Sound.h"
#include "LED.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <cstring>

namespace
{
    // LED-Pins
    constexpr uint8_t RED_LED_PIN   = 14;
    constexpr uint8_t GREEN_LED_PIN = 22;
    constexpr uint8_t BLUE_LED_PIN  = 21;

    // Tasten-Pins
    constexpr uint8_t RED_BUTTON_PIN   = 12;
    constexpr uint8_t GREEN_BUTTON_PIN = 27;
    constexpr uint8_t BLUE_BUTTON_PIN  = 26;

    // Buzzer-Pin
    constexpr uint8_t PIEZO_PIN = 13;

    // Display-Pins
    constexpr uint8_t TFT_CS  = 15;
    constexpr uint8_t TFT_DC  =  2;
    constexpr uint8_t TFT_RST =  4;
    constexpr uint8_t TFT_BL  = 32;

    constexpr uint16_t SCREEN_HEIGHT = 170;
    constexpr uint16_t SCREEN_WIDTH  = 320;
}

bool waitForApproval = false;
bool isPictureBeingChecked = false;
bool materialsNeedToBeRemoved = false;

// LED-Objekt erstellen
LED led(RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN);

// Tasten-Objekte erstellen
Button buttonRed(RED_BUTTON_PIN, ButtonColor::RED);
Button buttonGreen(GREEN_BUTTON_PIN, ButtonColor::GREEN);
Button buttonBlue(BLUE_BUTTON_PIN, ButtonColor::BLUE);

// Buzzer-Objekt erstellen
Sound buzzer(PIEZO_PIN);

// Display-Objekt erstellen
Display display(TFT_CS, TFT_DC, TFT_RST, TFT_BL, SCREEN_WIDTH, SCREEN_HEIGHT);


/**
 * @brief Initialisiert alle Hardware-Komponenten und startet den WLAN-TCP-Server.
 */
void startProgram()
{
    buzzer.begin();

    buttonRed.begin();
    buttonGreen.begin();
    buttonBlue.begin();

    display.begin();

    led.begin();
    led.setLED(255, 0, 0);
    delay(200);
    led.setLED(0, 255, 0);
    delay(200);
    led.setLED(0, 0, 255);
    delay(200);
    led.off();

    //startWLAN();
    //startServer();

    connectWiFi();

    buzzer.playStart();
}


/**
 * @brief Erstellt und sendet eine JSON-Antwort an den TCP-Client.
 *
 * @param client Aktive TCP-Verbindung zum Client.
 * @param httpStatus HTTP-Statuscode der Antwort.
 * @param status Status der Verarbeitung.
 * @param message Beschreibung des Ergebnisses.
 * @param request Ursprüngliche JSON-Anfrage.
 */
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

    // Ursprüngliche Anfrage in die Antwort übernehmen
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
    
    client.println(responseBody);  // JSON-Antwort senden
    client.flush();
    client.stop();                 // nur diese Verbindung schließen
}


enum class ResultType
{
    IO,
    NIO,
    RESET,
    UNKNOWN
};

/**
 * @brief Wandelt den Result-String in einen ResultType-Wert um.
 *
 * @param result Result-Wert aus der JSON-Anfrage.
 * @return Entsprechender ResultType oder ResultType::UNKNOWN.
 */
ResultType getResultType(const char *result)
{
    if (strcmp(result, "IO") == 0)
    {
        return ResultType::IO;
    }

    if (strcmp(result, "NIO") == 0)
    {
        return ResultType::NIO;
    }

    if (strcmp(result, "reset") == 0)
    {
        return ResultType::RESET;
    }

    return ResultType::UNKNOWN;
}


/**
 * @brief Verarbeitet ein erfolgreiches Prüfergebnis.
 *
 * Gibt einen Bestätigungston aus, setzt die LED auf Grün
 * und zeigt das erfolgreiche Ergebnis auf dem Display an.
 */
void handleSuccessfulResult()
{
    buzzer.playOK();
    led.setLED(0, 255, 0);
    Serial.println("Result: In Ordnung");

    String message = "Result: In Ordnung";

    display.setCursor(15, 50);
    display.showText(message, 18);
}


/**
 * @brief Fordert den Benutzer zur Bestätigung eines Prüfergebnisses auf.
 *
 * Gibt einen Signalton aus, aktiviert die blaue pulsierende LED
 * und zeigt die Bestätigungsoptionen auf dem Display an.
 */
void showApprovalRequest()
{
    buzzer.playFail();
    led.startPulse(255, 0, 0, 0, 255, 0);

    Serial.println("Result: Nicht in Ordnung");
    
    String message = "Result: Nicht in Ordnung\n "
                     "Sieht das Bild richtig aus?";

    display.setCursor(10, 50);
    display.showText(message, 18);
    display.showApproveButton("OK");
    display.showApproveButton("FAIL");
}


/**
 * @brief Fordert ein neues Bild an.
 *
 * Aktiviert die orange pulsierende LED, gibt einen Signalton aus
 * und fordert den Benutzer auf, das Material zu entfernen und zu bestätigen.
 */
void requestNewPicture()
{
    led.startPulse(0, 0, 255, 0, 0, 255);
    buzzer.playRequest();
    Serial.println("Result: reset");

    materialsNeedToBeRemoved = true;
    String message = "Material entfernen \n  und bestätigen.";

    display.setCursor(15, 50);
    display.showText(message, 24);
    display.showApproveButton("NEW_PICTURE");
}


/**
 * @brief Verarbeitet eine Betätigung einer Taste.
 *
 * Je nach Farbe der Taste wird die entsprechende Aktion ausgeführt
 * und eine Antwort an den TCP-Client gesendet.
 *
 * @param color Farbe der betätigten Taste.
 * @param client Aktive TCP-Verbindung zum Client.
 */
void handleButton(const String &color, WiFiClient &client)
{
    if (color == "green")
    {
        // IO-Ergebnis bestätigen
        Serial.println("Benutzer hat OK gewählt");

        // LED grün setzen
        led.setLED(0, 255, 0);

        // Display aktualisieren
        // display.showText(...);
        JsonDocument emptyRequest;
        sendResponse(
            client,
            400,
            "GREEN",
            "Green button was pressed",
            emptyRequest);
    }
    else if (color == "red")
    {
        // NIO-Ergebnis bestätigen
        Serial.println("Benutzer hat FAIL gewählt");

        // LED rot setzen
        led.setLED(255, 0, 0);

        // Display aktualisieren
        // display.showText(...);
        JsonDocument emptyRequest;
        sendResponse(
            client,
            400,
            "RED",
            "Red button was pressed",
            emptyRequest);
    }
    else if (color == "blue")
    {
        // RESET-Ergebnis bestätigen
        Serial.println("Benutzer hat FAIL gewählt");

        // LED blau setzen
        led.setLED(0, 0, 255);

        // Display aktualisieren
        // display.showText(...);
        JsonDocument emptyRequest;
        sendResponse(
            client,
            400,
            "BLUE",
            "Blue button was pressed",
            emptyRequest);
    }
}


/**
 * @brief Verarbeitet einen eingehenden Befehl vom TCP-Client.
 *
 * Liest die JSON-Anfrage, überprüft das enthaltene Result und
 * führt die entsprechende Aktion aus.
 *
 * @param client Aktive TCP-Verbindung zum Client.
 */
void handleCommand(WiFiClient &client)
{
    String message = client.readStringUntil('\n');

    message.trim();

    Serial.println("Empfangen:");
    Serial.println(message);

    JsonDocument doc;

    DeserializationError error =
        deserializeJson(doc, message);

    if (error)
    {
        Serial.println("Ungueltiges JSON");

        // Bei ungültigem JSON kann die ursprüngliche Anfrage nicht zurückgegeben werden.
        JsonDocument emptyRequest;
        sendResponse(
            client,
            400,
            "error",
            "Invalid JSON",
            emptyRequest);
        return;
    }

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

    // String in Enum umwandeln
    ResultType resultType = getResultType(result);

    switch (resultType)
    {
        case ResultType::IO:
        {
            Serial.println("Result: IO");
            sendResponse(client, 
                200, 
                "success",
                "Result: In Ordnung", 
                doc);
            handleSuccessfulResult();
            break;
        }
        case ResultType::NIO:
        {
            Serial.println("Result: NIO");
            showApprovalRequest();
            waitForApproval = true;
            isPictureBeingChecked = true;
            // Hier muss nun eine Taste gedrückt werden

            break;
        }
        case ResultType::RESET:
        {
            Serial.println("Result: reset");
            requestNewPicture();
            waitForApproval = true;
            // Hier muss nun eine Taste gedrückt werden
            break;
        }
        case ResultType::UNKNOWN:
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

