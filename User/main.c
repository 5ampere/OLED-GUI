#include "stm32f10x.h"
#include "./oled/OLED_drive.h"
#include "./font/fonts.h"

int main(void)
{		
	//OLED ≥ı ºªØ
	I2C_Configuration();
	OLED_Init();
	OLED_CLS();
	
	OLED_DispChar_UI( 0, 0, &Cover_Arrow);
	
	while ( 1 );
	//	antiColor(0, 0, 127, 63);
}




