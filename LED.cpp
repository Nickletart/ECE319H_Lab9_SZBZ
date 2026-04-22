#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"

void LED_Init(void){
      IOMUX->SECCFG.PINCM[PA15INDEX] = (uint32_t) 0x00000081;
      IOMUX->SECCFG.PINCM[PA16INDEX] = (uint32_t) 0x00000081;
      IOMUX->SECCFG.PINCM[PA17INDEX] = (uint32_t) 0x00000081;
      GPIOA->DOE31_0 |= (1<<15)|(1<<16)|(1<<17);
      GPIOA->DOUTCLR31_0 = (1<<15)|(1<<16)|(1<<17);
}

void LED_On(uint32_t data){
  GPIOA->DOUTSET31_0 = data<<15;
}

void LED_Off(uint32_t data){
  GPIOA->DOUTCLR31_0 = data<<15;
}

void LED_Toggle(uint32_t data){
  GPIOA->DOUTTGL31_0 = data<<15;
}
