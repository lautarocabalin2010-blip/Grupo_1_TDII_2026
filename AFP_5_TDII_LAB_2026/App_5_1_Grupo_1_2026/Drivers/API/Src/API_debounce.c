/*Includes -------------------------------------------------------------------*/
#include "main.h"
#include "API_debounce.h"
#include "API_delay.h"

/*Defines --------------------------------------------------------------------*/
#define DEBOUNCE_DELAY 40

/*Declaracion de variables */
static debounceState_t actualState;
static bool_t keyPressed = false;
static delay_t debounceDelay;

void debounceFSM_init(void)
{
    actualState = BUTTON_UP;
    delayInit(&debounceDelay, DEBOUNCE_DELAY);
}

void debounceFSM_update(void)
{
    // Leemos el estado del botón actual (Activo alto en la NUCLEO-F4)
    bool_t buttonRead = (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_SET);

    switch (actualState)
    {
    case BUTTON_UP:
        if(buttonRead == true)
        {
            actualState = BUTTON_FALLING;
            delayRead(&debounceDelay);   // arranca cuenta de 40 ms
        }
        break;

    case BUTTON_FALLING:
        if(delayRead(&debounceDelay))
        {
            if(buttonRead == true)
            {
                buttonPressed();   // Ejecuta la acción obligatoria (LED1)
                keyPressed = true;
                actualState = BUTTON_DOWN;
            }
            else
            {
                actualState = BUTTON_UP;
            }
        }
        break;

    case BUTTON_DOWN:
        if(buttonRead == false)
        {
            actualState = BUTTON_RISING;
            delayRead(&debounceDelay);   // arranca cuenta de 40 ms
        }
        break;

    case BUTTON_RISING:
        if(delayRead(&debounceDelay))
        {
            if(buttonRead == false)
            {
                buttonReleased();  // Ejecuta la acción obligatoria (LED3)
                keyPressed = false;
                actualState = BUTTON_UP;
            }
            else
            {
                actualState = BUTTON_DOWN;
            }
        }
        break;

    default:
        Error_Handler();
        break;
    }
}

bool_t readKey(void)
{
    bool_t keyPress = false;

    if(keyPressed)
    {
        keyPress = true;
        keyPressed = false; // Limpia la bandera
    }
    return keyPress;
}

void buttonPressed(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0); // Cambia por el pin de tu LED1
}

void buttonReleased(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7); // Cambia por el pin de tu LED3
}
