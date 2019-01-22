#include "./oled/OLED_input.h"

/************************ 用户自定义以下函数 ************************/

// 检测是否按下"上"键, 按下则返回1，否则返回0
inline uint8_t ifPress_UP(void)
{
//	if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )
//		return 1;
//	else
//		return 0;
}

// 检测是否按下"下"键, 按下则返回1，否则返回0
inline uint8_t ifPress_DOWN(void)
{
//	if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON )
//		return 1;
//	else
//		return 0;
}

// 检测是否按下"确认"键, 按下则返回1，否则返回0
inline uint8_t ifPress_YES(void)
{
//	if( Key_Scan(KEY3_GPIO_PORT,KEY3_GPIO_PIN) == KEY_ON )
//		return 1;
//	else
//		return 0;
}

// 检测是否按下"取消"键, 按下则返回1，否则返回0
inline uint8_t ifPress_NO(void)
{
//	if( Key_Scan(KEY4_GPIO_PORT,KEY4_GPIO_PIN) == KEY_ON )
//		return 1;
//	else
//		return 0;
}

/************************ 用户自定义以上函数 ************************/

// 检测哪个按键被按下，返回键值
/* 0-无 1-上 2-下 3-确认 4-取消 */
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
		case 0x00: break;					//	没有按下按键
		case 0x08: value = 1;break;			//	按下 UP
		case 0x04: value = 2;break;			//	按下 DOWN
		case 0x02: value = 3;break;			//	按下 YES
		case 0x01: value = 4;break;			//	按下 NO
		defult   : value = 0;break;			//	同时按下多个按键
	}
	
	return value;
}

