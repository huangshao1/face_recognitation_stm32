#ifndef __WARNING_H
#define __WARNING_H

#include "delay.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define BEEP 	PBout(8)	// BEEP,蜂鸣器接口	
#define LED0	 PBout(5)// PB5
#define LED1 	PEout(5)// PE5	


void BEEP_Init(void);	//BEEP初始化
void LED_Init(void);	//LED初始化

void buzz_3(void);
void  prompt_warning(float num,u8 warning_num);

#endif


