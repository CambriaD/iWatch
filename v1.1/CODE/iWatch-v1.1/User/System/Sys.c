#include "Sys.h"
#include "IIC.h"
#include "Delay.h"
#include "PowerManage.h"
#include "Buzzer.h"
#include "EEPROM.h"

//��EEPROM��û�����ݻ������ݲ����������������Ĭ������
const struct sys_config default_config = {
	//Ĭ����ʾ����
	10,						//�Զ�Ϣ��ʱ��
	5,						//�Զ�����ʱ��
	50,						//��Ļ���ȣ�0~255��
	NO_INVERSED,	//��Ļ�Ƿ�ɫ
	NORMAL,				//��Ļ����ʾ����
	OFF,					//�رհ�����
	//Ĭ�����Ӳ���
	8,							//Сʱ
	30,							//����
	1,							//1��
	1,							//��һ
	ALARM_DISABLE,	//���ӹر�
	//Ĭ������������
	1,			//����������
	97.1,		//������Ƶ��
	//Ĭ��У������
	0,			//x�����ƫ��
	0,			//y�����ƫ��
	0,			//������У��ϵ��
	0,			//������У��ϵ��
	0,			//������У��ϵ��
	1,			//������У��ϵ��
	1,			//������У��ϵ��
	{{0}, {0}, {0}, {0}, {0}, {0}, {0}},	//�Ʋ�������ʷ���ݣ���Ź�ȥ����Ĳ���
	0				//������������Ҫ�޸�
};

extern struct sys_config config;

//��Ҫ���׸Ķ���������������Ӱ�칦��
void PinInit()
{
	//P0��ΪOLED��Ļ�������ݿ�
	P0 = 0x00;
	P0M0 = 0x00;
	P0M1 = 0x00;
	//P10,P11Ϊ����2����
	//P13,P12Ϊ��ʾ��RES��D/C����
	//P14,P15ΪIIC���ţ��ⲿ�����������裬����������Ϊ��©
	//P16Ϊ����ʹ�����ţ��ߵ�ƽʱ����͹���
	//P17Ϊ���ʹ�����ţ�����ʱʹ�ܳ��
	P13 = 1;
	P16 = 1;
	P1M0 = 0x30;//00110000b
	P1M1 = 0xb0;//10110000b
	//P20ΪLED�������ţ�ǿ����ģʽ
	//P21,P22,P23Ϊ�ⲿ��չ����
	//P24,P25ΪIIC2���ţ��ⲿ���������裬����������Ϊͨ��˫��
	//P26Ϊ�������������ţ�ǿ����ģʽ
	//P27Ϊ����1��Ĭ�ϸߵ�ƽ
	P2 = 0xb1;
	P2M0 = 0x41;
	P2M1 = 0x00;
	//P37Ϊ����2���ⲿ�ж�3��Ĭ�ϸߵ�ƽ
	//P36Ϊ�ⲿ�ж�2
	//P34ΪT0������������
	//P33Ϊ�ⲿ�ж�1
	//P32Ϊ�ⲿ�ж�0
	//P30,P31Ϊ����1����
	P3 = 0x94;
	P3M0 = 0x00;
	P3M1 = 0x00;
	//P44,P43Ϊ��ʾ��E/RD��R/W����
	//P40Ϊ3.3vʹ���������
	P4 = 0x19;
	P4M0 = 0x00;
	P4M1 = 0x00;
	//P55Ϊ����3��Ĭ�ϸߵ�ƽ
	P5 = 0x20;
	P5M0 = 0x00;
	P5M1 = 0x00;
}
void Timer0Init(void)
{
	TMOD |= 0x04;    //�ⲿ����ģʽ
  TL0 = 0x00;
  TH0 = 0x00;
  TR0 = 0;       	//�رն�ʱ��
  ET0 = 1;       	//ʹ�ܶ�ʱ���ж�
}
void Timer3Init(void)		//1����@24.000MHz
{
	T4T3M |= 0x02;		//��ʱ��ʱ��1Tģʽ
	T3L = 0x40;				//���ö�ʱ��ֵ
	T3H = 0xA2;				//���ö�ʱ��ֵ
	T4T3M |= 0x08;		//��ʱ��3��ʼ��ʱ
	IE2 |= ET3;				//��ʱ��3������ж�
}
#ifdef	badapple
extern unsigned char xdata sub_cache2[];
unsigned int rx_buf_num = 0;
bit frame_received_flag = 0;
bit serial_busy = 0;
void UartInit(void)		//576000bps@24.000MHz
{
	SCON = 0x50;		//8???,?????
	AUXR |= 0x40;		//???1???Fosc,?1T
	AUXR &= 0xFE;		//??1?????1???????
	TMOD &= 0x0F;		//?????1?16???????
	TL1 = 0xF6;		//??????
	TH1 = 0xFF;		//??????
	ET1 = 0;		//?????1??
	TR1 = 1;		//?????1
	ES = 1;					//�������ж�
	//TI = 1;
}
void UART1_Isr() interrupt 4 using 1
{
	if(RI)
	{
		RI = 0;      				//���жϱ�־
		if(SBUF == MCU_RESET_CMD)
			MCUSoftReset();		//��λMCU
	}
	if(RI)
	{
		RI = 0;
		sub_cache2[rx_buf_num++] = SBUF;
		if(rx_buf_num == 1024)
		{
			rx_buf_num = 0;
			frame_received_flag = 1;
		}
	}
	if(TI)
	{
		TI = 0;
		serial_busy = 0;
	}
}
void UART1SendString(char *str)
{
	while(*str)
  {
		while(serial_busy);
		serial_busy = 1;
		SBUF = *str++;
  }
}
void StartFrameReceive()
{
	UART1SendString("ok");
	frame_received_flag = 0;
	rx_buf_num = 0;
}
unsigned char CheckFrameReceived()
{
	if(frame_received_flag)
	{
		frame_received_flag = 0;
		return 1;
	}
	else
		return 0;
}
#else
void UartInit(void)		//115200bps@24.000MHz
{
	SCON = 0x50;
	AUXR |= 0x40;
	AUXR &= 0xFE;
	TMOD &= 0x0F;
	TL1 = 0xCC;
	TH1 = 0xFF;
	ET1 = 0;
	TR1 = 1;
	TI = 1;
}
#endif
void Uart2Init(void)		//9600bps@24.000MHz
{
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0x8F;			//�趨��ʱ��ֵ
	T2H = 0xFD;			//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
	IE2 |= ES2; 		//ʹ�ܴ����ж�
}
void InitConfig(void)	//�жϳ�ʼ��
{
	IT0 = 1;       	//ʹ��INT0�½����жϣ���ӦPCF8563���ж��������
	EX0 = 1;       	//ʹ��INT0�ж�
	IT1 = 1;       	//ʹ��INT1�½����жϣ���ӦLSM6DSM���ж��������1
  EX1 = 1;       	//ʹ��INT1�ж�
	INTCLKO |= EX2;	//ʹ��INT2�ж�				��ӦLSM6DSM���ж��������2
	INTCLKO |= EX3; //ʹ��INT3�ж�				��Ӧ����2�ж�
	//INTCLKO |= EX4;	//ʹ��INT4�ж�
}
void MCUSoftReset(void)	//��Ƭ����λ
{
	IAP_CONTR = 0x60;			//��Ƭ����λָ��
}
void EnableWatchDog(void)
{
	WDT_CONTR = 0x27;			//�������4.194sûι����MCU�Զ���λ
}
void FeedWatchDog(void)
{
	WDT_CONTR |= 0x10;		//ι����������Ź�������
}
void EEPROMWriteConfiguration(struct sys_config *config)
{
	unsigned char i = 0;
	unsigned int temp = 0;
	for(i; i < CONFIG_SIZE - 2; i++)				//�Խṹ����ÿһ�ֽ����
		temp += ((unsigned char *)config)[i];
	config->check_sum = temp;
	EEPROM_SectorErase(EE_ADDRESS1);				//����ͽ������ڽṹ�����һ��������
	EEPROM_write_n(EE_ADDRESS1, (unsigned char *)config, CONFIG_SIZE);
}
unsigned char EEPROMReadConfiguration(struct sys_config *config)
{
	unsigned char i = 0;
	unsigned int temp = 0;
	EEPROM_read_n(EE_ADDRESS1, (unsigned char *)config, CONFIG_SIZE);
	for(i; i < CONFIG_SIZE - 2; i++)				//�Խṹ����ÿһ�ֽ����
		temp += ((unsigned char *)config)[i];
	if(temp == config->check_sum)				//���������Ƿ���ȷ�������ṹ��������ֽ�Ϊ��������ֽ�
		return 1;
	else
	{
		for(i = 0; i < CONFIG_SIZE; i++)
			((unsigned char *)config)[i] = ((unsigned char *)(&default_config))[i];
		return 0;
	}
}
void LED(unsigned char k)
{
	LED1 = k^0x01;
}
void SysInit(void)
{
	PinInit();		//���ų�ʼ��
	UartInit();		//����1��ʼ����ʹ�ö�ʱ��1
	Uart2Init();	//����2��ʼ����ʹ�ö�ʱ��3
	Timer0Init();	//��ʱ��0��ʼ�����ⲿ����ģʽ
	Timer3Init();	//��ʱ��3��ʼ��������1ms����ж�
	InitConfig();	//�ⲿ�жϳ�ʼ��
	ADCInit();		//ADC��ʼ�����ɼ���Դ��ѹ
	IIC_Init();		//Ӳ��IIC��ʼ��
	EnableWatchDog();					//ʹ�ܿ��Ź���������4s���Զ���λ
	EEPROMReadConfiguration(&config);//�����籣������
	BuzzerInit();							//��������ʼ����ʹ��Ӳ��PWM7
	EnableBuzzer(config.key_sound);
	LED(OFF);									//�ر�LED
	BatteryChargeEnable(1);		//�����س��
	Delay1ms(5);
	Enable3V3Output(1);				//����3.3v��Դ���
	EA = 1;										//��ȫ���ж�
}