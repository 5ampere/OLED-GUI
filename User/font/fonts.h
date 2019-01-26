#ifndef __FONT_H
#define __FONT_H       

#include "stm32f10x.h"
#include "./font/fonts.h"

// 英文字模结构体
typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height; 
} sFONT;

// 自定义素材结构体，table指向素材字模数组
typedef struct _tMaterial
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
} yourMaterial;

extern sFONT Font8x16;
extern yourMaterial Material_Arrow;
extern yourMaterial Cover_Arrow;

#define      WIDTH_EN_CHAR		                 8	    //英文字符宽度 
#define      HEIGHT_EN_CHAR		              	16		//英文字符高度 
#define 	 LINEY(y) 							((y) * (HEIGHT_EN_CHAR/8))

#endif /*end of __FONT_H    */
