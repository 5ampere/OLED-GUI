#include "stm32f10x.h"
#include "./oled/OLED_interface.h"
#include "./oled/OLED_drive.h"
#include "./font/fonts.h"
#include "./key/bsp_key.h"

int main(void)
{		
	//OLED ��ʼ��
	I2C_Configuration();
	OLED_Init();
	OLED_CLS();
	menuInit();
	Key_GPIO_Config();
	
	enterMainMenu();
	
	OLED_DispChar_UI( 0, 0, &Cover_Arrow);	// ��������

	while ( 1 );
}




