//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//"a CXC's Project"
//  �� �� ��   : main.c
//  �� �� ��   : v1.2
//  ��    ��   : CXC
//  ��������   : 2020-07-06
//  ����޸�   : 2020-12-25
//  ��������   : iWatch�̼�����
//              ˵��: ������ֻ������iWatch-v1.2Ӳ��
//              Ӳ�������뿴ԭ��ͼ
//��Ȩ���У�����ؾ���
//"a CXC's Project" 2020-07-06
//////////////////////////////////////////////////////////////////////////////////	
#include "iWatch.h"

void main()
{
	iWatchInit();
	while(1)
	{
		FeedWatchDog();				//ι���Ź�
		PageRun();						//ҳ������
		KeyHandle();					//��������
		iWatchStatusHandle();	//״̬����
		iWatchAutoWakeHandle();//�Զ����Ѵ���
	}
}