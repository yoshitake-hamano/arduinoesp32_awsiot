#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

#include <log/Log.h>
#include "Config.h"

const int LED_PIN = 2;

const int NEO_PIXEL_PIN     =  5;
const int NUM_OF_NEO_PIXELS = 24;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_OF_NEO_PIXELS,
                                             NEO_PIXEL_PIN,
                                             NEO_GRB + NEO_KHZ800);

const uint32_t BLACK_COLOR      = Adafruit_NeoPixel::Color(0, 0, 0);
const uint32_t ERROR_COLOR      = Adafruit_NeoPixel::Color(255, 0, 0);
const uint32_t WAITING_COLOR    = Adafruit_NeoPixel::Color(255, 255, 51);

static void showError(int times)
{
    for (int i=0; times<0 || i<times; i++) {
        for (int i=0; i<NUM_OF_NEO_PIXELS; i++) {
            pixels.setPixelColor(i, ERROR_COLOR);
        }
        pixels.show();
        delay(1000);

        for (int i=0; i<NUM_OF_NEO_PIXELS; i++) {
            pixels.setPixelColor(i, BLACK_COLOR);
        }
        pixels.show();
        delay(1000);
    }
}

static void clearLeds()
{
    for (int i=0; i<NUM_OF_NEO_PIXELS; i++) {
        pixels.setPixelColor(i, BLACK_COLOR);
    }
}

static void showConnectingWifi()
{
    clearLeds();
    pixels.show();
    delay(100);

    for (int i=0; i<NUM_OF_NEO_PIXELS; i++) {
        pixels.setPixelColor(i, WAITING_COLOR);
        pixels.show();
        delay(10);
    }

    for (int i=0; i<NUM_OF_NEO_PIXELS; i++) {
        pixels.setPixelColor(i, BLACK_COLOR);
        pixels.show();
        delay(10);
    }
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
    Serial.println("Hello ESP32 World!");
    pinMode(LED_PIN, OUTPUT);

    pixels.begin();
    pixels.setBrightness(255);

    reconnectWifi();
}

void loop()
{
    digitalWrite(LED_PIN, HIGH);
    for (int i=0; i<NUM_OF_NEO_PIXELS; i++) {
        pixels.setPixelColor(i, 255, 0, 0);
    }
    pixels.show();
    delay(500);

    digitalWrite(LED_PIN, LOW);
    for (int i=0; i<NUM_OF_NEO_PIXELS; i++) {
        pixels.setPixelColor(i, 0, 0, 0);
    }
    pixels.show();
    delay(500);
}
