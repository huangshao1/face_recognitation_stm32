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
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
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
 
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; // ���ڶ˿����ýṹ�����
    USART_InitTypeDef USART_InitStructure; // ���ڲ������ýṹ�����
    NVIC_InitTypeDef NVIC_InitStructure; // �����ж����ýṹ�����

RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // �򿪴��ڸ���ʱ��
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // ��PC�˿�ʱ��

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // PC.10
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // �趨IO�ڵ�����ٶ�Ϊ50MHz
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
GPIO_Init(GPIOA, & GPIO_InitStructure); // ��ʼ��GPIOC.10

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // PC.11
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������
GPIO_Init(GPIOA, & GPIO_InitStructure); // ��ʼ��GPIOC.10

NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�2
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; // �����ȼ�3
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQͨ��ʹ��
NVIC_Init( & NVIC_InitStructure); // ����ָ���Ĳ�����ʼ��VIC�Ĵ���

                                     // USART

USART_InitStructure.USART_BaudRate = 115200; // ���ڲ�����Ϊ115200
USART_InitStructure.USART_WordLength = USART_WordLength_8b; // �ֳ�Ϊ8λ���ݸ�ʽ
USART_InitStructure.USART_StopBits = USART_StopBits_1; // һ��ֹͣλ
USART_InitStructure.USART_Parity = USART_Parity_No; // ����żУ��λ
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������������
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // �շ�ģʽʹ��
USART_Init(USART1, & USART_InitStructure); // ��ʼ������4

USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // �������ڽ����ж�
USART_Cmd(USART1, ENABLE); // ʹ�ܴ���4
USART_ClearFlag(USART1, USART_FLAG_TC); // �崮��4���ͱ�־
}

// ����1�жϴ�����
void USART1_IRQHandler(void) // ����4ȫ���жϷ�����
{
    u8 temp;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); // ����жϱ�־
        temp = USART_ReceiveData(USART1);
        Openmv_Receive_Data(temp);// openmv���ݴ�����
	    Openmv_Data();
    }
}

void Openmv_Receive_Data(int16_t data) // ����Openmv������������
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
	else if (state == 3) // state == 3  ����Ƿ���ܵ�������־
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
	// ��ѭ��������

void Openmv_Data(void)
{
	boll_x=RxBuffer1[2];
	boll_y=RxBuffer1[3];
	boll_z=RxBuffer1[4];
	boll_b=RxBuffer1[5];
} 
