/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define M1_D1_Pin GPIO_PIN_2
#define M1_D1_GPIO_Port GPIOC
#define M1_E2_Pin GPIO_PIN_3
#define M1_E2_GPIO_Port GPIOC
#define M1_IN2_Pin GPIO_PIN_0
#define M1_IN2_GPIO_Port GPIOA
#define M1_IN1_Pin GPIO_PIN_1
#define M1_IN1_GPIO_Port GPIOA
#define M2_D1_Pin GPIO_PIN_3
#define M2_D1_GPIO_Port GPIOA
#define M2_IN2_Pin GPIO_PIN_6
#define M2_IN2_GPIO_Port GPIOA
#define M2_IN1_Pin GPIO_PIN_7
#define M2_IN1_GPIO_Port GPIOA
#define zawor1_Pin GPIO_PIN_4
#define zawor1_GPIO_Port GPIOC
#define zawor2_Pin GPIO_PIN_5
#define zawor2_GPIO_Port GPIOC
#define M3_IN2_Pin GPIO_PIN_0
#define M3_IN2_GPIO_Port GPIOB
#define M3_IN1_Pin GPIO_PIN_1
#define M3_IN1_GPIO_Port GPIOB
#define M3_D1_Pin GPIO_PIN_2
#define M3_D1_GPIO_Port GPIOB
#define M3_E2_Pin GPIO_PIN_10
#define M3_E2_GPIO_Port GPIOB
#define M2_E2_Pin GPIO_PIN_11
#define M2_E2_GPIO_Port GPIOB
#define krancowka1_Pin GPIO_PIN_12
#define krancowka1_GPIO_Port GPIOB
#define krancowka1_EXTI_IRQn EXTI15_10_IRQn
#define krancowka2_Pin GPIO_PIN_13
#define krancowka2_GPIO_Port GPIOB
#define krancowka2_EXTI_IRQn EXTI15_10_IRQn
#define zawor3_Pin GPIO_PIN_6
#define zawor3_GPIO_Port GPIOC
#define zawor4_Pin GPIO_PIN_7
#define zawor4_GPIO_Port GPIOC
#define zawor5_Pin GPIO_PIN_8
#define zawor5_GPIO_Port GPIOC
#define zawor6_Pin GPIO_PIN_9
#define zawor6_GPIO_Port GPIOC
#define zawor7_Pin GPIO_PIN_10
#define zawor7_GPIO_Port GPIOC
#define zawor8_Pin GPIO_PIN_11
#define zawor8_GPIO_Port GPIOC
#define TALON_PWM_Pin GPIO_PIN_7
#define TALON_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
