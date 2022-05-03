/**********************************************/
/*			ƽ̨��Keil U4 + STC12C5A60S       */
/*			���ƣ�����ͨ��					  */

/**********************************************/

#include <STC12C5A60S2.H>
#include "1302.h"
#include "uart1.h"

#define F_f 11059200UL   //����Ƶ��11.0692MHZ
#define Baud_rate 9600UL //������9600

//sfr AUXR = 0x8E; //�������⹦�ܼĴ���AUXR
//sfr BRT = 0x9C;	 //������������ʷ����Ĵ���BRT



bit busy;
static unsigned char uart1_recv_buf[4];   //3���ֽڵ����ݣ�1���ֽ�У���
static unsigned char uart1_recv_n = 0;
static unsigned char flag = 0;    //0bit time_recv 1bit date_recv
static unsigned char ret_flag = 0;    //���ڴ����յ����ݺ�Ӧ��1��ʾ������2-255��ʾ�쳣��0��ʾû���յ����ݣ����ظ�
#define TIME_HEAD 0xaa   //ʱ��ͷ��
#define DATE_HEAD 0xcc   //����ͷ��

void Send_char(char str)
{
	SBUF = str;
	while( !TI ); //�ȴ��������
	TI = 0; 	  //��������ж�����
}

void Send_string(char *str)
{
	while( *str )
	{
		Send_char(*str++);
	}
}

void Uart1Init()
{
	PCON &= 0x7F; // �����ʲ��ӱ�
	SCON = 0x50;  // ���ô���ͨ�Ź�����ʽ1���������н���
	AUXR &= 0xFB; // ���������ʷ�����ʱ��12T
	BRT = 256 - F_f / (12 * Baud_rate * 32); //���������ʷ�����װ�س�ֵ
	AUXR |= 0x11; // ������������ʷ�������Ϊ���ڲ����ʷ�������������
	EA = 1;	      // �������ж�
	ES = 1;		  // ���������ж�
}


unsigned char check_sum(unsigned char * dat,unsigned char len)
{
	unsigned char i;
	unsigned char sum = 0;
	
	for(i=0;i<len;i++)
	{
		sum += dat[i];
	}	
	
	return sum;
}




/*----------------------------
UART interrupt service routine
----------------------------*/
void Uart_Isr() interrupt 4
{
	unsigned char recv = 0;
    
	if (RI)
    {
        RI = 0;             //Clear receive interrupt flag
		recv = SBUF;        //��������
		
		switch(flag)
		{
			case 0: //û���յ��κ�ͷ��
				if(TIME_HEAD == recv)
				{
					flag = 1;  //�յ�ʱ������ͷ��					
				}
				else if (DATE_HEAD == recv)
				{
					flag = 2;  //�յ���������ͷ��
					//uart1_recv_n = 0;
				}
				uart1_recv_n = 0;
			break;
			case 1:
			case 2:
				uart1_recv_buf[uart1_recv_n++] = recv;
				if(uart1_recv_n > 3)
				{
					uart1_recv_n = 0;
					ret_flag = 1;   //�յ�һ֡����
				}
			
				//uart1_recv_buf[uart1_recv_n++] = recv;
				break;
			default:
				break;
		}
        //P0 = SBUF;          //P0 show UART data
        //bit9 = RB8;         //P2.2 show parity bit
    }
    if (TI)
    {
        TI = 0;             //Clear transmit interrupt flag
        busy = 0;           //Clear transmit busy flag
    }
}

/*----------------------------
Send a byte data to UART
Input: dat (data to be sent)
Output:None
----------------------------*/
void SendData(BYTE dat)
{
    while (busy);           //Wait for the completion of the previous data is sent
    ACC = dat;              //Calculate the even parity bit P (PSW.0)

    busy = 1;
    SBUF = ACC;             //Send data to UART buffer
}

/*----------------------------
Send a string to UART
Input: s (address of string)
Output:None
----------------------------*/
void SendString(char *s)
{
    while (*s)              //Check the end of the string
    {
        SendData(*s++);     //Send current char and increment string ptr
    }
}






void uart1_data_handle(void)
{
	if(ret_flag == 1)
	{
		if (check_sum(uart1_recv_buf,3) == uart1_recv_buf[3])
		{
			//����ʱ�������
			if(flag == 1)
			{
				Ds1302SetTime(uart1_recv_buf[0],uart1_recv_buf[1],uart1_recv_buf[2]);				
			}
			else if(flag == 2)
			{				
				Ds1302SetDate(uart1_recv_buf[0],uart1_recv_buf[1],uart1_recv_buf[2]);
				//flag = 0;
			}			
			SendString("ok");	//����Ӧ��		
		}
		else
			SendString("err");  //����Ӧ��	

		flag = 0;
		ret_flag = 0; //�Ѿ�����
	}
}
