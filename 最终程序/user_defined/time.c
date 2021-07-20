#include "time.h"

void time3_init(u16 per,u16 pre)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//ʹ��TIM3��ʱ��
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//TIM_CKD_DIV1��.h�ļ����Ѿ�����õģ�TIM_CKD_DIV1=0��Ҳ����ʱ�ӷ�Ƶ����Ϊ0
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//������ʽΪ���ϼ���
    TIM_TimeBaseStructure.TIM_Period=per;//����
    TIM_TimeBaseStructure.TIM_Prescaler=pre;//��Ƶϵ��
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//ʹ��TIM3�ĸ����ж�
    TIM_Cmd(TIM3,ENABLE);//ʹ��TIM3
}
void NVIC_INIT(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//�趨�ж����ȼ�����0
    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;//�趨�ж�����   ������ΪTIM3���ж�
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ʹ����Ӧ�ж��������ж���Ӧ
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//�����ж���������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;//�����ж���������Ӧ���ȼ�
    NVIC_Init(&NVIC_InitStructure);//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

void TIM3_IRQHandler(void) //TIME3�жϷ�����  ��Ҫ�趨�ж����ȼ�  ��NVIC����
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)//�ж��Ƿ����˸���(���)�ж�
    {
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//�������жϱ�־λ
    }

    //LED0=!LED0;
}