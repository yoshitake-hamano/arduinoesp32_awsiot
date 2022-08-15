#include <Arduino.h>
#include <WiFi.h>
#include <Ambient.h>
#include <log/Log.h>

#include "Config.h"

const int LED_PIN = 2;

const int ANALOG_PIN = 34;

const int HIGH_INTERVAL_MS   = 250;
const int NORMAL_INTERVAL_MS = 500;
const int LOW_INTERVAL_MS    = 60000;

WiFiClient client;
Ambient ambient;

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

void printVolume(int v)
{
    double t = micros();
    Serial.print(t/1000000);
    Serial.print("\t");
    Serial.println(v);
}

int getVolume()
{
    const int SAMPLING_SIZE = 5;
    int min = 0;
    int max = 0;
    int sum = 0;
    for (int i=0; i<SAMPLING_SIZE; i++) {
        int tmp = analogRead(ANALOG_PIN);
        sum += tmp;
        if (min > tmp || min == 0) {
            min = tmp;
        }
        if (max < tmp || max == 0) {
            max = tmp;
        }
    }
    return (sum - max - min) / (SAMPLING_SIZE - 2);
}

int getMaxVolume(int ms, int existingMaxVolume)
{
    int maxVolume = existingMaxVolume;
    int times = ms * 3.661; // Set numerical value by actual measurement
    for (int i=0; i<times; i++) {
        int v = getVolume();
        if (v > maxVolume) {
            maxVolume = v;
        }
    }
    return maxVolume;
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(115200);
    Log::Info("esp32 sound sensor");
    reconnectWifi();

    ambient.begin(AMBIENT_CHANNEL_ID, AMBIENT_WRITE_KEY, &client);
}

void loop()
{
    int maxVolume = 0;
    digitalWrite(LED_PIN, HIGH);
    maxVolume = getMaxVolume(NORMAL_INTERVAL_MS, maxVolume);
    digitalWrite(LED_PIN, LOW);
    maxVolume = getMaxVolume(NORMAL_INTERVAL_MS, maxVolume);

    maxVolume = getMaxVolume(LOW_INTERVAL_MS, maxVolume);

    double t = micros();
    String msg = String(t/1000000) + String("\t") + String(maxVolume);
    Log::Info(msg.c_str());
    ambient.set(7, maxVolume);
    ambient.send();
}
