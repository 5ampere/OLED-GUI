#ifndef __FONT_H
#define __FONT_H       

#include "stm32f10x.h"
#include "./font/fonts.h"

/** @defgroup FONTS_Exported_Types
  * @{
  */ 
typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  
} sFONT;
extern sFONT Font8x16;

#define      WIDTH_EN_CHAR		                 8	    //英文字符宽度 
#define      HEIGHT_EN_CHAR		              	16		//英文字符高度 
#define 	 LINEY(y) 							((y) * (HEIGHT_EN_CHAR/8))

#endif /*end of __FONT_H    */
