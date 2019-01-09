#include "./oled/OLED.h"
#include "./i2c/bsp_i2c.h"
#include "./font/fonts.h"

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
    WriteCmd(0xb0+y);
    WriteCmd(((x&0xf0)>>4)|0x10);
    WriteCmd((x&0x0f)|0x01);
}

//ȫ�����
void OLED_Fill(unsigned char fill_Data)
{
    unsigned char m,n;
    for(m=0; m<8; m++)
    {
        WriteCmd(0xb0+m);		//page0-page1
        WriteCmd(0x00);		//low column start address
        WriteCmd(0x10);		//high column start address
        for(n=0; n<128; n++)
        {
            WriteDat(fill_Data);
        }
    }
}

//����
void OLED_CLS(void)
{
    OLED_Fill(0x00);
}

//��OLED�������л���
void OLED_ON(void)
{
    WriteCmd(0X8D);  //���õ�ɱ�
    WriteCmd(0X14);  //������ɱ�
    WriteCmd(0XAF);  //OLED����
}

//Description    : ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
void OLED_OFF(void)
{
    WriteCmd(0X8D);  //���õ�ɱ�
    WriteCmd(0X10);  //�رյ�ɱ�
    WriteCmd(0XAE);  //OLED����
}

//��ʾ�ַ���
void OLED_ShowStr(unsigned char x, unsigned char y, char * pStr)
{
    unsigned char c = 0,i = 0,j = 0;
    uint16_t usCh;

    while ( * pStr != '\0' )
    {
        if ( * pStr <= 126 && * pStr >= 32 )	           	//Ӣ���ַ�
        {
            if ( x+WIDTH_EN_CHAR > 128 )
            {
                x = 0;
                y += 1;
            }
            OLED_DispChar_EN(x, y, *pStr);
            x += WIDTH_EN_CHAR;
            pStr += 1;
        }
        else												//ASCII��С��32�������ַ�
        {
            pStr += 1;
        }
    }
}

//OLED����ʾһ��Ӣ���ַ�
void OLED_DispChar_EN( uint16_t usX, uint16_t usY, const char cChar )
{
    uint8_t  rowCount, bitCount,fontLength;
    uint16_t ucRelativePositon;
    uint8_t usTemp, *Pfont;
    uint8_t i;

    //��ascii���ƫ�ƣ���ģ������ASCII���ǰ32����ͼ�η��ţ�
    ucRelativePositon = cChar - ' ';

    //ÿ����ģ���ֽ���
    fontLength = (Font8x16.Width*Font8x16.Height)/8;

    //��ģ�׵�ַ
    Pfont = (uint8_t *)&Font8x16.table[ucRelativePositon * fontLength];

    //һҳһҳ��ʾ����
    for ( rowCount=0; rowCount<(HEIGHT_EN_CHAR/8); rowCount++ )
    {
        //������ʾλ��
        OLED_SetPos(usX, LINEY(usY)+rowCount);
        for ( bitCount=0; bitCount<WIDTH_EN_CHAR; bitCount++ )
        {
            for ( i=0; i<8; i++ )
            {
                usTemp |= ( ( (Pfont[8*rowCount+i]) >> (7 - bitCount) )&0x01 ) << i ;
            }
            WriteDat(usTemp);
            usTemp = 0;
        }
    }
}
