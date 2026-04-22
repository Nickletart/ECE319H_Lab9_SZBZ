#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"

void Switch_Init(void){
  IOMUX->SECCFG.PINCM[PA24INDEX] = 0x00040081;
  IOMUX->SECCFG.PINCM[PA25INDEX] = 0x00040081;
}

int Switch_In(void){
  return GPIOA->DIN31_0>>24 & 3;
}
