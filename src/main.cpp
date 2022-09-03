#include <Arduino.h>
#include <WiFi.h>
#include <DHTesp.h>
#include <Ambient.h>
#include <log/Log.h>

#include "Config.h"
#include "helper.h"

DHTesp dht;
const int DHT_PIN = 32;

WiFiClient client;
Ambient ambient;

void setup()
{
    initializeSystem("temperature system");

    dht.setup(DHT_PIN, DHTesp::DHT11);
    ambient.begin(AMBIENT_CHANNEL_ID, AMBIENT_WRITE_KEY, &client);
}

void loop()
{
    turnOnActiveLed();
    delay(NORMAL_INTERVAL_MS);
    turnOffActiveLed();
    delay(NORMAL_INTERVAL_MS);

    TempAndHumidity newValues = dht.getTempAndHumidity();
    if (dht.getStatus() != 0) {
        Serial.println("DHT11 error status: " + String(dht.getStatusString()));
        restartSystem();
    }
    float temp = newValues.temperature;
    float hemi = newValues.humidity;
    String msg = String(temp) + String(" ") + String(hemi);
    Log::Info(msg.c_str());
    ambient.set(1, temp);
    ambient.set(2, hemi);
    if (!ambient.send()) {
        restartSystem();
    }
    delay(LOW_INTERVAL_MS);
}
