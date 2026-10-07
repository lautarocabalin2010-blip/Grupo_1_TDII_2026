/*
 * API_debounce.h
 *
 *  Created on: 4 oct 2026
 *      Author: Chaile, Mariano Oscar
 *      Legajo: 57362
 *      Comisión: 4R1
 *      Function of driver: Implementación de una máquina de estados para eliminar el rebote mecánico de pulsadores.
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
