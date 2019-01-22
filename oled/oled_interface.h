#ifndef __OLED_INTERFACE_H
#define	__OLED_INTERFACE_H

#include <stm32f10x.h>

#define null			0


// 代表某一级目录的结构体，通过组成数组构成一系列的目录
typedef struct typedef_Menu
{
	uint8_t 			*menuName;		// 当前菜单名称
	uint8_t				thisMenuID;		// 当前菜单ID
	uint8_t				fatherMenuID;	// 父菜单ID
	uint8_t				numSonMenu;		// 子菜单个数
	struct typedef_Menu	*pFatherMenu;	// 父菜单指针
	struct typedef_Menu	**pSonMenu;		// 子菜单指针数组指针
	void				(*pFunction)();	// 函数指针
}Menu;

typedef struct typedef_ManageMenu
{
	Menu 	*nowMenu;					// 当前所在菜单的指针
	Menu	*choiceMenu;				// 当前所选择菜单的指针	
	Menu	*optionMenuHead;			// 当前界面的头选项
	Menu	*optionMenuTail;			// 当前界面的尾选项
}Manage;

extern Menu yourMenu[];

void menuInit(void);
void enterMainMenu(void);

#endif /* __OLED_INTERFACE_H */
