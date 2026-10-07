/*
 * API_GPIO.h
 *
 *  Created on: 31 jul 2026
 *      Author: Chehuán, Aaron
 *      Legajo: 56016
 *      Comisión: 4R1
 *      Function of driver: Driver que contiene funciones para manejo de puertos GPIO que utiliza funciones de la HAL de STM32 Nucleo F439
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include "API_delay.h"

typedef enum {
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RISING
} debounceState_t;

void debounceFSM_init(void);
void debounceFSM_update(void);
bool_t readKey(void);

void buttonPressed(void);
void buttonReleased(void);

#endif /* API_INC_API_DEBOUNCE_H_ */
