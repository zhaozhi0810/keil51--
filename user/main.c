
#include "1302.h"
#include "tm1623.h"
#include "uart1.h"
#include "ad.h"
#include "timer0.h"
#include "ntc.h"
#include "gpio.h"

#include "stdio.h"


//unsigned char temperature;
//extern unsigned int NTC_R;
//void T_dis();
unsigned char ceshi_flag = 0;  //0 ��ʾ��ˮλ��1��ʾ��ˮ��

void delay_nms(unsigned int n)
{
	unsigned int i;
	while(n--)
	{
		for(i=0;i<550;i++);
	}
		
}


void delay_nus(unsigned int n)
{
	unsigned int i;
	while(n--)
	{
		for(i=0;i<50;i++);
	}
}


void ds1302_io_init(void)
{
	P2M1 &= ~0x7;  //����λ����
	P2M0 |= 0x7;   //P2.1 Ϊ׼˫��
}

void tm1623_io_init(void)
{
	P1M1 &= ~0x7; //����λ����
	P1M0 |= 0x7;  //��1
}


//�¶Ⱥ�ˮλ������ģʽ,P1.5,P1.6
void temp_level_io_init(void)
{
	P1M1 &= ~(1<<5|1<<6); //����λ����
	P1M0 |= (1<<5|1<<6);  //��1
}




void disp_temp_level(void)
{
	char temp1 = 0,temp = 0;    //��һ�ε�ֵ
	unsigned int res;
	unsigned int temp_res;
	unsigned char level1 ;
	static char temp2;      //���ڱ���֮ǰ��ֵ
	static unsigned char j = 0,i=0;  //���ڼ���
	
	
	if(timer0_count<1000)   //��ȡ�¶�
	{			
		if(!ceshi_flag)				
		{
			ceshi_flag = 1;
			i=0;
		}			
		else
		{
			if(timer0_count%400 == 31)  //adc�ɼ�  100ms���
			{	
				if(i==3)		
				{
					GetADCResult(&temp_res);  //ͨ����������
				}
				i++;
				ADC_start();
			}
		}
	}
	else if(timer0_count<2000)    //1000-2000֮��   //��ȡˮλ
	{
		if(ceshi_flag)	//��һ�ν�����ֻ�л�io�������Ե�ѹ
		{				
			ceshi_flag = 0;   //�л�Ϊˮλ���
			i=0;
		}
		else
		{			
			if(timer0_count%400 == 31)  //adc�ɼ�  100ms���
			{
				if(i==3)		//ֻ��ȡһ��
				{
					level1 = GetADCResult(&res);   //ͨ������ֵ����
					{
						SendData(0xfe);
						SendData(level1);
					}
				}						
				i++;
				ADC_start();	
			}
		}		
	}
	
	//ˮλ��ʾ
	if(timer0_count%500 == 89)			
		state_water_level(level1);
	
	//�¶���ʾ
	if(timer0_count%500 == 0)
	{
		if(temp_res < 1000)
		{	
			temp = T_dis(temp_res);  //תλ�¶�
			
			if(!temp2)   //��ʼֵΪ0���޸�
			{
				temp2 = temp;
				temp1 = temp;
			}
			else if((temp!=temp1) && (temp==temp2))   //��ʾֵtemp1�������ֵ��ͬ�����Ҽ�¼ֵtemp2 �����ڶ���ֵ
			{
				j++;
				if(j>10)
				{
					j = 0;
					temp1 = temp2;  //��ʾ��ֵ����
				}
			}
			else  //��ʾ��ֵ�������ֵ��ͬ�����߶�����ֵtemp���¼��temp2�����
			{
				j = 0;    //���������
				temp2 = temp;    //���¶�����ֵ
			}
				
			if(temp1 == 120)  //֮ǰ�ǲ���ʾ
				temp1 = temp ;   //����ǰһ���¶ȸı�
		}
		else
			temp1 = 120;  //����
					
		display_watertemp(temp1); 
	}	
}



void main()
{
	//��ʼ��
	ds1302_io_init();
	tm1623_io_init();
	gpio_init();
	
	temp_level_io_init();
	shangshui_enable(0);  //��ˮ��ŷ��ϵ�
	
	
	InitADC();    //P1.7��ΪAD��������
	timer0_init();
	Uart1Init();
	display_start();
//	Ds1302SetTime(16,56,20);
//	Set_DS1302_Time(0x17,0x33,0x20); //��ʾ17��33��20��
	Send_string("date: 2023-02-18\r\n");
	printf("%d\r\n",2023);
	
//	P2 = 0xff;
//	while(1);
	
	while(1)	
	{	
		if(timer0_count%1000 == 330)   //0.5�����һ��ʱ����ʾ
		{
			Ds1302readTime();   //��ȡDs1302ʱ��
		}	
		
		uart1_data_handle();   //���ڽ������ݵĴ���

		if(timer0_count%500 == 0)   //0.5�����һ��ʱ����ʾ
		{
			display_time(TimeData[2],TimeData[1]);
			state_miao(TimeData[0]&1);   //ð����������˸
		}

		disp_temp_level();	

		//�����Ĵ�������
		key_poll_handle();
		
	}
}




