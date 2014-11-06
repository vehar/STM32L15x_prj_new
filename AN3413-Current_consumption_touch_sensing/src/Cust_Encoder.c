#include <Cust_encoder.h>

void Timer3_init_encoder_mode(void) //TODO add filter
{
       RCC->AHBENR  |= RCC_AHBENR_GPIOAEN; //enable gpioA    
       RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  // Enable TIM3 Periph clock //?
     
     //С детектора фронтов возьмем не инверсный, т.е. активный уровень высокий (для первого и второго входа)
     TIM3->CCER |= TIM_CCER_CC1P | TIM_CCER_CC2P;//--
     //Настраиваем второй мультиплексор (для первого и второго входа):
     TIM3->CCMR1 = TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;//--      
      
    TIM3->PSC = 2; 
    TIM3->ARR = 255;  //RELOAD AFTER ...
    
   // TIM3->SMCR |= TIM_SMCR_TS_2; //?
      TIM3->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;  /*!<SMS[2:0] bits (Slave mode selection) */ //1 КАНАЛ=+2, 2 КАНАЛА=+4;
      TIM3->SMCR |= TIM_SMCR_TS_2;// | TIM_SMCR_TS_0; //Select the timer TIx as the trigger input source by writing TS bits in the TIMx_SMCR register
   // TIM3->SMCR |= TIM_SMCR_ETF_2; //filter
      
    TIM3->CR1 |= TIM_CR1_CEN;   // Start count   
    
    //TIM3->DIER |= TIM_DIER_UIE; // Enable tim3 Update interrupt
    //NVIC_EnableIRQ(TIM3_IRQn);  // Enable IRQ 
}

void encoder_init(void)
{  
PIN_CONFIGURATION(ENCODER_BUTTON); // кнопка
PIN_CONFIGURATION(ENCODER_CH1);
PIN_CONFIGURATION(ENCODER_CH2);

 Timer3_init_encoder_mode(); // encoder
}



void Encoder_btn_init_vs_irq (void){
   RCC->AHBENR  |= RCC_AHBENR_GPIOAEN; //enable gpioA
   RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;  /*!< System Configuration SYSCFG clock enable */
   //GPIOA->PUPDR |=  GPIO_PUPDR_PUPDR0_1; //BUTTON F in - pull down   
    GPIOA->PUPDR |=  GPIO_PUPDR_PUPDR5_1; //BUTTON  in - pull down
    EXTI->EMR |= EXTI_EMR_MR5 ;// PIN 5
    EXTI->IMR |= EXTI_IMR_MR5; //для выводf 5
    EXTI->FTSR|= EXTI_FTSR_TR5;//конфигурация - по падающему фронту
   ///SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA; // Connect EXTI line 0 to PA.0//?
  /// RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 
   NVIC_EnableIRQ(EXTI0_IRQn); // Enable IRQn
}


uint8_t Encoder_proc (void)
{
static uint8_t Encoder_prev_pos;
static uint8_t Encoder_curr_pos;

   // Encoder_curr_pos = (TIM3->CNT & 254)>>1; //get encoder data (1 byte, div 2)
      Encoder_curr_pos = TIM3->CNT>>1 ; //get encoder data (1 byte, div 2)
   if(Encoder_curr_pos != Encoder_prev_pos)
   {
     //send_str_u("ENC");
     Encoder_prev_pos = Encoder_curr_pos;   
      return   Encoder_curr_pos;
   }
 return 0;
}