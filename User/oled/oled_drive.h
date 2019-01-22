#ifndef __OLED_DRIVE_H
#define	__OLED_DRIVE_H

#include "stm32f10x.h"
#include "./font/fonts.h"


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

void WriteCmd(unsigned char Command);
void WriteDat(unsigned char Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_ON(void);
void OLED_OFF(void);
static void refreshRange(uint8_t X_L, uint8_t X_R, uint8_t Y_H, uint8_t Y_L);
void refreshArea(void);
void OLED_DispChar_EN( uint16_t usX, uint16_t usY, const char cChar );
void OLED_AntiColor(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t ifReflash);
void OLED_Clear(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t ifReflash);
void OLED_Fill(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t ifReflash);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char * pStrn, uint8_t ifReflash);
void OLED_DispChar_UI( uint16_t usX, uint16_t usY, yourMaterial *pMaterial, uint8_t ifReflash);

#endif /* __OLED_DRIVE_H */
