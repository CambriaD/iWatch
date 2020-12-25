#include "stc8a.h"
#include "EEPROM.h"
#include "intrins.h"

//========================================================================
// ����: void	DisableEEPROM(void)
// ����: ��ֹ����ISP/IAP.
// ����: non.
// ����: non.
// �汾: V1.0, 2012-10-22
//========================================================================
void	DisableEEPROM(void)
{
	IAP_CONTR = 0;			//��ֹISP/IAP����
	IAP_CMD   = 0;			//ȥ��ISP/IAP����
	IAP_TRIG  = 0;			//��ֹISP/IAP�����󴥷�
	IAP_ADDRH = 0xff;		//��0��ַ���ֽ�
	IAP_ADDRL = 0xff;		//��0��ַ���ֽڣ�ָ���EEPROM������ֹ�����
}
//========================================================================
// ����: void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number)
// ����: ��ָ��EEPROM�׵�ַ����n���ֽڷ�ָ���Ļ���.
// ����: EE_address:  ����EEPROM���׵�ַ.
//       DataAddress: �������ݷŻ�����׵�ַ.
//       number:      �������ֽڳ���.
// ����: non.
// �汾: V1.0, 2012-10-22
//========================================================================
void EEPROM_read_n(unsigned int EE_address, unsigned char *DataAddress, unsigned int number)
{
	F0 = EA;
	EA = 0;		//��ֹ�ж�
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD   = CMD_READ;
	do
	{
		IAP_ADDRH = EE_address / 256;		//�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
		IAP_ADDRL = EE_address % 256;		//�͵�ַ���ֽ�
		IAP_TRIG  = 0x5a;	IAP_TRIG  = 0xa5;
		_nop_();
		*DataAddress = IAP_DATA;			//��������������
		EE_address++;
		DataAddress++;
	}while(--number);

	DisableEEPROM();
	EA = F0;		//���������ж�
}
/******************** ������������ *****************/
//========================================================================
// ����: void EEPROM_SectorErase(u16 EE_address)
// ����: ��ָ����ַ��EEPROM��������.
// ����: EE_address:  Ҫ����������EEPROM�ĵ�ַ.
// ����: non.
// �汾: V1.0, 2013-5-10
//========================================================================
void EEPROM_SectorErase(unsigned int EE_address)
{
	F0 = EA;
	EA = 0;		//��ֹ�ж�
	IAP_ADDRH = EE_address / 256;			//��������ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
	IAP_ADDRL = EE_address % 256;			//��������ַ���ֽ�
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD   = CMD_ERASE;
	IAP_TRIG  = 0x5a;	IAP_TRIG  = 0xa5;
	_nop_();
	DisableEEPROM();
	EA = F0;		//���������ж�
}

//========================================================================
// ����: void EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number)
// ����: �ѻ����n���ֽ�д��ָ���׵�ַ��EEPROM.
// ����: EE_address:  д��EEPROM���׵�ַ.
//       DataAddress: д��Դ���ݵĻ�����׵�ַ.
//       number:      д����ֽڳ���.
// ����: non.
// �汾: V1.0, 2012-10-22
//========================================================================
void EEPROM_write_n(unsigned int EE_address,unsigned char *DataAddress,unsigned int number)
{
	F0 = EA;
	EA = 0;		//��ֹ�ж�
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD   = CMD_PROGRAM;
	do
	{
		IAP_ADDRH = EE_address / 256;		//�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
		IAP_ADDRL = EE_address % 256;		//�͵�ַ���ֽ�
		IAP_DATA  = *DataAddress;			//�����ݵ�IAP_DATA��ֻ�����ݸı�ʱ����������
		IAP_TRIG  = 0x5a;	IAP_TRIG  = 0xa5;
		_nop_();
		EE_address++;
		DataAddress++;
	}while(--number);

	DisableEEPROM();
	EA = F0;		//���������ж�
}
