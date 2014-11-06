#ifndef __CUST_ENCODER_H
#define __CUST_ENCODER_H

#include "stm32l1xx.h"
#include "defines.h"

void Timer3_init_encoder_mode(void);
void Encoder_btn_init_vs_irq (void);
void encoder_init(void);

uint8_t Encoder_proc (void);




#endif /* __CUST_ENCODER_H */