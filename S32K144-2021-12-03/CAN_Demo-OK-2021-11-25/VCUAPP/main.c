#include "device_registers.h"
#include <stdint.h>
#include "drvGPIO.h"
#include "drvCAN.h"
#include "drvflash.h"


#pragma pack(1)   // 预处理命名，告诉编译器以1字节为单位进行对齐，否则，sizeof的值有可能不对

uint8_t CANTXdata0[8]={0X11,0X22,0X33,0X44,0X55,0X66,0X77,0X88};
uint8_t CANTXdata1[8]={0X11,0X22,0X33,0X44,0X55,0X66,0X77,0X88};
uint8_t CANTXdata2[8]={0X88,0X77,0X66,0X55,0X44,0X33,0X22,0X11};
uint8_t RXData0[8],RXData1[8],RXData2[8];

uint32_t ID1,ID2,ID0;
uint8_t Datalen0,Datalen1,Datalen2;
uint8_t flag0, flag1,flag2;

// 前一个元素表示ADC编号，后一个元素表示ADC通道
uint8_t ADC_CH[6][2]={{1,8},{1,7},{0,3},{1,15},{1,14},{1,9} };

int main(void)
{
	int Cnt =0;
	Clock_Config();
	GPIO_enable_port ();                  //GPIO端口时钟使能

	CANInit(CAN0CH,250) ;									//CAN0通道初始化，250K
 	CANInit(CAN1CH,250) ;                 //CAN1通道初始化，250K
  CANInit(CAN2CH,250) ;                 //CAN2通道初始化，250K		
	
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
			CANSendData(CAN0CH, 1 , ID0, Datalen0, RXData0);   // 回复输入状态 				
    }
		
		
		flag1=CANRecData(CAN1CH,&ID1,&Datalen1,RXData1);
		if(flag1==0)
    {      
			CANSendData(CAN1CH, 1 , ID1, Datalen1, RXData1);   // 回复输入状态 				
    }
		
		flag2=CANRecData(CAN2CH,&ID2,&Datalen2,RXData2);
		if(flag2==0)
    {
			CANSendData(CAN2CH, 1 , ID2, Datalen2, RXData2);   //  回复ADC4,5的数据		 
    }								
	}
}


 




