#include <Arduino.h>
#include <WiFi.h>
#include <Ambient.h>
#include <log/Log.h>

#include "Config.h"

const int LED_PIN = 2;
const int ANALOG_PIN = 34;
const int AMBIENT_VOLUME_FIELD = 7;

const int HIGH_INTERVAL_MS         = 250;
const int NORMAL_INTERVAL_MS       = 500;
const int SEND_AMBIENT_INTERVAL_MS = 300000;
const int RESTART_INTERVAL_MS      = 60000;

WiFiClient gClient;
Ambient gAmbient;

volatile int gMaxVolume;

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

void printVolume(int v)
{
    double t = micros();

    // Serial.print(t/1000000);
    // Serial.print("\t");
    // Serial.println(v);

    String msg = String(t/1000000) + String("\t") + String(v);
    Log::Info(msg.c_str());
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

void resetMaxVolume()
{
    gMaxVolume = 0;
}

int getMaxVolume()
{
    return gMaxVolume;
}

void taskUpdateVolume(void *pv)
{
    resetMaxVolume();
    while (1) {
        int v = getVolume();
        if (v > gMaxVolume) {
            gMaxVolume = v;
        }
        delay(10);
    }
}

void restartSystem()
{
    Log::Error("going to die...");
    delay(RESTART_INTERVAL_MS);
    ESP.restart();
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(115200);
    Log::Info("esp32 sound sensor");
    reconnectWifi();

    gAmbient.begin(AMBIENT_CHANNEL_ID, AMBIENT_WRITE_KEY, &gClient);

    int volume = getVolume();
    printVolume(volume);

    gAmbient.set(AMBIENT_VOLUME_FIELD, volume);
    if (!gAmbient.send()) {
        Log::Error("send error: send to ambient #1");
        restartSystem();
    }

    xTaskCreateUniversal(
        taskUpdateVolume, "taskUpdateVolume",
        1024, NULL, 1, NULL, APP_CPU_NUM);
}

void loop()
{
    resetMaxVolume();
    int maxTimes = SEND_AMBIENT_INTERVAL_MS / (NORMAL_INTERVAL_MS * 2);
    for (int i=0; i<maxTimes; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(NORMAL_INTERVAL_MS);
        digitalWrite(LED_PIN, LOW);
        delay(NORMAL_INTERVAL_MS);
    }
    int maxVolume = getMaxVolume();
    printVolume(maxVolume);

    gAmbient.set(AMBIENT_VOLUME_FIELD, maxVolume);
    if (!gAmbient.send()) {
        Log::Error("send error: send to ambient");
        restartSystem();
    }
}
