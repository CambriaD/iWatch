#include "LIS3MDL.h"
#include "Sys.h"
#include "math.h"

/*****************************У׼����************************************/											
int mag_x_offset = 0, mag_y_offset = 0, mag_z_offset = 0;		//��λУ׼����
float mag_y_scale = 1, mag_z_scale = 1;											//����У׼����
/*************************************************************************/

/**
  * @brief  ʹ��IIC������LIS3MDL�ļĴ�����дһ�ֽ�����
  * @param  addr: �Ĵ����ĵ�ַ
  * @param  dat: 	��д�������
  * @retval ��
  */
void LIS3MDL_Write_Byte(unsigned char addr, unsigned char dat)
{
	Single_WriteIIC(LIS3MDL_IIC_ADDR, addr, dat);
}
/**
  * @brief  ʹ��IIC���ߴ�LIS3MDL�ļĴ����ж�һ�ֽ�����
  * @param  addr: �Ĵ����ĵ�ַ
  * @retval ������һ�ֽ�����
  */
unsigned char LIS3MDL_Read_Byte(unsigned char addr)
{
	unsigned char temp;
	temp = Single_ReadIIC(LIS3MDL_IIC_ADDR, addr);
	return temp;
}
/**
  * @brief  �����LIS3MDL�������Ƿ�����
  * @param  ��
  * @retval 1���ɹ�����0��ʧ�ܣ�
  */
unsigned char CheckLIS3MDLConnection(void)
{
	if(LIS3MDL_Read_Byte(0x0f) != 0x3d)
		return 0;
	else
		return 1;
}
/**
	* @brief 	��LIS3MDL�ж�ȡ��������ԭʼ����
	* @param  buf��ԭʼ���������ָ�룬int*��
	* @retval ��
	*/
void Read_LIS3MDL(int *buf)
{
	unsigned char tp[6];
	tp[0] = LIS3MDL_Read_Byte(0x29);
	tp[1] = LIS3MDL_Read_Byte(0x28);
	tp[2] = LIS3MDL_Read_Byte(0x2b);
	tp[3] = LIS3MDL_Read_Byte(0x2a);
	tp[4] = LIS3MDL_Read_Byte(0x2d);
	tp[5] = LIS3MDL_Read_Byte(0x2c);
	//I2C_Read_MultiBytes(LIS3MDL_IIC_ADDR, 0x03, 6, tp);
	buf[0] = tp[2] << 8 | tp[3]; //Combine MSB and LSB of Y Data output register  �����Чλ
  buf[1] = -(tp[0] << 8 | tp[1]); //Combine MSB and LSB of X Data output register
  buf[2] = tp[4] << 8 | tp[5]; //Combine MSB and LSB of Z Data output register
	buf[0] -= mag_x_offset;
	buf[1] -= mag_y_offset;
	buf[2] -= mag_z_offset;
	buf[1] *= mag_y_scale;
	buf[2] *= mag_z_scale;
}
/**
	* @brief 	����LIS3MDLԭʼ���ݵ�У׼����
	* @param  x_offset, x_offset, x_offset����Ӧ���ƫ��
	*					y_scale��y���x��ı���У׼
	*					z_scale��z���x��ı���У׼
	* @retval ��
	*/
void LIS3MDL_Set_Calibration_Value(int x_offset, int y_offset, int z_offset, float y_scale, float z_scale)
{
	mag_x_offset = x_offset;
	mag_y_offset = y_offset;
	mag_z_offset = z_offset;
	mag_y_scale = y_scale;
	mag_z_scale = z_scale;
}
/**
	* @brief 	����LIS3MDL�Ĺ���ģʽ
	* @param  mode��0	��������ģʽ
	*								1	���β���ģʽ
	*								2	����ģʽ
	* @retval ��
	*/
void LIS3MDL_Set_Mode(unsigned char mode)
{
	unsigned char CTRL_REG3 = 0x00;
	CTRL_REG3 = LIS3MDL_Read_Byte(0x22);
	CTRL_REG3 &= (~0x03);
	CTRL_REG3 |= mode;
	LIS3MDL_Write_Byte(0x22, CTRL_REG3);
}
/**
	* @brief 	LIS3MDL��ʼ������
	* @param  ��
	* @retval 0��ʧ��
	*					1���ɹ�
	*/
unsigned char LIS3MDL_Init(void)
{
	unsigned char time_out = 200;
	while(CheckLIS3MDLConnection() == 0)
	{
		Delay1ms(1);
		if(--time_out == 0)
			return 0;
	}
	LIS3MDL_Write_Byte(0x20, 0x7c);//CTRL_REG1
	LIS3MDL_Write_Byte(0x21, 0x00);//CTRL_REG2
	LIS3MDL_Write_Byte(0x22, 0x02);//CTRL_REG3
	LIS3MDL_Write_Byte(0x23, 0x0c);//CTRL_REG4
	LIS3MDL_Write_Byte(0x24, 0x00);//CTRL_REG5
	LIS3MDL_Write_Byte(0x30, 0x00);//INT_CFG
	//HMC5883L_Set_Calibration_Value(-83, 10, -74, 0.99041592, 1.16104081);
	return 1;
}
/**
	* @brief 	����x���y�����ǲ�����ƫ��
	* @param  mag_data�������������
	*					ax��ay��x���y������
	* @retval Yaw��������Ĵ�ƫ��
	*/
float LIS3MDL_Get_AngleXY(int *mag_data, float ax, float ay)
{
	float Yaw, RadX, RadY, Xh, Yh;
	RadX = -ax / 57.2957795f;
	RadY = -ay / 57.2957795f;
	Xh = mag_data[0] * cos(RadX) + mag_data[1] * sin(RadY) * sin(RadX) - mag_data[2] * cos(RadY) * sin(RadX);
	Yh = mag_data[1] * cos(RadY) + mag_data[2] * sin(RadY);
	Yaw = atan2(Yh, Xh) * 180 / 3.14159 + 180;
	return Yaw;
}