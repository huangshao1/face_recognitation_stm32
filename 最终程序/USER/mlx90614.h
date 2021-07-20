
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MLX90614_H
#define __MLX90614_H



/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8);
u8 SMBus_SendByte(u8);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8);
void SMBus_Delay(u16);
void SMBus_Init(void);
u8 PEC_Calculation(u8*);
float SMBus_ReadTemp(u8 mode);

void temp_display(float a);//�¶���ʾ
void templimit_warning(float b,u8 c);//b-�¶ȣ�c-������ֵ
void 	warning(float b);//�����¶��򱨾�


#endif

/*********************************END OF FILE*********************************/
