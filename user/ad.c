
#include <STC12C5A60S2.H>
#include "intrins.h"




///*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks



//void InitADC();


void ADC_start(void)
{
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | 7 | ADC_START;
	_nop_();

}


///*----------------------------
//Get ADC result
//----------------------------*/
unsigned char GetADCResult(unsigned int * res)
{
//	unsigned int ADC_RES_10BIT;

    while (!(ADC_CONTR & ADC_FLAG));//Wait complete flag
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

//	return ADC_RES;
    * res = ADC_RES;	//�õ���8λ
    * res <<= 2;
    * res |= ADC_RESL;	//�õ���2λ
	
    return ADC_RES;      //�õ���8λ           //Return ADC result
}




/*----------------------------
��ʼ��ADC
----------------------------*/
/*
	n=0xff, ����P1��ΪADC��
	n=0x00, ����P1��Ϊ��ADC��
*/
void InitADC(void)
{
	P1ASF = 0x80;	//����P1��
	ADC_RES = 0;	//�������Ĵ��� // ADC�ϵ�	
//	AUXR1 |= (1<<2);   //��2λ��ADC_RES ��8λADC_RESL
	P1M1 |= (1<<7), P1M0 &= ~(1<<7);  // P1.4�˿�����Ϊ��������ģʽ�����ڶ�ȡNTC��ѹ
	_nop_();
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | 7 | ADC_START;
	
}

