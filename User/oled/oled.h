#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f10x.h"

#define OLED_ADDRESS  0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78
#define I2C_DELAY     254

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);//全屏点亮(0XFF)   全屏灭(0x00)
void OLED_CLS(void);//清屏
void OLED_ON(void);//测试OLED休眠后唤醒
void OLED_OFF(void);//测试OLED休眠
void OLED_ShowStr(unsigned char x, unsigned char y, char * pStr);
void OLED_DispChar_CH ( uint16_t usX, uint16_t usY, uint16_t usChar );
void OLED_DispChar_EN( uint16_t usX, uint16_t usY, const char cChar );

#endif
