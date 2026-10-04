/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "API_GPIO.h"
#include "API_delay.h" // ¡Incluimos nuestro driver de retardos!
#include "API_debounce.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Cantidad_Led 3
#define LED_AZUL_PORT   GPIOB
#define LED_AZUL_PIN    GPIO_PIN_0
#define LED_ROJO_PORT   GPIOB
#define LED_ROJO_PIN    GPIO_PIN_7
#define LED_VERDE_PORT  GPIOB
#define LED_VERDE_PIN   GPIO_PIN_14
uint8_t secuencia=1;

Led_t Leds[Cantidad_Led] =
{
    {LED_AZUL_PORT, LED_AZUL_PIN},
    {LED_ROJO_PORT, LED_ROJO_PIN},
    {LED_VERDE_PORT, LED_VERDE_PIN}
};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

ETH_HandleTypeDef heth;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);

int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();

  /* USER CODE BEGIN 2 */

  // 1. Declaramos nuestro objeto retardo utilizando la API
  delay_t retardoFrecuencia;

  // 2. Inicializamos el retardo en 100 ms (Tiempo 1 por defecto)
  delayInit(&retardoFrecuencia, 100);

  uint8_t estado_leds = 0; // 0 = Apagados, 1 = Encendidos

  /* USER CODE END 2 */

  while (1)
  {
	  debounceFSM_update();

	          // 2. LÓGICA DE CAMBIO DE SECUENCIA DIRECTO EN EL WHILE
	          // readKey() ya se encarga de los flancos y el anti-rebote.
	          // Solo entrará a este 'if' una vez cada vez que presiones el botón.
	          if (readKey() == true)
	          {
	              secuencia = secuencia + 1;
	              if (secuencia > 4)
	              {
	                  secuencia = 1;
	              }
	          }

      switch (secuencia)
      {
          case 1:
              delayWrite(&retardoFrecuencia, 100);
              break;
          case 2:
              delayWrite(&retardoFrecuencia, 250);
              break;
          case 3:
              delayWrite(&retardoFrecuencia, 500);
              break;
          case 4:
              delayWrite(&retardoFrecuencia, 1000);
              break;
          default:
              delayWrite(&retardoFrecuencia, 100);
              secuencia = 1;
              break;
      }

      // 3. Evaluamos si el tiempo se cumplió utilizando delayRead de nuestra API
      if (delayRead(&retardoFrecuencia) == true)
      {
          // Alternamos el estado de TODOS los LEDs simultáneamente
          if (estado_leds == 0)
          {
              for (int i = 0; i < Cantidad_Led; i++)
              {
                  PrenderLed(Leds[i]);
              }
              estado_leds = 1;
          }
          else
          {
              for (int i = 0; i < Cantidad_Led; i++)
              {
                  ApagarLed(Leds[i]);
              }
              estado_leds = 0;
          }
      }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_ETH_Init(void)
{
   static uint8_t MACAddr[6];

  heth.Instance = ETH;
  MACAddr[0] = 0x00; MACAddr[1] = 0x80; MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00; MACAddr[4] = 0x00; MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;
  if (HAL_ETH_Init(&heth) != HAL_OK) Error_Handler();

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
}

static void MX_USART3_UART_Init(void)
{
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK) Error_Handler();
}

static void MX_USB_OTG_FS_PCD_Init(void)
{
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK) Error_Handler();
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {}
#endif
