#include "iWatch.h"

extern unsigned char xdata main_cache[];
extern unsigned char xdata sub_cache[];

static PageExecuteRate_TypeDef Rate_50hz = {20, 0};

static unsigned char code Text[] = "PEDOMETER";
static unsigned char code Icon[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x60,0x30,0x10,0x18,0x08,0x0C,0x04,
0x06,0x06,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x06,0x06,
0x04,0x0C,0x08,0x18,0x10,0x30,0x60,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xC0,0x70,0x1C,0x06,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,
0xE0,0xE0,0x70,0x70,0x78,0x38,0x78,0x70,0xF0,0xE0,0xE0,0xEC,0xFE,0x3F,0x3F,0x3F,
0x3E,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x06,0x1C,0x70,0xE0,0x00,0x00,
0xF0,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xB0,0xB0,0xB0,0xB0,0xB0,
0xB0,0x10,0x00,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0x7F,0x3F,0x1F,0x1F,0x3F,0x7E,0x78,
0x70,0x70,0x38,0x38,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xF0,
0x0F,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x0D,0x0D,0x0D,0x0D,
0x0D,0x0D,0x8C,0xC9,0xC3,0xC7,0xC7,0xEF,0xFF,0xFF,0xFC,0xF8,0x70,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0x0F,
0x00,0x00,0x03,0x0E,0x38,0x60,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x01,0x01,0x01,0xE1,0xF9,0x7C,0x3E,0x0E,0x06,0x03,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x60,0x38,0x0E,0x07,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x06,0x0C,0x08,0x18,0x10,0x30,0x20,
0x60,0x60,0x40,0x40,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x40,0x40,0x60,0x60,
0x20,0x30,0x30,0x18,0x08,0x0C,0x06,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
/* (48 X 48 )*/
};

extern pcf8563_time time;	
extern iWatch_config config;	//������Ϣ�ṹ��

static unsigned int step;
static unsigned char buf[8];

/**
  * @brief  ҳ���ʼ���¼�
  * @param  ��
  * @retval ��
  */
static void Setup()
{
	PCF8563ReadTime(&time);
	ClearCache(sub_cache);
	ShowString(0, 0, "Today:    History:", sub_cache, FONT6X8, NO_INVERSED);
	sprintf(buf, "%d ", step);
	ShowString(0, 1, buf, sub_cache, FONT8X16, NO_INVERSED);		
	PageOpenAnim(ANIM_RIGHT);
}
/**
  * @brief  ҳ���˳��¼�
  * @param  ��
  * @retval ��
  */
static void Exit()
{
	PageCloseAnim(ANIM_RIGHT);
}
/**
  * @brief  ҳ��ѭ��ִ�е�����
  * @param  ��
  * @retval ��
  */
static void Loop()
{
	unsigned char n;
	if(PageExecuteRate(&Rate_50hz) == 1)
	{
		step = LSM6DSMGetCurrentStep();
		sprintf(buf, "%d ", step);
		ShowString(0, 1, buf, main_cache, FONT8X16, NO_INVERSED);
		for(n = 0; n < 7; n++)
		{
			ShowString(64, n + 1, config.history_step[n], main_cache, FONT6X8, NO_INVERSED);
		}
		ScreenRefreshAll(main_cache);
	}
}
/**
  * @brief  ҳ���¼�
  * @param  btn:�����¼��İ���
  * @param  event:�¼����
  * @retval ��
  */
static void Event(unsigned char event)
{
	if(event == KEY1)
		PageShift(page_menu);
}

void PageRegister_page_pedometer(unsigned char pageID)
{
	PageRegister(pageID, Text, Icon, Setup, Loop, Exit, Event);
}