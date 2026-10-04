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
#include "API_delay.h" // ¡Incluimos nuestro driver modular de retardos!
#include "API_debounce.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();

  /* USER CODE BEGIN 2 */

  // Declaramos nuestros objetos retardos utilizando el tipo delay_t de la API
  delay_t delay_seq1;
  delay_t delay_seq2;
  delay_t delay_seq4;
  delay_t delay_led1, delay_led2, delay_led3;

  // Inicializamos los retardos con los tiempos de cada secuencia
  delayInit(&delay_seq1, 150);
  delayInit(&delay_seq2, 300);
  delayInit(&delay_seq4, 150);
  delayInit(&delay_led1, 100);
  delayInit(&delay_led2, 300);
  delayInit(&delay_led3, 600);

  // Variables de estado
  uint8_t indice_led = 0;
  uint8_t estado_general = 0; // Para secuencias 1, 2 y 4
  uint8_t e_led1 = 0, e_led2 = 0, e_led3 = 0; // Estados para la Secuencia 3

  /* USER CODE BEGIN 2 */

    // Variables para el control de las secuencias
    uint8_t secuencia_anterior = 1;
    // (Asegúrate de que tus variables delay_t y estados estén declaradas aquí como las tenías)
    // ...

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        // 1. Actualizamos la máquina de estados del botón en cada ciclo
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

        // 3. DETECTOR DE CAMBIO DE SECUENCIA
        // Si el botón cambió la secuencia, apagamos todo y reseteamos variables
        if (secuencia != secuencia_anterior)
        {
            for (int i = 0; i < Cantidad_Led; i++)
            {
                ApagarLed(Leds[i]);
            }
            estado_general = 0;
            indice_led = 0;
            e_led1 = 0; e_led2 = 0; e_led3 = 0;

            secuencia_anterior = secuencia;
        }

        // 4. EJECUCIÓN DE SECUENCIAS UTILIZANDO EL DRIVER API_DELAY
        switch (secuencia)
        {
            case 1: // Secuencia 1: 150 ms (Uno por uno)
                if (delayRead(&delay_seq1) == true)
                {
                    if (estado_general == 0) {
                        PrenderLed(Leds[indice_led]);
                        estado_general = 1;
                    } else {
                        ApagarLed(Leds[indice_led]);
                        estado_general = 0;
                        indice_led++;
                        if (indice_led >= Cantidad_Led) indice_led = 0;
                    }
                }
                break;

            case 2: // Secuencia 2: 300 ms (Todos juntos)
                if (delayRead(&delay_seq2) == true)
                {
                    if (estado_general == 0) {
                        for (int i = 0; i < Cantidad_Led; i++) PrenderLed(Leds[i]);
                        estado_general = 1;
                    } else {
                        for (int i = 0; i < Cantidad_Led; i++) ApagarLed(Leds[i]);
                        estado_general = 0;
                    }
                }
                break;

            case 3: // Secuencia 3: Tiempos independientes (100ms, 300ms, 600ms)
                if (delayRead(&delay_led1) == true) {
                    if (e_led1 == 0) { PrenderLed(Leds[0]); e_led1 = 1; }
                    else { ApagarLed(Leds[0]); e_led1 = 0; }
                }
                if (delayRead(&delay_led2) == true) {
                    if (e_led2 == 0) { PrenderLed(Leds[1]); e_led2 = 1; }
                    else { ApagarLed(Leds[1]); e_led2 = 0; }
                }
                if (delayRead(&delay_led3) == true) {
                    if (e_led3 == 0) { PrenderLed(Leds[2]); e_led3 = 1; }
                    else { ApagarLed(Leds[2]); e_led3 = 0; }
                }
                break;

            case 4: // Secuencia 4: 150 ms (Inversos)
                if (delayRead(&delay_seq4) == true)
                {
                    if (estado_general == 0) {
                        PrenderLed(Leds[0]);
                        ApagarLed(Leds[1]);
                        PrenderLed(Leds[2]);
                        estado_general = 1;
                    } else {
                        ApagarLed(Leds[0]);
                        PrenderLed(Leds[1]);
                        ApagarLed(Leds[2]);
                        estado_general = 0;
                    }
                }
                break;
        }
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
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

  /** Initializes the CPU, AHB and APB buses clocks
  */
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

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{
   static uint8_t MACAddr[6];

  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
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
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
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
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
