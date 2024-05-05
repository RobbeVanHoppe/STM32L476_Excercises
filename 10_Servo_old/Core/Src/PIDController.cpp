//
// Created by robbe on 3/05/24.
//

#include "PIDController.h"

#include "Servo.h"

float PIDController::calculate(float setpoint, float position) {
    // Calculate error
    const float error = setpoint - position;

    // Proportional term
    const float proportional = Kp * error;

    // Integral term
    integralTerm += Ki * error;

    // Derivative term
    const float derivative = Kd * (error - prevError);
    prevError = error;

    // Calculate control signal
    const float controlSignal = proportional + integralTerm + derivative;

    return controlSignal;
}

// uint16_t PIDController::getServoPosition(TIM_HandleTypeDef* htim, uint16_t channel)
// {
//     // Assuming a linear relationship between PWM duty cycle and position
//     // You may need to adjust these values based on your servo's characteristics
//     const float minDutyCycle = 5.0f;  // Minimum duty cycle for 0 degrees
//     const float maxDutyCycle = 10.0f; // Maximum duty cycle for 180 degrees
//
//     // Read the current PWM duty cycle from the TIM channel
//     uint16_t currentDutyCycle = __HAL_TIM_GET_COMPARE(htim, channel);
//
//     // Estimate the position based on the PWM duty cycle
//     float position = Servo::map(currentDutyCycle, minDutyCycle, maxDutyCycle, 0, 180);
//
//     return static_cast<uint16_t>(position);
// }
