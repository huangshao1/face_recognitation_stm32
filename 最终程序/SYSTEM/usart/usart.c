#include "sys.h"
#include "usart.h"	
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

int8_t boll_x;
int8_t boll_y;
int8_t boll_z;
int8_t boll_b;
int8_t RxBuffer1[10];
int i=0;
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; // 串口端口配置结构体变量
    USART_InitTypeDef USART_InitStructure; // 串口参数配置结构体变量
    NVIC_InitTypeDef NVIC_InitStructure; // 串口中断配置结构体变量

RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // 打开串口复用时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 打开PC端口时钟

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // PC.10
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设定IO口的输出速度为50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
GPIO_Init(GPIOA, & GPIO_InitStructure); // 初始化GPIOC.10

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // PC.11
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
GPIO_Init(GPIOA, & GPIO_InitStructure); // 初始化GPIOC.10

NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级2
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; // 子优先级3
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQ通道使能
NVIC_Init( & NVIC_InitStructure); // 根据指定的参数初始化VIC寄存器

                                     // USART

USART_InitStructure.USART_BaudRate = 115200; // 串口波特率为115200
USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 字长为8位数据格式
USART_InitStructure.USART_StopBits = USART_StopBits_1; // 一个停止位
USART_InitStructure.USART_Parity = USART_Parity_No; // 无奇偶校验位
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 收发模式使能
USART_Init(USART1, & USART_InitStructure); // 初始化串口4

USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 开启串口接受中断
USART_Cmd(USART1, ENABLE); // 使能串口4
USART_ClearFlag(USART1, USART_FLAG_TC); // 清串口4发送标志
}

// 串口1中断处理函数
void USART1_IRQHandler(void) // 串口4全局中断服务函数
{
    u8 temp;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除中断标志
        temp = USART_ReceiveData(USART1);
        Openmv_Receive_Data(temp);// openmv数据处理函数
	    Openmv_Data();
    }
}

void Openmv_Receive_Data(int16_t data) // 接收Openmv传过来的数据
{
	static u8 state = 0;
	if (state == 0 && data == 0x2C)
	{
	    state = 1;
	    RxBuffer1[0]=data;
	}
	else if (state == 1 && data == 18)
	{
		state=2;
		RxBuffer1[1]=data;

	}
	else if (state == 2)
	{
		state=3;
	     RxBuffer1[2]=data; // the last of char is openmv[19]
		
	}
	else if (state == 3) // state == 3  检测是否接受到结束标志
	{
 		state=4;
	     RxBuffer1[3]=data; // the last of char is openmv[19]
	}
	else if(state==4)
	{
		state=5;
	     RxBuffer1[4]=data;
	}
	else if(state==5) // wrong thing
	{
		state=6;
		RxBuffer1[5]=data;
	}
	else if(state==6)
	{
		if(data==0x5B)
		{
			state=0;
			RxBuffer1[6]=data;
			Openmv_Data();
			
		}
		else if(data!=0x5B)
		{
			state=0;
			for(i=0;i<7;i++)
			{
				RxBuffer1[i]=0x00;
			}
		}
	}
	else
	{
		  state = 0;
            for(i=0;i<7;i++)
            {
                RxBuffer1[i]=0x00;
            }
	}

}
	// 主循环处理函数

void Openmv_Data(void)
{
	boll_x=RxBuffer1[2];
	boll_y=RxBuffer1[3];
	boll_z=RxBuffer1[4];
	boll_b=RxBuffer1[5];
} 
