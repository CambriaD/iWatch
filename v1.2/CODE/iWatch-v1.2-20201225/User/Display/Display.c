#include "font.h"
#include "Display.h"

unsigned char xdata main_cache[1024] = {0};	//���Դ�
unsigned char xdata sub_cache[1024] = {0};	//���Դ�

extern iWatch_config config;			//������Ϣ

/**
	* @brief 	����sys_config�ṹ������ݳ�ʼ����ʾ
	* @param  config��sys_config�ͽṹ���ָ��
	* @retval ��
	*/
void DisplayInit(void)
{
	OLED_Init();																		//��ʼ��OLED
	ScreenSetBrightness(config.screen_brightness);	//�趨��Ļ����
	ScreenSetInverse(config.screen_inverse);				//�趨��Ļ�Ƿ�ɫ
	ScreenOnOff(ON);																//��Ļ����ʾ
}
/**
	* @brief 	��BMPͼƬд��ָ���Դ��е�ָ��λ��
	* @param 	x�������꣬y�������꣬width��ͼƬ�Ŀ�ȣ�height��ͼƬ�ĸ߶ȣ�
	*					buf1��BMPͼ��ָ�룬buf2���Դ��ָ��
	*					k��ѡ�񸲸����Դ��ϻ����ص����Դ���
	*						COVER	����
	*						BLEND	����
	* @retval ��
	*/
void BMPToCache(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned char *buf1, unsigned char *buf2, unsigned char k)
{
	unsigned char data i, j;
	unsigned int data num1, num2, num3;
	num1 = height / 8;
	if(k == 0)				
	{
		for(i = 0; i < num1; i++)
		{
			num2 = (i + y) * 128 + x;
			num3 = i * width;
			for(j = 0; j < width; j++)
				buf2[num2 + j] = buf1[num3 + j];
		}
	}
	else
	{
		for(i = 0; i < num1; i++)
		{
			num2 = (i + y) * 128 + x;
			num3 = i * width;
			for(j = 0; j < width; j++)
				buf2[num2 + j] |= buf1[num3 + j];
		}
	}
	for(i = 0; i < num1; i++)
	{
		num2 = (i + y) * 128 + x;
		num3 = i * width;
		for(j = 0; j < width; j++)
			buf2[num2 + j] = buf1[num3 + j];
	}
}
/**
	* @brief 	��ָ���Դ�����
	* @param  buf���Դ��ָ��
	* @retval ��
	*/
void ClearCache(unsigned char *buf)
{
	unsigned int data i;
	for(i = 0; i < 1024; i++)
		buf[i] = 0x00;
}
/**
	* @brief 	��ָ���Դ��ָ����������
	* @param  x����ʼ�����꣬y����ʼ�����꣬width������Ŀ�ȣ�height������ĸ߶ȣ�
	*					buf1���Դ��ָ��
	* @retval ��
	*/
void ClearCacheArea(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned char *buf1)
{
	unsigned char data i, j;
	unsigned int data num1, num2;
	num1 = height / 8;
	for(i = 0; i < num1; i++)
	{
		num2 = (i + y) * 128 + x;
		for(j = 0; j < width; j++)
		{
			buf1[num2 + j] = 0x00;
		}
	}
}
/**
	* @brief 	��ָ���Դ�ˢ�µ�OLED��Ļ��
	* @param  cache_buf���Դ��ָ��
	* @retval ��
	*/
void ScreenRefreshAll(unsigned char *cache_buf)
{
	OLED_Refresh(cache_buf);
}
/**
	* @brief  �����Դ�ָ��λ�á�ָ����Ⱥ�ָ�����ȵ�����ˢ�µ�OLED��Ļ�ϣ��ֲ�ˢ�£�
	* @param  x����ʼ�����꣨0~127����y����ʼ�����꣨0~7��
	*					width��ˢ������Ŀ�ȣ�0~127����height��ˢ������ĸ߶ȣ�0~7��
	* @retval ��
	*/
void ScreenRefreshArea(unsigned char x, unsigned char y, unsigned char width, unsigned char height)
{
	unsigned char data i,j;
	unsigned int data n;	
	for(i = y; i < height; i++)  
	{
		OLED_Set_Pos(x, i);
		n = i * 128;
		for(j = 0; j < width; j++)
			OLED_WR_Byte(main_cache[n + j], OLED_DATA); 
	}
}
/**
	* @brief  ʵ�ֵ�ǰ��Ļ���ݻ����뿪������ʾ�����Դ�����ݽ��ӽ���������ʾ�Ķ�̬Ч��
	* @param  cache�����Դ��ָ�루�Դ�Ĵ�С��1024bytes��
	*					direction��������Ļ�ķ���
	*							UP		����
	*							DOWN	����
	*							LEFT	����
	*							RIGHT	����
	* @retval ��
	*/
/**
	* @brief 	�����͹ر���Ļ��ʾ
	* @param  k��ON		����
	*						 OFF	�ر�
	* @retval ��
	*/
void ScreenOnOff(unsigned char k)
{
	if(k)
		OLED_Display_On();
	else
		OLED_Display_Off();
}
/**
	* @brief 	������Ļ������
	* @param  screen_brightness����Ļ������ֵ�İٷֱ�*100
	* @retval ��
	*/
void ScreenSetBrightness(unsigned char screen_brightness)
{
	if(screen_brightness > 100)	//�޷�
		screen_brightness = 100;
	OLED_Set_Brightness(screen_brightness * 255 / 100);
}
/**
	* @brief 	������Ļ�Ƿ�ɫ
	* @param  inverse��
	*						NO_INVERSED	����ɫ���ڵװ���
	*						INVERSED		��ɫ���׵׺���
	* @retval ��
	*/
void ScreenSetInverse(unsigned char k)
{
	OLED_Inverse(k);
}
/**
	* @brief 	��ָ���Դ��ָ��λ������ʾ�ַ��������ܹ��Զ�����
	*					���ﵽ��Ļ�ײ�ʱ���������Ϲ����Լ�����ʾ
	* @param  x���ַ�����ʼλ�õĺ����꣨0~127��
	*					y���ַ�����ʼλ�õ������꣨0~7��
	*					str������ʾ���ַ�����ָ��
	*					cache_buf���Դ��ָ��
	*					front_size��ѡ�����壬	FONT8X16		�������
	*																	FONT6X8			С������
	*					inverse��		�Ƿ�ɫ��	NO_INVERSED	����ɫ
	*														 			INVERSED		��ɫ
	*					do_refresh����ˢ����Ļ��0						��ˢ��
	*																	1						ˢ��
	* @retval ��ǰ�ַ�����ռ������
	*/
unsigned char ShowString(unsigned char x, unsigned char y, unsigned char *str, unsigned char *cache_buf, unsigned char front_size, unsigned char inverse)
{
	unsigned char data temp = 0;
	unsigned char data str_len = 0;
	unsigned char data line;
	unsigned char data n,i,j;
	unsigned int data num1, num2;
	if(inverse == INVERSED)
		temp = 0xff;
	while(str[str_len] != '\0')
	{
		str_len ++;
	}
	if(front_size == FONT8X16)
	{
		for(n = 0; n < str_len; n++)
		{
			line = y + (n / 16) * 2;
			for(i = 0; i < 2; i++)
			{
				num1 = (line + i) * 128 + x + (n % 16) * 8;
				num2 = i * 8;
				for(j = 0; j < 8; j++)
					cache_buf[num1 + j] = temp ^ F8X16[str[n] - ' '][num2 + j];
			}
		}
	}
	else
	{
		for(n = 0; n <str_len; n++)
		{
			line = y + (n / 21);
			num1 = line * 128 + x + (n % 21) * 6;
			for(j = 0; j < 6; j++)
				cache_buf[num1 + j] = temp ^ F6X8[str[n] - ' '][j];
		}
	}
	if(front_size == FONT8X16)
		return (str_len / 16 + 1) * 2;
	else
		return (str_len / 21 + 1);
}
void DrawDot(unsigned char x, unsigned char y)
{
	main_cache[(y / 8) * 128 + x] |= (0x01 << (y % 8));
}
void DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
	float k;
	float b;
	if(x1 != x2)
	{
		k = ((float)y2 - (float)y1) / ((float)x2 - (float)x1);
		b = y1 - k * x1;
		if(k < 1 && k > -1)
		{
			if(x1 <= x2)
			{
				for(x1; x1 <= x2; x1++)
					DrawDot(x1, k * x1 + b);
			}
			else
			{
				for(x2; x2 <= x1; x2++)
					DrawDot(x2, k * x2 + b);
			}
		}
		else
		{
			if(y1 <= y2)
			{
				for(y1; y1 <= y2; y1++)
					DrawDot((y1 - b) / k, y1);
			}
			else
			{
				for(y2; y2 <= y1; y2++)
					DrawDot((y2 - b) / k, y2);
			}
		}
	}
	else
	{
		if(y1 <= y2)
		{
			for(y1; y1 <= y2; y1++)
				DrawDot(x1, y1);
		}
		else
		{
			for(y2; y2 <= y1; y2++)
				DrawDot(x2, y2);
		}
	}
}
/*
void DrawCircle(unsigned char x, unsigned char y, unsigned char radius)
{
	float x0, y0, k, rad;
	for(rad = 0; rad <= 6.28; rad += 0.02)
	{
		k = tan(rad);
		if((rad >= 0) && (rad < 1.57))
			x0 = (float)radius / sqrt(k * k + 1);
		else if(rad < 3.14)
			x0 = - (float)radius / sqrt(k * k + 1);
		else if(rad < 4.71)
			x0 = - (float)radius / sqrt(k * k + 1);
		else
			x0 = (float)radius / sqrt(k * k + 1);
		y0 = x0 * k;
		DrawDot((x + x0), (y + y0));
	}
}
*/
void DrawArm(unsigned char x, unsigned char y, unsigned char radius, int angle)
{
	float k, rad, x0, y0;
	rad = angle * 0.0174;	//�Ƕ�ת����
	k = tan(rad);
	if((rad >= 0) && (rad < 1.57)) //1.57=pi/2
		x0 = (float)radius / sqrt(k * k + 1);
	//else if(rad < 3.14)
	//	x0 = - (float)radius / sqrt(k * k + 1);
	else if(rad < 4.71)
		x0 = - (float)radius / sqrt(k * k + 1);
	else
		x0 = (float)radius / sqrt(k * k + 1);
	y0 = x0 * k;
	DrawLine(x, y, x+x0, y+y0);
}