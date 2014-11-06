#include <Cust_MatrKeypad.h>


void Matrix_kbd_init (void){
///////////////////////////////////
///////Port configuration//////////
   RCC->AHBENR  |= RCC_AHBENR_GPIOCEN; //enable gpioC
   /*?*/ RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;  /*!< System Configuration SYSCFG clock enable */
 PIN_CONFIGURATION(IN_1); 
 PIN_CONFIGURATION(IN_2);
 PIN_CONFIGURATION(IN_3);
 PIN_CONFIGURATION(IN_4);
 PIN_CONFIGURATION(IN_5);
 PIN_CONFIGURATION(IN_6);

PIN_CONFIGURATION(OUT_1); 
PIN_CONFIGURATION(OUT_2);  
///////Port configuration//////////
///////////////////////////////////
}

char Get_key(void)
{
  char key = 0;
  uint16_t button_port  = 0;
  
  PIN_ON(OUT_1); //in open-drain mode = Hi-Z !
  PIN_ON(OUT_2);
  
   // for(char i=0; i<2; i++)
   // {
  if(key==0)
{ 
    PIN_ON(OUT_1);  //Hi-Z
    PIN_OFF(OUT_2); //open-drain
    button_port = GPIOC->IDR;
    button_port =~button_port;
    button_port &= (uint16_t)0x003F; //select key bits
        switch(button_port)
        {
                case 1: 
            key = 1;
            break;
                case 2: 
            key = 2;
            break;
                case 4: 
            key = 3;
            break;
                case 8: 
            key = 4;
            break;
                case 16: 
            key = 5;
            break;
                case 32: 
            key = 6;
            break;
            default : key = 0;
     }
}
if(key==0)
{ 
    PIN_ON(OUT_2);
    PIN_OFF(OUT_1);    
    button_port = GPIOC->IDR;
    button_port =~button_port;
    button_port &= (uint16_t)0x003F; //select key bits
        switch(button_port)
        {
                case 1: 
            key = 7;
            break;
                case 2: 
            key = 8;
            break;
                case 4: 
            key = 9; //
            break;
                case 8: 
            key = 10; //
            break;
                case 16: 
            key = 11; //
            break;
                case 32: 
            key = 12; //
            break;
            default : key = 0;
     }
 }   
//} 
        
 PIN_ON(OUT_1); //in open-drain mode = Hi-Z !
 PIN_ON(OUT_2);

 if(key)
 {
   //SPI2->DR = key; //To DM
   //send_to_usart(key); //debug OutData
   return key;
 }; 
return 0;
}