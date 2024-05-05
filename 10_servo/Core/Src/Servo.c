//
// Created by robbe on 5/05/24.
//

#include "Servo.h"
#include <stdlib.h>
#include <math.h>

#include "stm32l4xx.h"

void Servo_moveToPosition(const TIM_HandleTypeDef* htim, const uint16_t channel, const uint16_t position) {
    uint16_t translatedPosition = Servo_map(position, 0, 180, SERVO_MIN_VALUE, SERVO_MAX_VALUE);
    __HAL_TIM_SET_COMPARE(htim, channel, translatedPosition);
}

void Servo_sweep(const TIM_HandleTypeDef* htim, const uint16_t channel) {
    const int stepSize = 1;
    const int delayTime = 10;

    for (int pulse = SERVO_MIN_VALUE; pulse <= SERVO_MAX_VALUE; pulse += stepSize) {
        __HAL_TIM_SET_COMPARE(htim, channel, pulse);
        HAL_Delay(delayTime);
    }

    for (int pulse = SERVO_MAX_VALUE; pulse >= SERVO_MIN_VALUE; pulse -= stepSize) {
        __HAL_TIM_SET_COMPARE(htim, channel, pulse);
        HAL_Delay(delayTime);
    }
}

uint16_t Servo_map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Servo_moveWithEase(const TIM_HandleTypeDef* htim, const uint16_t channel, const uint16_t start, const uint16_t end, const uint16_t duration) {
    const uint16_t steps = 200;
    const uint16_t delayTime = duration / steps;

    for (uint16_t i = 0; i <= steps; ++i) {
        float t = (float)i / steps;
        float factor = Servo_easeInOut(t);
        uint16_t position = start + (end - start) * factor;
        Servo_moveToPosition(htim, channel, position);
        HAL_Delay(delayTime);
    }
}

float Servo_easeInOut(const float t) {
    if (t < 0.5) {
        return 2 * t * t;
    }
    else {
        return -1 + (4 - 2 * t) * t;
    }
}

void Servo_moveWithTrapezoidalProfile(const TIM_HandleTypeDef* htim, const uint16_t channel, const uint16_t start, const uint16_t end, const uint16_t maxSpeed, const uint16_t maxAcceleration) {
    int16_t distance = end - start;
    float accelerationTime = (float)maxSpeed / maxAcceleration;
    float accelerationDistance = 0.5 * maxAcceleration * accelerationTime * accelerationTime;

    if (2 * accelerationDistance < abs(distance)) {
        float timeToMaxSpeed = sqrt(abs(distance) / maxAcceleration);
        float constantVelocityDistance = abs(distance) - 2 * accelerationDistance;
        float totalTime = 2 * timeToMaxSpeed + accelerationTime;

        for (float t = 0; t < totalTime; t += 0.01) {
            float velocity;
            if (t < timeToMaxSpeed) {
                velocity = maxAcceleration * t;
            }
            else if (t < totalTime - timeToMaxSpeed) {
                velocity = maxSpeed;
            }
            else {
                velocity = maxSpeed - maxAcceleration * (t - (totalTime - timeToMaxSpeed));
            }
            float position = start + velocity * t;
            Servo_moveToPosition(htim, channel, position);
            HAL_Delay(10);
        }
    }
    else {
        Servo_moveToPosition(htim, channel, end);
    }
}
