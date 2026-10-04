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

#include "API_delay.h" // Para usar bool_t y delay_t

// Definición de los estados de la máquina de estados
typedef enum {
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RISING
} debounceState_t;

// Prototipos de funciones
void debounceFSM_init(void);
void debounceFSM_update(void);
bool_t readKey(void);

#endif /* API_INC_API_DEBOUNCE_H_ */
