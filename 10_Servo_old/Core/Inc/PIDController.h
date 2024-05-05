//
// Created by robbe on 3/05/24.
//
#pragma once
#include "stm32l4xx_hal_tim.h"


class PIDController {
private:
    float Kp; // Proportional gain
    float Ki; // Integral gain
    float Kd; // Derivative gain

    float integralTerm; // Integral term accumulator
    float prevError;    // Previous error

public:
    PIDController(float kp, float ki, float kd) : Kp(kp), Ki(ki), Kd(kd), integralTerm(0), prevError(0) {}

    [[nodiscard]] float calculate(float setpoint, float position);
    [[nodiscard]] uint16_t getServoPosition(TIM_HandleTypeDef* htim, uint16_t channel);

    // Reset the integral term
    void resetIntegral() { integralTerm = 0; }
};
