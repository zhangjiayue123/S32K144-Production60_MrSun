#include <stdint.h>
//#include "hardware.h"
//#include "drvGPIO.h"
#include "drvflash.h"


uint8_t 	FLASH_Erase_OneSector(uint32_t	Addr)  //
{
	LMEM->PCCCR = 0x85000001;    /* Invalidate cache & enable write buffer, cache */ 
	MSCM->OCMDR[0] = 0x00000020; /* Bit 5 = 1: Enable program flash prefetch buffer */ 
	MSCM->OCMDR[1] = 0x00000020; /* Bit 5 = 1: Enable data flash prefetch buffer */ //erase 4KB flash sector (the smallest entity that can be erased) at 0x0004_0000 
	while((FTFC->FSTAT & FTFC_FSTAT_CCIF_MASK) == 0); //wait if operation in progress 
	FTFC->FSTAT = FTFC_FSTAT_FPVIOL_MASK | FTFC_FSTAT_ACCERR_MASK | FTFC_FSTAT_FPVIOL_MASK; //clear flags if set 
	FTFC->FCCOB[3] = 0x09; //Erase Flash Sector command (0x09) 
	FTFC->FCCOB[2] = (uint8_t)(Addr>>16); //Flash address [23:16] 
	FTFC->FCCOB[1] = (uint8_t)(Addr>>8); //Flash address [15:08] 
	FTFC->FCCOB[0] = (uint8_t)(Addr>>0); //Flash address [7:0]   
	FTFC->FSTAT = FTFC_FSTAT_CCIF_MASK; //launch command    
	while((FTFC->FSTAT & FTFC_FSTAT_CCIF_MASK) == 0); //wait for done //program phrase at address 0x0004_0000 
	while((FTFC->FSTAT & FTFC_FSTAT_CCIF_MASK) == 0); //wait if operation in progress 
}


void Flash_Write(uint32_t Addr, uint32_t len, uint8_t *dat) 
{ 
	FTFC->FSTAT = FTFC_FSTAT_FPVIOL_MASK | FTFC_FSTAT_ACCERR_MASK | FTFC_FSTAT_FPVIOL_MASK; 
	FTFC->FCCOB[3] = 0x07; //Program Phrase command (0x07) 
	FTFC->FCCOB[2] = (uint8_t)(Addr>>16); //Flash address [23:16] 
	FTFC->FCCOB[1] = (uint8_t)(Addr>>8); //Flash address [15:08] 
	FTFC->FCCOB[0] = (uint8_t)(Addr>>0); //Flash address [7:0] 
  
	FTFC->FCCOB[4]  = dat[0]; //data  
	FTFC->FCCOB[5]  = dat[1]; 
	FTFC->FCCOB[6]  = dat[2]; 
	FTFC->FCCOB[7]  = dat[3]; 
	
	FTFC->FCCOB[8]  = dat[4]; 
	FTFC->FCCOB[9]  = dat[5]; 
	FTFC->FCCOB[10] =  dat[6]; 
	FTFC->FCCOB[11] =  dat[7]; 
	FTFC->FSTAT = FTFC_FSTAT_CCIF_MASK; //launch command    
	while((FTFC->FSTAT & FTFC_FSTAT_CCIF_MASK) == 0); //wait for done 
}



// 一次写入4096字节
void Flash_Write_OneSector(uint32_t Addr, uint8_t *dat )
{
	for(int i=0;i<4096/8;i++)
	{
		Flash_Write(Addr,8,dat);
		Addr = Addr+8;
		dat = dat+8;
	}
}


// 一次写入4096字节
void Flash_Write_1024B(uint32_t Addr, uint8_t *dat )
{
	for(int i=0;i<1024/8;i++)
	{
		Flash_Write(Addr,8,dat);
		Addr = Addr+8;
		dat = dat+8;
	}
}

uint8_t		buf_updata[Flash_Sector_Bytes];

void 	FLASH_Update(int16_t Sectors)
{
			static uint8_t Erase_Flag1 =0;
			
			uint8_t		b;
			uint8_t		*p;
			long			i,j;	
			p = (uint8_t *)(Flash_Update_Addr);
			if(Erase_Flag1 ==0)
			{
					for(i=0; i<(Sectors/4+1); i++)    // 最小擦出扇区为4096字节
					{
						FLASH_Erase_OneSector(0 + i*4096);  //擦除需要数据大小的扇区
					}
					Erase_Flag1 =1;
			}				
			for(i=0;i<Sectors;i++)
			{
				for(j=0;j<Flash_Sector_Bytes;j++)		buf_updata[j] = *p++;
				Flash_Write_1024B(  i * Flash_Sector_Bytes, buf_updata);	
			}
			while(1);   // 升级完成等待看门狗复位
}


uint32_t Flash_Read(uint32_t Addr)
{
	uint32_t *p;
	p = (uint32_t *)Addr;
	return *p;
}



void SOSC_init_8MHz(void) 
{ 
	SCG->SOSCDIV=0x00000101; /* SOSCDIV1 & SOSCDIV2 =1: divide by 1 */ 
	SCG->SOSCCFG=0x00000024; 
	/* Range=2: Medium freq (SOSC between 1MHz-8MHz)*/ 
	/* HGO=0: Config xtal osc for low power */ 
	/* EREFS=1: Input is external XTAL */ 
	while(SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK); /* Ensure SOSCCSR unlocked */ 
	SCG->SOSCCSR=0x00000001; 
	/* LK=0: SOSCCSR can be written */
	/* SOSCCMRE=0: OSC CLK monitor IRQ if enabled */ 
	/* SOSCCM=0: OSC CLK monitor disabled */ 
	/* SOSCERCLKEN=0: Sys OSC 3V ERCLK output clk disabled */ 
	/* SOSCLPEN=0: Sys OSC disabled in VLP modes */ 
	/* SOSCSTEN=0: Sys OSC disabled in Stop modes */ 
	/* SOSCEN=1: Enable oscillator */ 
	while(!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK)); /* Wait for sys OSC clk valid */ 
} 

void SPLL_init_160MHz(void) 
{ 
	while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */ 
	SCG->SPLLCSR = 0x00000000; /* SPLLEN=0: SPLL is disabled (default) */ 
	SCG->SPLLDIV = 0x00000302; /* SPLLDIV1 divide by 2; SPLLDIV2 divide by 4 */ 
	SCG->SPLLCFG = 0x00180000; /* PREDIV=0: Divide SOSC_CLK by 0+1=1 */
	/* MULT=24: Multiply sys pll by 4+24=40 */ 
	/* SPLL_CLK = 8MHz / 1 * 40 / 2 = 160 MHz */ 
	while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */ 
	SCG->SPLLCSR = 0x00000001; 
	/* LK=0: SPLLCSR can be written */ 
	/* SPLLCMRE=0: SPLL CLK monitor IRQ if enabled */ 
	/* SPLLCM=0: SPLL CLK monitor disabled */ 
	/* SPLLSTEN=0: SPLL disabled in Stop modes */ 
	/* SPLLEN=1: Enable SPLL */ 
	while(!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK)); /* Wait for SPLL valid */ 
} 

void NormalRUNmode_80MHz (void) 
{ /* Change to normal RUN mode with 8MHz SOSC, 80 MHz PLL*/ 
	SCG->RCCR=SCG_RCCR_SCS(6) | SCG_RCCR_DIVCORE(1) | SCG_RCCR_DIVBUS(1) | SCG_RCCR_DIVSLOW(2); 
	while (((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT ) != 6) 
	{} /* Wait for sys clk src = SPLL */ 
} 

void Clock_Config(void) 
{ 
	SOSC_init_8MHz(); /* Initialize system oscillator for 8 MHz xtal */ 
	SPLL_init_160MHz(); /* Initialize sysclk to 160 MHz with 8 MHz SOSC */ 
	NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */ 
} 



