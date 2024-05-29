/**
 * @file PDL_RGB_Indicator.h
 * @brief This file contains the definition of the RGB_Indicator class and related structures.
 */

#ifndef PDL_RGB_INDICATOR_H
#define PDL_RGB_INDICATOR_H

#include "FreeRTOS.h"
#include "task.h"
#include <Arduino.h>
#include <stdint.h>

#define MAX_PATTERN_LENGTH 8

struct Color_t
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct PatternEntry
{
    Color_t color;
    uint32_t duration; // duration in milliseconds
};

struct Pattern_t
{
    PatternEntry entries[MAX_PATTERN_LENGTH];
    uint8_t length;
};

// Predefined colors
const Color_t COLOR_RED = {255, 0, 0};
const Color_t COLOR_GREEN = {0, 255, 0};
const Color_t COLOR_BLUE = {0, 0, 255};
const Color_t COLOR_YELLOW = {255, 255, 0};
const Color_t COLOR_CYAN = {0, 255, 255};
const Color_t COLOR_MAGENTA = {255, 0, 255};
const Color_t COLOR_WHITE = {255, 255, 255};
const Color_t COLOR_OFF = {0, 0, 0};

// Predefined patterns
const Pattern_t OFF = {{
                           {COLOR_OFF, 1000},
                       },
                       1};

const Pattern_t WHITE_CONST = {{
                                   {COLOR_WHITE, 1000},
                               },
                               1};

const Pattern_t GREEN_CONST = {{
                                   {COLOR_GREEN, 1000},
                               },
                               1};

const Pattern_t YELLOW_CONST = {{
                                    {COLOR_YELLOW, 1000},
                                },
                                1};

const Pattern_t RED_CONST = {{
                                 {COLOR_RED, 1000},
                             },
                             1};

const Pattern_t BLUE_CONST = {{
                                  {COLOR_BLUE, 1000},
                              },
                              1};

const Pattern_t GREEN_BREATHING = {{
                                       {COLOR_GREEN, 1000},
                                       {COLOR_OFF, 1000},
                                   },
                                   2};
const Pattern_t GREEN_BLINK = {{
                                   {COLOR_GREEN, 200},
                                   {COLOR_OFF, 200},
                               },
                               2};

const Pattern_t YELLOW_BLINK = {{
                                    {COLOR_YELLOW, 200},
                                    {COLOR_OFF, 200},
                                },
                                2};

const Pattern_t RED_BLINK = {{
                                 {COLOR_RED, 200},
                                 {COLOR_OFF, 200},
                             },
                             2};

const Pattern_t BLUE_BLINK = {{
                                 {COLOR_BLUE, 200},
                                 {COLOR_OFF, 200},
                             },
                             2};

const Pattern_t RAINBOW = {{
                               {COLOR_RED, 200},
                               {COLOR_YELLOW, 200},
                               {COLOR_GREEN, 200},
                               {COLOR_CYAN, 200},
                               {COLOR_BLUE, 200},
                               {COLOR_MAGENTA, 200},
                           },
                           6};

class RGB_Indicator
{
public:
    RGB_Indicator(uint8_t pinR, uint8_t pinG, uint8_t pinB, bool on_logic = HIGH);
    ~RGB_Indicator();
    void begin();
    void setPattern(const Pattern_t &pattern);
    void setRampRate(uint32_t rate);

private:
    const uint8_t _pinR, _pinG, _pinB;
    const static uint32_t STACK_SIZE = 1024;
    const bool _ON_LOGIC = HIGH;
    TickType_t _ramp_rate = pdMS_TO_TICKS(30); // ms
    Color_t _currentColor = COLOR_OFF;
    Pattern_t _pattern = OFF;

    TaskHandle_t _taskHandle = NULL;

    void _fade(const Color_t &from, const Color_t &to, uint32_t stepCount, TickType_t stepTick);
    void _setRGB(uint8_t red, uint8_t green, uint8_t blue);
    void _setRGB(const Color_t &color);
    static void _taskWrapper(void *parameter);
    void _task();
};

#endif // PDL_RGB_INDICATOR_H
