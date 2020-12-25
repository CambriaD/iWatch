#ifndef __OLED_H
#define __OLED_H

#include "stc8a.h"

#define  u8 unsigned char 
#define  u32 unsigned int 
	
//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 1

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

#define OLED_DAT	P0
sbit OLED_RST = P1^1;		//��λ
sbit OLED_DC =	P5^3;		//����/�������
sbit OLED_WR =	P5^2;
sbit OLED_RD =	P1^0;
//sbit OLED_CS=P2^0; //Ƭѡ
#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_WR_Clr() OLED_WR=0
#define OLED_WR_Set() OLED_WR=1

#define OLED_RD_Clr() OLED_RD=0
#define OLED_RD_Set() OLED_RD=1


#define XLevelL			0x02
#define XLevelH			0x10
#define Max_Column	128
#define Max_Row			64
#define X_WIDTH 		128
#define Y_WIDTH 		64	    						  
//-----------------OLED�˿ڶ���----------------  					   

void delay_ms(unsigned int ms);

//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	
void OLED_Set_Brightness(unsigned char brightness);
//void OLED_Horizental_Reverse(unsigned char reverse);
//void OLED_Vertical_Reverse(unsigned char reverse);
void OLED_Inverse(unsigned char k);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Refresh(unsigned char *buf);


#endif  
	 



