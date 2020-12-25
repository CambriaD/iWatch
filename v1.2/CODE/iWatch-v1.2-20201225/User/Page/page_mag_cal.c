#include "iWatch.h"

extern unsigned char xdata main_cache[];
extern unsigned char xdata sub_cache[];

static PageExecuteRate_TypeDef Rate_50hz = {20, 0};


extern iWatch_config config;		//������Ϣ�ṹ��
extern int magnet_data[3];
static unsigned char str[16];
static unsigned int data_cnt = 0;
struct cal_data magnet_cal_data;		//������У׼���ݽṹ��
/**
  * @brief  ҳ���ʼ���¼�
  * @param  ��
  * @retval ��
  */
static void Setup()
{
	//ClearCache(sub_cache);
	LIS3MDL_Set_Mode(0);	//������������ģʽ
	LIS3MDL_Set_Calibration_Value(0, 0, 0, 1, 1);
}
/**
  * @brief  ҳ���˳��¼�
  * @param  ��
  * @retval ��
  */
static void Exit()
{
	Ellipsoid_fitting_Process(&magnet_cal_data);		//����У׼�㷨
	LIS3MDL_Set_Calibration_Value(magnet_cal_data.X0, magnet_cal_data.Y0, magnet_cal_data.Z0, 
																magnet_cal_data.A / magnet_cal_data.B, 
																magnet_cal_data.A / magnet_cal_data.C);//����У׼����
	config.cal_magnet_x0 = magnet_cal_data.X0;
	config.cal_magnet_y0 = magnet_cal_data.Y0;
	config.cal_magnet_z0 = magnet_cal_data.Z0;
	config.cal_magnet_ab = magnet_cal_data.A / magnet_cal_data.B;
	config.cal_magnet_ac = magnet_cal_data.A / magnet_cal_data.C;
	iWatchSaveConfig(&config);
	data_cnt = 0;
	LIS3MDL_Set_Mode(2);
	//PageCloseAnim(1);
}
/**
  * @brief  ҳ��ѭ��ִ�е�����
  * @param  ��
  * @retval ��
  */
static void Loop()
{
	if(PageExecuteRate(&Rate_50hz) == 1)
	{
		Read_LIS3MDL(magnet_data);
		CalcData_Input(magnet_data[0], magnet_data[1], magnet_data[2]);
		ClearCache(main_cache);
		sprintf(str, "x:%d", magnet_data[0]);
		ShowString(0, 0, str, main_cache, FONT8X16, NO_INVERSED);
		sprintf(str, "y:%d", magnet_data[1]);
		ShowString(0, 2, str, main_cache, FONT8X16, NO_INVERSED);
		sprintf(str, "z:%d", magnet_data[2]);
		ShowString(0, 4, str, main_cache, FONT8X16, NO_INVERSED);
		sprintf(str, "data_cnt:%d", data_cnt);
		ShowString(0, 6, str, main_cache, FONT8X16, NO_INVERSED);
		ScreenRefreshAll(main_cache);
		if(++data_cnt == 5000)
		{
			Bee();
			PageShift(page_compass);
		}
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
	{
		PageShift(page_compass);
	}
}

void PageRegister_page_mag_cal(unsigned char pageID)
{
	PageRegister(pageID, 0, 0, Setup, Loop, Exit, Event);
}