#include "./oled/OLED_drive.h"
#include "./i2c/bsp_i2c.h"
#include "./font/fonts.h"

videoMemory videoMem = {	//�Դ�ṹ��
	0,
	0,
	0,
	0,
	0
};

//Ms��ʱ
static void DelayMs(unsigned int nms)
{
    unsigned char a,b;
    for(; nms>0; nms--)
        for(b=109; b>0; b--)
            for(a=26; a>0; a--);
}

//I2C���߳�ʼ��
void I2C_Configuration(void)
{
    i2c_CfgGpio();
}

//I2Cдһ���ֽ�
void I2C_WriteByte(uint8_t addr,uint8_t data)
{
    i2c_Start();

    i2c_SendByte(OLED_ADDRESS | EEPROM_I2C_WR);
    i2c_WaitAck();

    i2c_SendByte(addr);
    i2c_WaitAck();

    i2c_SendByte(data);
    i2c_WaitAck();

    i2c_Stop();
}

//д����
void WriteCmd(unsigned char I2C_Command)
{
    I2C_WriteByte(0x00, I2C_Command);
}

//д����
void WriteDat(unsigned char I2C_Data)
{
    I2C_WriteByte(0x40, I2C_Data);
}

//OLED��ʼ��
void OLED_Init(void)
{
    DelayMs(100); //�������ʱ����Ҫ

    WriteCmd(0xAE); //display off
    WriteCmd(0x20);	//Set Memory Addressing Mode
    WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8);	//Set COM Output Scan Direction
    WriteCmd(0x00); //---set low column address
    WriteCmd(0x10); //---set high column address
    WriteCmd(0x40); //--set start line address
    WriteCmd(0x81); //--set contrast control register
    WriteCmd(0xff); //���ȵ��� 0x00~0xff
    WriteCmd(0xa1); //--set segment re-map 0 to 127
    WriteCmd(0xa6); //--set normal display
    WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
    WriteCmd(0x3F); //
    WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    WriteCmd(0xd3); //-set display offset
    WriteCmd(0x00); //-not offset
    WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
    WriteCmd(0xf0); //--set divide ratio
    WriteCmd(0xd9); //--set pre-charge period
    WriteCmd(0x22); //
    WriteCmd(0xda); //--set com pins hardware configuration
    WriteCmd(0x12);
    WriteCmd(0xdb); //--set vcomh
    WriteCmd(0x20); //0x20,0.77xVcc
    WriteCmd(0x8d); //--set DC-DC enable
    WriteCmd(0x14); //
    WriteCmd(0xaf); //--turn on oled panel
}

//������ʼ������
void OLED_SetPos(unsigned char x, unsigned char y)
{
    WriteCmd(0xb0+(y & 0x0f));
    WriteCmd((x & 0xf0)>>4 | 0x10);
    WriteCmd((x & 0x0f) | 0x00);

}

//��OLED�������л���
void OLED_ON(void)
{
    WriteCmd(0X8D);  //���õ�ɱ�
    WriteCmd(0X14);  //������ɱ�
    WriteCmd(0XAF);  //OLED����
}

//��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
void OLED_OFF(void)
{
    WriteCmd(0X8D);  //���õ�ɱ�
    WriteCmd(0X10);  //�رյ�ɱ�
    WriteCmd(0XAE);  //OLED����
}

//���ַ���д���Դ沢��ʾ
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char * pStr)
{
	if ( x > OLED_WIDTH || y > OLED_HEIGHT )				// λ�ò���������
		return;
	videoMem.refreshXL = x;
	videoMem.refreshYH = y;
    while ( * pStr != '\0' )
    {
        if ( * pStr <= 126 && * pStr >= 32 )	           	// Ӣ���ַ�
        {
            OLED_DispChar_EN(x, y, *pStr);
            x += WIDTH_EN_CHAR;			
        }
		pStr += 1;
    }
	videoMem.refreshXR = x;
	videoMem.refreshYL = y + HEIGHT_EN_CHAR - 1;
	refreshArea();
}

//��һ��Ӣ���ַ�д���Դ沢��ʾ
void OLED_DispChar_EN( uint16_t usX, uint16_t usY, const char cChar )
{
    uint16_t ucRelativePositon;
    uint8_t *Pfont;
    uint8_t i, j, k;
   
	if ( usX+WIDTH_EN_CHAR > OLED_WIDTH || usY+HEIGHT_EN_CHAR > OLED_HEIGHT )					// λ�ò���������
		return;
	
	//������ģλ��
    ucRelativePositon = cChar - ' ';								// ��ascii���ƫ�ƣ���ģ������ASCII���ǰ32����ͼ�η��ţ�
    Pfont = (uint8_t *)&Font8x16.table[ucRelativePositon * 16];		// ��ģ�׵�ַ

    //���Դ������������
	j = usY / 8;
	k = usY % 8;
	if ( k == 0 )
	{
		for ( i = 0; i < WIDTH_EN_CHAR; i++ )
		{
			videoMem.pVideoMem[usX+i][j]   = *(Pfont + 2*i);		// ����д��	
			videoMem.pVideoMem[usX+i][j+1] = *(Pfont + 2*i + 1);
		}			
	}
	else
	{
		for ( i = 0; i < WIDTH_EN_CHAR; i++ )
		{
			videoMem.pVideoMem[usX+i][j]   &= 0xff << (8 - k);			// �����Ҫд���λ
			videoMem.pVideoMem[usX+i][j+1] &= 0x00;
			videoMem.pVideoMem[usX+i][j+2] &= 0xff >> k;
			videoMem.pVideoMem[usX+i][j]   |= (*(Pfont + 2*i) >> k);		// ����д��
			videoMem.pVideoMem[usX+i][j+1] |= (*(Pfont+2*i)<<(8-k)) | (*(Pfont+2*i+1) >> k);		
			videoMem.pVideoMem[usX+i][j+2] |= (*(Pfont + 2*i + 1) << (8-k));	
		}
	}
}

//���Դ�����������һ�����״̬
static void OLED_Set_Point( uint8_t usX, uint8_t usY, uint8_t Color )
{
	uint8_t page_Y = usY / 8;
	
	if ( usX>127 || usX<0 || usY > 63 || usY < 0 )
		return;
	
	if ( Color ) // 1Ϊ��ɫ
	{
		videoMem.pVideoMem[usX][page_Y] |= (0x80 >> (usY % 8));
	}
	else
	{
		videoMem.pVideoMem[usX][page_Y] &= (0xff ^ (0x80 >> (usY % 8)));
	}
}

//��һ���Զ���ͼ��д���Դ�
void OLED_DispChar_UI( uint16_t usX, uint16_t usY, yourMaterial *pMaterial )
{
	uint8_t i, j, k;
	uint8_t Material_Y;
	
	if ( usX >= OLED_WIDTH || usY >= OLED_HEIGHT )					// λ�ò���������
		return;
	
	//���Դ������������
	Material_Y = pMaterial->Height / 8 + (pMaterial->Height % 8 != 0); // ÿ�е��ֽ���
	for ( i = 0; i < pMaterial->Width; i++ )
	{
		for ( j = 0; j < pMaterial->Height; j++ )
		{
			k = *(pMaterial->table + i*Material_Y + (j/8)) >> (7 - (j % 8));
			k &= 0x01;
			OLED_Set_Point(i + usX, j + usY, k);
		}
	}
	
	//����ˢ�����򲢽���ˢ��
	videoMem.refreshXL = usX;
	videoMem.refreshXR = usX + pMaterial->Width - 1;
	videoMem.refreshYH = usY ;
	videoMem.refreshYL = usY + pMaterial->Height - 1;
	refreshArea();
}

//��ָ������ɫ x1,y1 x2,y2 Ϊ���ζԽ������˵������
void antiColor(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2)
{
	uint8_t i, j, k;
	
	videoMem.refreshXL = X1 > X2 ? X2 : X1;
	videoMem.refreshXR = X1 < X2 ? X2 : X1;
	videoMem.refreshYH = Y1 > Y2 ? Y2 : Y1;
	videoMem.refreshYL = Y1 < Y2 ? Y2 : Y1;
		
	for ( i = videoMem.refreshXL; i <= videoMem.refreshXR; i++ )
	{
		for ( j = videoMem.refreshYH; j <= videoMem.refreshYL; j++ )
		{
			k = (videoMem.pVideoMem[i][j/8]) >> (7 - (j % 8));
			k &= 0x01;		
			OLED_Set_Point(i, j, (k == 0));
		}
	}
	refreshArea();
}

//��ָ��������� x1,y1 x2,y2 Ϊ���ζԽ������˵������
void OLED_Clear(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2)
{
	uint8_t i, j;
	
	videoMem.refreshXL = X1 > X2 ? X2 : X1;
	videoMem.refreshXR = X1 < X2 ? X2 : X1;
	videoMem.refreshYH = Y1 > Y2 ? Y2 : Y1;
	videoMem.refreshYL = Y1 < Y2 ? Y2 : Y1;
		
	for ( i = videoMem.refreshXL; i <= videoMem.refreshXR; i++ )
	{
		for ( j = videoMem.refreshYH; j <= videoMem.refreshYL; j++ )
		{	
			OLED_Set_Point(i, j, 0);
		}
	}
	refreshArea();
}

//��ָ��������� x1,y1 x2,y2 Ϊ���ζԽ������˵������
void OLED_Fill(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2)
{
	uint8_t i, j;
	
	videoMem.refreshXL = X1 > X2 ? X2 : X1;
	videoMem.refreshXR = X1 < X2 ? X2 : X1;
	videoMem.refreshYH = Y1 > Y2 ? Y2 : Y1;
	videoMem.refreshYL = Y1 < Y2 ? Y2 : Y1;
		
	for ( i = videoMem.refreshXL; i <= videoMem.refreshXR; i++ )
	{
		for ( j = videoMem.refreshYH; j <= videoMem.refreshYL; j++ )
		{	
			OLED_Set_Point(i, j, 1);
		}
	}
	refreshArea();
}
	
//ˢ��ָ������
void refreshArea(void)
{
	uint8_t page_L, page_H, i, j, dataBack, data;
	page_L = videoMem.refreshYL / 8;
	page_H = videoMem.refreshYH / 8;
	
//	for ( i = page_H; i <= page_L; i++ )
//	{
//		OLED_SetPos(videoMem.refreshXL, i);
//		for ( j = videoMem.refreshXL; j <= videoMem.refreshXR; j++ )
//		{
//			dataBack = 0;
//			data = videoMem.pVideoMem[j][i];	// ���Դ����ݷ������
//			dataBack |= ((data & 0x80) >> 7);
//			dataBack |= ((data & 0x40) >> 5);
//			dataBack |= ((data & 0x20) >> 3);
//			dataBack |= ((data & 0x10) >> 1);
//			dataBack |= ((data & 0x08) << 1);
//			dataBack |= ((data & 0x04) << 3);
//			dataBack |= ((data & 0x02) << 5);
//			dataBack |= ((data & 0x01) << 7);
//			WriteDat(dataBack);
//		}
//	}
	
	for ( i = 0; i <= 7; i++ )
	{
		OLED_SetPos(videoMem.refreshXL, i);
		for ( j = 0; j <= 127; j++ )
		{
			dataBack = 0;
			data = videoMem.pVideoMem[j][i];	// ���Դ����ݷ������
			dataBack |= ((data & 0x80) >> 7);
			dataBack |= ((data & 0x40) >> 5);
			dataBack |= ((data & 0x20) >> 3);
			dataBack |= ((data & 0x10) >> 1);
			dataBack |= ((data & 0x08) << 1);
			dataBack |= ((data & 0x04) << 3);
			dataBack |= ((data & 0x02) << 5);
			dataBack |= ((data & 0x01) << 7);
			WriteDat(dataBack);
		}
	}
}








