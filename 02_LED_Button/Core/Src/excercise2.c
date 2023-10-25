//
// Created by robbevh on 10/25/23.
//

#include "excercise2.h"
#include "main.h"

int isButtonPressed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    // check if the pin is SET
    if (HAL_GPIO_ReadPin(GPIOx , GPIO_Pin) == GPIO_PIN_SET) {
        return 1;
    }
    return 0;
}



