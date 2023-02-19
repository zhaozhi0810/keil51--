#include "1302.h"
#include "stdio.h"


/*
DS1302д��Ͷ�ȡʱ����ĵ�ַ����
���ʱ�������� ���λ��дλ;
*/
code const uchar code READ_RTC_ADDR[7]=
        {0x81,0x83,0x85,0x87,0x89,0x8B,0x8D};
code const uchar code WRITE_RTC_ADDR[7]=
        {0x80,0x82,0x84,0x86,0x88,0x8A,0x8C};
/*
DS1302ʱ�ӳ�ʼ��2022��2��27��������22��10��20�롣
�洢˳�������ʱ��������,�洢��ʽ����BCD��
*/
uchar TimeData[7]={20,10,0x22,0x027,0x02,0x07,0x22};

/*
DS1302��ͨ��SPI�������߸���Ƭ��ͨ�ŵģ�������һ�ζ�д����ʱ���ٵö�д�����ֽڣ���һ���ֽ��ǿ����ֽڣ�����һ���������DS1302�Ƕ�����д�������Ƕ�RAM���Ƕ�CLOK�Ĵ����������ڶ����ֽھ���Ҫ����д�������ˡ�

���ֽڶ�д��
    ֻ����SCLKΪ�͵�ƽʱ�����ܽ�CE��Ϊ�ߵ�ƽ��
    �����ڽ��в���֮ǰ�Ƚ�SCLK�õ͵�ƽ��Ȼ��CE��Ϊ�ߵ�ƽ�����ſ�ʼ��IO�������Ҫ���͵ĵ�ƽ�źţ�Ȼ������SCLK��
    ������SCLK������ʱ��DS1302��д���ݣ���SCLK�½���ʱ��DS1302�������ݵ�IO��

*/

sbit DS1302_RST = P2^0;      //ʵʱʱ�Ӹ�λ������
sbit DS1302_IO = P2^1;      //ʵʱʱ������������
sbit DS1302_CLK = P2^2;      //ʵʱʱ��ʱ�������� 


void ds1302_io_set_input(void)
{
	P2M1 |= 0x2;  //P2.1 ����Ϊ����
	P2M0 &= ~0x2;   
}

void ds1302_io_set_output(void)
{
	P2M0 |= 0x2;  //P2.1 ����Ϊ�������
	P2M1 &= ~0x2;  
}

//****************************************************
//ʵʱʱ��д��һ�ֽ�(�ڲ�����)
//****************************************************
static void DS1302_InputByte(unsigned char dat) 	
{ 
    unsigned char i;
	ds1302_io_set_input();
    for(i=0; i<8; i++)
    {
        DS1302_IO = dat & 0x01;   
		
		DS1302_CLK = 0;				//������д������
		_nop_();
		_nop_();        
        
        DS1302_CLK = 1;
		_nop_();
		_nop_();
        dat >>= 1; 
    } 
}

//****************************************************
//ʵʱʱ�Ӷ�ȡһ�ֽ�(�ڲ�����)
//****************************************************
static unsigned char DS1302_OutputByte(void) 	
{ 
    unsigned char i;
	unsigned char dat;
	ds1302_io_set_output();
    for(i=0; i<8; i++)
    {
		DS1302_CLK = 1;				    //�½��ض�������
		_nop_();
		_nop_();
        DS1302_CLK = 0;
		_nop_();
		_nop_();
        dat >>= 1;         			
        if( DS1302_IO == 1 )
			dat |= 0x80;				//���λ��һ
		else
			dat &= 0x7F;				//���λ����       
    } 
    return(dat); 
}

//****************************************************
//ucAddr: DS1302��ַ, ucData: Ҫд������
//****************************************************
static void Ds1302Write(unsigned char ucAddr, unsigned char ucDa)	
{
    DS1302_RST = 0;
	_nop_();
    DS1302_CLK = 0;
	_nop_();
    DS1302_RST = 1;
	_nop_();
    DS1302_InputByte(ucAddr);       	// ��ַ������ 
    DS1302_InputByte(ucDa);       		// д1Byte����
    DS1302_CLK = 1;
	_nop_();
    DS1302_RST = 0;
	_nop_();
}

//****************************************************
//��ȡDS1302ĳ��ַ������
//****************************************************
static unsigned char Ds1302Read(unsigned char ucAddr)	
{
    unsigned char ucData;
    DS1302_RST = 0;
	_nop_();
    DS1302_CLK = 0;
	_nop_();
    DS1302_RST = 1;
	_nop_();
    DS1302_InputByte(ucAddr|0x01);        // ��ַ������ 
    ucData = DS1302_OutputByte();         // ��1Byte����
    DS1302_CLK = 1;
	_nop_();
    DS1302_RST = 0;
	_nop_();
    return(ucData);
}




void Ds1302SetTime(unsigned char hour,unsigned char min,unsigned char sec)
{   
	Ds1302Write(0x8e,0x00);//��ֹд����     
    Ds1302Write(WRITE_RTC_ADDR[0],sec);
    Ds1302Write(WRITE_RTC_ADDR[1],min);
	Ds1302Write(WRITE_RTC_ADDR[2],hour);
    Ds1302Write(0x8e,0x10);//��д����
}


void Ds1302SetDate(unsigned char year,unsigned char month,unsigned char day)
{
    Ds1302Write(0x8e,0x00);//��ֹд����
    Ds1302Write(WRITE_RTC_ADDR[3],day);
    Ds1302Write(WRITE_RTC_ADDR[4],month);
	Ds1302Write(WRITE_RTC_ADDR[6],year);
    Ds1302Write(0x8e,0x10);//��д����
}


void Ds1302readTime()//��ȡʱ����Ϣ
{
	int a=12,b=23,c=6;
	TimeData[0]=Ds1302Read(READ_RTC_ADDR[0]);
	TimeData[1]=Ds1302Read(READ_RTC_ADDR[1]);
	TimeData[2]=Ds1302Read(READ_RTC_ADDR[2]);
	
	a = TimeData[0];
	b = TimeData[1];
	c = TimeData[2];
	printf("time:%d:%d:%d\r\n",c,b,a);
}

//void Ds1302readDate()//��ȡʱ����Ϣ
//{
//	TimeData[3]=Ds1302Read(READ_RTC_ADDR[3]);
//	TimeData[4]=Ds1302Read(READ_RTC_ADDR[4]);
//	TimeData[6]=Ds1302Read(READ_RTC_ADDR[6]);
//}



