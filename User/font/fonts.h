#ifndef __FONT_H
#define __FONT_H       

#include "stm32f10x.h"
#include "./font/fonts.h"

// Ӣ����ģ�ṹ��
typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height; 
} sFONT;

// �Զ����زĽṹ�壬tableָ���ز���ģ����
typedef struct _tMaterial
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
} yourMaterial;

extern sFONT Font8x16;
extern yourMaterial Material_Arrow;
extern yourMaterial Cover_Arrow;

#define      WIDTH_EN_CHAR		                 8	    //Ӣ���ַ���� 
#define      HEIGHT_EN_CHAR		              	16		//Ӣ���ַ��߶� 
#define 	 LINEY(y) 							((y) * (HEIGHT_EN_CHAR/8))

#endif /*end of __FONT_H    */
