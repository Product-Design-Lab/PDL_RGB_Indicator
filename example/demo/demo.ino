#include <Arduino.h>
#include "PDL_RGB_Indicator.h"
#include "Adafruit_TinyUSB.h"

// Define test pins
const uint8_t TEST_PIN_R = 9;
const uint8_t TEST_PIN_G = 10;
const uint8_t TEST_PIN_B = 11;

// Create an instance of RGB_Indicator
RGB_Indicator rgbIndicator(TEST_PIN_R, TEST_PIN_G, TEST_PIN_B);

void setup() {
    Serial.begin(9600);

    // Start the RGB_Indicator
    rgbIndicator.begin();

    // Run tests
    testSetRGB();
    testSetPattern();
    testRampRate();
}

void loop() {
    // Nothing to do here
}

void testSetRGB() {
    Serial.println("Testing setRGB...");

    // Test setting RGB to red
    rgbIndicator.setRGB(255, 0, 0);
    delay(1000);

    // Test setting RGB to green
    rgbIndicator.setRGB(0, 255, 0);
    delay(1000);

    // Test setting RGB to blue
    rgbIndicator.setRGB(0, 0, 255);
    delay(1000);

    // Test setting RGB to a custom color
    Color_t customColor = {128, 64, 32};
    rgbIndicator.setRGB(customColor);
    delay(1000);

    Serial.println("setRGB test completed.");
}

void testSetPattern() {
    Serial.println("Testing setPattern...");

    // Test setting the RED_BLINK pattern
    rgbIndicator.setPattern(RED_BLINK);
    delay(3000);

    // Test setting the RAINBOW pattern
    rgbIndicator.setPattern(RAINBOW);
    delay(3000);

    // Test setting the OFF pattern
    rgbIndicator.setPattern(OFF);
    delay(1000);

    Serial.println("setPattern test completed.");
}

void testRampRate() {
    Serial.println("Testing setRampRate...");

    // Test setting a fast ramp rate
    rgbIndicator.setRampRate(5);
    rgbIndicator.setPattern(GREEN_BREATHING);
    delay(3000);

    // Test setting a slow ramp rate
    rgbIndicator.setRampRate(100);
    rgbIndicator.setPattern(GREEN_BREATHING);
    delay(3000);

    Serial.println("setRampRate test completed.");
}
