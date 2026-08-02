/*
 * API_delay.h
 *
 *  Created on: 31 jul 2026
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

#include <stdint.h>
#include <stdbool.h>

// Definición de tipos personalizados pedidos en el TP
typedef uint32_t tick_t;
typedef bool bool_t;

// Estructura de retardo
typedef struct {
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

// Prototipos de funciones públicas
void delayInit(delay_t * delay, tick_t duration);
bool_t delayRead(delay_t * delay);
void delayWrite(delay_t * delay, tick_t duration);

#endif /* API_INC_API_DELAY_H_ */
