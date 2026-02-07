#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "stm32f1xx_hal.h"

// Definicja struktury silnika
typedef struct {
    // Timery PWM dla wejść IN1 i IN2
    TIM_HandleTypeDef *htim;
    uint32_t channel_IN1;
    uint32_t channel_IN2;

    // Piny sterujące D1 (Disable) i E2 (Enable/Sleep)
    GPIO_TypeDef *D1_Port;
    uint16_t D1_Pin;
    GPIO_TypeDef *E2_Port;
    uint16_t E2_Pin;
} Motor_Config_t;

// Kierunek obrotów
typedef enum {
    DIR_CW = 0, // Clockwise
    DIR_CCW = 1 // Counter-Clockwise
} Motor_Dir_t;

// Obiekty silników (deklaracja zewnętrzna)
extern Motor_Config_t Motor1;
extern Motor_Config_t Motor2;
extern Motor_Config_t Motor3;

// Funkcje
void Motor_Init_Structs(void); // Mapowanie pinów (wywołaj raz)
void Motor_Init_Hardware(Motor_Config_t *motor); // Odpalenie sprzętu
void Motor_SetSpeed(Motor_Config_t *motor, Motor_Dir_t dir, uint8_t speed_percent);
void Motor_Stop(Motor_Config_t *motor);
void Motor_Sleep(Motor_Config_t *motor, uint8_t enable_sleep);

#endif
