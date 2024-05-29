#include <Arduino.h>
#include "PDL_RGB_Indicator.h"
#include "Adafruit_TinyUSB.h"

// Define test pins
const uint8_t TEST_PIN_R = LED_RED;
const uint8_t TEST_PIN_G = LED_GREEN;
const uint8_t TEST_PIN_B = LED_BLUE;

// Create an instance of RGB_Indicator
RGB_Indicator led(TEST_PIN_R, TEST_PIN_G, TEST_PIN_B, false);

void setup()
{
    Serial.begin(115200);
    Serial.begin(115200);

    // Start the RGB_Indicator
    led.begin();
    Pattern_t myPattern;
    Color_t myColor;
    myColor.red = 128;
    myColor.green = 234;
    myColor.blue = 10;

    myPattern.entries[0].color = COLOR_RED;
    myPattern.entries[0].duration = 100;
    myPattern.entries[1].color = COLOR_GREEN;
    myPattern.entries[1].duration = 100;
    myPattern.entries[2].color = COLOR_BLUE;
    myPattern.entries[2].duration = 100;
    myPattern.entries[3].color = myColor;
    myPattern.entries[3].duration = 1000;
    myPattern.length = 4;
    led.setPattern(myPattern);
}

void loop()
{
    if(Serial.available())
    {
        char c = Serial.read();
        Serial.printf("Received: %c\n", c);
        if(c == 'r')
        {
            led.setPattern(RED_BLINK);
            Serial.println("RED Blink");
        }
        else if(c == 'g')
        {
            led.setPattern(GREEN_BREATHING);
            Serial.println("GREEN Blink");
        }
        else if(c == 'b')
        {
            led.setPattern(BLUE_BLINK);
            Serial.println("BLUE Blink");
        }
        else if(c == 'y')
        {
            led.setPattern(YELLOW_BLINK);
            Serial.println("YELLOW Blink");
        }
        else if(c == 'R')
        {
            led.setPattern(RED_CONST);
        }
        else if(c == 'G')
        {
            led.setPattern(GREEN_BREATHING);
        }
        else if(c == 'B')
        {
            led.setPattern(BLUE_CONST);
        }
        else if(c == 'Y')
        {
            led.setPattern(YELLOW_CONST);
        }
        else if(c == 'A')
        {
            led.setPattern(RAINBOW);
        }
        else{
            led.setPattern(OFF);
            Serial.println("OFF");
        }
    }
    // led.setPattern(RED_BLINK);
    // delay(4000);
    // led.setPattern(GREEN_BLINK);
    // delay(4000);
    // led.setPattern(BLUE_BLINK);
    delay(100);

}
