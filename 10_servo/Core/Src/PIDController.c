//
// Created by robbe on 5/05/24.
//
#include "PIDController.h"


float Kp = 0.1; // Proportional gain
float Ki = 0.1; // Integral gain
float Kd = 0.1; // Derivative gain

float integralTerm = 0; // Integral term accumulator
float prevError = 0; // Previous error

float PID_calculate(float setpoint, float position)
{
    const float error = setpoint - position;
    integralTerm += error;
    const float derivative = error - prevError;
    prevError = error;
    return Kp * error + Ki * integralTerm + Kd * derivative;
}

uint16_t PID_getServoPosition(TIM_HandleTypeDef* htim, uint16_t channel)
{
    // Assuming a linear relationship between PWM duty cycle and position
    // You may need to adjust these values based on your servo's characteristics
    const uint16_t minDutyCycle = 5;  // Minimum duty cycle for 0 degrees
    const uint16_t maxDutyCycle = 10; // Maximum duty cycle for 180 degrees

    // Read the current PWM duty cycle from the TIM channel
    uint16_t currentDutyCycle = __HAL_TIM_GET_COMPARE(htim, channel);

    // Estimate the position based on the PWM duty cycle
    float position = Servo_map(currentDutyCycle, minDutyCycle, maxDutyCycle, 0, 180);

    return (uint16_t)position;
}
