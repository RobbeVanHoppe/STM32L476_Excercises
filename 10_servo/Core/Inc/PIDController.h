//
// Created by robbe on 5/05/24.
//
#pragma once
#include "stm32l4xx.h"
#include "servo.h"

extern float Kp; // Proportional gain
extern float Ki; // Integral gain
extern float Kd; // Derivative gain

extern float integralTerm; // Integral term accumulator
extern float prevError; // Previous error

float PID_calculate(float setpoint, float position);
uint16_t PID_getServoPosition(TIM_HandleTypeDef* htim, uint16_t channel);

// Reset the integral term
inline void PID_resetIntegral() { integralTerm = 0; }
