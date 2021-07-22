#include <Arduino.h>
#include <WiFi.h>
#include <DHTesp.h>
#include <log/Log.h>

#include "Config.h"

DHTesp dht;
const int DHT_PIN = 32;
const int LED_PIN = 2;

const int HIGH_INTERVAL_MS   = 250;
const int NORMAL_INTERVAL_MS = 500;
const int LOW_INTERVAL_MS    = 1000;

static void showConnectingWifi()
{
    digitalWrite(LED_PIN, HIGH);
    delay(HIGH_INTERVAL_MS);
    digitalWrite(LED_PIN, LOW);
    delay(HIGH_INTERVAL_MS);
}

static void reconnectWifi()
{
    // If forget mode(WIFI_STA), mode might be WIFI_AP_STA.
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

void setup()
{
    Serial.begin(115200);
    Log::Info("Hello ESP32 World!");
    reconnectWifi();

    dht.setup(DHT_PIN, DHTesp::DHT11);
}

void loop()
{
    digitalWrite(LED_PIN, HIGH);
    delay(NORMAL_INTERVAL_MS);
    digitalWrite(LED_PIN, LOW);
    delay(NORMAL_INTERVAL_MS);

    TempAndHumidity newValues = dht.getTempAndHumidity();
    if (dht.getStatus() != 0) {
        Serial.println("DHT11 error status: " + String(dht.getStatusString()));
    }
    float temp = newValues.temperature;
    float hemi = newValues.humidity;
    String msg = String(temp) + String(" ") + String(hemi);
    Log::Info(msg.c_str());
    delay(LOW_INTERVAL_MS);
}
