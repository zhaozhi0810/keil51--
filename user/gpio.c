
/*
	GPIO�����������ݣ�
	1.4��������ȡ�����ã����ȣ����£���ˮ��
	2.��ˮ���ص�ŷ����ƣ��ߵ�ƽ��Ч   P04
	3.���ȼ̵�������					P05
	4.��ѹ�̵�������					P06
	5.����������						P07

	ʹ��P0�ӿڣ�P00-P03Ϊ����.
*/
#include <STC12C5A60S2.H>

#include "gpio.h"

typedef struct btn_info{
	unsigned char  which;
	unsigned char  value;	     //ֵ��0��ʾ�ɿ���1��ʾ����
	unsigned char  reportEn;   //1��������⵽�ˣ�0û�м�⵽����
	unsigned int  pressCnt;     //��������
}BTN_INFO;


static BTN_INFO btns;   //ֻ��4������
//static char btn_state;

sbit ShangShui_En = P1^4;   //��ˮʹ��

#define ShangShuiBtn 1   //��ˮ��ť���ű��
#define SheZhiBtn 2		 //	���ð�ť���ű��
#define JiaReBtn 3		//���Ȱ�ť���ű��
#define BaoWenBtn 4		//���°�ť���ű��	


void gpio_init(void)
{
	P0M1 = 0xf, P0M0 = 0xf0;  // P0.0-P0.3�˿�����Ϊ��������ģʽ����ȡ����
							  // P0.4-P0.7 Ϊ�������	
	
	P1M1 &= ~(1<<4), P1M0 |= (1<<4);   //P1.4 �������
	
}




void shangshui_enable(char en)
{
	ShangShui_En = !!en;    //��0ʹ�ܣ�0ȥʹ��
}




//�ɶ�ʱ��10ms��ѯ��
void keys_scan(void)
{
	char i;
	if((P0 & 0xf) != 0xf)  //�ػ�֮�����ɨ��
	{
		for(i=0;i<4;i++)
		{
			if(P0 & (1<<i) == 0)   //������
			{	
				btns.pressCnt++;
				
				if(btns.pressCnt > 3000)   //ʶ�𵽰���
				{
					btns.value = 2;      //����
					btns.which = i+1;
					btns.pressCnt = 3000; //��ֹ����Խ��
				}
				else if(btns.pressCnt>3)   //ʶ�𵽰���
				{
					btns.value = 1;      //�̰�
					btns.which = i+1;
				}
								
			}
			else  //�ɿ�
			{
				btns.pressCnt = 0;   //�ɿ�����������
//				if(btns.value > 1) //��⵽�̰�
//				{
//					//��������	
//					btns.reportEn = 2;	  //��ʱ���в�����������				
//				}
//				else if(btns.value == 1)
//				{
//					//�̰�����
//					btns.reportEn = 1;   //��ʱ���в�����������
//				}					
			//	btn2.value = 0; //ֵ������
			}				
		}	
	}
}

//��main����ѭ��ɨ�账��
void key_poll_handle(void)
{
	if(btns.value)
	{
		if(ShangShuiBtn == btns.which)  //��ˮ������
		{ //��ʱû�д������Ͷ̰�����һ����ˮ���ٰ�һ��ȡ����ˮ
			ShangShui_En = !ShangShui_En;   //ȡ��
			btns.which = 0;   //������ֵ
		}
		
		
		btns.value = 0;
	}
}




