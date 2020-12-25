#ifndef _IWATCH_H
#define _IWATCH_H

#include "Sys.h"
#include "Display.h"
#include "Sensor.h"
#include "GUI_PAGE.h"

#include "PCF8563.h"

#include "intrins.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define	IWATCH_ACTION		0X01
#define	IWATCH_SCREENON	0X02
#define	IWATCH_ACTIVE		0X04
#define	IWATCH_DILE			0X08
#define	IWATCH_GOINGTOSLEEP	0X10
#define	IWATCH_SLEEP		0X20

//������Ϣ�Ľṹ��
typedef struct{
	unsigned char t_inactive_max;				//��󲻻�Ծ��ʱ��
	unsigned char wrist_up_wake;				//̧����
	unsigned char always_on_disp;				//������ʾ
	unsigned char screen_brightness;		//��Ļ����
	unsigned char screen_inverse;				//��Ļ�Ƿ�ɫ
	unsigned char key_sound;						//�Ƿ��а�����
	unsigned char alarm_hour;						//���ӵ�ʱ��
	unsigned char alarm_min;
	unsigned char alarm_day;
	unsigned char alarm_weekday;
	unsigned char alarm_mode;						//���ӵ�ģʽ
	float cal_anglex;										//x�����ƫ��
	float cal_angley;										//y�����ƫ��
	int cal_magnet_x0;									//������У��ϵ��
	int cal_magnet_y0;									//������У��ϵ��
	int cal_magnet_z0;									//������У��ϵ��
	float cal_magnet_ab;								//������У��ϵ��
	float cal_magnet_ac;								//������У��ϵ��
	unsigned char history_step[7][11];	//�Ʋ�������ʷ���ݣ���Ź�ȥ����Ĳ���
	unsigned int check_sum;							//����������Ϣ����ͽ����������֤�����Ƿ�����
}iWatch_config;
#define	CONFIG_SIZE	sizeof(iWatch_config)


void iWatchStatusUpdate(void);
void iWatchStatusHandle(void);
unsigned char iWatchGetStatus(void);
void iWatchKeepActive(void);
void iWatchAlwaysOn(void);
void iWatchAutoWakeHandle(void);
void iWatchSleep(void);
void iWatchPowerDown(void);
float iWatchGetBatteryLife(void);
void iWatchSaveConfig(iWatch_config *config);
unsigned char iWatchReadConfig(iWatch_config *config);
void iWatchInit(void);



#endif