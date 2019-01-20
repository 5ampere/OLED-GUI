#include "./oled/OLED_interface.h"
#include "./oled/OLED_input.h"
#include "./oled/OLED_drive.h"
#include <stdlib.h>

// �˵�����
// ���ݽṹ : ��ǰĿ¼���� ��ǰĿ¼ID ��Ŀ¼ID �Ӳ˵����� ��Ŀ¼ָ�� ��Ŀ¼ָ������ָ�� ����ָ��
Menu yourMenu[sizeOfMenu] = 
{	
/************************ �û��Զ���˵����� ************************/
	{ (uint8_t*)"", 0, 0, 2, null, null, null },	// ���ǲ˵�������
	{ (uint8_t*)"Menu_1", 1, 0, 0, null, null, null },
	{ (uint8_t*)"Menu_2", 2, 0, 4, null, null, null },
	{ (uint8_t*)"Menu_3", 3, 2, 0, null, null, null },
	{ (uint8_t*)"Menu_4", 4, 2, 0, null, null, null },
	{ (uint8_t*)"Menu_5", 5, 2, 0, null, null, null },
	{ (uint8_t*)"Menu_6", 6, 2, 0, null, null, null },
/************************ �û��Զ���˵����� ************************/
};

// �˵�����ṹ��
Manage menuManage = {
	&yourMenu[0],
	&yourMenu[0],
	null,
	null,
	null
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
	uint8_t i, j, numberChose, numberHead, numberTail;
	switch ( keyValue )
	{
		for ( i = 0; i < menuManage.nowMenu->numSonMenu; i++ )	// ���������Ӳ˵����ҵ���ǰѡ��ġ���ʾ�ĵ�һ�������һ���˵����Ӳ˵��������λ��
		{
			if ( menuManage.nowMenu->pSonMenu[i] == menuManage.choiceMenu )
				numberChose = i;
			else if ( menuManage.nowMenu->pSonMenu[i] == menuManage.optionMenuHead )
				numberHead = i;
			else if ( menuManage.nowMenu->pSonMenu[i] == menuManage.optionMenuTail )
				numberTail = i;
		}
		case 1: 
		{			
			if  ( numberChose > 0 )	// numberChose>0�������ƣ��������κη�Ӧ
			{
				menuManage.choiceMenu = menuManage.nowMenu->pSonMenu[numberChose-1];
				if ( numberChose-1 < numberHead )	// ��ͷ���ٰ�������������������
				{
					menuManage.optionMenuHead = menuManage.nowMenu->pSonMenu[numberHead-1];
					menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[numberTail-1];
					// ��ͼ
					OLED_Clear(0, 16, 127, 63);
					for ( j = 0; j <= numberTail-numberHead; j++ )
					{
						OLED_ShowStr(0, j*16+16, (*menuManage.nowMenu->pSonMenu[numberHead+j]).menuName);
					}
					antiColor(0, 16, 127, 31);
				}
				else	// �������;�������������岻�����������
				{
					j = numberChose -  numberHead;
					j = 16 + j * 16;
					antiColor(0, j-16, 127, j+15);
				}
			}
		}break;
		case 2: 
		{
			if  ( numberChose < menuManage.nowMenu->numSonMenu )	// numberChose<�Ӳ˵������������ƣ��������κη�Ӧ
			{
				menuManage.choiceMenu = menuManage.nowMenu->pSonMenu[numberChose+1];
				if ( numberChose+1 > numberTail )	// ��ͷ���ٰ�������������������
				{
					menuManage.optionMenuHead = menuManage.nowMenu->pSonMenu[numberHead+1];
					menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[numberTail+1];
					// ��ͼ
					OLED_Clear(0, 16, 127, 63);
					for ( j = 0; j <= numberTail-numberHead; j++ )
					{
						OLED_ShowStr(0, j*16+16, (*menuManage.nowMenu->pSonMenu[numberHead+j]).menuName);
					}
					antiColor(0, 48, 127, 63);
				}
				else	// �������;�������������岻�����������
				{
					j = numberChose -  numberHead;
					j = 16 + j * 16;
					antiColor(0, j, 127, j+31);
				}
			}
		}break;
	}
}

// ������Yes No ���ĺ���
static inline uint8_t keyPress_YesandNo(uint8_t keyValue)
{
	uint8_t i, j, k = 0;
	switch ( keyValue )
	{
		case 3 : 
		{
			menuManage.nowMenu = menuManage.choiceMenu;	//��ǰ�˵�ָ�����
			if ( menuManage.nowMenu->pSonMenu == null )	// û���Ӳ˵�����ú���
			{
				if ( menuManage.choiceMenu->pFunction != null )
					k = (*menuManage.choiceMenu->pFunction)();
				return k;
			}
			else	// ���Ӳ˵������
			{
				menuManage.choiceMenu = menuManage.nowMenu->pSonMenu[0];	//��ǰѡ��Ĳ˵�����
				menuManage.optionMenuHead = menuManage.choiceMenu;
				if ( menuManage.nowMenu->numSonMenu < 3 )
				{
					menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[menuManage.nowMenu->numSonMenu];
					OLED_Clear(0, 0, 127, 63);
					for ( j = 0; j < menuManage.nowMenu->numSonMenu ; j++ )
					{
						OLED_ShowStr(0, j*16+16, menuManage.nowMenu->pSonMenu[j]->menuName);
					}
				}
				else					
				{
					menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[2];
					OLED_Clear(0, 0, 127, 63);
					for ( j = 0; j < 3 ; j++ )
					{
						OLED_ShowStr(0, j*16+16, menuManage.nowMenu->pSonMenu[j]->menuName);
					}				
				}
				OLED_ShowStr(0, 64, menuManage.nowMenu->menuName);
				antiColor(0, 0, 127, 31);
			}
		}break;
		case 4 : 
		{
			menuManage.nowMenu = menuManage.nowMenu->pFatherMenu;	//��ǰ�˵�ָ�����
			menuManage.choiceMenu = menuManage.nowMenu->pSonMenu[0];	//��ǰѡ��Ĳ˵�����
			OLED_Clear(0, 0, 127, 63);
			if ( menuManage.nowMenu->numSonMenu < 3 )
			{
				menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[menuManage.nowMenu->numSonMenu];			
				for ( j = 0; j < menuManage.nowMenu->numSonMenu ; j++ )
				{
					OLED_ShowStr(0, j*16+16, menuManage.nowMenu->pSonMenu[j]->menuName);
				}
			}
			else					
			{
				menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[2];
				for ( j = 0; j < 3 ; j++ )
				{
					OLED_ShowStr(0, j*16+16, menuManage.nowMenu->pSonMenu[j]->menuName);
				}
			}
			OLED_ShowStr(0, 64, menuManage.nowMenu->menuName);
			antiColor(0, 0, 127, 31);
		}break;
	}
}

// ����˵�������
void enterMainMenu(void)
{
	uint8_t flag = 0;
	
	OLED_DispChar_UI( 0, 0, &Cover_Arrow);	// ��������
	
	while ( testPress() == 0 );				// �������������˵�
	
	while ( flag == 0 )
	{
		switch ( testPress() )
		{
			case 0:break;
			case 1: keyPress_UpandDown(1); break;
			case 2: keyPress_UpandDown(2); break;
			case 3: flag = keyPress_YesandNo(3);  break;
			case 4: keyPress_YesandNo(4);  break;
		}
	}
}

// ������һ�ε�Ŀ¼
void enterLastMenu(void)
{
	
}


