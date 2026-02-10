/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */

#include "ax12.h"
#include "MC34931.h"
CAN_RxHeaderTypeDef	RxHeader;
uint8_t				RxData[8];
uint16_t currPosition1 = 0;
uint16_t currPosition2 = 0;
extern TIM_HandleTypeDef htim4;
//ograniczenie pradowe, jedna rzecz na raz wlaczona
uint8_t current_limit=0;
//krancowki do ruchu wiertla
uint8_t up_limit=0;
uint8_t down_limit=0;
//id dynamixeli
uint8_t AX_ID_1 = 0x1E;
uint8_t AX_ID_2 = 0x01;
/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 4;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
  	CAN_FilterTypeDef sFilterConfig;

    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
    {
      Error_Handler();
    }

    // 2. Uruchomienie modułu CAN
    if (HAL_CAN_Start(&hcan) != HAL_OK)
    {
      Error_Handler();
    }

    // 3. Włączenie przerwania od odbioru (FIFO0 Message Pending)
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
      Error_Handler();
    }
  /* USER CODE END CAN_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_HP_CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if(RxHeader.StdId == 0x96)
	  {

		  if(RxData[0] == 1) // zadany kat w prawo
		  {
			  uint16_t position = RxData[1]<<8;
			  position+=RxData[2];
			  uint16_t setPosition = currPosition1 + position;
			  if(setPosition>1023) setPosition = 1023;
			  AX12_SetGoalPosition(AX_ID_1,setPosition);
			  currPosition1 = setPosition;
		  }
		  if(RxData[0] == 2) // zadany kat w lewo
		  {
			  uint16_t position = RxData[1]<<8;
			  position+=RxData[2];
			  uint16_t setPosition;
			  if(position > currPosition1) setPosition = 0;
			  else setPosition = currPosition1 - position;
			  AX12_SetGoalPosition(AX_ID_1,setPosition);
			  currPosition1 = setPosition;
		  }
		  switch(RxData[3])
		  {
		  case 0: //wylaczyc silniki
			  Motor_SetSpeed(&Motor2, DIR_CCW, 0);
			  Motor_SetSpeed(&Motor3, DIR_CCW, 0);
			  current_limit = 0;
			  break;
		  case 1: //mieszadlo
			  if(!current_limit)
			  {
				  Motor_SetSpeed(&Motor2, DIR_CCW, RxData[4]);
				  current_limit = 1;
			  }
			  break;
		  case 2: //mieszadlo w druga strone
			  if(!current_limit)
			  {
				  Motor_SetSpeed(&Motor2, DIR_CW, RxData[4]);
				  current_limit = 1;
			  }
			  break;
		  case 3: //ruch wiertla
			  if(!current_limit && !up_limit)
			  {
				  Motor_SetSpeed(&Motor3, DIR_CCW, RxData[4]);
				  down_limit = 0;
				  current_limit = 1;
			  }
		  	  break;
		  case 4://ruch wiertla w druga strone
			  if(!current_limit && !down_limit)
			  {
				  Motor_SetSpeed(&Motor3, DIR_CW, RxData[4]);
				  up_limit =0;
				  current_limit = 1;
			  }
		  	  break;
		  default:
			  break;
		  };
		  if(RxData[5] == 1)
		  { // wiertlo kreci sie
			  uint16_t temp = RxData[6]*1600/255;
			  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 4800+temp);
		  }
		  if(RxData[5] == 2)
		  { // wiertlo kreci sie w druga strone
			  uint16_t temp = RxData[6]*1600/255;
			  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 4800-temp);
		  }
		  if(RxData[5] == 0)
		  { // wiertlo nie  kreci sie
			  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 4800);
		  }
	  }
	  if(RxHeader.StdId == 0x97)
	  {
		  if(RxData[0] == 1) // zadany kat w prawo
		  	{
			  uint16_t position = RxData[1]<<8;
			  position+=RxData[2];
			  uint16_t setPosition = currPosition2 + position;
			  if(setPosition>1023) setPosition = 1023;
			  AX12_SetGoalPosition(AX_ID_2,setPosition);
			  currPosition2 = setPosition;
		  	}
		  if(RxData[0] == 2) // zadany kat w lewo
		  	{
			  uint16_t position = RxData[1]<<8;
			  position+=RxData[2];
			  uint16_t setPosition;
			  if(position > currPosition2) setPosition = 0;
			  else setPosition = currPosition2 - position;
			  AX12_SetGoalPosition(AX_ID_2,setPosition);
			  currPosition2 = setPosition;
		  	}
	  }
}
/* USER CODE END 1 */
