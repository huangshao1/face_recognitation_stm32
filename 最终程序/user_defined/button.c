#include "button.h"
#include "delay.h"
#include "oled.h"
	
unsigned char Key_Data=0;  //�������µ�����
unsigned char Key_flag=0;  //�������±�־λ�����ְ�������һ�ε�

//u8	start,L,R,temp_modify,b1;//start:��ʼ  L:����  R:����  temp_modify���¶���ֵ�޸�
//start=0;L=0;R=0;temp_modify=0;b1=0;

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);//����ʱ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//�������� 
	GPIO_InitStructure.GPIO_Pin =C1_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(C1_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//�������� 
	GPIO_InitStructure.GPIO_Pin =C2_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(C2_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//�������� 
	GPIO_InitStructure.GPIO_Pin =C3_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(C3_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//�������� 
	GPIO_InitStructure.GPIO_Pin =C4_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(C4_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//�������� 
	GPIO_InitStructure.GPIO_Pin =R1_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(R1_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//�������� 
	GPIO_InitStructure.GPIO_Pin =R2_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(R2_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//�������� 
	GPIO_InitStructure.GPIO_Pin =R3_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(R3_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//�������� 
	GPIO_InitStructure.GPIO_Pin =R4_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(R4_port, &GPIO_InitStructure);
	
//-----------------------��������------------------------- 
 	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//������ 
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_9;//KEY0-KEY2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

}


void Key_assignment(unsigned char z) //������ֵ�������ǽ��˸�����ƴ�ӳ�һ��51��Ƭ�������Io��Ŀǰ���Կ���
{
	Key_Data=0x00;   //����
	if(z==0) 
	{
		key1_0;  key2_0;  key3_0;  key4_0;
		key5_1;  key6_1;  key7_1;  key8_1;   //ʹ�õķ����Ƿ�ת��
	}
	else 
	{
		key1_1;  key2_1;  key3_1;  key4_1;
		key5_0;  key6_0;  key7_0;  key8_0;   //ʹ�õķ����Ƿ�ת�� 
	}
	
	delay_us(222);  //����ʱһ��
	
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
		 else        Key_Data=Key_Data|0x00;  //����˸�IOƴ��һ������Ҳ���ǵ���51�����һ��IO
	
}

/////////////////////////////////////////////////////////////////////////////
/*-----------------------------����ɨ��-----------------------------*/
//mode:0,��֧��������;1,֧��������
u8 Key_Scan() //����������
{	
	//static u8 key_up=1;//�������ɿ���־
		
	u8 num;
	unsigned char Key_x;
	Key_assignment(0); //״̬ 0 ����ʱ����û�а��µĻ��� 0x0f
	delay_us(50);   //����ʱ
	

	if(Key_Data!=0x0f) //ֻҪ�ǰ������£��ͻ�ʹ�� Key_Data!=0x0f
	{
		 Key_x=Key_Data;//����ʱ�ļ�ֵ��ֵ
		 Key_assignment(1); //�л�״̬ 1 
		 delay_us(10);  //����ʱһ��
		 Key_Data=Key_Data&Key_x;  //�������ݰ�λ�룬�ͻ������һ�޶�������
		 
		 
		 //��ǰ����µļ�ֵ	��ʮ����������
		 //   88 48 28 18    ��    1   2   3   15 
		 //   84 44 24 14    ��    4   5   6   14
		 //   82 42 22 12    ��    7   8   9   13
		 //	 81 41 21 11 	   ��    10  0   11  12
		 switch(Key_Data)
		 {
			 case 0x11 : num=12;   break; //�������У����µ���
			 case 0x12 : num=13;   break;
			 case 0x14 : num=14;   break;
			 case 0x18 : num=15;  break;

			 case 0x21 : num=11;   break;//�������У����µ���
			 case 0x22 : num=9;   break;
			 case 0x24 : num=6;   break;
			 case 0x28 : num=3;  break;

			 case 0x41 : num=0;   break;//�ڶ����У����µ���
			 case 0x42 : num=8;   break;
			 case 0x44 : num=5;   break;
			 case 0x48 : num=2;  break;

			 case 0x81 : num=10;  break;//��һ���У����µ���
			 case 0x82 : num=7;   break;
			 case 0x84 : num=4;  break;
			 case 0x88 : num=1;  break;	
		   }  
	}
	else if(Key_Data==0x0f)
		num=16;
	return num;
}

/*-------------------------------���¿�ʼָ��-------------------------------*/
u8 start_stop(u8 key)//��һ��data=1��ʼ���£����ڶ���data=2���½���
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
/*-------------------------------���¶���ѡ��-------------------------------*/
u8 temp_target(u8 key)//��������
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

/*-------------------------------���ñ�����ֵӦ���ź�-------------------------------*/
u8 set_templimt(u8 key)//d-����Ӧ��key-���������ֵ
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

/*-------------------------------ȷ�ϰ���ѡ��-------------------------------*/
u8 sure_key(u8 key)//��������
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

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY_UP!!
u8 duli_key(u8 mode)
{
	static   u8 key_up=1;//�������ɿ���־
	if(mode) key_up=1;  //֧������		  
	if(key_up&&(duli_1==0||duli_2==0||duli_3==0||duli_4==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		
		if(duli_1==0) return 1;
		else if(duli_2==0) return 2;
		else if(duli_3==0) return 3;
		else if(duli_4==0) return 4;
	}
	else if(key_up && duli_1==1 && duli_2==1 && duli_3==1 && duli_4==1)
		key_up=1; 	    
 	return 0;	// �ް�������
}



