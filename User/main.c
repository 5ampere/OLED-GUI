#include "stm32f10x.h"
#include "./oled/OLED_interface.h"
#include "./oled/OLED_drive.h"
#include "./font/fonts.h"
#include "./key/bsp_key.h"

int main(void)
{		
	//OLED ≥ı ºªØ
	I2C_Configuration();
	OLED_Init();
	menuInit();
	Key_GPIO_Config();
	
	enterMainMenu();
	
antiColor(0, 0, 127, 63);
	while ( 1 );
}




