#include "iWatch.h"

extern bit action;
/********************������ر���************************/
unsigned char Trg = 0;			//��������ֵ
unsigned char Cont = 0;			//��������ֵ

unsigned char KeyScan(void)					//����ɨ�躯��
{
	static unsigned int t_key_press = 0;	//�����������µ�ʱ��
	unsigned char dat = 0x00;
	if(K1 == 0)
		dat |= KEY1;
	if(K2 == 0)
		dat |= KEY2;
	if(K3 == 0)
		dat |= KEY3;
	Trg = dat&(dat^Cont);
	Cont = dat;
	if(Trg)
	{
		Bee();
		action = 1;
		if((iWatchGetStatus()&IWATCH_ACTIVE) == 0)
			Trg = 0;
	}
	if(Cont)
	{
		if(++t_key_press > (T_KEYLONGPRESS/T_KEYSCAN))	//�ж��Ƿ񳤰�
		{
			Trg = Cont;																		//�ظ�����
		}
	}
	else
		t_key_press = 0;
	return Trg;
}
void KeyHandle(void)
{
	if(Trg)
	{
		PageEventTransmit(Trg);
		Trg = 0;
	}
}
void INT2_Isr() interrupt 10 using 1 //˫���ж�
{
	action = 1;
	Bee();
	if(iWatchGetStatus()&IWATCH_ACTIVE)
	{
		Trg = DOUBLE_TAP;
	}
}
void INT1_Isr() interrupt 2 using 1	//̧�����ж�
{
	action = 1;
	if(iWatchGetStatus()&IWATCH_ACTIVE)
	{
		Trg = AWT;
	}
}
void INT3_Isr() interrupt 11					//����2�ж�
{
	action = 1;
}