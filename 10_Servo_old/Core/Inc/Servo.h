//
// Created by robbe on 23/04/24.
//
#pragma once

#include "main.h"

struct Servo {
    static uint8_t const servoMinValue = 75;
    static uint8_t const servoMaxValue = 250;

    static void MoveToPosition(const TIM_HandleTypeDef* htim, uint16_t channel, const uint16_t position);
    static void Sweep(const TIM_HandleTypeDef* htim, uint16_t channel);
    static uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
    static void MoveWithEase(const TIM_HandleTypeDef* htim, uint16_t channel, const uint16_t start, const uint16_t end, const uint16_t duration);
    static float EaseInOut(const float t);
    static void MoveWithTrapezoidalProfile(const TIM_HandleTypeDef* htim, const uint16_t channel, const uint16_t start, const uint16_t end, const uint16_t maxSpeed, const uint16_t maxAcceleration);

};

