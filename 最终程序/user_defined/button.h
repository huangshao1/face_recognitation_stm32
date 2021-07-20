#ifndef __BUTTON_H_
#define __BUTTON_H_

#include "delay.h"
#include "sys.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

//*****************按键接口定义区******************
#define C1_port GPIOB
#define C2_port GPIOB
#define C3_port GPIOB
#define C4_port GPIOB
#define R1_port GPIOB
#define R2_port GPIOB
#define R3_port GPIOB
#define R4_port GPIOB


#define C1_pin GPIO_Pin_0
#define C2_pin GPIO_Pin_1
#define C3_pin GPIO_Pin_10
#define C4_pin GPIO_Pin_11
#define R1_pin GPIO_Pin_12
#define R2_pin GPIO_Pin_13
#define R3_pin GPIO_Pin_14
#define R4_pin GPIO_Pin_15
//*****************按键接口定义区******************

#define key1_1 GPIO_SetBits(C1_port, C1_pin)
#define key1_0 GPIO_ResetBits(C1_port, C1_pin)

#define key2_1 GPIO_SetBits(C2_port, C2_pin)
#define key2_0 GPIO_ResetBits(C2_port, C2_pin)

#define key3_1 GPIO_SetBits(C3_port, C3_pin)
#define key3_0 GPIO_ResetBits(C3_port, C3_pin)

#define key4_1 GPIO_SetBits(C4_port, C4_pin)
#define key4_0 GPIO_ResetBits(C4_port, C4_pin)

#define key5_1 GPIO_SetBits(R1_port, R1_pin)
#define key5_0 GPIO_ResetBits(R1_port, R1_pin)

#define key6_1 GPIO_SetBits(R2_port, R2_pin)
#define key6_0 GPIO_ResetBits(R2_port, R2_pin)

#define key7_1 GPIO_SetBits(R3_port, R3_pin)
#define key7_0 GPIO_ResetBits(R3_port, R3_pin)

#define key8_1 GPIO_SetBits(R4_port, R4_pin)
#define key8_0 GPIO_ResetBits(R4_port, R4_pin)

#define key1 GPIO_ReadInputDataBit(C1_port,C1_pin)
#define key2 GPIO_ReadInputDataBit(C2_port,C2_pin)
#define key3 GPIO_ReadInputDataBit(C3_port,C3_pin)
#define key4 GPIO_ReadInputDataBit(C4_port,C4_pin)
#define key5 GPIO_ReadInputDataBit(R1_port,R1_pin)
#define key6 GPIO_ReadInputDataBit(R2_port,R2_pin)
#define key7 GPIO_ReadInputDataBit(R3_port,R3_pin)
#define key8 GPIO_ReadInputDataBit(R4_port,R4_pin)   //按键相关操作定义


//独立按键
#define duli_1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define duli_2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define duli_3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define duli_4 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)

void KEY_Init(void);
void Key_assignment(unsigned char z);
u8 Key_Scan(void);

u8 start_stop(u8 key);
u8 temp_target(u8 key);//测温的对象选择

u8 set_templimt(u8 key);//按键设置报警温度阈值
u8 chang_templimt(u8 left,u8 right,u8 key_number);
u8 sure_key(u8 key);
u8 duli_key(u8);  	//按键扫描函数	

#endif /* BUTTON_H_ */





