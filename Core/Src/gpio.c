/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, M1_D1_Pin|zawor1_Pin|zawor2_Pin|zawor3_Pin
                          |zawor4_Pin|zawor5_Pin|zawor6_Pin|zawor7_Pin
                          |zawor8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(M1_E2_GPIO_Port, M1_E2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(M2_D1_GPIO_Port, M2_D1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, M3_D1_Pin|M3_E2_Pin|M2_E2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : M1_D1_Pin M1_E2_Pin zawor1_Pin zawor2_Pin
                           zawor3_Pin zawor4_Pin zawor5_Pin zawor6_Pin
                           zawor7_Pin zawor8_Pin */
  GPIO_InitStruct.Pin = M1_D1_Pin|M1_E2_Pin|zawor1_Pin|zawor2_Pin
                          |zawor3_Pin|zawor4_Pin|zawor5_Pin|zawor6_Pin
                          |zawor7_Pin|zawor8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : M2_D1_Pin */
  GPIO_InitStruct.Pin = M2_D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(M2_D1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : M3_D1_Pin M3_E2_Pin M2_E2_Pin */
  GPIO_InitStruct.Pin = M3_D1_Pin|M3_E2_Pin|M2_E2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : krancowka1_Pin krancowka2_Pin */
  GPIO_InitStruct.Pin = krancowka1_Pin|krancowka2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 2 */
#include "MC34931.h"
extern uint8_t current_limit;
extern uint8_t up_limit;
extern uint8_t down_limit;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == krancowka1_Pin)
    {
    	Motor_SetSpeed(&Motor3, DIR_CW, 0);
    	up_limit = 1;
    	current_limit = 0;
    }
    if (GPIO_Pin == krancowka2_Pin)
    {
       Motor_SetSpeed(&Motor3, DIR_CW, 0);
       down_limit = 1;
       current_limit = 0;
    }
}
/* USER CODE END 2 */
