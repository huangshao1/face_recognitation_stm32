#include "time.h"

void time3_init(u16 per,u16 pre)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能TIM3的时钟
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//TIM_CKD_DIV1是.h文件中已经定义好的，TIM_CKD_DIV1=0，也就是时钟分频因子为0
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//计数方式为向上计数
    TIM_TimeBaseStructure.TIM_Period=per;//周期
    TIM_TimeBaseStructure.TIM_Prescaler=pre;//分频系数
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//使能TIM3的更新中断
    TIM_Cmd(TIM3,ENABLE);//使能TIM3
}
void NVIC_INIT(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//设定中断优先级分组0
    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;//设定中断向量   本程序为TIM3的中断
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//使能响应中断向量的中断响应
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//配置中断向量的抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;//配置中断向量的响应优先级
    NVIC_Init(&NVIC_InitStructure);//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void TIM3_IRQHandler(void) //TIME3中断服务函数  需要设定中断优先级  即NVIC配置
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)//判断是否发生了更新(溢出)中断
    {
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清除溢出中断标志位
    }

    //LED0=!LED0;
}