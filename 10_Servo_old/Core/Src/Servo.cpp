//
// Created by robbe on 23/04/24.
//

#include "Servo.h"

#include <cmath>

#include "stm32l4xx.h"

/**
 * position can range from 0 - 180 degrees.
 * **/
void Servo::MoveToPosition(const TIM_HandleTypeDef* htim, const uint16_t channel, const uint16_t position)
{
    // Map position from degrees to PWM range
    uint16_t translatedPosition = map(position, 0, 180, servoMinValue, servoMaxValue);
    __HAL_TIM_SET_COMPARE(htim, channel, translatedPosition);
}

void Servo::Sweep(const TIM_HandleTypeDef* htim, const uint16_t channel)
{
    const int stepSize = 1; // Adjust step size as needed
    const int delayTime = 10; // Adjust delay for desired speed

    for (int pulse = servoMinValue; pulse <= servoMaxValue; pulse += stepSize)
    {
        __HAL_TIM_SET_COMPARE(htim, channel, pulse);
        HAL_Delay(delayTime);
    }

    for (int pulse = servoMaxValue; pulse >= servoMinValue; pulse -= stepSize)
    {
        __HAL_TIM_SET_COMPARE(htim, channel, pulse);
        HAL_Delay(delayTime);
    }
}

uint16_t Servo::map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Servo::MoveWithEase(const TIM_HandleTypeDef* htim, const uint16_t channel, const uint16_t start, const uint16_t end, const uint16_t duration)
{
    constexpr uint16_t steps = 200;
    const uint16_t delayTime = duration / steps;

    for (uint16_t i = 0; i <= steps; ++i)
    {
        // Calculate interpolation factor (0 to 1)
        float t = static_cast<float>(i) / steps;

        // Apply easing function (e.g., ease-in-out)
        float factor = EaseInOut(t);

        // Interpolate between start and end positions
        auto const position = static_cast<uint16_t>(start + (end - start) * factor);

        // Set servo position
        MoveToPosition(htim, channel, position);

        // Delay between steps
        HAL_Delay(delayTime);
    }
}

float Servo::EaseInOut(const float t)
{
    // Ease-in-out function (quadratic)
    if (t < 0.5)
    {
        return 2 * t * t;
    }
    else
    {
        return -1 + (4 - 2 * t) * t;
    }
}

void Servo::MoveWithTrapezoidalProfile(const TIM_HandleTypeDef* htim, const uint16_t channel, const uint16_t start, const uint16_t end, const uint16_t maxSpeed, const uint16_t maxAcceleration)
{
    // Calculate distance to travel
    int16_t distance = end - start;

    // Calculate time to reach maximum speed
    float accelerationTime = static_cast<float>(maxSpeed) / maxAcceleration;

    // Calculate distance covered during acceleration phase
    float accelerationDistance = 0.5 * maxAcceleration * accelerationTime * accelerationTime;

    // Check if the distance allows for acceleration to maximum speed
    if (2 * accelerationDistance < abs(distance))
    {
        // Calculate time to reach maximum speed
        float timeToMaxSpeed = sqrt(abs(distance) / maxAcceleration);

        // Calculate distance covered during constant velocity phase
        float constantVelocityDistance = abs(distance) - 2 * accelerationDistance;

        // Calculate total time
        float totalTime = 2 * timeToMaxSpeed + accelerationTime;

        // Calculate velocity profile
        for (float t = 0; t < totalTime; t += 0.01)
        {
            float velocity;
            if (t < timeToMaxSpeed)
            {
                // Acceleration phase
                velocity = maxAcceleration * t;
            }
            else if (t < totalTime - timeToMaxSpeed)
            {
                // Constant velocity phase
                velocity = maxSpeed;
            }
            else
            {
                // Deceleration phase
                velocity = maxSpeed - maxAcceleration * (t - (totalTime - timeToMaxSpeed));
            }
            float position = start + velocity * t;
            MoveToPosition(htim, channel, position);
            HAL_Delay(10);
        }
    }
    else
    {
        // Distance is too short to reach maximum speed
        MoveToPosition(htim, channel, end);
    }
}
