#include <stdint.h>
#include "S32K144.h"
#include "drvCAN.h"
#include "drvGPIO.h"

/**********************************  CAN    ***************************************/
//      模块通道    端口          可选范围              建议
#define CAN0_TX    PTE5        // PTE5、PTC3、
#define CAN0_RX    PTE4        // PTE4、PTC2、

#define CAN1_TX    PTA13       // PTA13、PTC7、
#define CAN1_RX    PTA12       // PTA12、PTC6、

#define CAN2_TX    PTB13       // PTB13、PTC17、
#define CAN2_RX    PTB12       // PTB12、PTC16、

/*************************************************************************
*  函数名称：CANInit
*  参数说明：CANChannel：模块号 0,1,2
//		       baudrateKHz: 波特率, 250,500,1000
*  函数返回：0：成功；1：失败
*************************************************************************/
uint8_t CANInit(uint8_t CANChannel,uint32_t baudrateKHz)
{
    uint8_t i;
    CAN_MemMapPtr CANBaseAdd;
		MailBox				*pMBox;
    
    //通过模块号选择模块基地址,使能FlexCAN外部时钟
    if(CANChannel == 0)
    {
        CANBaseAdd = CAN0;
        PCC-> PCCn[PCC_FlexCAN0_INDEX] = PCC_PCCn_CGC_MASK;   //BUSCLK
    }
    else if(CANChannel == 1)
    {
        CANBaseAdd = CAN1;
        PCC-> PCCn[PCC_FlexCAN1_INDEX] = PCC_PCCn_CGC_MASK;   //BUSCLK
    }
    else if(CANChannel == 2)
    {
        CANBaseAdd = CAN2;
        PCC-> PCCn[PCC_FlexCAN2_INDEX] = PCC_PCCn_CGC_MASK;   //BUSCLK
    }
  
    //配置PIN脚
    if(CANChannel == 0)
    {
      if(CAN0_TX == PTE5 )
      {
        PORTE->PCR[5] |= 0X503 ;
        PORTE->PCR[4] |= 0X503 ;
      }
      else
      {
        PORTC->PCR[3] |= 0X303 ;
        PORTC->PCR[2] |= 0X303 ;        
      }
    }
		
    else if(CANChannel == 1)
    {
      if(CAN1_TX == PTA13 )
      {
        PORTA->PCR[13] |= 0X303 ;
        PORTA->PCR[12] |= 0X303 ;
      }
      else
      {
        PORTC->PCR[7] |= 0X303 ;
        PORTC->PCR[6] |= 0X303 ;        
      }
    }
		
    else if(CANChannel == 2)
    {
      
      if(CAN2_TX == PTC17 )
      {
        PORTC->PCR[17] |= 0X303 ;
        PORTC->PCR[16] |= 0X303 ;
      }
      else
      {
        PORTB->PCR[13] |= 0X403 ;
        PORTB->PCR[12] |= 0X403 ;        
      }
    }
		
    // The CAN engine clock source is XTAL  8MHz
    CANBaseAdd->MCR |= CAN_MCR_MDIS_MASK;
    CANBaseAdd->CTRL1 &= ~CAN_CTRL1_CLKSRC_MASK;
    CANBaseAdd->MCR &= ~CAN_MCR_MDIS_MASK;
    
    // Soft Reset
    CANBaseAdd->MCR ^= CAN_MCR_SOFTRST_MASK; 
    while(CAN_MCR_SOFTRST_MASK & CANBaseAdd->MCR);
    
    // Enter Fraze Mode
    CANBaseAdd->MCR |= CAN_MCR_FRZ_MASK ;
    CANBaseAdd->MCR |= CAN_MCR_HALT_MASK ;
    while(!(CAN_MCR_FRZACK_MASK & CANBaseAdd->MCR));
		

		// 设置波特率
    if(baudrateKHz>=1000)
    {
					// PE clock frequency = XTAL 8M
					// PRESDIV = 0, Sclock frequency = 8M / (0+1) = 8M
					// RJW = 0,			1Tq
					// ----------------
					// Sync			Fix	1Tq
				  // PROPSEG = 3,	4Tq
					// PSEG1 = 0,		1Tq
					// PSEG2 = 1,		2Tq
					// ----------------
					CANBaseAdd->CTRL1 = (
																CAN_CTRL1_PRESDIV(0)
 												      | CAN_CTRL1_RJW(0)
															| CAN_CTRL1_PROPSEG(3) 
												      | CAN_CTRL1_PSEG1(0)
												      | CAN_CTRL1_PSEG2(1));
		 }
		 else if(baudrateKHz>=500)
     {
					// PE clock frequency = XTAL 8M
					// PRESDIV = 1, Sclock frequency = 8M / (1+1) = 4M
					// RJW = 0,			1Tq
					// ----------------
					// Sync			Fix	1Tq
				  // PROPSEG = 3,	4Tq
					// PSEG1 = 0,		1Tq
					// PSEG2 = 1,		2Tq
					// ----------------
					CANBaseAdd->CTRL1 = (
																CAN_CTRL1_PRESDIV(1)
 												      | CAN_CTRL1_RJW(0)
															| CAN_CTRL1_PROPSEG(3) 
												      | CAN_CTRL1_PSEG1(0)
												      | CAN_CTRL1_PSEG2(1));
		 }
		 else
     {
					// PE clock frequency = XTAL 8M
					// PRESDIV = 3, Sclock frequency = 8M / (3+1) = 2M
					// RJW = 0,			1Tq
					// ----------------
					// Sync			Fix	1Tq
				  // PROPSEG = 3,	4Tq
					// PSEG1 = 0,		1Tq
					// PSEG2 = 1,		2Tq
					// ----------------
					CANBaseAdd->CTRL1 = (
																CAN_CTRL1_PRESDIV(3)
 												      | CAN_CTRL1_RJW(0)
															| CAN_CTRL1_PROPSEG(3) 
												      | CAN_CTRL1_PSEG1(0)
												      | CAN_CTRL1_PSEG2(1));
		 }
		

    //初始化掩码寄存器
   	CANBaseAdd->RXMGMASK = 0;
 	  CANBaseAdd->RXFGMASK = 0;
		for(i=0;i<16;i++)  CANBaseAdd->RXIMR[i] = 0;
	
	
		// 设置发送接受MailBox 
    CANBaseAdd->MCR |= CAN_MCR_SRXDIS_MASK;
    CANBaseAdd->MCR |= 15; 									// 16个MailBox
    CANBaseAdd->MCR |= CAN_MCR_RFEN_MASK; 	// Rx FIFO 	 
		CANBaseAdd->CTRL2 = 0x00A00000;
	  pMBox	= (MailBox *)(CANBaseAdd->RAMn);
		//	Rx Filter
		for(i=0;i<8;i++)
		{
				pMBox->RxFilter[i].RXIDA	= 0;
				pMBox->RxFilter[i].IDE 		= i&1;
				pMBox->RxFilter[i].RTR 		= 0;
		}
    // Tx MailBox
		for(i=0;i<8;i++)
		{
				pMBox ->TxMBox[i].CODE = FLEXCAN_MB_CODE_TX_INACTIVE;
		}

		
    // Exit Fraze Mode
	  CANBaseAdd->MCR &= ~(CAN_MCR_FRZ_MASK);
	  while( CANBaseAdd->MCR & CAN_MCR_FRZACK_MASK);    
	  while( CANBaseAdd->MCR & CAN_MCR_NOTRDY_MASK);
		
    return 0;
}


/*************************************************************************
*  函数名称：CANSendData
*  功能说明：发送数据
*  参数说明：CANChannel：模块号 
//	         id: ID号
//           length：数据长度
//	         Data[8]:发送数据缓冲区
*  函数返回：0：成功，1：失败
*************************************************************************/
uint8_t CANSendData(uint8_t CANChannel, uint32_t id_ext, uint32_t id, uint8_t length,uint8_t Data[])
{
    CAN_MemMapPtr CANBaseAdd;
		MailBox		*pMBox;
    uint16_t  i;  
    

    if(CANChannel == 0)
        CANBaseAdd = CAN0;  
    else if(CANChannel == 1)
        CANBaseAdd = CAN1;
    else if(CANChannel == 2)
        CANBaseAdd = CAN2;
 
	  pMBox	= (MailBox *)(CANBaseAdd->RAMn);
		
		for(i=0;i<8;i++)
		{
				
			  if(pMBox->TxMBox[i].CODE == FLEXCAN_MB_CODE_TX_INACTIVE) break;
			  if(pMBox->TxMBox[i].CODE == FLEXCAN_MB_CODE_TX_ABORT) break;
		}
		if(i==8)	return 1;
		
		if(id_ext) 	pMBox->TxMBox[i].ID = id;
		else				pMBox->TxMBox[i].ID = id<<18;
		pMBox->TxMBox[i].DLC = length;
		
		pMBox->TxMBox[i].Data[3] = Data[0];
		pMBox->TxMBox[i].Data[2] = Data[1];
		pMBox->TxMBox[i].Data[1] = Data[2];
		pMBox->TxMBox[i].Data[0] = Data[3];
		pMBox->TxMBox[i].Data[7] = Data[4];
		pMBox->TxMBox[i].Data[6] = Data[5];
		pMBox->TxMBox[i].Data[5] = Data[6];
		pMBox->TxMBox[i].Data[4] = Data[7];
		
		pMBox->TxMBox[i].IDE  = id_ext;
		pMBox->TxMBox[i].RTR  = 0;
		pMBox->TxMBox[i].CODE = FLEXCAN_MB_CODE_TX_ONCE;
		
    return 0;
}

/*************************************************************************
*  函数名称：接收数据
*  功能说明：使能接收缓冲区
*  参数说明：CANChannel：CAN模块号
*  函数返回：0：成功，1：失败
//	         id: ID号
//	         lenght：数据长度
//	         Data: 接收数据缓冲区
*************************************************************************/
uint8_t CANRecData(uint8_t CANChannel, uint32_t *id,uint8_t *Datalenght,uint8_t *Data)
{
    CAN_MemMapPtr CANBaseAdd;
		MailBox		*pMBox;
    

     if(CANChannel == 0)
        CANBaseAdd = CAN0;  
    else if(CANChannel == 1)
        CANBaseAdd = CAN1;
    else if(CANChannel == 2)
        CANBaseAdd = CAN2;
 
	  pMBox	= (MailBox *)(CANBaseAdd->RAMn);

		if((CANBaseAdd->IFLAG1 & CAN_IFLAG1_BUF5I_MASK)==0)	return 1;

		*id = pMBox->RxMBox[0].ID;
    if(pMBox->RxMBox[0].IDE==0) *id >>= 18;

		
		*Datalenght = pMBox->RxMBox[0].DLC;
		
    Data[0] = pMBox->RxMBox[0].Data[3];
    Data[1] = pMBox->RxMBox[0].Data[2];
    Data[2] = pMBox->RxMBox[0].Data[1];
    Data[3] = pMBox->RxMBox[0].Data[0];
    Data[4] = pMBox->RxMBox[0].Data[7];
    Data[5] = pMBox->RxMBox[0].Data[6];
    Data[6] = pMBox->RxMBox[0].Data[5];
    Data[7] = pMBox->RxMBox[0].Data[4];

		CANBaseAdd->IFLAG1 |= CAN_IFLAG1_BUF5I_MASK;
		return 0;
}

