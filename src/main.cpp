#include <Arduino.h>
#include <log/Log.h>

const int LED_PIN = 2;

void setup()
{
    Serial.begin(115200);
    Log::Info("Hello ESP32 World!");
    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
}
