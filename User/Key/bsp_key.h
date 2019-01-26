#ifndef __KEY_H
#define	__KEY_H


#include "stm32f10x.h"

//  引脚定义
#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY1_GPIO_PORT    GPIOC			   
#define    KEY1_GPIO_PIN     GPIO_Pin_8

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY2_GPIO_PORT    GPIOC		   
#define    KEY2_GPIO_PIN     GPIO_Pin_9

#define    KEY3_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY3_GPIO_PORT    GPIOC		   
#define    KEY3_GPIO_PIN	 GPIO_Pin_10

#define    KEY4_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY4_GPIO_PORT    GPIOC		   
#define    KEY4_GPIO_PIN     GPIO_Pin_11

 /** 按键按下标置宏
	*  按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
	*  若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
	*/
#define KEY_ON	0
#define KEY_OFF	1

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);


#endif /* __KEY_H */

