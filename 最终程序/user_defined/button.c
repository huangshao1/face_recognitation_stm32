#include "button.h"
#include "delay.h"
#include "oled.h"
	
unsigned char Key_Data=0;  //按键按下的数据
unsigned char Key_flag=0;  //按键按下标志位，区分按键按下一次的

//u8	start,L,R,temp_modify,b1;//start:开始  L:左移  R:右移  temp_modify：温度阈值修改
//start=0;L=0;R=0;temp_modify=0;b1=0;

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);//开启时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//下拉输入 
	GPIO_InitStructure.GPIO_Pin =C1_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(C1_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//下拉输入 
	GPIO_InitStructure.GPIO_Pin =C2_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(C2_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//下拉输入 
	GPIO_InitStructure.GPIO_Pin =C3_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(C3_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//下拉输入 
	GPIO_InitStructure.GPIO_Pin =C4_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(C4_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//下拉输入 
	GPIO_InitStructure.GPIO_Pin =R1_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(R1_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//下拉输入 
	GPIO_InitStructure.GPIO_Pin =R2_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(R2_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//下拉输入 
	GPIO_InitStructure.GPIO_Pin =R3_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(R3_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//下拉输入 
	GPIO_InitStructure.GPIO_Pin =R4_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(R4_port, &GPIO_InitStructure);
	
//-----------------------独立按键------------------------- 
 	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//拉输入 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_9;//KEY0-KEY2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成上拉输入
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOE2,3,4

}


void Key_assignment(unsigned char z) //按键赋值，这里是将八个按键拼接成一组51单片机里面的Io，目前测试可行
{
	Key_Data=0x00;   //清零
	if(z==0) 
	{
		key1_0;  key2_0;  key3_0;  key4_0;
		key5_1;  key6_1;  key7_1;  key8_1;   //使用的方法是反转法
	}
	else 
	{
		key1_1;  key2_1;  key3_1;  key4_1;
		key5_0;  key6_0;  key7_0;  key8_0;   //使用的方法是反转法 
	}
	
	delay_us(222);  //短延时一下
	
		 if(key1==1) Key_Data=Key_Data|0x80;
		 else        Key_Data=Key_Data|0x00;
	
		 if(key2==1) Key_Data=Key_Data|0x40;
		 else        Key_Data=Key_Data|0x00;
		
		 if(key3==1) Key_Data=Key_Data|0x20;
		 else        Key_Data=Key_Data|0x00;
		
		 if(key4==1) Key_Data=Key_Data|0x10;
		 else        Key_Data=Key_Data|0x00;

		 if(key5==1) Key_Data=Key_Data|0x08;
		 else        Key_Data=Key_Data|0x00;	
		
		 if(key6==1) Key_Data=Key_Data|0x04;
		 else        Key_Data=Key_Data|0x00;		
		
		 if(key7==1) Key_Data=Key_Data|0x02;
		 else        Key_Data=Key_Data|0x00;	
		
		 if(key8==1) Key_Data=Key_Data|0x01;
		 else        Key_Data=Key_Data|0x00;  //把这八个IO拼成一个数，也就是当成51里面的一组IO
	
}

/////////////////////////////////////////////////////////////////////////////
/*-----------------------------按键扫描-----------------------------*/
//mode:0,不支持连续按;1,支持连续按
u8 Key_Scan() //按键处理函数
{	
	//static u8 key_up=1;//按键按松开标志
		
	u8 num;
	unsigned char Key_x;
	Key_assignment(0); //状态 0 ，此时按键没有按下的话是 0x0f
	delay_us(50);   //短延时
	

	if(Key_Data!=0x0f) //只要是按键按下，就会使得 Key_Data!=0x0f
	{
		 Key_x=Key_Data;//将此时的键值赋值
		 Key_assignment(1); //切换状态 1 
		 delay_us(10);  //短延时一下
		 Key_Data=Key_Data&Key_x;  //两个数据按位与，就会产生独一无二的数据
		 
		 
		 //当前情况下的键值	，十六进制数据
		 //   88 48 28 18    →    1   2   3   15 
		 //   84 44 24 14    →    4   5   6   14
		 //   82 42 22 12    →    7   8   9   13
		 //	 81 41 21 11 	   →    10  0   11  12
		 switch(Key_Data)
		 {
			 case 0x11 : num=12;   break; //第四竖行，从下到上
			 case 0x12 : num=13;   break;
			 case 0x14 : num=14;   break;
			 case 0x18 : num=15;  break;

			 case 0x21 : num=11;   break;//第三竖行，从下到上
			 case 0x22 : num=9;   break;
			 case 0x24 : num=6;   break;
			 case 0x28 : num=3;  break;

			 case 0x41 : num=0;   break;//第二竖行，从下到上
			 case 0x42 : num=8;   break;
			 case 0x44 : num=5;   break;
			 case 0x48 : num=2;  break;

			 case 0x81 : num=10;  break;//第一竖行，从下到上
			 case 0x82 : num=7;   break;
			 case 0x84 : num=4;  break;
			 case 0x88 : num=1;  break;	
		   }  
	}
	else if(Key_Data==0x0f)
		num=16;
	return num;
}

/*-------------------------------测温开始指令-------------------------------*/
u8 start_stop(u8 key)//按一下data=1表开始测温，按第二下data=2测温结束
{
	static u8 data=0;
	if(key==15)
	{
		while(Key_Scan()==15);
		data++;
	}
	if(data>=2)
		data=0;
	return data;
}
/*-------------------------------测温对象选择-------------------------------*/
u8 temp_target(u8 key)//测量体温
{
	static u8 data=0;

	if(key==14)
	{
		while(Key_Scan()==14);
		data++;
	}
	if(data>=2)
		data=0;

	return data;
}

/*-------------------------------设置报警阈值应答信号-------------------------------*/
u8 set_templimt(u8 key)//d-测温应答，key-按键输入的值
{
	static u8 data=0;
	
		if(key==12)
		{
			while(Key_Scan()==12);
			data++;
		}
		if(data>=2)
			data=0;
	
	return data;
}

/*-------------------------------确认按键选择-------------------------------*/
u8 sure_key(u8 key)//测量体温
{
	static u8 data=0;

	if(key==12)
	{
		while(Key_Scan()==12);
		data++;
	}
	if(data>=2)
		data=0;

	return data;
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY_UP!!
u8 duli_key(u8 mode)
{
	static   u8 key_up=1;//按键按松开标志
	if(mode) key_up=1;  //支持连按		  
	if(key_up&&(duli_1==0||duli_2==0||duli_3==0||duli_4==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		
		if(duli_1==0) return 1;
		else if(duli_2==0) return 2;
		else if(duli_3==0) return 3;
		else if(duli_4==0) return 4;
	}
	else if(key_up && duli_1==1 && duli_2==1 && duli_3==1 && duli_4==1)
		key_up=1; 	    
 	return 0;	// 无按键按下
}



