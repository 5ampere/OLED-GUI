#include "stm32f10x.h"
#include "./oled/OLED.h"

int main(void)
{		
	//OLED ≥ı ºªØ
	I2C_Configuration();
	OLED_Init();
	OLED_CLS();
	
	OLED_ShowStr(0, 0, "Test OK Mother Fucker !!!");
	
	while ( 1 );
}


/* ------------------------------------------end of file---------------------------------------- */

