//
// Created by robbe on 5/05/24.
//
#pragma once

#include "main.h"

#define SERVO_MIN_VALUE 75
#define SERVO_MAX_VALUE 250

void Servo_moveToPosition(const TIM_HandleTypeDef* htim, uint16_t channel, const uint16_t position);
void Servo_sweep(const TIM_HandleTypeDef* htim, uint16_t channel);
uint16_t Servo_map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
void Servo_moveWithEase(const TIM_HandleTypeDef* htim, uint16_t channel, const uint16_t start, const uint16_t end, const uint16_t duration);
float Servo_easeInOut(const float t);
void Servo_moveWithTrapezoidalProfile(const TIM_HandleTypeDef* htim, const uint16_t channel, const uint16_t start, const uint16_t end, const uint16_t maxSpeed, const uint16_t maxAcceleration);
