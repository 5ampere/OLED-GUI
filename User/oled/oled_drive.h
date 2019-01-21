#ifndef __OLED_DRIVE_H
#define	__OLED_DRIVE_H

#include "stm32f10x.h"
#include "./font/fonts.h"

#define OLED_ADDRESS 	 0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78
#define I2C_DELAY    	 254

#define OLED_WIDTH		 128
#define OLED_HEIGHT		 64

/*
 ___________X
|    H
|
|L       R
|
|    L
|
Y

����
0---------->
|         X
|
|
|
v Y
*/
typedef struct _videoMemory
{
	uint8_t pVideoMem[128][8];	//�Դ�����
	uint8_t refreshXL;			//��Ҫˢ�µľ�������x���������
	uint8_t refreshXR;			//��Ҫˢ�µľ�������x���Ҷ�����
	uint8_t refreshYH;			//��Ҫˢ�µľ�������y���϶�����
	uint8_t refreshYL;			//��Ҫˢ�µľ�������y���¶�����
}videoMemory;


void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_ON(void);//����OLED���ߺ���
void OLED_OFF(void);//����OLED����
void refreshArea(void);
void antiColor(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);
void OLED_Clear(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);
void OLED_Fill(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);
void OLED_DispChar_EN( uint16_t usX, uint16_t usY, const char cChar );
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char * pStr);
void OLED_DispChar_UI( uint16_t usX, uint16_t usY, yourMaterial *pMaterial );

#endif /* __OLED_DRIVE_H */
