#include "./i2c/bsp_i2c.h"
#include "stm32f10x.h"

/*
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*/
inline static void i2c_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
		����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż� 
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	//for (i = 0; i < 10; i++);
}

/*
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*/
void i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	EEPROM_I2C_SDA_1();
	EEPROM_I2C_SCL_1();
	i2c_Delay();
	EEPROM_I2C_SDA_0();
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();
}

/*
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*/
void i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	EEPROM_I2C_SDA_0();
	EEPROM_I2C_SCL_1();
	i2c_Delay();
	EEPROM_I2C_SDA_1();
}

/*
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			EEPROM_I2C_SDA_1();
		}
		else
		{
			EEPROM_I2C_SDA_0();
		}
		i2c_Delay();
		EEPROM_I2C_SCL_1();
		i2c_Delay();	
		EEPROM_I2C_SCL_0();
		if (i == 7)
		{
			 EEPROM_I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}

/*
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		EEPROM_I2C_SCL_1();
		i2c_Delay();
		if (EEPROM_I2C_SDA_READ())
		{
			value++;
		}
		EEPROM_I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

/*
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	EEPROM_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	EEPROM_I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (EEPROM_I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	EEPROM_I2C_SCL_0();
	i2c_Delay();
	return re;
}

/*
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*/
void i2c_Ack(void)
{
	EEPROM_I2C_SDA_0();	/* CPU����SDA = 0 */
	i2c_Delay();
	EEPROM_I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();
	EEPROM_I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*/
void i2c_NAck(void)
{
	EEPROM_I2C_SDA_1();	/* CPU����SDA = 1 */
	i2c_Delay();
	EEPROM_I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();	
}

/*
*	�� �� ��: i2c_CfgGpio
*	����˵��: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
*	��    �Σ���
*	�� �� ֵ: ��
*/
void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(EEPROM_RCC_I2C_PORT, ENABLE);	/* ��GPIOʱ�� */

	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN | EEPROM_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	/* ��©��� */
	GPIO_Init(EEPROM_GPIO_PORT_I2C, &GPIO_InitStructure);

	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	i2c_Stop();
}

/*
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*/
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	i2c_CfgGpio();		/* ����GPIO */
	
	i2c_Start();		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(_Address | EEPROM_I2C_WR);
	ucAck = i2c_WaitAck();	/* ����豸��ACKӦ�� */

	i2c_Stop();			/* ����ֹͣ�ź� */

	return ucAck;
}