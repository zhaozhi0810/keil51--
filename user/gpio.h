

#ifndef __GPIO_H__
#define __GPIO_H__

void gpio_init(void);
void shangshui_enable(char en);

//�ɶ�ʱ��10ms��ѯ��
void keys_scan(void);

//��main����ѭ��ɨ�账��
void key_poll_handle(void);

#endif


