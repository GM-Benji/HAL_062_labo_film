#ifndef __AX12__H__

#define __AX12__H__

#include <stdint.h>
//Adresy urządzeń
#define AX12_ID_BROADCAST   0xFE
#define AX12_ID_MOTOR       0x1E
#define AX12_ID_MOTOR2      0x13


// Instrukcje
#define INST_WRITE_DATA     0x03
#define INST_READ_DATA      0x02

// Adresy
#define ADDR_GOAL_POSITION  0x1E
#define ADDR_LED            0x19
#define ADDR_MOVING_SPEED   0x20


uint8_t AX12_CalculateChecksum(uint8_t id, uint8_t len, uint8_t instruction, uint8_t* params, uint8_t params_len);
void AX12_SendPacket(uint8_t id, uint8_t instruction, uint8_t* params, uint8_t params_len);
void AX12_SetLED(uint8_t id, uint8_t state);
void AX12_SetGoalPosition(uint8_t id, uint16_t position);
void AX12_ScanBus(void);
void AX12_SetMode_Wheel(uint8_t id);
void AX12_Wheel_Move(uint8_t id, int16_t speed);
void AX12_SetMode_Joint(uint8_t id);



#endif