#ifndef LED_H_
#define LED_H_

void LED_Init(void);

//1 is red, 2 is yellow, 4 is green
void LED_On(uint32_t data);
void LED_Off(uint32_t data);
void LED_Toggle(uint32_t data);

#endif
