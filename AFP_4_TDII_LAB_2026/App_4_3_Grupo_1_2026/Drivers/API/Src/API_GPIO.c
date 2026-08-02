/*
 * API_GPIO.c
 *
 *  Created on: 31 jul 2026
 *      Author: Ayala Cabalin, Lautaro Leonel
 *      Legajo: 56018
 *      Comisión: 4R1
 */

#include "main.h"
#include "API_GPIO.h"

// Function definition

/**
 * @brief Enciende el LED recibido como parámetro
 * @param led Estructura de tipo Led_t que contiene el puerto y el pin del LED
 * @retval None
 */

void PrenderLed(Led_t led)
{
    HAL_GPIO_WritePin(led.Puerto, led.Pin, GPIO_PIN_SET);
}

/**
 * @brief Apaga el LED recibido como parámetro
 * @param led Estructura de tipo Led_t que contiene el puerto y el pin del LED
 * @retval None
 */

void ApagarLed (Led_t led)
{
	HAL_GPIO_WritePin (led.Puerto, led.Pin, GPIO_PIN_RESET);
}

/**
 * @brief Cambia la secuencia de LEDs cuando se presiona el pulsador
 * @param secuencia Número de la secuencia actual
 * @retval Devuelve el número de la nueva secuencia
 */
uint8_t Cambiar_Secuencia (uint8_t secuencia)
{
    // Memoria del estado del botón en la lectura anterior
    static GPIO_PinState estado_anterior = GPIO_PIN_RESET;

    GPIO_PinState estado_actual = HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin);

    // Solo cambiamos si hay un flanco de subida (recién presionado)
    if (estado_actual == GPIO_PIN_SET && estado_anterior == GPIO_PIN_RESET)
    {
        secuencia = secuencia + 1;
        if (secuencia > 4)
        {
            secuencia = 1;
        }
    }

    estado_anterior = estado_actual; // Guardamos para la próxima vuelta

    return secuencia;
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

