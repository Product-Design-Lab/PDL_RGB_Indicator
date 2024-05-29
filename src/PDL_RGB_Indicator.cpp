/**
 * @file PDL_RGB_Indicator.cpp
 * @brief This file contains the implementation of the RGB_Indicator class.
 */

#include "PDL_RGB_Indicator.h"
#include <Arduino.h>

RGB_Indicator::RGB_Indicator(uint8_t pinR, uint8_t pinG, uint8_t pinB)
    : _pinR(pinR), _pinG(pinG), _pinB(pinB), _currentColor{0, 0, 0}
{
    pinMode(_pinR, OUTPUT);
    pinMode(_pinG, OUTPUT);
    pinMode(_pinB, OUTPUT);
}

RGB_Indicator::~RGB_Indicator()
{
    if (_taskHandle != NULL)
    {
        vTaskDelete(_taskHandle);
    }
}

void RGB_Indicator::begin()
{
    if (_taskHandle == NULL)
    {
        xTaskCreate(_taskWrapper, "RGBIndicatorTask", STACK_SIZE, this, 1, &_taskHandle);
    }
}

void RGB_Indicator::setPattern(const Pattern_t &pattern)
{
    _pattern = pattern;
    if (_taskHandle == NULL)
    {
        begin();
    }

    // resume task if suspended
    if (_taskHandle != NULL && eTaskGetState(_taskHandle) == eSuspended)
    {
        vTaskResume(_taskHandle); // Corrected function name
    }

    xTaskNotifyGive(_taskHandle); // notify task to start new pattern
}

void RGB_Indicator::_prvSetRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    red = map(red, 0, 255, 0, 1023);
    green = map(green, 0, 255, 0, 1023);
    blue = map(blue, 0, 255, 0, 1023);

    analogWrite(_pinR, red);
    analogWrite(_pinG, green);
    analogWrite(_pinB, blue);
}

void RGB_Indicator::_prvSetRGB(const Color_t &color)
{
    _prvSetRGB(color.red, color.green, color.blue);
}

void RGB_Indicator::setRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    _currentColor.red = red;
    _currentColor.green = green;
    _currentColor.blue = blue;

    _prvSetRGB(red, green, blue);

    // suspend task
    if (_taskHandle != NULL)
        vTaskSuspend(_taskHandle);
}

void RGB_Indicator::setRGB(const Color_t &color)
{
    setRGB(color.red, color.green, color.blue);
}

void RGB_Indicator::_taskWrapper(void *parameter)
{
    static_cast<RGB_Indicator *>(parameter)->_task();
}

void RGB_Indicator::setRampRate(uint32_t rate)
{
    _ramp_rate = pdMS_TO_TICKS(rate);
}

void RGB_Indicator::ramp_to_color(const Color_t &color, uint32_t duration)
{
    Color_t temp_color = _currentColor;
    int16_t stepCount = duration / _ramp_rate;

    if (stepCount < 2)
    {
        setRGB(color);
        return;
    }

    _currentColor = color;

    int16_t stepSizeR = (color.red - temp_color.red) / stepCount;
    int16_t stepSizeG = (color.green - temp_color.green) / stepCount;
    int16_t stepSizeB = (color.blue - temp_color.blue) / stepCount;

    // ramp
    for (uint32_t i = 0; i < stepCount; i++)
    {
        temp_color.red += stepSizeR;
        temp_color.green += stepSizeG;
        temp_color.blue += stepSizeB;

        _prvSetRGB(temp_color);
        if (ulTaskNotifyTake(pdFALSE, _ramp_rate) > 0)
        {
            return;
        }
    }
}

void RGB_Indicator::_task()
{
    while (1)
    {
        for (uint8_t i = 0; i < _pattern.length; i++)
        {
            ramp_to_color(_pattern.entries[i].color, _pattern.entries[i].duration);
            if (ulTaskNotifyTake(pdTRUE, _pattern.entries[i].duration) > 0)
            {
                break;
            }
        }
    }

    vTaskDelete(NULL);
}
