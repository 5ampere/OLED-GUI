#include "./oled/OLED.h"
#include "./i2c/bsp_i2c.h"
#include "./font/fonts.h"

//Ms延时
static void DelayMs(unsigned int nms)
{
    unsigned char a,b;
    for(; nms>0; nms--)
        for(b=109; b>0; b--)
            for(a=26; a>0; a--);
}

//I2C总线初始化
void I2C_Configuration(void)
{
    i2c_CfgGpio();
}

//I2C写一个字节
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

//写命令
void WriteCmd(unsigned char I2C_Command)
{
    I2C_WriteByte(0x00, I2C_Command);
}

//写数据
void WriteDat(unsigned char I2C_Data)
{
    I2C_WriteByte(0x40, I2C_Data);
}

//OLED初始化
void OLED_Init(void)
{
    DelayMs(100); //这里的延时很重要

    WriteCmd(0xAE); //display off
    WriteCmd(0x20);	//Set Memory Addressing Mode
    WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8);	//Set COM Output Scan Direction
    WriteCmd(0x00); //---set low column address
    WriteCmd(0x10); //---set high column address
    WriteCmd(0x40); //--set start line address
    WriteCmd(0x81); //--set contrast control register
    WriteCmd(0xff); //亮度调节 0x00~0xff
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

//设置起始点坐标
void OLED_SetPos(unsigned char x, unsigned char y)
{
    WriteCmd(0xb0+y);
    WriteCmd(((x&0xf0)>>4)|0x10);
    WriteCmd((x&0x0f)|0x01);
}

//全屏填充
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

//清屏
void OLED_CLS(void)
{
    OLED_Fill(0x00);
}

//将OLED从休眠中唤醒
void OLED_ON(void)
{
    WriteCmd(0X8D);  //设置电荷泵
    WriteCmd(0X14);  //开启电荷泵
    WriteCmd(0XAF);  //OLED唤醒
}

//Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
void OLED_OFF(void)
{
    WriteCmd(0X8D);  //设置电荷泵
    WriteCmd(0X10);  //关闭电荷泵
    WriteCmd(0XAE);  //OLED休眠
}

//显示字符串
void OLED_ShowStr(unsigned char x, unsigned char y, char * pStr)
{
    unsigned char c = 0,i = 0,j = 0;
    uint16_t usCh;

    while ( * pStr != '\0' )
    {
        if ( * pStr <= 126 && * pStr >= 32 )	           	//英文字符
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
        else												//ASCII码小于32的特殊字符
        {
            pStr += 1;
        }
    }
}

//OLED上显示一个英文字符
void OLED_DispChar_EN( uint16_t usX, uint16_t usY, const char cChar )
{
    uint8_t  rowCount, bitCount,fontLength;
    uint16_t ucRelativePositon;
    uint8_t usTemp, *Pfont;
    uint8_t i;

    //对ascii码表偏移（字模表不包含ASCII表的前32个非图形符号）
    ucRelativePositon = cChar - ' ';

    //每个字模的字节数
    fontLength = (Font8x16.Width*Font8x16.Height)/8;

    //字模首地址
    Pfont = (uint8_t *)&Font8x16.table[ucRelativePositon * fontLength];

    //一页一页显示数据
    for ( rowCount=0; rowCount<(HEIGHT_EN_CHAR/8); rowCount++ )
    {
        //设置显示位置
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
