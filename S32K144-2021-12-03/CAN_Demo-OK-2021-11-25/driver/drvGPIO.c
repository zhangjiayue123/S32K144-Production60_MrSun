#include <stdint.h>
#include "S32K144.h"
#include "drvGPIO.h"

#define SYSTEMCLOCK    SPLL100
#define BUSCLOCKMAX    40    
#define FLASHCLOCKMAX  20  
#define OSCCLK         8  

uint32_t SystemCoreClock ;
uint32_t BUSClock ;
uint32_t FALSHClock ;

typedef struct
{
    uint8_t   pllPRDIV;            //外部晶振除数因子
    uint8_t   pllVDIV;             //外部晶振乘积因子
}MCGPLL ;

typedef struct
{
    uint8_t   coreDIV;             //Core除数因子
    uint8_t   busDIV;              //bus除数因子
    uint8_t   FlashDIV;            //Flash除数因子
}SIMDIV ;

MCGPLL  Gmcgpll ;
SIMDIV  Gsimdiv ;

typedef enum Source_option
{  
   UNDEFINITION0 = 0 ,
   S0USYSOSC = 1 ,
   S0USIRC = 2 ,
   S0UFIRC = 3 ,
   S0UROSC = 4 ,
   UNDEFINITION1 = 5 ,
   S0USPLL = 6 ,
   UNDEFINITION2 = 7 ,  
}Source_option;

/*-------------------------------------------------------------------------*
//函数名: syscloption                                               
//功  能: 根据选择系统时钟选择乘除因子               
//参  数: SYSPLL选择时钟							  	  
//返  回: 无                                                           
//说  明: 计算公式 PLL =   OSCCLK *(pllVDIV+16)  /(pllPRDIV+1)
//-------------------------------------------------------------------------*/
void syscloption(system_option SYSPLL)
{
       uint32_t systemclock ;
       uint8_t  OSCSll ;
       volatile float  SPLLOSDiv ,divdata,divdataMin;   //用最小逼急法进行时钟配置
       uint8_t  temp0 ,temp1;
       uint8_t  coreDIV ;
       uint32_t PLLclock ;
       /**************
       计算pll分频因子,
       **************/     
         #if( OSCCLK == 50)  
           if(SYSPLL <= 360 ){
             Gmcgpll.pllPRDIV = 4 ; 
             OSCSll = OSCCLK/(Gmcgpll.pllPRDIV+1) ;    //进行基准分频
             SPLLOSDiv = (float)SYSPLL/OSCSll ;             //公式商
             //求合适的配比参数
             divdataMin = 10 ;
             for(temp0=1;temp0 < 17;temp0++){
               divdata =   SPLLOSDiv*temp0 ;
               if((16 <= divdata )&&(divdata <= 47))
							 {
                 temp1 = (uint8)divdata ;  //取整数
                 divdata -= temp1 ;
                 if(divdata < divdataMin) 
								 {
                   coreDIV = temp0 ;
                   divdataMin = divdata ;
                 }
               }}
             //计算PLL倍数

             Gmcgpll.pllVDIV = (uint8)(SPLLOSDiv * coreDIV) ;  //计算公倍数
             Gsimdiv.coreDIV =  coreDIV - 1 ;
             PLLclock = OSCSll * Gmcgpll.pllVDIV ;             //计算PLL时钟
             Gmcgpll.pllVDIV -= 16 ;
             if(PLLclock >360) {
                 #pragma warning( pll error)
             }
           }
            else {
              Gmcgpll.pllPRDIV = 4 ;
              Gmcgpll.pllVDIV = 1;
              #pragma warning( pll error)
            }
          #elif( OSCCLK == 8)
             if(SYSPLL <= 360 ){
               Gmcgpll.pllPRDIV = 0 ;      
               OSCSll = OSCCLK/(Gmcgpll.pllPRDIV+1) ;    //进行基准分频
                SPLLOSDiv = (float)SYSPLL/OSCSll ;             //公式商
             //求合适的配比参数
             divdataMin = 10.3 ;
             for(temp0=1;temp0 < 17;temp0++){
               divdata =   SPLLOSDiv*temp0 ;
               if((16 <= divdata )&&(divdata <= 47))
							 {
                 temp1 = (uint8_t)divdata ;  //取整数
                 divdata -= temp1 ;
                 if(divdata < divdataMin) 
                 {
                   coreDIV = temp0 ;
                   divdataMin = divdata ;
                 }
               }}
             //计算PLL倍数

             Gmcgpll.pllVDIV = (uint8_t)(SPLLOSDiv * coreDIV ); //计算公倍数
             Gsimdiv.coreDIV =  coreDIV - 1 ;
             PLLclock = OSCSll * Gmcgpll.pllVDIV ;              //计算PLL时钟
             Gmcgpll.pllVDIV -= 16 ;
             if(PLLclock >360) 
						 {
//							 #pragma warning(PLL error)
             }
              
             }
              else 
							{
                 Gmcgpll.pllPRDIV = 1 ;
                 Gmcgpll.pllVDIV  = 20 ;
//               #pragma warning(PLL error)
              } 
            #endif
           
       /**************
       计算系统时钟
       **************/
       if(Gmcgpll.pllVDIV > 0x1F) Gmcgpll.pllVDIV = 0x1F ;       
        systemclock = OSCCLK *(Gmcgpll.pllVDIV+16)/(Gmcgpll.pllPRDIV+1) ;     
     //  systemclock /= (Gsimdiv.coreDIV);
       Gsimdiv.busDIV = systemclock / BUSCLOCKMAX ;
       Gsimdiv.FlashDIV = systemclock/FLASHCLOCKMAX ;
       if(Gsimdiv.coreDIV > 0xF) Gsimdiv.coreDIV = 0X0F ;
       if(Gsimdiv.busDIV > 0xF) Gsimdiv.busDIV = 0X0F ;    
       if(Gsimdiv.FlashDIV > 0x7) Gsimdiv.FlashDIV = 0X07 ;  
     }

/* ----------------------------------------------------------------------------
   -- SystemCoreClockSet() 设置系统时钟
   ---------------------------------------------------------------------------- */   
void SystemCoreClockSet (void) 
{
     float PLLCLK ;
     uint32_t splld ;
     //计算分频因子
     syscloption(SYSTEMCLOCK);  

     PLLCLK = OSCCLK *(Gmcgpll.pllVDIV+16)/(Gmcgpll.pllPRDIV+1);
     SystemCoreClock =  PLLCLK/(Gsimdiv.coreDIV + 1)*1000 ;
     BUSClock    =  PLLCLK/(Gsimdiv.busDIV + 1) *1000;
     FALSHClock  =  PLLCLK/(Gsimdiv.FlashDIV + 1) *1000;
     PLLCLK = SystemCoreClock/1000 ;
     
     //配置OSC
     SCG->SOSCCFG = SCG_SOSCCFG_HGO(1) + SCG_SOSCCFG_RANGE(3) + SCG_SOSCCFG_EREFS(1) ;
     SCG->SOSCCSR |=  SCG_SOSCCSR_SOSCEN(1) + SCG_SOSCCSR_SOSCCM_MASK;
     SCG->SOSCDIV = SCG_SOSCDIV_SOSCDIV1(1) + SCG_SOSCDIV_SOSCDIV2(1) ;
     //等待晶振稳定
     while(!(SCG->SOSCCSR&SCG_SOSCCSR_SOSCVLD_MASK)){};     
     
     SMC->PMPROT |= SMC_PMPROT_AHSRUN_MASK +SMC_PMPROT_AVLP_MASK;
     SCG->VCCR =  SCG_VCCR_SCS(S0USIRC) + SCG_VCCR_DIVCORE(3) + SCG_VCCR_DIVBUS(3) + SCG_VCCR_DIVSLOW (7) ;
     //先把RUN用内部的FIRC
      SMC->PMCTRL = SMC_PMCTRL_RUNM(2) ;  //设置成正常速度模式
     if((SCG->RCCR&SCG_RCCR_SCS_MASK) !=SCG_RCCR_SCS(S0UFIRC))
     {
       SCG->RCCR =  SCG_RCCR_SCS(S0UFIRC) ;
       while(SCG->CSR != SCG->RCCR){} ;    //等待系统稳定
     }
     SMC->PMCTRL = SMC_PMCTRL_RUNM(0) ;  //设置成正常速度模式
     
     //选择PLL写分频因子
     SCG->SPLLCSR &= ~SCG_SPLLCSR_SPLLEN_MASK  ;  //关闭SPLL
     SCG->SPLLCFG = SCG_SPLLCFG_MULT(Gmcgpll.pllVDIV) + SCG_SPLLCFG_PREDIV(Gmcgpll.pllPRDIV);        
     
     splld = SCG_HCCR_SCS(S0USPLL) + SCG_HCCR_DIVSLOW(Gsimdiv.FlashDIV)  + SCG_HCCR_DIVBUS(Gsimdiv.busDIV) + SCG_HCCR_DIVCORE(Gsimdiv.coreDIV) ;

     if(PLLCLK > 70)
		 {
     //允许高速时钟和低功耗模式
       SCG->HCCR = splld ;
       SMC->PMCTRL = SMC_PMCTRL_RUNM(3) ;  //设置成正常速度模式
       SCG->SPLLCSR |= SCG_SPLLCSR_SPLLEN_MASK  ;  //开启SPLL
       while(SCG->CSR != SCG->HCCR){} ;    //等待系统稳定
     }
     else
		 {
       SCG->RCCR = splld ;
       SMC->PMCTRL = SMC_PMCTRL_RUNM(0) ;  //设置成正常速度模式
       SCG->SPLLCSR |= SCG_SPLLCSR_SPLLEN_MASK  ;  //开启SPLL
       while(SCG->CSR != SCG->RCCR){} ;    //等待系统稳定
     }    
     //设置SIRC
     SCG->SIRCCSR = SCG_SIRCCSR_SIRCEN_MASK + SCG_SIRCCSR_SIRCSTEN_MASK + SCG_SIRCCSR_SIRCLPEN_MASK ;
     SCG->SIRCDIV = SCG_SIRCDIV_SIRCDIV1(1) + SCG_SIRCDIV_SIRCDIV2(1);
     
     //设置FIRC
     SCG->FIRCCSR = SCG_FIRCCSR_FIRCEN_MASK + SCG_FIRCCSR_FIRCREGOFF_MASK ;
     SCG->FIRCDIV = SCG_FIRCDIV_FIRCDIV1(1) + SCG_FIRCDIV_FIRCDIV2(1);
     //设置时钟源的触发
     SCG->SPLLDIV = SCG_SPLLDIV_SPLLDIV1(1) + SCG_SPLLDIV_SPLLDIV2(1); 
}

void SystemClockInit(void)
{
  SCG->SOSCCFG = 0x3C;      /*外部时钟，高增益，高速晶振时钟*/
  SCG->SOSCCSR |= 1<<0;     /* SOSCEN=1 允许SOSC时钟 */
  /*等待时钟有效*/
  while((SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK) == 0);

  SCG->SPLLCSR &= ~(0x1<<0) ;  /* SPLLEN=0: 禁止PLL*/
  SCG->SPLLCFG &= ~(0x7<<8);   /* PREDIV=0: 1分频 */
  SCG->SPLLCFG |=  0x4U<<16;   /* MULT=4: 28倍频  PLL VCO =  8/1*(4+16) = 160M  */
  SCG->SPLLCSR |=  0x1<<0;     /* SPLLEN=1: 允许PLL */
  /* 等待PLL输出有效*/
  while((SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK) == 0);
  
  SCG->RCCR |= SCG_RCCR_DIVCORE(0);  /* DIVCORE=0: 1分频,  CORE/SYS_CLK 频率为 80MHz */
  SCG->RCCR |= SCG_RCCR_DIVBUS(1);   /* DIVBUS=1:  2分频,  BUS_CLK 频率为 40MHz */
  SCG->RCCR |= SCG_RCCR_DIVSLOW(3);  /* DIVSLOW=2: 4分频   FLASH_CLK 频率为  20MHz */
  SCG->RCCR &= 0xFEFFFFFF;           /* Initially set to SIRC so that LSB could be set as '0' */
  SCG->RCCR |= SCG_RCCR_SCS(6);      /* SCS=6: 系统时钟源为System PLL */ 
}

void GPIO_enable_port (void)
{
    PCC-> PCCn[PCC_PORTA_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT A */ 
    PCC-> PCCn[PCC_PORTB_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT B */
    PCC-> PCCn[PCC_PORTC_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT C */
    PCC-> PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT D */
		PCC-> PCCn[PCC_PORTE_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT E */
}

void PINS_GPIO_WritePin(GPIO_Type *base1,PORT_Type *base2, uint16_t pin, uint16_t value)
{
	base2->PCR[pin] = 0x00000100; /* Port D0: MUX = GPIO */
	base1->PDDR |= 1<<pin;        /* Port D0: Data Direction= output */
	if(value==1)
	{
		base1->PSOR |= 1<<pin;
	}
	else
    {
		base1->PCOR |= 1<<pin;
    }
}

uint16_t PINS_GPIO_ReadPin(GPIO_Type *base1,PORT_Type *base2, uint16_t pin)
{
	if( (base1 == PTB) && ( base2 ==PORTB ) && (pin ==5))   // 钥匙开关，输入为外部中断输入，不可重配输入模式
	{
		;
	}
	else
	{
			base2->PCR[pin] = 0x00000100; /* Port D0: MUX = GPIO */
			base1->PDDR &= ~(1<<pin);     /* Port D0: Data Direction= input */
	}	
	return	(base1->PDIR &(1<<pin))>>pin ;	
}

void PINS_GPIO_TogglePin(GPIO_Type *base1,PORT_Type *base2, uint16_t pin)
{
  base2->PCR[pin] = 0x00000100; /* Port D0: MUX = GPIO */
	base1->PDDR |= 1<<pin;        /* Port D0: Data Direction= output */
	base1->PTOR |= 1<<pin;
}
