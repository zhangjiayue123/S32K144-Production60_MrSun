#include "S32K144.h"

#define	Flash_Sector_Bytes			1024
#define	Flash_Config_Addr				0x0001F000
#define	Flash_Update_Addr				0x00040000



uint8_t 	FLASH_Erase_OneSector(uint32_t	Addr);
void 			Flash_Write_OneSector(uint32_t Addr, uint8_t *dat );
void 			Flash_Write_1024B(uint32_t Addr, uint8_t *dat );
void 			FLASH_Update(int16_t Sectors);
void   		Clock_Config();
void 			Flash_Write(uint32_t Addr, uint32_t len, uint8_t *dat);
