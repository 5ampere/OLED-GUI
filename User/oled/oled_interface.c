#include "./oled/OLED_interface.h"
#include "./oled/OLED_input.h"
#include "./oled/OLED_drive.h"
#include <stdlib.h>
#include <string.h>

// 菜单数组
// 数据结构 : 当前目录名称 当前目录ID 父目录ID 子菜单个数 父目录指针 子目录指针数组指针 函数指针
#define sizeOfMenu		6 
Menu yourMenu[sizeOfMenu] = 
{	
/************************ 用户自定义菜单数组 ************************/
	{ (uint8_t*)"ROOT", 1, 0, 2, null, null, null },	// 这是根菜单,不能删除，可以更改名称、子菜单数量
	{ (uint8_t*)"Menu_1", 2, 1, 0, null, null, null },
	{ (uint8_t*)"Menu_2", 3, 1, 3, null, null, null },
	{ (uint8_t*)"Menu_3", 4, 3, 0, null, null, null },
	{ (uint8_t*)"Menu_4", 5, 3, 0, null, null, null },
	{ (uint8_t*)"Menu_5", 6, 3, 0, null, null, null },
/************************ 用户自定义菜单数组 ************************/
};

// 菜单管理结构体
Manage menuManage = {
	&yourMenu[0],
	&yourMenu[0],
	&yourMenu[0],
	&yourMenu[0],
}; 

// 菜单数结构的初始化
void menuInit(void)
{
	uint16_t i, j, k;
	
	for ( i = 0; i < sizeOfMenu; i++ )	// 遍历一遍菜单树的每一个成员
	{
		k = 0;
		
		if ( yourMenu[i].numSonMenu == 0 )	// 没有子目录了
			yourMenu[i].pSonMenu = null;
		else
			yourMenu[i].pSonMenu = (Menu**)malloc(yourMenu[i].numSonMenu * sizeof(Menu*));
		
		for ( j = 0; j < sizeOfMenu; j++ )	// 遍历一遍菜单树的每一个成员
		{
			if ( yourMenu[j].thisMenuID == yourMenu[i].fatherMenuID )	// 完成父目录指针的赋值
				yourMenu[i].pFatherMenu = &yourMenu[j];
			if ( yourMenu[j].fatherMenuID == yourMenu[i].thisMenuID )	// 完成子目录指针数组的赋值
			{
				yourMenu[i].pSonMenu[k] = &yourMenu[j];
				k++;
			}
		}
	}
}

// 处理按下up dowm 键的函数
static inline void keyPress_UpandDown(uint8_t keyValue)
{
	uint8_t i, j, numberChose = 0, numberHead, numberTail;
	
	for ( i = 0; i < menuManage.nowMenu->numSonMenu; i++ )	// 遍历所有子菜单，找到当前选择的、显示的第一个、最后一个菜单在子菜单数组里的位置
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
			if  ( numberChose > 0 )	// numberChose>0则向上移，否则不做任何反应
			{
				menuManage.choiceMenu = menuManage.nowMenu->pSonMenu[numberChose-1];
				if ( numberChose-1 < numberHead )	// 到头了再按下向上则整体向上移
				{
					numberChose -= 1;
					numberHead -= 1;
					numberTail -= 1;
					menuManage.optionMenuHead = menuManage.nowMenu->pSonMenu[numberHead];
					menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[numberTail];
					// 绘图
					OLED_Clear(0, 16, 127, 63, 0);
					OLED_DispChar_UI(0, 16, &Material_Arrow, 0);
					for ( j = 0; j <= numberTail-numberHead; j++ )
					{
						OLED_ShowStr(16, j*16+16, (*menuManage.nowMenu->pSonMenu[numberHead+j]).menuName, 0);
					}
					OLED_AntiColor(0, 16, 127, 31, 0);
				}
				else	// 光标在中途按下向上则整体不动，光标向上
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
			if  ( numberChose < menuManage.nowMenu->numSonMenu - 1 )	// numberChose<子菜单数量则向下移，否则不做任何反应
			{				
				menuManage.choiceMenu = menuManage.nowMenu->pSonMenu[numberChose+1];
				if ( numberChose+1 > numberTail )	// 到头了再按下向下则整体向下移
				{	
					numberChose += 1;
					numberHead += 1;
					numberTail += 1;					
					menuManage.optionMenuHead = menuManage.nowMenu->pSonMenu[numberHead];
					menuManage.optionMenuTail = menuManage.nowMenu->pSonMenu[numberTail];
					// 绘图
					OLED_Clear(0, 16, 127, 63, 0);
					OLED_DispChar_UI(0, 48, &Material_Arrow, 0);
					for ( j = 0; j <= numberTail-numberHead; j++ )
					{
						OLED_ShowStr(16, j*16+16, (*menuManage.nowMenu->pSonMenu[numberHead+j]).menuName, 0);
					}
					OLED_AntiColor(0, 48, 127, 63, 0);
				}
				else	// 光标在中途按下向下则整体不动，光标向下
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

// 处理按下Yes No 键的函数 返回值： 0-不退出 1-退出小循环，从封面开始
static inline uint8_t keyPress_YesandNo(uint8_t keyValue)
{
	uint8_t i, j;
	switch ( keyValue )
	{
		case 3 : 
		{		
			if ( menuManage.nowMenu->pSonMenu == null )	// 没有子菜单则调用函数
			{
				if ( menuManage.choiceMenu->pFunction != null )
					(*menuManage.choiceMenu->pFunction)();
			}
			else	// 有子菜单的情况
			{
                                                                                                                                        				
				menuManage.nowMenu = menuManage.choiceMenu;	//当前菜单指针更新
				menuManage.choiceMenu = menuManage.nowMenu->pSonMenu[0];	//当前选择的菜单更新
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
			if ( menuManage.nowMenu->pFatherMenu != null )	// 不是根菜单
			{
				menuManage.nowMenu = menuManage.nowMenu->pFatherMenu;	//当前菜单指针更新
				menuManage.choiceMenu = menuManage.nowMenu->pSonMenu[0];	//当前选择的菜单更新
				menuManage.optionMenuHead = menuManage.choiceMenu;	// 头指针更新
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
			else	// 在根菜单按下了返回，将指针恢复初始并返回1，退出菜单界面的循环
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

// 进入菜单主界面
void enterMainMenu(void)
{
	uint8_t flag = 0, value = 0;
	
	while ( 1 )
	{
		OLED_DispChar_UI( 0, 0, &Cover_Arrow, 1);	// 画主界面
		
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



