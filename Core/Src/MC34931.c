#include "MC34931.h"

// Zakładam, że timery są zadeklarowane w main.c przez CubeMX
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

Motor_Config_t Motor1;
Motor_Config_t Motor2;
Motor_Config_t Motor3;

// 1. Przypisanie pinów i timerów do struktur (zgodnie z Twoim schematem)
void Motor_Init_Structs(void) {
    // --- SILNIK 1 ---
    Motor1.htim = &htim2;
    Motor1.channel_IN1 = TIM_CHANNEL_2; // PA1
    Motor1.channel_IN2 = TIM_CHANNEL_1; // PA0
    Motor1.D1_Port = GPIOC;
    Motor1.D1_Pin = GPIO_PIN_2;         // M1_D1
    Motor1.E2_Port = GPIOC;
    Motor1.E2_Pin = GPIO_PIN_3;         // M1_E2

    // --- SILNIK 2 ---
    Motor2.htim = &htim3;
    Motor2.channel_IN1 = TIM_CHANNEL_2; // PA7
    Motor2.channel_IN2 = TIM_CHANNEL_1; // PA6
    Motor2.D1_Port = GPIOA;
    Motor2.D1_Pin = GPIO_PIN_3;         // M2_D1
    Motor2.E2_Port = GPIOB;
    Motor2.E2_Pin = GPIO_PIN_11;        // M2_E2 (zgodnie z obrazkiem)

    // --- SILNIK 3 ---
    Motor3.htim = &htim3;
    Motor3.channel_IN1 = TIM_CHANNEL_4; // PB1
    Motor3.channel_IN2 = TIM_CHANNEL_3; // PB0
    Motor3.D1_Port = GPIOB;
    Motor3.D1_Pin = GPIO_PIN_2;         // M3_D1
    Motor3.E2_Port = GPIOB;
    Motor3.E2_Pin = GPIO_PIN_10;        // M3_E2
}

// 2. Inicjalizacja sprzętowa (start PWM, ustawienie pinów Enable/Disable)
void Motor_Init_Hardware(Motor_Config_t *motor) {
    // Ustawienie pinów sterujących zgodnie z datasheetem MC34931
    // D1 (Disable) -> LOW (aby mostek był aktywny)
    HAL_GPIO_WritePin(motor->D1_Port, motor->D1_Pin, GPIO_PIN_RESET);

    // E2 (Enable/Sleep) -> HIGH (aby wybudzić układ ze Sleep)
    HAL_GPIO_WritePin(motor->E2_Port, motor->E2_Pin, GPIO_PIN_SET);

    // Start PWM z wypełnieniem 0
    __HAL_TIM_SET_COMPARE(motor->htim, motor->channel_IN1, 0);
    __HAL_TIM_SET_COMPARE(motor->htim, motor->channel_IN2, 0);

    HAL_TIM_PWM_Start(motor->htim, motor->channel_IN1);
    HAL_TIM_PWM_Start(motor->htim, motor->channel_IN2);
}

// 3. Główna funkcja sterująca
// speed_percent: 0 - 100
void Motor_SetSpeed(Motor_Config_t *motor, Motor_Dir_t dir, uint8_t speed_percent) {
    // Zabezpieczenie zakresu
    if (speed_percent > 100) speed_percent = 100;

    // Sterowanie mostkiem H: Jeden pin PWM, drugi LOW (GND)
    // Zgodnie z tabelą prawdy: H/L = Forward, L/H = Reverse [cite: 566]

    if (dir == DIR_CW) {
        // IN1 = PWM, IN2 = LOW
        __HAL_TIM_SET_COMPARE(motor->htim, motor->channel_IN1, speed_percent*63);
        __HAL_TIM_SET_COMPARE(motor->htim, motor->channel_IN2, 0);
    } else {
        // IN1 = LOW, IN2 = PWM
        __HAL_TIM_SET_COMPARE(motor->htim, motor->channel_IN1, 0);
        __HAL_TIM_SET_COMPARE(motor->htim, motor->channel_IN2, speed_percent*63);
    }
}

// Funkcja wprowadzająca driver w tryb Sleep (oszczędzanie energii < 12uA) [cite: 18, 207]
void Motor_Sleep(Motor_Config_t *motor, uint8_t enable_sleep) {
    if (enable_sleep) {
        // EN/D2 = LOW -> Sleep Mode
        HAL_GPIO_WritePin(motor->E2_Port, motor->E2_Pin, GPIO_PIN_RESET);
    } else {
        // EN/D2 = HIGH -> Wake up
        HAL_GPIO_WritePin(motor->E2_Port, motor->E2_Pin, GPIO_PIN_SET);
    }
}
