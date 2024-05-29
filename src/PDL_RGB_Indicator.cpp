/**
 * @file PDL_RGB_Indicator.cpp
 * @brief This file contains the implementation of the RGB_Indicator class.
 */

#include "PDL_RGB_Indicator.h"
#include <Arduino.h>

RGB_Indicator::RGB_Indicator(uint8_t pinR, uint8_t pinG, uint8_t pinB, bool on_logic)
    : _pinR(pinR), _pinG(pinG), _pinB(pinB), _currentColor{0, 0, 0}, _ON_LOGIC(on_logic)
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
        // Serial.println("Resuming task");
        vTaskResume(_taskHandle);
    }

    xTaskNotifyGive(_taskHandle); // notify task to start new pattern
}

void RGB_Indicator::_setRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    _currentColor.red = red;
    _currentColor.green = green;
    _currentColor.blue = blue;

    if (_ON_LOGIC == false)
    {
        red = 255 - red;
        green = 255 - green;
        blue = 255 - blue;
    }

    // red = map(red, 0, 255, 0, 1023);
    // green = map(green, 0, 255, 0, 1023);
    // blue = map(blue, 0, 255, 0, 1023);

    analogWrite(_pinR, red);
    analogWrite(_pinG, green);
    analogWrite(_pinB, blue);
    // Serial.printf("R:%d, G:%d, B:%d\n", red, green, blue);
}

void RGB_Indicator::_setRGB(const Color_t &color)
{
    _setRGB(color.red, color.green, color.blue);
}

void RGB_Indicator::_taskWrapper(void *parameter)
{
    static_cast<RGB_Indicator *>(parameter)->_task();
}

void RGB_Indicator::setRampRate(uint32_t rate)
{
    _ramp_rate = pdMS_TO_TICKS(rate);
}

void RGB_Indicator::_fade(const Color_t &from, const Color_t &to, uint32_t stepCount, TickType_t stepTick)
{
    int step_red = int(to.red - from.red) / (int)stepCount;
    int step_green = int(to.green - from.green) / (int)stepCount;
    int step_blue = int(to.blue - from.blue) / (int)stepCount;

    // Serial.printf("Step Red: %d, Step Green: %d, Step Blue: %d, Step Count: %d\n", step_red, step_green, step_blue, stepCount);

    Color_t temp_color = from;

    _setRGB(from);

    for (uint32_t i = 0; i < stepCount; i++)
    {
        temp_color.red += step_red;
        temp_color.green += step_green;
        temp_color.blue += step_blue;

        _setRGB(temp_color);
        if (ulTaskNotifyTake(false, stepTick) > 0)
        {
            break;
        }
    }

    _setRGB(to);
}

void RGB_Indicator::_task()
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    Color_t from = COLOR_OFF, to = COLOR_OFF;
    uint32_t step_count = 0;
    while (1)
    {
        if (_pattern.length <= 1)
        {
            // Serial.println("suspend task");
            _setRGB(_pattern.entries[0].color);
            vTaskSuspend(NULL);
        }

        for (uint8_t i = 0; i < _pattern.length; i++)
        {
            from = i == 0 ? COLOR_OFF : to;
            to = _pattern.entries[i].color;
            step_count = _pattern.entries[i].duration / _ramp_rate;

            _fade(from, to, step_count, _ramp_rate);

            if (ulTaskNotifyTake(pdTRUE, 0) > 0)
            {
                break;
            }
        }
    }

    vTaskDelete(NULL);
}
