#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

const int LED_PIN = 2;

const int NEO_PIXEL_PIN     =  5;
const int NUM_OF_NEO_PIXELS = 24;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_OF_NEO_PIXELS,
                                             NEO_PIXEL_PIN,
                                             NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello ESP32 World!");
    pinMode(LED_PIN, OUTPUT);

    pixels.begin();
    pixels.setBrightness(255);
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
