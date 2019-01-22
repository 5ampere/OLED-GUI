#include "./oled/OLED_input.h"

/************************ �û��Զ������º��� ************************/

// ����Ƿ���"��"��, �����򷵻�1�����򷵻�0
inline uint8_t ifPress_UP(void)
{
//	if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )
//		return 1;
//	else
//		return 0;
}

// ����Ƿ���"��"��, �����򷵻�1�����򷵻�0
inline uint8_t ifPress_DOWN(void)
{
//	if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON )
//		return 1;
//	else
//		return 0;
}

// ����Ƿ���"ȷ��"��, �����򷵻�1�����򷵻�0
inline uint8_t ifPress_YES(void)
{
//	if( Key_Scan(KEY3_GPIO_PORT,KEY3_GPIO_PIN) == KEY_ON )
//		return 1;
//	else
//		return 0;
}

// ����Ƿ���"ȡ��"��, �����򷵻�1�����򷵻�0
inline uint8_t ifPress_NO(void)
{
//	if( Key_Scan(KEY4_GPIO_PORT,KEY4_GPIO_PIN) == KEY_ON )
//		return 1;
//	else
//		return 0;
}

/************************ �û��Զ������Ϻ��� ************************/

// ����ĸ����������£����ؼ�ֵ
/* 0-�� 1-�� 2-�� 3-ȷ�� 4-ȡ�� */
uint8_t testPress(void)
{
	uint8_t value = 0;
	
	value |= ifPress_UP();
	value <<= 1;
	value |= ifPress_DOWN();
	value <<= 1;
	value |= ifPress_YES();
	value <<= 1;
	value |= ifPress_NO();
	
	switch (value)
	{
		case 0x00: break;					//	û�а��°���
		case 0x08: value = 1;break;			//	���� UP
		case 0x04: value = 2;break;			//	���� DOWN
		case 0x02: value = 3;break;			//	���� YES
		case 0x01: value = 4;break;			//	���� NO
		defult   : value = 0;break;			//	ͬʱ���¶������
	}
	
	return value;
}

