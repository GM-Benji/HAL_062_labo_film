/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "main.h"
#define AX12_INST_WRITE       0x03  // Instrukcja zapisu
#define AX12_GOAL_POSITION_L  0x1E  // Adres pozycji zapisu
#define AX12_TORQUE_ENABLE    0x18  // Adres włączania momentu
#define AX12_ID_BROADCAST     0xFE  // ID, na które reagują wszystkie serwa
#define AX12_LED_ADDR 0x19



/*void AX12_SetPosition(UART_HandleTypeDef *huart, uint8_t id, uint16_t position) {
    uint8_t buffer[9];
    uint8_t length = 5; // Długość parametrów

    if (position > 1023) position = 1023;

    //  Budowa ramki
    buffer[0] = 0xFF; // Nagłówek
    buffer[1] = 0xFF; // Nagłówek
    buffer[2] = id;   // ID Serwa
    buffer[3] = length; // Długość
    buffer[4] = AX12_INST_WRITE; // Instrukcja zapisu
    buffer[5] = AX12_GOAL_POSITION_L; // Adres rejestru gdzie zaczynamy pisać
    buffer[6] = (uint8_t)(position & 0xFF); // Młodszy bajt pozycji
    buffer[7] = (uint8_t)((position >> 8) & 0xFF); // Starszy bajt pozycji

    // Obliczanie sumy kontrolnej
    uint16_t checksum = buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6] + buffer[7];
    buffer[8] = (uint8_t)(~checksum); // Negacja bitowa

    HAL_UART_Transmit(huart, buffer, 9, 10);
}
void AX12_TorqueEnable(UART_HandleTypeDef *huart, uint8_t id, uint8_t enable) {
    uint8_t buffer[8];
    uint8_t length = 4; // Parametry: Adres(1) + Dane(1) + 2 = 4

    buffer[0] = 0xFF;
    buffer[1] = 0xFF;
    buffer[2] = id;
    buffer[3] = length;
    buffer[4] = AX12_INST_WRITE;
    buffer[5] = AX12_TORQUE_ENABLE; // Adres 0x18
    buffer[6] = enable;             // 1 = ON, 0 = OFF

    uint16_t checksum = buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6];
    buffer[7] = (uint8_t)(~checksum);

    HAL_UART_Transmit(huart, buffer, 8, 10);
}
void AX12_SetLED(UART_HandleTypeDef *huart, uint8_t id, uint8_t state) {
    uint8_t buffer[8];
    uint8_t length = 4; // Parametry: Adres(1) + Dane(1) + 2

    buffer[0] = 0xFF;
    buffer[1] = 0xFF;
    buffer[2] = id;
    buffer[3] = length;
    buffer[4] = AX12_INST_WRITE; // 0x03
    buffer[5] = AX12_LED_ADDR;   // 0x19
    buffer[6] = state;           // 0 = OFF, 1 = ON

    // Checksum
    uint16_t checksum = buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6];
    buffer[7] = (uint8_t)(~checksum);

    HAL_UART_Transmit(huart, buffer, 8, 10);
}*/
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 1000000;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
