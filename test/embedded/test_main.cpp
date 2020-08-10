#include <unity.h>
#include <Arduino.h>

const int LED_PIN = 2;
const int LED_INTERVAL_MS = 500;

void setUp(void)
{
    // set stuff up here
    pinMode(LED_PIN, OUTPUT);
}

void tearDown(void)
{
    // clean stuff up here
}

void test_led_high()
{
    digitalWrite(LED_PIN, HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_PIN));
    delay(LED_INTERVAL_MS);
}

void test_led_low()
{
    digitalWrite(LED_PIN, LOW);
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_PIN));
    delay(LED_INTERVAL_MS);
}

void setup()
{
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_led_high);
    RUN_TEST(test_led_low);

    UNITY_END();
}

void loop()
{
    // nothing to be done here.
}
