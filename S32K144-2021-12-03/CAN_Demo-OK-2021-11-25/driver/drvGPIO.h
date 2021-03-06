#ifndef __DRV_GPIO_H
#define __DRV_GPIO_H

typedef enum system_option
{
    SPLL50    = 50,
    SPLL60    = 60,
    SPLL70    = 70,
    SPLL80    = 80,
    SPLL90    = 90,
    SPLL100   = 100,
    SPLL110   = 110,
    SPLL128   = 128,
    SPLL136   = 136,
    SPLL144   = 144,
    SPLL152   = 152,
    SPLL160   = 160,
    SPLL168   = 168,
    SPLL176   = 176,
    SPLL184   = 184,
    SPLL192   = 192,
    SPLL200   = 200, 
    SPLL208   = 208,
    SPLL216   = 216,
    SPLL224   = 224,
    SPLL232   = 232,
    SPLL240   = 240,
    SPLL248   = 248,
    SPLL256   = 256,
    SPLL264   = 264,
    SPLL272   = 272,
    SPLL280   = 280,
    SPLL296   = 296,
    SPLL304   = 304,
} system_option;

typedef enum PTxn
{
    /*  PTA?˿?    */ //0~31
    PTA0,
    PTA1,
    PTA2,
    PTA3,
    PTA4,
    PTA5,
    PTA6,
    PTA7,
    PTA8,
    PTA9,
    PTA10,
    PTA11,
    PTA12,
    PTA13,
    PTA14,
    PTA15,
    PTA16,
    PTA17,
    PTA18,
    PTA19,
    PTA20,
    PTA21,
    PTA22,
    PTA23,
    PTA24,
    PTA25,
    PTA26,
    PTA27,
    PTA28,
    PTA29,
    PTA30,
    PTA31,

    /*  PTB?˿?    */   //32~63
    PTB0,
    PTB1,
    PTB2,
    PTB3,
    PTB4,
    PTB5,
    PTB6,
    PTB7,
    PTB8,
    PTB9,
    PTB10,
    PTB11,
    PTB12,
    PTB13,
    PTB14,
    PTB15,
    PTB16,
    PTB17,
    PTB18,
    PTB19,
    PTB20,
    PTB21,
    PTB22,
    PTB23,
    PTB24,
    PTB25,
    PTB26,
    PTB27,
    PTB28,
    PTB29,
    PTB30,
    PTB31,

    /*  PTC?˿?    */
    PTC0,
    PTC1,
    PTC2,
    PTC3,
    PTC4,
    PTC5,
    PTC6,
    PTC7,
    PTC8,
    PTC9,
    PTC10,
    PTC11,
    PTC12,
    PTC13,
    PTC14,
    PTC15,
    PTC16,
    PTC17,
    PTC18,
    PTC19,
    PTC20,
    PTC21,
    PTC22,
    PTC23,
    PTC24,
    PTC25,
    PTC26,
    PTC27,
    PTC28,
    PTC29,
    PTC30,
    PTC31,

    /*  PTD?˿?    */
    PTD0,
    PTD1,
    PTD2,
    PTD3,
    PTD4,
    PTD5,
    PTD6,
    PTD7,
    PTD8,
    PTD9,
    PTD10,
    PTD11,
    PTD12,
    PTD13,
    PTD14,
    PTD15,
    PTD16,
    PTD17,
    PTD18,
    PTD19,
    PTD20,
    PTD21,
    PTD22,
    PTD23,
    PTD24,
    PTD25,
    PTD26,
    PTD27,
    PTD28,
    PTD29,
    PTD30,
    PTD31,

    /*  PTE?˿?    */
    PTE0,
    PTE1,
    PTE2,
    PTE3,
    PTE4,
    PTE5,
    PTE6,
    PTE7,
    PTE8,
    PTE9,
    PTE10,
    PTE11,
    PTE12,
    PTE13,
    PTE14,
    PTE15,
    PTE16,
    PTE17,
    PTE18,
    PTE19,
    PTE20,
    PTE21,
    PTE22,
    PTE23,
    PTE24,
    PTE25,
    PTE26,
    PTE27,
    PTE28,
    PTE29,
    PTE30,
    PTE31,

} PTxn;

void syscloption(system_option SYSPLL);
void SystemCoreClockSet (void); 
void SystemClockInit(void);
void GPIO_enable_port (void);
void PINS_GPIO_WritePin(GPIO_Type *base1,PORT_Type *base2, uint16_t pin, uint16_t value);
uint16_t PINS_GPIO_ReadPin(GPIO_Type *base1,PORT_Type *base2, uint16_t pin);
void PINS_GPIO_TogglePin(GPIO_Type *base1,PORT_Type *base2, uint16_t pin);


#endif /* __DRV_GPIO_H */
