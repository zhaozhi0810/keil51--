

#ifndef __TM1623_H__
#define __TM1623_H__

#define TM_ACTIVATE 0x8F // Start up
#define TM_BUTTONS_MODE 0x42 // Buttons mode
#define TM_WRITE_LOC 0x44 // Write to a location
#define TM_WRITE_INC 0x40 // Incremental write
#define TM_SEG_ADR 0xC0  // leftmost segment Address C0 C2 C4 C6 C8 CA CC CE
//#define TM_LEDS_ADR 0xC1  // Leftmost LED address C1 C3 C5 C7 C9 CB CD CF
#define TM_BRIGHT_ADR 0x88 // Brightness address
#define TM_BRIGHT_MASK 0x07 // Brightness mask 
#define TM_DEFAULT_BRIGHTNESS 0x02 //can be 0x00 to 0x07 
#define TM_DISPLAY_SIZE 4 //size of display

#define TM_ASCII_OFFSET 47 // Ascii table offset to jump over first missing 32 chars


//ˮ������ã�0-100����ʾ�����̶ȣ�����150����ʾ�쳣��
void state_water_level(unsigned char state);

//��ʱ��ˮ��ʾ����
//state �����ʾѡ�У�0��ʾûѡ��
void state_dingshi_shangshui(unsigned char state);

//��ˮ����ʾ����
void state_shangshui(unsigned char state);

//��ѹ����ʾ����
void state_zengya(unsigned char state);

//ʱ��ð�ŵ���ʾ����
void state_miao(unsigned char state);

//������ʾˮ��
void display_watertemp(unsigned char temp);

//������ʾʱ��
void display_time(unsigned char hour,unsigned char min);

//��ʼ��ʾˢ��
void display_start(void);
#endif


