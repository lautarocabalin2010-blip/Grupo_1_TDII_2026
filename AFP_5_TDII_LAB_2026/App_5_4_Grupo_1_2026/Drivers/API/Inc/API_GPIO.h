/*
 * API_GPIO.h
 *
 *  Created on: 31 jul 2026
 *      Author: Ayala Cabalin, Lautaro Leonel
 *      Legajo: 56018
 *      Comisión: 4R1
 *      Function of driver: Driver que contiene funciones para manejo de puertos GPIO que utiliza funciones de la HAL de STM32 Nucleo F439
 */

#ifndef API_INC_API_GPIO_H_
#define API_INC_API_GPIO_H_


typedef struct
{
    GPIO_TypeDef *Puerto;
    uint16_t Pin;
} Led_t;

void PrenderLed(Led_t led);

void ApagarLed (Led_t led);

void MX_GPIO_Init(void);

#endif /* API_INC_API_GPIO_H_ */
