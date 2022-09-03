#include <Arduino.h>
#include <WiFi.h>
#include <log/Log.h>

#include "Config.h"
#include "helper.h"

const int LED_PIN            = 2;
const int RESTART_INTERVAL_MS = 60000;

static void initializeActiveLed()
{
    pinMode(LED_PIN, OUTPUT);
}

static void showConnectingWifi()
{
    turnOnActiveLed();
    delay(HIGH_INTERVAL_MS);
    turnOffActiveLed();
    delay(HIGH_INTERVAL_MS);
}

static void reconnectWifi()
{
    // If forget mode(WIFI_STA), mode might be WIFI_AP_STA.
    WiFi.disconnect(true,true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASS);

    Log::Info("WiFi connecting...");
    while (WiFi.status() != WL_CONNECTED) {
        showConnectingWifi();
        Log::Debug(".");
    }

    Log::Info("WiFi connected.");
    String log = "IP address: ";
    IPAddress ip = WiFi.localIP();
    log += ip[0]; log += ".";
    log += ip[1]; log += ".";
    log += ip[2]; log += ".";
    log += ip[3];
    Log::Info(log.c_str());
}

void initializeSystem(const char *title)
{
    initializeActiveLed();

    Serial.begin(115200);
    Log::Info(title);
    reconnectWifi();
}

void restartSystem()
{
    Log::Error("going to die...");
    delay(RESTART_INTERVAL_MS);
    ESP.restart();
}

void turnOnActiveLed()
{
    digitalWrite(LED_PIN, HIGH);
}

void turnOffActiveLed()
{
    digitalWrite(LED_PIN, LOW);
}
