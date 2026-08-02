/*
 * API_delay.c
 *
 *  Created on: 31 jul 2026
 */

#include "API_delay.h"
#include "main.h" // Necesario para usar HAL_GetTick()

/**
 * @brief Cargar duración, inicializar flag running en false, no iniciar conteo.
 */
void delayInit(delay_t * delay, tick_t duration)
{
    delay->duration = duration;
    delay->running = false;
}

/**
 * @brief Verificar estado, tomar marca de tiempo, evaluar cumplimiento y reiniciar flag.
 */
bool_t delayRead(delay_t * delay)
{
    bool_t timeArrived = false;

    if (delay->running == false)
    {
        delay->startTime = HAL_GetTick(); // Toma marca de tiempo
        delay->running = true;            // Cambia flag a true
    }
    else
    {
        if ((HAL_GetTick() - delay->startTime) >= delay->duration)
        {
            timeArrived = true;       // Se cumplió el tiempo
            delay->running = false;   // Reinicia el flag
        }
    }

    return timeArrived;
}

/**
 * @brief Cambiar duración de un delay existente.
 */
void delayWrite(delay_t * delay, tick_t duration)
{
    delay->duration = duration;
}
