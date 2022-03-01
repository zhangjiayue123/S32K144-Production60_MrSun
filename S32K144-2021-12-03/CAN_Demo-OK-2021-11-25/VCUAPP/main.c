#include "device_registers.h"
#include <stdint.h>
#include "drvGPIO.h"
#include "drvCAN.h"
#include "drvflash.h"


#pragma pack(1)   // Ԥ�������������߱�������1�ֽ�Ϊ��λ���ж��룬����sizeof��ֵ�п��ܲ���

uint8_t CANTXdata0[8]={0X11,0X22,0X33,0X44,0X55,0X66,0X77,0X88};
uint8_t CANTXdata1[8]={0X11,0X22,0X33,0X44,0X55,0X66,0X77,0X88};
uint8_t CANTXdata2[8]={0X88,0X77,0X66,0X55,0X44,0X33,0X22,0X11};
uint8_t RXData0[8],RXData1[8],RXData2[8];

uint32_t ID1,ID2,ID0;
uint8_t Datalen0,Datalen1,Datalen2;
uint8_t flag0, flag1,flag2;

// ǰһ��Ԫ�ر�ʾADC��ţ���һ��Ԫ�ر�ʾADCͨ��
uint8_t ADC_CH[6][2]={{1,8},{1,7},{0,3},{1,15},{1,14},{1,9} };

int main(void)
{
	int Cnt =0;
	Clock_Config();
	GPIO_enable_port ();                  //GPIO�˿�ʱ��ʹ��

	CANInit(CAN0CH,250) ;									//CAN0ͨ����ʼ����250K
 	CANInit(CAN1CH,250) ;                 //CAN1ͨ����ʼ����250K
  CANInit(CAN2CH,250) ;                 //CAN2ͨ����ʼ����250K		
	
	for(;;)
	{    
		Cnt++;
		if( (Cnt & 0x10000) == 0 )
				PINS_GPIO_WritePin(PTB,PORTB,0,0);
		else
				PINS_GPIO_WritePin(PTB,PORTB,0,1);

		flag0=CANRecData(CAN0CH,&ID0,&Datalen0,RXData0);
		if(flag0==0)
    {      
			CANSendData(CAN0CH, 1 , ID0, Datalen0, RXData0);   // �ظ�����״̬ 				
    }
		
		
		flag1=CANRecData(CAN1CH,&ID1,&Datalen1,RXData1);
		if(flag1==0)
    {      
			CANSendData(CAN1CH, 1 , ID1, Datalen1, RXData1);   // �ظ�����״̬ 				
    }
		
		flag2=CANRecData(CAN2CH,&ID2,&Datalen2,RXData2);
		if(flag2==0)
    {
			CANSendData(CAN2CH, 1 , ID2, Datalen2, RXData2);   //  �ظ�ADC4,5������		 
    }								
	}
}


 




