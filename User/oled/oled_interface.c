#include "./oled/OLED_interface.h"
#include "./oled/OLED_input.h"
#include "./oled/OLED_drive.h"
#include <stdlib.h>
#include <string.h>

// �˵�����
// ���ݽṹ : ��ǰĿ¼���� ��ǰĿ¼ID ��Ŀ¼ID �Ӳ˵����� ��Ŀ¼ָ�� ��Ŀ¼ָ������ָ�� ����ָ��
#define sizeOfMenu		6 
Menu yourMenu[sizeOfMenu] = 
{	
/************************ �û��Զ���˵����� ************************/
	{ (uint8_t*)"ROOT", 1, 0, 2, null, null, null },	// ���Ǹ��˵�,����ɾ�������Ը������ơ��Ӳ˵�����
	{ (uint8_t*)"Menu_1", 2, 1, 0, null, null, null },
	{ (uint8_t*)"Menu_2", 3, 1, 3, null, null, null },
	{ (uint8_t*)"Menu_3", 4, 3, 0, null, null, null },
	{ (uint8_t*)"Menu_4", 5, 3, 0, null, null, null },
	{ (uint8_t*)"Menu_5", 6, 3, 0, null, null, null },
/************************ �û��Զ���˵����� ************************/
};

// �˵�����ṹ��
Manage menuManage = {
	&yourMenu[0],
	&yourMenu[0],
	&yourMenu[0],
	&yourMenu[0],
}; 

// �˵����ṹ�ĳ�ʼ��
void menuInit(void)
{
	uint16_t i, j, k;
	
	for ( i = 0; i < sizeOfMenu; i++ )	// ����һ��˵�����ÿһ����Ա
	{
		k = 0;
		
		if ( yourMenu[i].numSonMenu == 0 )	// û����Ŀ¼��
			yourMenu[i].pSonMenu = null;
		else
			yourMenu[i].pSonMenu = (Menu**)malloc(yourMenu[i].numSonMenu * sizeof(Menu*));
		
		for ( j = 0; j < sizeOfMenu; j++ )	// ����һ��˵�����ÿһ����Ա
		{
			if ( yourMenu[j].thisMenuID == yourMenu[i].fatherMenuID )	// ��ɸ�Ŀ¼ָ��ĸ�ֵ
				yourMenu[i].pFatherMenu = &yourMenu[j];
			if ( yourMenu[j].fatherMenuID == yourMenu[i].thisMenuID )	// �����Ŀ¼ָ������ĸ�ֵ
			{
				yourMenu[i].pSonMenu[k] = &yourMenu[j];
				k++;
			}
		}
	}
}

// ������up dowm ���ĺ���
static inline void keyPress_UpandDown(uint8_t keyValue)
{
	uint8_t i, j, numberChose = 0, numberHead, numberTail;
	
	for ( i = 0; i < menuManage.nowMenu->numSonMenu; i++ )	// ���������Ӳ˵����ҵ���ǰѡ��ġ���ʾ�ĵ�һ�������һ���˵����Ӳ˵��������λ��
	{
		if ( menuManage.nowMenu->pSonMenu[i] == menuManage.choiceMenu )
			numberChose = i;
		if ( menuManage.nowMenu->pSonMenu[i] == menuManage.optionMenuHead )
			numberHead = i;
		if ( menuManage.nowMenu->pSonMenu[i] == menuManage.optionMenuTail )
			numberTail = i;
	}
	
	switch ( keyValue )
	{	
		case 1: 
		{			
			if  ( numberChose > 0 )	// numberChose>0�������ƣ��������κη�Ӧ
			{
				menuManage.choiceMenu = menuManage.nowMenu->pSonMenu[numberChose-1];
				if ( numberChose-1 < numberHead )	// ��ͷ���ٰ�������������������
				{
					numberChose -= 1;
					numberHead -= 1;
					numberTail -= 1;
					menuManage.optionMenuHead = menuManage.nowMenu->pSonMenu[numberHead];
					menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[numberTail];
					// ��ͼ
					OLED_Clear(0, 16, 127, 63, 0);
					OLED_DispChar_UI(0, 16, &Material_Arrow, 0);
					for ( j = 0; j <= numberTail-numberHead; j++ )
					{
						OLED_ShowStr(16, j*16+16, (*menuManage.nowMenu->pSonMenu[numberHead+j]).menuName, 0);
					}
					OLED_AntiColor(0, 16, 127, 31, 0);
				}
				else	// �������;�������������岻�����������
				{
					j = numberChose -  numberHead;
					j = 16 + j * 16;
					OLED_Fill(0, j, 15, j+15, 0);
					OLED_DispChar_UI(0, j-16, &Material_Arrow, 0);
					OLED_AntiColor(0, j-16, 127, j+15, 0);
				}
			}
			refreshArea();
		}break;
		case 2: 
		{
			if  ( numberChose < menuManage.nowMenu->numSonMenu - 1 )	// numberChose<�Ӳ˵������������ƣ��������κη�Ӧ
			{				
				menuManage.choiceMenu = menuManage.nowMenu->pSonMenu[numberChose+1];
				if ( numberChose+1 > numberTail )	// ��ͷ���ٰ�������������������
				{	
					numberChose += 1;
					numberHead += 1;
					numberTail += 1;					
					menuManage.optionMenuHead = menuManage.nowMenu->pSonMenu[numberHead];
					menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[numberTail];
					// ��ͼ
					OLED_Clear(0, 16, 127, 63, 0);
					OLED_DispChar_UI(0, 48, &Material_Arrow, 0);
					for ( j = 0; j <= numberTail-numberHead; j++ )
					{
						OLED_ShowStr(16, j*16+16, (*menuManage.nowMenu->pSonMenu[numberHead+j]).menuName, 0);
					}
					OLED_AntiColor(0, 48, 127, 63, 0);
				}
				else	// �������;�������������岻�����������
				{
					j = numberChose -  numberHead;
					j = 16 + j * 16;
					OLED_Fill(0, j, 15, j+16, 0);
					OLED_DispChar_UI(0, j+16, &Material_Arrow, 0);
					OLED_AntiColor(0, j, 127, j+31, 0);
				}
			}
			refreshArea();
		}break;
	}
}

// ������Yes No ���ĺ��� ����ֵ�� 0-���˳� 1-�˳�Сѭ�����ӷ��濪ʼ
static inline uint8_t keyPress_YesandNo(uint8_t keyValue)
{
	uint8_t i, j;
	switch ( keyValue )
	{
		case 3 : 
		{		
			if ( menuManage.nowMenu->pSonMenu == null )	// û���Ӳ˵�����ú���
			{
				if ( menuManage.choiceMenu->pFunction != null )
					(*menuManage.choiceMenu->pFunction)();
			}
			else	// ���Ӳ˵������
			{
                                                                                                                                        				
				menuManage.nowMenu = menuManage.choiceMenu;	//��ǰ�˵�ָ�����
				menuManage.choiceMenu = menuManage.nowMenu->pSonMenu[0];	//��ǰѡ��Ĳ˵�����
				menuManage.optionMenuHead = menuManage.choiceMenu;
				if ( menuManage.nowMenu->numSonMenu < 3 )
				{
					menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[menuManage.nowMenu->numSonMenu-1];
					OLED_Clear(0, 0, 127, 63, 0);
					OLED_DispChar_UI(0, 16, &Material_Arrow, 0);
					for ( j = 0; j < menuManage.nowMenu->numSonMenu ; j++ )
					{
						OLED_ShowStr(16, j*16+16, menuManage.nowMenu->pSonMenu[j]->menuName, 0);
					}
				}
				else					
				{
					menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[2];
					OLED_Clear(0, 0, 127, 63, 0);
					OLED_DispChar_UI(0, 16, &Material_Arrow, 0);
					for ( j = 0; j < 3 ; j++ )
					{
						OLED_ShowStr(16, j*16+16, menuManage.nowMenu->pSonMenu[j]->menuName, 0);
					}				
				}
				OLED_ShowStr((128-(8*strlen(menuManage.nowMenu->menuName)))/2, 0, menuManage.nowMenu->menuName, 0);
				OLED_AntiColor(0, 0, 127, 31, 0);
			}
			refreshArea();
		}break;
		case 4 : 
		{
			if ( menuManage.nowMenu->pFatherMenu != null )	// ���Ǹ��˵�
			{
				menuManage.nowMenu = menuManage.nowMenu->pFatherMenu;	//��ǰ�˵�ָ�����
				menuManage.choiceMenu = menuManage.nowMenu->pSonMenu[0];	//��ǰѡ��Ĳ˵�����
				menuManage.optionMenuHead = menuManage.choiceMenu;	// ͷָ�����
				OLED_Clear(0, 0, 127, 63, 0);
				OLED_DispChar_UI(0, 16, &Material_Arrow, 0);
				if ( menuManage.nowMenu->numSonMenu < 3 )
				{
					menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[menuManage.nowMenu->numSonMenu - 1];			
					for ( j = 0; j < menuManage.nowMenu->numSonMenu ; j++ )
					{
						OLED_ShowStr(16, j*16+16, menuManage.nowMenu->pSonMenu[j]->menuName, 0);
					}
				}
				else					
				{
					menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[2];
					for ( j = 0; j < 3 ; j++ )
					{
						OLED_ShowStr(16, j*16+16, menuManage.nowMenu->pSonMenu[j]->menuName, 0);
					}
				}
				OLED_ShowStr((128-(8*strlen(menuManage.nowMenu->menuName)))/2, 0, menuManage.nowMenu->menuName, 0);
				OLED_AntiColor(0, 0, 127, 31, 0);
			}
			else	// �ڸ��˵������˷��أ���ָ��ָ���ʼ������1���˳��˵������ѭ��
			{
				menuManage.choiceMenu = &yourMenu[0];
				menuManage.nowMenu = &yourMenu[0];
				return 1;
			}
			refreshArea();
		}break;
	}
	return 0;
}

// ����˵�������
void enterMainMenu(void)
{
	uint8_t flag = 0, value = 0;
	
	while ( 1 )
	{
		OLED_DispChar_UI( 0, 0, &Cover_Arrow, 1);	// ��������
		
		do 
		{
			value = testPress();
		}while ( value != 3 && value != 4 );
		
		if ( value == 4 )
			return;
		else
		{	
			flag = 0;
			keyPress_YesandNo(3);
			while ( flag == 0 )
			{
				switch ( testPress() )
				{
					case 0: break;
					case 1: keyPress_UpandDown(1); break;
					case 2: keyPress_UpandDown(2); break;
					case 3: flag = keyPress_YesandNo(3); break;
					case 4: flag = keyPress_YesandNo(4); break;
				}
			}
		}
	}
}



