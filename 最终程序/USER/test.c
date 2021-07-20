#include "sys.h"
#include "usart.h"		
#include "stdio.h"
#include "delay.h"
#include "led.h"
#include "oled.h"
#include "mlx90614.h"	
#include "button.h"
#include "warning.h"
#include "misc.h"


int main(void)
{	
/*--------------变量声明--------------------*/
	u8 limt,left,right,chang=0,start,display,display1;
	float temp;

/*--------------配置初始化--------------------*/
	Stm32_Clock_Init(9);	//系统时钟设置
	delay_init(72);	  		//延时初始化
	USART1_Init(115200); 	//串口初始化为115200
	OLED_Init();
	SMBus_Init();
	KEY_Init();
	LED_Init();
	BEEP_Init();	
 
//  OLED_Refresh_Gram();		//更新显示到OLED 
	
	while(1)
	{
/*start=1(测温开始)，start=2(人脸识别开始)，start=3(口罩识别开始)*/
		switch (duli_key(1))
		{
			//case 0: start=0;break;
			case 1: {start=1;break;}
			case 2: {start=2;break;}
			case 3: {start=3;break;}
			case 4: {start=4;break;}
		}
		
//--------------测温部分------------------------------------		
		if(start==1)
		{
			if(start_stop(Key_Scan()))
			{		
				
				if(display==2)
				{
					OLED_Clear();
					OLED_ShowString(25,0,"temp test",16);
					OLED_ShowString(0,25,"temp:",16);
					OLED_ShowChar(60,25,'.',16,1);
					delay_ms(10);
					OLED_ShowString(0,40,"temp_limit:37",16);
					display=1;
				}
				
				if(temp_target(Key_Scan()))//测温:1--测量液体温度 0--测量人体温度
				{
					temp=SMBus_ReadTemp(0);
				}
				else
					temp=SMBus_ReadTemp(1);
				
		/*------------温度阈值设定------------------*/
				while(set_templimt(Key_Scan()))
				{
					LED0=1;	
					if(Key_Scan()==10)
					{
						while(Key_Scan()==10);
						chang=1;
					}
					else if(Key_Scan()==11)
					{
						while(Key_Scan()==11);
						chang=2;	
					}
					
					switch(chang)
					{
						case 0:	{left=3;right=7;break;}
						case 1:	if(Key_Scan()<5) left=Key_Scan();break;
						case 2:	if(Key_Scan()<10) right=Key_Scan();;break;
					}

					limt=left*10+right;
					OLED_ShowNum(90,40,limt,2,16);
					OLED_Refresh_Gram();		//更新显示到OLED 
				}/*结束while阈值设定*/
				
		/*---------温度范围报警--------------*/		
				warning(temp);//温度范围检测与报警
				prompt_warning(temp,limt);
				LED0=0;					//GPIOB.5
				temp_display(temp);//温度显示函数
				OLED_Refresh_Gram();		//更新显示到OLED 

			}
			else
			{
				if(display<2)
				{
					OLED_Clear();
					display=2;
				}
				LED0=1;	
				OLED_ShowString(0,0,"please key temp .....",16);
			}
		}
//----------------人脸识别-------------------//
		if(start==2)
		{
			static display1=2;
			if(display1==2)
			{
				OLED_Clear();
				display1=1;
			}
			OLED_ShowString(0,0,"Please click reset...",16);
			delay_ms(4000);
			OLED_ShowString(0,40,"Identify in the face....",16);
			delay_ms(10);
			
			if((boll_x==1)||(boll_y==2)||(boll_z==3)||(boll_b==4))
			{
				OLED_ShowString(10,0,"identification          ",16);
				delay_ms(2);
				if(boll_x==1)
				{
					OLED_ShowString(0,25,"name:xiaokangsong",16);
					OLED_Refresh_Gram();
				}
				else if(boll_y==2)
				{
					OLED_ShowString(0,25,"name:chengrujia  ",16);
					OLED_Refresh_Gram();
				}
				else if(boll_z==3)
				{
					OLED_ShowString(0,25,"name:mayumao    ",16);
					OLED_Refresh_Gram();
				}
			
				else if(boll_b==4)
				{
					
					OLED_ShowString(0,0,"error2",16);
					OLED_Refresh_Gram();
				}
			}
			else
			{
				OLED_ShowString(0,0,"Identification failure     ",16);
				display1=1;
			}
		}
		OLED_Refresh_Gram();		//更新显示到OLED
	} 
}


 
