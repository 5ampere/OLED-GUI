#ifndef __OLED_INTERFACE_H
#define	__OLED_INTERFACE_H

#include <stm32f10x.h>

#define null			0
#define sizeOfMenu		10

// ����ĳһ��Ŀ¼�Ľṹ�壬ͨ��������鹹��һϵ�е�Ŀ¼
typedef struct typedef_Menu
{
	uint8_t 			*menuName;		// ��ǰ�˵�����
	uint8_t				thisMenuID;		// ��ǰ�˵�ID
	uint8_t				fatherMenuID;	// ���˵�ID
	uint8_t				numSonMenu;		// �Ӳ˵�����
	struct typedef_Menu	*pFatherMenu;	// ���˵�ָ��
	struct typedef_Menu	**pSonMenu;		// �Ӳ˵�ָ������ָ��
	uint8_t				(*pFunction)();	// ����ָ��
}Menu;

typedef struct typedef_ManageMenu
{
	Menu 	*nowMenu;					// ��ǰ���ڲ˵���ָ��
	Menu	*lastMenu;					// �ϴ��˳�ǰ�Ĳ˵�ָ��
	Menu	*choiceMenu;				// ��ǰ��ѡ��˵���ָ��	
	Menu	*optionMenuHead;			// ��ǰ�����ͷѡ��
	Menu	*optionMenuTail;			// ��ǰ�����βѡ��
}Manage;

extern Menu yourMenu[];

void menuInit(void);
void enterMainMenu(void);

#endif /* __OLED_INTERFACE_H */
