/**
  ******************************************************************************
  * @file    main.c
  * @author  Vladyslav
  * @version V1.0
  * @date    September 2014
  * @brief   Main program body
  ******************************************************************************

 //Includes ------------------------------------------------------------------*/

 //Standard STM32L1xxx driver headers 
#include "misc.h"
#include "main.h"
#include "stm32l1xx_adc.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_rtc.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_pwr.h"
#include "stm32l1xx_syscfg.h"
#include "stm32l1xx_dbgmcu.h"


 //discovery board and specific drivers headers*/
#include "discover_board.h"
#include "icc_measure.h"
#include "discover_functions.h"

#include <Cust_encoder.h>
#include <Cust_MatrKeypad.h>
    
#include "CustADC.h"
#include "CustI2C.h"
#include "CustGps.h"    

#define DEBUG


/* Private variables ---------------------------------------------------------*/
bool majorInterrupt = false;
/*******************************************************************************/

static volatile uint32_t TimingDelay;
extern unsigned char Bias_Current;     //Bias Current stored in E�Prom used for ICC mesurement precision 
//extern uint8_t t_bar[2];               //LCD bar graph: used for displaying active function 
extern bool Auto_test;                 //Auto_test activation flag: set by interrupt handler if user button is pressed for a few seconds 
extern bool Idd_WakeUP;                //*/
extern volatile bool KeyPressed;       //*/
extern bool UserButton;                //Set by interrupt handler to indicate that user button is pressed  
uint8_t state_machine = 0;                 //Machine status used by main() wich indicats the active function, set by user button in interrupt handler 
uint16_t Int_CurrentSTBY;              //*/


void All_init(void)
{
    SystemInit();
    //HSI_on_16MHz(); 
    All_clk_On(); 
 //Timer2_init_vs_irq(); //ovr_irq

/******Btn`s init_section******/  
    Init_GPIOs();    //Init leds, user btn, enc btn

 Button_init_vs_irq();   
 Matrix_kbd_init();
 
/****Encoder init_section****/    
 encoder_init(); 
 
/****Interface init_section****/  
 Usart_init_vs_irq ();
 Spi2_init_vs_irq  (); //2- slave, 1- mstr
 
 ///i2c1_init();///i2c1_rx(0xA0);
   //ADC_Icc_Init();   //Initializes ADC 
 ADC_init();

  enableInterrupts();	   //Enable General interrupts 
} 


void Debug_section(void){
 //**********Debug section***************//
 //*************************************//
   //  ADC_measure(10);  /*!< Start Conversion of regular channels */
   //   ADC_measure(11);
    //   ADC_measure(16);
// send_str_u(  (char*)NVIC_GetPriority (EXTI0_IRQn));
 //spi_exchange(send_data); 
 //send_data = 0x05;
 
  send_str_u("===STM_GO!===\n");
 
 
Get_key();

//*************************************//
//**********Debug section***************/  
}




char out_data_t = 0; //tmp debug



/*******************************************************************************/
/**
  * @brief main entry point.
  * @par Parameters None
  * @retval void None
  * @par Required preconditions: None
*/  
int main(void)
{ 
 // bool StanbyWakeUp ;
 // float Current_STBY;
     
 /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32l1xx_md.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32l1xx.c file
 */     
 
 //////////////////////////////////////////////////////////////////////////////////
 ///////////////////////On-StartTestSection_start////////////////////////////////// 
 
  RCC_Configuration();   //Configure Clocks for Application need 
  PWR_VoltageScalingConfig(PWR_VoltageScaling_Range1);    //Set internal voltage regulator to 1.8V 
  while (PWR_GetFlagStatus(PWR_FLAG_VOS) != RESET) ;   //Wait Until the Voltage Regulator is ready 
  
 ///////////////////////On-StartTestSection_end//////////////////////////////////// 
 //////////////////////////////////////////////////////////////////////////////////


  
  All_init();
  Debug_section();
  
  /*Until application reset*/
  while (1)
  {
    
  OutData.encoder_state = Encoder_proc();
  OutData.btn_state |= Get_key();
 
  if((OutData.encoder_state)||(OutData.btn_state))
  {
    OutData.flags |= ANY_DATA; //����, ��� ���������
    SPI2->DR = OutData.encoder_state; //To DM
    SPI2->DR = OutData.btn_state; //To DM
    send_to_usart(OutData.encoder_state); //debug 
    send_to_usart(OutData.btn_state); //debug 
    OutData.flags |= OUT_DATA_SENDED; //����������, ����� ������� ������!
  }  
   

//button_port = GPIOC->IDR;  
//send_to_usart(button_port>>8);
 
  
    ///////////////////////////////////////////////////////--------------------
     //����� ������� ������ �������.
    //� �������� ��������� ���������� ���������� �������� 0x01 ��� 0x03
 
       //spi_exchange(send_data); 
    //SPI2->DR = Encoder_curr_pos; //����� � ����� ����������� SPI2 
   // send_to_usart(Encoder_curr_pos);
 //   SPI1->DR = 0x05+512; //����� � ����� ����������� SPI1. ����� ����� �������� ����� �������
                     //�� ��������� � ����������
   //spi_exchange(send_data);  
    switch(temp)
    {/*���� SPI1 ������ 0x01, �������� ��������������� ���������, 
      � ������ �������� ���������, ������������� ������� ������ �������
      ��� ��������� ������ �� 0x03*/
    case 0x01:
      //GPIOB->BSRRH |= GPIO_BSRR_BS_7;
      //GPIOB->BSRRL |= GPIO_BSRR_BS_6;
      send_str_u("SPI=01");
      send_data = 0x03;
      break;
      /*���� SPI1 ������ 0x03, �������� ��������������� ���������, 
      � ������ �������� ���������, ������������� ������� ������ �������
      ��� ��������� ������ �� 0x01*/
    case 0x03:
      //GPIOB->BSRRH |= GPIO_BSRR_BS_6;
      //GPIOB->BSRRL |= GPIO_BSRR_BS_7;
       send_str_u("SPI=03");
      send_data = 0x01;
      break;     
     default :  //GPIOB->BSRRH |= GPIO_BSRR_BS_7;
    }
    //////////////////////////////////////////////////////////////-------------------
//delay(1000);

#ifdef DEBUG  
  delay(100000); //+0 
#endif  
  //send_to_usart(temp);
 //   if (USART1->SR & USART_SR_RXNE) {usart_data=USART1->DR;  
  //switch(usart_data)  {
 switch(usart_data)  {
    case '1':
       Info(); 
       break;
    case '2':
       Timer2_init_vs_irq(); //ADC_measure(10);  //every second ad-conventiuon flag (in irq)setts
       break;
    case '3':
       GPS_get_data();      
       break;
    case '4':
       Current_monitor_get_data();
       break;     
    case '5':
        SET(ADC_start_flag);  /*!< Start Conversion of regular channels */
       break;
    case '6':
        send_str_u("===HELLO_FROM_USART!===");
      break;
      case '7':
        SPI2->DR=0xABCD;
      break;
       case '8':  //Keybord_debug
  //pressed_key = Get_key();    
  //send_to_usart(pressed_key);
 
      break;
     //default :    send_str_u("Invalid_comand!");
 }
 
 
 /*
  if(strcmp(UsartRxBuffer,"led0")==0) //���� ������ ������� "led0"
   {
    GPIOB->BSRRH |= GPIO_BSRR_BS_6;  // Reset PB6 bit
   }
 if(strcmp(UsartRxBuffer,"led1")==0) //���� ������ ������� "led1"
   {
     GPIOB->BSRRL |= GPIO_BSRR_BS_6;   // Set PB6 bit
   }
*/ 

     //    }
          
    //  if (A_flag){  GPIOB->ODR ^= GPIO_ODR_ODR_6  ;  } // 
    //  RESET(A_flag);
         
         
//flag state machine
 if(ADC_start_flag)    { ADC_measure(10); ADC_measure(11); RESET(ADC_start_flag);} 
 if(Get_current_flag)  { Get_current(); RESET(Get_current_flag);} 
 if(Get_RTC_flag)      { Get_time(); RESET(Get_RTC_flag);} 
 
 
 //End of main cycle
 //Cleaning all buffers 
  if(UsartRxBuffer[0]!=0)
  {
      // send_str_u(UsartRxBuffer); //return to console
       memset(UsartRxBuffer, 0, sizeof(UsartRxBuffer)); //clearing buffer
       usart_data=0;
       usart1_rx_bit=0; //reset counter
  }
   if(spi1_buf[0]!=0)
  {
       memset(spi1_buf, 0, sizeof(spi1_buf)); //������� �����
       spi1_data=0;
       spi1_bit=0; //���������� �������
  } 
 
 //if ((OutData.flags == ANY_DATA) && (OutData.flags == OUT_DATA_SENDED))
  if (OutData.flags == (ANY_DATA + OUT_DATA_SENDED))
 {   
   OutData.flags = 0;
   OutData.btn_state = 0;
   OutData.encoder_state = 0;
   OutData.current_state = 0;
   OutData.voltage_state = 0;
 }
    
  }//End While(1)
} //End main()		


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
 */ 
void RCC_Configuration(void)
{  
  RCC_HSICmd(ENABLE);//Enable HSI Clock  
  while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);//!< Wait till HSI is ready  
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI); //Set HSI as sys clock   
  RCC_MSIRangeConfig(RCC_MSIRange_6);//Set MSI clock range to ~4.194MHz*/  
   //Enable the GPIOs clocks 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC| RCC_AHBPeriph_GPIOD| RCC_AHBPeriph_GPIOE| RCC_AHBPeriph_GPIOH, ENABLE);      
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_COMP |  RCC_APB1Periph_PWR , ENABLE);   //Enable comparator and PWR mngt clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_SYSCFG , ENABLE);//Enable ADC & SYSCFG clocks  
  PWR_RTCAccessCmd(ENABLE);//Allow access to the RTC 
   //Reset RTC Backup Domain 
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);   
  RCC_LSEConfig(RCC_LSE_ON);//LSE Enable  
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//Wait until LSE is ready    
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //RTC Clock Source Selection  
  RCC_RTCCLKCmd(ENABLE);   //Enable the RTC   
  RCC_HSEConfig(RCC_HSE_OFF);  /*Disable HSE*/
  if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET )
  {    
    while(1); //Stay in infinite loop if HSE is not disabled*/
  }
}


 void DeInit_GPIOs(void)
{
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
}


/**
  * @brief  To initialize the I/O ports
  * @caller main
  * @param None
  * @retval None
 */ 
void  Init_GPIOs (void)
{
   //GPIO, EXTI and NVIC Init structure declaration 
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
 DeInit_GPIOs();
 
   //Configure User Button pin as input 
#ifdef DEBUG
    GPIO_InitStructure.GPIO_Pin = USERBUTTON_GPIO_PIN;
#endif
  GPIO_InitStructure.GPIO_Pin |=  ENCODERBTN_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(USERBUTTON_GPIO_PORT, &GPIO_InitStructure);

   //Select User Button pin as input source for EXTI Line 
#ifdef DEBUG
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
#else 
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);
#endif
   //Configure EXT1 Line 0 in interrupt mode trigged on Rising edge 
#ifdef DEBUG
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;  // PA0 for User button AND IDD_WakeUP
#endif
  EXTI_InitStructure.EXTI_Line |=  EXTI_Line5;  // 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

   //Enable and set EXTI0 Interrupt to the lowest priority 
#ifdef DEBUG
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
#endif
  NVIC_InitStructure.NVIC_IRQChannel |=  EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 

  //��������� ��� ����������
#ifdef DEBUG
NVIC_EnableIRQ (EXTI0_IRQn);
#endif
NVIC_EnableIRQ (EXTI9_5_IRQn);
  
#ifdef DEBUG  
   //Configure the LED_pin as output push-pull for LD3 & LD4 usage
  GPIO_InitStructure.GPIO_Pin = LD_GREEN_GPIO_PIN | LD_BLUE_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(LD_GPIO_PORT, &GPIO_InitStructure); 
   //Force a low level on LEDs*/ 
 // GPIO_LOW(LD_GPIO_PORT,LD_GREEN_GPIO_PIN);	
 // GPIO_LOW(LD_GPIO_PORT,LD_BLUE_GPIO_PIN);
#endif 
/*    
 //Counter enable: GPIO set in output for enable the counter 
  GPIO_InitStructure.GPIO_Pin = CTN_CNTEN_GPIO_PIN;
  GPIO_Init( CTN_GPIO_PORT, &GPIO_InitStructure);
  
 //To prepare to start counter 
  GPIO_HIGH(CTN_GPIO_PORT,CTN_CNTEN_GPIO_PIN);
  */
/*      
 //Configure Port A LCD Output pins as alternate function 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init( GPIOA, &GPIO_InitStructure);
  
 //Select LCD alternate function for Port A LCD Output pins 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,GPIO_AF_LCD) ;
  
   //Configure Port B LCD Output pins as alternate function  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 ;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init( GPIOB, &GPIO_InitStructure);
  
   //Select LCD alternate function for Port B LCD Output pins 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4,GPIO_AF_LCD) ;
  
   //Configure Port C LCD Output pins as alternate function  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1  ;                               
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init( GPIOC, &GPIO_InitStructure);  

   //Select LCD alternate function for Port B LCD Output pins 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource0,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource1,GPIO_AF_LCD) ; 
 */ 
  
  /*
   //Configure ADC (IDD_MEASURE) pin as Analogue 
  GPIO_InitStructure.GPIO_Pin = IDD_MEASURE  ;                               
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init( IDD_MEASURE_PORT, &GPIO_InitStructure);
  */
}  



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

//My funktions_start
void HSI_on_16MHz (void){
   /*  � �������� ��������� ������������ ���������� ��������� HSI � �������� 16 ���. */  
  RCC->CR |= RCC_CR_HSION; //�������� �������� ��������� HSI
  while(!(RCC_CR_HSION)); //���� ��� ������������
  RCC->CFGR |= RCC_CFGR_SW_HSI; //�������� ���������� �������� ������� SYSCLK ��������� HSI
  RCC->CR &= ~RCC_CR_MSION; //��������� ��������� MSI.   
}

void All_clk_On(void){ //������������ ������ �����-������ � ���������
  RCC->AHBENR  |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
  RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_I2C1EN | RCC_APB1ENR_I2C2EN | RCC_APB1ENR_USART2EN | RCC_APB1ENR_SPI2EN ;
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_SPI1EN;
}


void Timer2_init_vs_irq(void){
     RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Enable TIM2 Periph clock
     
    TIM2->PSC = SystemCoreClock / 1000 - 1; // 1000 tick/sec
    TIM2->ARR = 100;  // 1 Interrupt/0.1 sec
  
    TIM2->DIER |= TIM_DIER_UIE; // Enable tim2 interrupt
    TIM2->CR1 |= TIM_CR1_CEN;   // Start count
    
  //  NVIC_SetPriority(TIM2_IRQn, 1);
    NVIC_EnableIRQ(TIM2_IRQn);  // Enable IRQ 
}


void Button_init_vs_irq (void){
   RCC->AHBENR  |= RCC_AHBENR_GPIOAEN; //enable gpioA
   RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;  /*!< System Configuration SYSCFG clock enable */
  /// GPIOA->CRH |=  GPIO_CRH_MODE0_1;
   //GPIOA->PUPDR |=  GPIO_PUPDR_PUPDR0_1; //BUTTON F in - pull down
   
  /// SYSCFG->EXTICR1 |= SYSCFG_EXTICR1_EXTI1 | SYSCFG_EXTICR1_EXTI1_PA;//?
   
   EXTI->EMR |= EXTI_EMR_MR0 ;// PIN 0     
   EXTI->IMR |= EXTI_IMR_MR0; //��� ������� 0 
   EXTI->FTSR|= EXTI_FTSR_TR0 ; //������������ - �� ��������� ������
   
    
   ///SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA; // Connect EXTI line 0 to PA.0//?
  /// RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
   
   NVIC_EnableIRQ(EXTI0_IRQn); // Enable IRQn
}


//////////////



void Info(){
send_str_u(VERSION);  
send_str_u(DATE);
}

void GPS_get_data(void){

ParseGpsData();  
  
send_str_u("GPS");
send_str_u((uint8_t *) &gpsData);

 //spi1_buf = (uint8_t *) &gpsData;
 //spi1_rx_buf_size = gpsDataSize;
}


//160521.109,0000.0000,N,00000.0000,E




void Current_monitor_get_data(void){
send_str_u("Current_monitor_get_data");  
  
}

void delay (int a)
{
    volatile int i,j;
    for (i=0 ; i < a ; i++)
    {
        j++;
    }
    return;
}

enum rcc_cfgr_mco_select
{
	RCC_CFGR_MCOSEL_DISABLED = 0,
	RCC_CFGR_MCOSEL_SYSCLK = RCC_CFGR_MCOSEL_0,
	RCC_CFGR_MCOSEL_HSI = RCC_CFGR_MCOSEL_1,
	RCC_CFGR_MCOSEL_MSI = RCC_CFGR_MCOSEL_1 | RCC_CFGR_MCOSEL_0,
	RCC_CFGR_MCOSEL_HSE = RCC_CFGR_MCOSEL_2,
	RCC_CFGR_MCOSEL_PLL = RCC_CFGR_MCOSEL_2 | RCC_CFGR_MCOSEL_0,
	RCC_CFGR_MCOSEL_LSI = RCC_CFGR_MCOSEL_2 | RCC_CFGR_MCOSEL_1,
	RCC_CFGR_MCOSEL_LSE = RCC_CFGR_MCOSEL_2 | RCC_CFGR_MCOSEL_1
			| RCC_CFGR_MCOSEL_0
};




void i2c1_init(){
  
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_I2C1EN;
  
  PIN_CONFIGURATION(PIN_I2C1_SCL);
  PIN_CONFIGURATION(PIN_I2C1_SDA);
        
  	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
            I2C1->CR2 |= I2C_CR2_ITBUFEN;  /*!< Buffer Interrupt Enable */
            I2C1->CR2 |= I2C_CR2_ITEVTEN;  /*!< Event Interrupt Enable */
                           
	// ������� �������� ������� PCLK1 (�� 2 �� 32, ������������)
	I2C1->CR2 &= ~I2C_CR2_FREQ;
	I2C1->CR2 |= 32;
	// ��������� ������� ������
	I2C1->CCR &= ~I2C_CCR_CCR;
	I2C1->CCR |= 10;
	// ����� ��������� �������
	I2C1->TRISE = 3;
}

  uint8_t eeprom_data;
void i2c1_rx (int adress){

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// ���������� ������
	I2C1->CR1 |= I2C_CR1_PE;

        
	// ������������ ������� �����
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB)){} 	// �������� ������������ ������� �����
	(void) I2C1->SR1;

	// �������� ������ �������� + ������
	I2C1->DR =  adress + 0;//0xA0
	while (!(I2C1->SR1 & I2C_SR1_ADDR)){} 	// �������� ��������� �������� ������
	(void) I2C1->SR1;
	(void) I2C1->SR2;

	// �������� ������ 10-� ������
	I2C1->DR = 10;
	while (!(I2C1->SR1 & I2C_SR1_BTF)){} 	// ��� �������� �������� ������

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// ������������ ������� ��������� �����
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB)){}
	(void) I2C1->SR1;

	// ������� ����� �������� + ������
	I2C1->DR = adress + 1;//0xA1
	while (!(I2C1->SR1 & I2C_SR1_ADDR)){} // ���
        
	(void) I2C1->SR1;
	(void) I2C1->SR2;
	while (!(I2C1->SR1 & I2C_SR1_RXNE)){}	// ��� ��������� ����� ������
	eeprom_data = I2C1->DR; // ��������� �������� ��������
	I2C1->CR1 |= I2C_CR1_STOP; // ������������ ������� ����
  
}

void i2c1_rx2 (void){
        I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB)){}
	(void) I2C1->SR1;

	// ����� �������� + ������
	I2C1->DR = 0xA0;
	while (!(I2C1->SR1 & I2C_SR1_ADDR)){}
	(void) I2C1->SR1;
	(void) I2C1->SR2;


	I2C1->DR = 10;
	while (!(I2C1->SR1 & I2C_SR1_BTF)){}

	// ������� ������
	I2C1->DR = ++eeprom_data;
	while (!(I2C1->SR1 & I2C_SR1_BTF)){}

	I2C1->CR1 |= I2C_CR1_STOP;
}

uint16_t Get_current (void){
  
}

uint16_t Get_time (void){
  
  
}




void Usart_init_vs_irq (void)
{
RCC->AHBENR  |= RCC_AHBENR_GPIOAEN; //������������ �����
  
    PIN_CONFIGURATION(USART1_TX);
    PIN_CONFIGURATION(USART1_RX); 
    /*   
   GPIOA->MODER |= GPIO_MODER_MODER9_1; // PA9  AF  
   GPIOA->OTYPER &= ~ GPIO_OTYPER_OT_9; //out push-pull
   GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;//40 MHZ
   
   GPIOA->MODER &=~ GPIO_MODER_MODER10_1; // PA10 IN no pull-up no pull-down
   GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;//40 MHZ
   */
    
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;// ������������ ������
 
  USART1->BRR  = (SystemCoreClock + 9600/2)/9600 ; // 0x9c4 ; baud rate 9600 f=24Mhz 
  USART1->CR1 |=  USART_CR1_RXNEIE; // RX Interrupt Enable 
 // USART1->CR1 |= USART_CR1_TCIE; // Transmission Complete Interrupt Enable
  USART1->CR1 |= USART_CR1_UE;//UART ENABLE
  USART1->CR1 |=USART_CR1_RE | USART_CR1_TE; // rx,tx enable
  
  NVIC_EnableIRQ(USART1_IRQn);// Enable IRQ
}


void send_to_usart(uint8_t data) {
  while(!(USART1->SR & USART_SR_TC)); 
  USART1->DR=data; 
}


void send_str_u(char * string) { //SEND STRING TO UART
uint8_t i=0;
  while(string[i]) {
  send_to_usart(string[i]);
  i++;
 }
send_to_usart('\r');
send_to_usart('\n');
}


void Spi2_init_vs_irq(void){
   RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN; //������������ ������ A, B
  //----------------------------------------------------------------------------  
  // /*
   //������ SPI1 (Master)
  //PA12(MOSI), PA11(MISO), PA5(SCK), PA4(NSS) - AF, Push-Pull, AF5(SPI1)
  GPIOA->MODER |= GPIO_MODER_MODER12_1 | GPIO_MODER_MODER11_1 | GPIO_MODER_MODER5_1 | GPIO_MODER_MODER4_1; //Alternate function
  GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_12 | GPIO_OTYPER_OT_11 | GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_4); //Push-Pull
  GPIOA->AFR[1] |= (5<<16 | 5<<12); //PA12 = AF5, PA11 = AF5
  GPIOA->AFR[0] |= (5<<20 | 5<<16); //PA5 = AF5, PA4 = AF5
  // */
  
  //����� SPI2 (Slave)
  //PB15(MOSI), PB14(MISO), PB13(SCK), PB12(NSS) - AF, Push-Pull, AF5(SPI1)
  GPIOB->MODER |= GPIO_MODER_MODER15_1 | GPIO_MODER_MODER14_1 | GPIO_MODER_MODER13_1 | GPIO_MODER_MODER12_1; //Alternate function
  GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_15 | GPIO_OTYPER_OT_14 | GPIO_OTYPER_OT_13 | GPIO_OTYPER_OT_11); //Push-Pull
  GPIOB->AFR[1] |= (5<<28 | 5<<24 | 5<<20 | 5<<16); //PB15, PB14, P13, PB12 = AF5 
  //---------------------------------------------------------------------------- 
  ///*
   //��������� SPI1 (Master)
 // 8 ��� ������, MSB ���������� ������, ����������� ����� ���������� NSS, 
 // ����� NSS (PA4) ��������� ������������ � �������� ������
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //������������ ������ SPI1
  SPI1->CR1 |= (SPI_CR1_BR_0 | SPI_CR1_BR_1); //Baud rate = Fpclk/16 //if system clk =16MHz -> BR=1MHz
 // SPI1->CR1 |= SPI_CR1_CPOL; //���������� ��������� �������
 // SPI1->CR1 |= SPI_CR1_CPHA; //���� ��������� ������� ///????
  SPI1->CR1 |= SPI_CR1_DFF; //16 ��� ������               ///?
 // SPI1->CR1 |= SPI_CR1_LSBFIRST; //LSB ���������� ������
  SPI1->CR1 |= SPI_CR1_SSM; //����������� ����� NSS
  SPI1->CR1 |= SPI_CR1_SSI; //���������� ���������, ����� �� ����� NSS ������� �������
  SPI1->CR2 |= SPI_CR2_SSOE; //����� NSS - ����� ���������� slave select
   //SPI1->CR2 |= SPI_CR2_FRF;
  SPI1->CR1 |= SPI_CR1_MSTR; //����� Master
  
 //  SPI1->CR2 |= SPI_CR2_TXEIE; /*!< Tx buffer Empty Interrupt Enable */
   SPI1->CR2 |= SPI_CR2_RXNEIE;  /*!< RX buffer Not Empty Interrupt Enable */
  
 // SPI1->CR1 |= SPI_CR1_SPE; //�������� SPI1
 // NVIC_EnableIRQ(SPI1_IRQn);
 // */
  
  /*��������� SPI2 (Slave)
  8 ��� ������, MSB ���������� ������, ���������� ����� ���������� NSS, 
  ����� NSS (PB12) - ����*/
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; //������������ ������ SPI2
  SPI2->CR1 |= SPI_CR1_DFF; //16 ��� ������
 //SPI2->CR1 |= SPI_CR1_CPOL; //���������� ��������� �������
 // SPI2->CR1 |= SPI_CR1_CPHA; //���� ��������� �������
  //SPI2->CR1 |= SPI_CR1_LSBFIRST; //LSB ���������� ������
  SPI2->CR1 |= SPI_CR1_SSM; //����������� ���������� ������ NSS
  SPI2->CR1 &= ~SPI_CR1_MSTR; //����� Slave
  
 //  SPI2->CR2 |= SPI_CR2_TXEIE; /*!< Tx buffer Empty Interrupt Enable */
   SPI2->CR2 |= SPI_CR2_RXNEIE;  /*!< RX buffer Not Empty Interrupt Enable */
  
  SPI2->CR1 |= SPI_CR1_SPE; //�������� SPI2  
  NVIC_EnableIRQ(SPI2_IRQn);
}

void recieve_from_spi_1(void) {
  while(!(SPI1->SR & SPI_SR_RXNE)); //������� ��������� ������ ������ ������� SPI1 (RXNE =1 - �������� ����� �������� ������)
  temp = SPI1->DR;//��������� ������ �� ��������� ������ SPI1. ��� ���� �������� ���������� ������� ������ � ����� ����� RXNE
}


/*������� ������ ������� ����� �������� SPI1 � SPI2.
� �������� ��������� ��� ������ ������� ���������� ����� 0x01 ��� 0x03, 
������� ����� ������������ � ����� ����������� SPI2.
��������, ������������ � ����� ����������� SPI1 �������, � ������ ������ 0x0F.
����� ������ ������� ������, ��������, �������� ������� SPI1 ����������� �� 
�������� ������ SPI1_DR � ���������� temp*/
void spi_exchange(uint8_t send_data)
{
  SPI2->DR = send_data; //����� � ����� ����������� SPI2 
  SPI1->DR = 0x0F; //����� � ����� ����������� SPI1. ����� ����� �������� ����� �������
  recieve_from_spi_1();
}

void ADC_init(void){
     RCC->AHBENR  |= RCC_AHBENR_GPIOAEN; //������������ �����
     //Predefined channels
     //PIN_CONFIGURATION( ADC_CH_10); //C0
     //PIN_CONFIGURATION( ADC_CH_11); //C1
     
     PIN_CONFIGURATION( ADC_CH_3); //A2
     PIN_CONFIGURATION( ADC_CH_4); //A3
  
  RCC->CR |= RCC_CR_HSION; //�������� ���������� ��������� HSI - 16���
  while(!(RCC->CR&RCC_CR_HSIRDY)); //���� ��� ������������
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //��������� ������������ ���
  //----------------------------------------------------------------------------
 // ADC1->CR2 |=ADC_CR2_DELS_0; //: Until the converted data have been read 
    ADC1->CR1 |= ADC_CR1_RES_1; //resolution 8 bit
    
    /*������������������ ��0������� (������������� ������ ��������� ��������������)*/
               /// ADC1->SQR5 |= ADC_SQR1_L_0;
               // ADC1->SQR5 |= ADC_SQR5_SQ1_1 | ADC_SQR5_SQ1_3; //10 CH
               // ADC1->SQR5 |= ADC_SQR5_SQ2_0 | ADC_SQR5_SQ2_1 | ADC_SQR5_SQ2_3; //11 CH
 
  ADC1->CR2 &= ~ADC_CR2_ALIGN; //������������ ���������� ������ 
  //----------------------------------------------------------------------------
  ADC1->CR2 |= ADC_CR2_ADON; //�������� ���
  while(!(ADC1->SR&ADC_SR_ADONS)); //���� ���������� ���
  //----------------------------------------------------------------------------
   ADC1->CR1 |= ADC_CR1_EOCIE; /*!< Interrupt enable for EndOfConversion */
   NVIC_EnableIRQ(ADC1_IRQn);
       
   // ADC1->CR2 |= ADC_CR2_SWSTART;  /*!< Start Conversion of regular channels */
   //ADC1->CR2 |= ADC_CR2_JSWSTART;  /*!< Start Conversion of injected channels */    
}

void ADC_measure(char ch){
 ADC1->SQR5 = 0; 
switch(ch)
 {
  case 10:
      ADC1->SQR5 |= ADC_SQR5_SQ1_1 | ADC_SQR5_SQ1_3; //10 CH
      break;
  case 11:
    ADC1->SQR5 |= ADC_SQR5_SQ1_0 | ADC_SQR5_SQ1_1 | ADC_SQR5_SQ1_3; //11 CH
      break;
      
  case 3:
    //  ADC1->SQR5 |= ADC_SQR5_SQ1_1 | ADC_SQR5_SQ1_3; //3 CH
      break;
  case 4:
   // ADC1->SQR5 |= ADC_SQR5_SQ1_0 | ADC_SQR5_SQ1_1 | ADC_SQR5_SQ1_3; //4 CH
      break;
      
  default : ADC1->SQR5 |= ADC_SQR5_SQ1_1 | ADC_SQR5_SQ1_3; //10 CH
 }   
ADC1->CR2 |= ADC_CR2_SWSTART;  /*!< Start single Conversion of regular channels */
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//������ ������� ����� ���� �� ��������, ������ ����� 2 ���
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void DM_spi_test(void){
  
}

//My funktions_end

////////////////////////////////////////////
/////////////////NOTES_START///////////////

 //----------------------------------------------------------------
        //ENEBLE INTERRUPTS
        //1
 //__enable_irq ();   //Global enable
       //2
// NVIC_EnableIRQ(EXTI1_IRQn); // Enable IRQn
 //NVIC_DisableIRQ(EXTI1_IRQn); // Disable IRQn      
      //3 ��������� � ��������� ���������� ���������� � ���������
 //EXTI->IMR |=(EXTI_IMR_MR1 | EXTI_IMR_MR2); //��� ������� 1 � 2
// EXTI->RTSR |= EXTI_RTSR_TR1 | EXTI_FTSR_TR2 ; //������������ - �� ��������� � ��������� �������
 //-------------------------------------------------------------------
 
/////////////////NOTES_END////////////////// 
////////////////////////////////////////////

 
 

//////////////////////////////////////////
//////////////////////////////////////////
//From examlpe start

/*
  bool StanbyWakeUp ;
  float Current_STBY;
    
  Int_CurrentSTBY = Current_Measurement();    //store Standby Current
  if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)    //Check if the StandBy flag is set 
  {
     //System resumed from STANDBY mode 
     //Clear StandBy flag 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
    PWR_ClearFlag(PWR_FLAG_SB);    
    StanbyWakeUp = TRUE;  //set StandbyWakeup indicator
  } else
  {
    StanbyWakeUp = FALSE;  //Reset StandbyWakeup indicator 
  } 
  
  RCC_Configuration();   //Configure Clocks for Application need 
  PWR_VoltageScalingConfig(PWR_VoltageScaling_Range1);    //Set internal voltage regulator to 1.8V 
  while (PWR_GetFlagStatus(PWR_FLAG_VOS) != RESET) ;   //Wait Until the Voltage Regulator is ready 
  

  Init_GPIOs();    //Init I/O ports 
  ADC_Icc_Init();   //Initializes ADC 
  enableInterrupts();	   //Enable General interrupts 
  
 
  //TSL_Init();   //Init Touch Sensing configuration  
  
   // Multichanel Key settings
  sMCKeyInfo[0].Setting.b.IMPLEMENTED = 1;
  sMCKeyInfo[0].Setting.b.ENABLED = 1;
  sMCKeyInfo[0].DxSGroup = 0x00; 
 
   //Initializes the LCD glass 
  //LCD_GLASS_Init();
  //LCD_GLASS_DisplayString("HELLO");
        
    KeyPressed = FALSE;  //Reset Keypressed flag used in interrupt and Scrollsentence 
    UserButton = TRUE;  //user button activ   

   //Check if User button press at Power ON  	
  if ((USERBUTTON_GPIO_PORT->IDR & USERBUTTON_GPIO_PIN) != 0x0)
  {
   Bias_measurement();   //Measure operational amplifier bias current and store value in E�Prom for application need
  }

   //Standard application startup 

  if ( !StanbyWakeUp )
  {    
        AUTOTEST(FALSE) ; // Reset autotest flag stored in memory 
    /// Display Welcome message  
   //LCD_GLASS_ScrollSentence(" ** STM32L-DISCOVERY **",1,SCROLL_SPEED);
    if (!KeyPressed)
    {
       // if welcome message not skipped Display blinking message JP1 ON
      LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM,LCD_BlinkFrequency_Div512);
      LCD_GLASS_DisplayString("JP1 EE");
      LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div32);	
    }
  // Wake up from Standby or autotest 
  }  else  {
    //Check Autotest value stored in flash to get wakeup context
    if (Auto_test)
    {
      auto_test_part2();      //Wake UP: Return of RESET by Auto test
    } else {
      //Wake UP: Return of RESET by Current STAND BY measurement
      LCD_GLASS_ScrollSentence("     STANDBY WAKEUP",1,SCROLL_SPEED);
      //Substract bias current from operational amplifier
      if ( Int_CurrentSTBY > Bias_Current )
        Int_CurrentSTBY -= Bias_Current;
      Current_STBY = Int_CurrentSTBY * Vdd_appli()/ADC_CONV;  
      Current_STBY *= 20L;
      display_MuAmp((uint32_t)Current_STBY); // Wait for user button press to continue 
      while(!KeyPressed);
    }
  }
  
  KeyPressed = FALSE;    //Reset KeyPress Flag 
  //BAR0_OFF;    //Clear LCD bars 
   //Switch off the leds
 // GPIO_HIGH(LD_GPIO_PORT,LD_GREEN_GPIO_PIN);	
  GPIO_LOW(LD_GPIO_PORT,LD_BLUE_GPIO_PIN);	
  state_machine = STATE_VREF ; //Set application state machine to VREF state 
  
*/


  /*Until application reset*/
 /*
  while (1)
  {
    if (Auto_test)  auto_test();    //run autotest if requested by the user 
     //Perform Actions depending on current application State  
    switch (state_machine)
    {
         //VREF State : Display VRef value 
        case STATE_VREF:
          GPIO_TOGGLE(LD_GPIO_PORT,LD_BLUE_GPIO_PIN);
          GPIO_TOGGLE(LD_GPIO_PORT,LD_GREEN_GPIO_PIN);
          Vref_measure();
          TEMPO ;
        break;       
         //Slider Value State : Display the TS slider value 
        case STATE_SLIDER_VALUE:
          TSL_Action();
          Slider_value();   
        break;       
         //Slider button State : Display the curent TS button pressed  
        case STATE_SLIDER_BUTTON: 
          TSL_Action();
          Button_value();         
          break;        
         //ICC RUN State : ICC mesurements in Run and Sleep modes   
        case STATE_ICC_RUN:
          LCD_GLASS_DisplayString(" RUN  ");         
          Icc_RUN();
          LCD_GLASS_DisplayString(" SLEEP ");
          Icc_SLEEP(); 
        break;        
         //ICC LOW POWER RUN State : ICC mesurements in LowPower run and LowPower Sleep modes 
        case STATE_ICC_LP_RUN:
          LCD_GLASS_DisplayString("LP RUN");     
          Icc_LPRUN();
          LCD_GLASS_DisplayString("LP SLP");
          Icc_LPSLEEP();             
        break;        
         //ICC STOP  State : ICC mesurements in Stop and STOP NoRTC modes 
        case STATE_ICC_STOP:
          LCD_GLASS_DisplayString(" STOP ");         
          Icc_STOP();       
          LCD_GLASS_DisplayString("SP-NRTC");         
          Icc_Stop_NoRTC();                
          break;      
         //ICC Standby  State : ICC mesurements in Standby mode 
        case STATE_ICC_STBY:
          LCD_GLASS_DisplayString("STBY  ");          
        ADC_Icc_Test(MCU_STBY);
         //Following break never performed dues to software reset in previous function        
        break;          
         //for safe: normaly never reaches  			
        default:
          LCD_GLASS_Clear();
          LCD_GLASS_DisplayString("ERROR");
        break;
      }     
      KeyPressed = FALSE; //Reset KeyPress flag
    }
*/

//From examlpe _end
//////////////////////////////////////////
//////////////////////////////////////////


uint8_t i=0; 


void TIM2_IRQHandler(void)
{   
    TIM2->SR &= ~TIM_SR_UIF; //Clean UIF Flag
  //  SET(ADC_start_flag);
  
    
    //if(PIN_SIGNAL(LED_GREEN)){ PIN_OFF(LED_GREEN);}
   // else  PIN_ON(LED_GREEN) ;
    
    if (1 == (i++ & 0x01)) {
      GPIOB->BSRRH |= GPIO_BSRR_BS_7;  // Reset PB7 bit
    } else {
      GPIOB->BSRRL |= GPIO_BSRR_BS_7;   // Set PB7 bit
    }    
  
    
  //  send_str_u("Timer2_IRQ!\n");
}

void TIM3_IRQHandler(void)
{   
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    //if (!capture_is_first)
    //  capture_is_ready = 1;

   // capture_is_first = 0;
    /* � ���� TIM_CR1_DIR �������� TIM3_CR1 ��������
       ����������� �������� ��������, ���������� ���. */
   // captured_direction = (TIM3->CR1 & TIM_CR1_DIR ? FORWARD : BACKWARD);
  }

  
  // TIM3->SR &= ~TIM_SR_UIF; //Clean UIF Flag
 //  TIM3->SR &= ~TIM_SR_TIF; //Clean TIF Flag
 //  TIM3->SR &= ~TIM_SR_CC4IF;
  // TIM3->SR &= ~TIM_SR_CC3IF;
  // send_str_u("Tim3_IRQ!\n");
}

//------------------------------------------------------------------------------
uint8_t j=0; 

/*
void EXTI0_IRQHandler(void)
{
  EXTI->PR |= EXTI_PR_PR0; // ����� ����� ����������
 // t^=1;
//  SET(A_flag);
  
    send_str_u("You!");
  
 if (1 == (j++ & 0x01)) {
        //GPIOB->BSRR |= GPIO_BSRR_BS_6;   // Set PB4 bit
       // GPIOB->BSRR |= GPIO_BSRR_BR_7;  // Reset PB5 bit
      
   //    GPIOB->ODR &= ~GPIO_ODR_ODR_6  ;
    }
 else {
      // GPIOB->BSRR |= GPIO_BSRR_BS_7;   // Set PB5 bit
       // GPIOB->BSRR |= GPIO_BSRR_BR_6;  // Reset PB4 bit 
 
   //   GPIOB->ODR |= GPIO_ODR_ODR_6  ;
          }
}
*/
uint16_t debug_spi_t = 1; 

 void EXTI0_IRQHandler(void)
{
  /* Disable general interrupts */
  disableInterrupts();
  
  /* UserButton usage activated*/ 
  if (!UserButton)
  {
    UserButtonHandler();
    SPI1->DR = debug_spi_t*2;//debug
  }
  else 
  {
    /*Idd_Wakeup detected */
    Idd_WakeUP = TRUE;
  }  
  EXTI_ClearITPendingBit(EXTI_Line0);
  enableInterrupts();
}
 
// ���������� EXTI 1
void EXTI1_IRQHandler(void)
{
uint32_t	i;
EXTI->PR |= EXTI_PR_PR1;  // ����� ����� ����������
 SET(B_flag);
}

// ���������� EXTI 2
void EXTI2_IRQHandler(void)
{
uint32_t	i;
EXTI->PR |= EXTI_PR_PR2;  // ����� ����� ����������
 SET(C_flag);
}


// ���������� EXTI 3
void EXTI3_IRQHandler(void)
{
uint32_t	i;
EXTI->PR |= EXTI_PR_PR3;  // ����� ����� ����������
 SET(D_flag);
 
send_str_u("ENC_btn_pressed!");
}


// ���������� EXTI 4
void EXTI4_IRQHandler(void)
{
uint32_t	i;
EXTI->PR |= EXTI_PR_PR4;  // ����� ����� ����������
 SET(E_flag);
}

/*
// ���������� EXTI 9-5
void EXTI9_5_IRQHandler(void)
{
uint32_t	i;
EXTI->PR |= EXTI_PR_PR5;  // ����� ����� ����������
 SET(F_flag);
}
*/

void EXTI9_5_IRQHandler(void)
{
  /* Disable general interrupts */
  disableInterrupts();
  
  OutData.flags = BTN_PRESSED;
  OutData.btn_state = 128; //Indicates encoder btn pressed
    //UserButtonHandler();

  EXTI_ClearITPendingBit(EXTI_Line5);
  enableInterrupts();
}

// ���������� EXTI 10-15
void EXTI15_10_IRQHandler(void)
{
uint32_t	i;
EXTI->PR |= EXTI_PR_PR10;  // ����� ����� ����������
 SET(ENKODER_flag);
}



void USART1_IRQHandler (void){

  if (USART1->SR & USART_SR_RXNE) {
    usart_data=USART1->DR;   
//USART1->DR = USART1->DR; //ECHO MODE
   UsartRxBuffer[usart1_rx_bit]=usart_data; //put recieved byte in buffer
   usart1_rx_bit++; //buffer count ++ 
  }
  USART1->SR |=  USART_SR_TC; //clear flags  
  USART1->SR |=  USART_SR_RXNE; 
}



void SPI1_IRQHandler (void){
      send_str_u("S");
 
  if( SPI1->SR & SPI_SR_RXNE){ // RXNE =1   Receive buffer contains data 
       //  GPIOB->BSRRH |= GPIO_BSRR_BS_7;   //RESet PB6 bit
   spi1_data = SPI1->DR;//��������� ������ �� ��������� ������ SPI1. ��� ���� �������� ���������� ������� ������ � ����� ����� RXNE 
   spi1_buf[spi1_bit]=spi1_data; //�������� �������� ���� � �����.
   temp=spi1_data;
   spi1_bit++; //���������� ������� ������ ������. 
  }
  
   if( SPI2->SR & SPI_SR_TXE){ // (TXE =1 - Transmit buffer empty))
       //  send_str_u("SPI1_IRQ  Transmit buffer empty");
     //   GPIOB->BSRRL |= GPIO_BSRR_BS_7;   // Set PB6 bit
  }
}


void SPI2_IRQHandler (void)
{
#ifdef DEBUG  
    send_str_u("SPI2_IRQ");
#endif  
  if( SPI2->SR & SPI_SR_RXNE){ // (RXNE =1 - �������� ����� �������� ������))
  
       //  GPIOB->BSRRH |= GPIO_BSRR_BS_6;   //RESet PB6 bit
        
    temp = SPI2->DR;//��������� ������ �� ��������� ������ SPI2. ��� ���� �������� ���������� ������� ������ � ����� ����� RXNE
   // send_to_usart(temp); //?/??    
    USART1->DR = temp; 
    
    SPI2->DR = (short)OutData.encoder_state; //����� � ����� ����������� SPI2 ?
   // send_to_usart(Encoder_curr_pos);
    
  }
   if( SPI2->SR & SPI_SR_TXE){ // (TXE =1 - Transmit buffer empty))

     //   GPIOB->BSRRL |= GPIO_BSRR_BS_6;   // Set PB6 bit
  }
}


void ADC1_IRQHandler (void){
  if(ADC1->SR & ADC_SR_EOC) { /*!< End of conversion */
    ADC_data=ADC1->DR;
    
    send_str_u("ADC");
   send_to_usart(ADC_data);
   }
}

/* In CustI2C
void I2C1_EV_IRQHandler (void){}*/


///////////////////////////////////////////////////////





//�����

/*
//Test
#define   ENCODER_TIM             TIM3
// Tim clock division is 1, so the FDTS = 72MHz //?
// Fsample = FDTS/4 = 18MHz, and sample 6 times
#define   ENC_FILTER              6
#define   MAX_COUNT               255
#define   ENCODER_TIM_PERIOD      (MAX_COUNT*3)

 // definitions for the quadrature encoder pins

// Left Motor Channels
#define ENC_1CH_PIN               GPIO_Pin_7
#define ENC_2CH_PIN               GPIO_Pin_6
#define ENC_GPIO_PORT         GPIOA
#define ENC_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define ENC_1CH_SOURCE            GPIO_PinSource7
#define ENC_2CH_SOURCE            GPIO_PinSource6
#define ENC_AF                GPIO_AF_TIM3

// determine the timers to use
#define ENC_TIMER_CLK          RCC_APB1Periph_TIM3

#define LEFT_COUNT()            ENCL_TIMER->CNT
#define RIGHT_COUNT()           ENCR_TIMER->CNT
void Enc_Init_3ver(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;//RCC_AHB1PeriphClockCmd (ENC_GPIO_CLK, ENABLE);
  
   // now configure the pins themselves
  // they are all going to be inputs with pullups
  GPIO_StructInit (&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //GPIO_PuPd_UP changed for GPIO_PuPd_DOWN
  GPIO_InitStructure.GPIO_Pin = ENC_1CH_PIN | ENC_2CH_PIN;
  GPIO_Init (ENC_GPIO_PORT, &GPIO_InitStructure);
  

  // Connect the pins to their Alternate Functions
  GPIO_PinAFConfig (ENC_GPIO_PORT, ENC_1CH_SOURCE | ENC_1CH_SOURCE, ENC_AF);

  // Timer peripheral clock enable
  RCC_APB1PeriphClockCmd (ENC_TIMER_CLK, ENABLE);

  // set them up as encoder inputs
  // set both inputs to rising polarity to let it use both edges
  TIM_EncoderInterfaceConfig (ENCODER_TIM, 
                              TIM_EncoderMode_TI12, 
                              TIM_ICPolarity_Falling, //Rising
                              TIM_ICPolarity_Falling);//Rising
    TIM_SetAutoreload (ENC_TIMER, 0x00ff);

  // turn on the timer/counters
  TIM_Cmd (ENC_TIMER, ENABLE);
  //encodersReset();
}*/


/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
*/  
void Delay(uint32_t nTime)
{
  TimingDelay = nTime;
  while(TimingDelay != 0); 
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @caller SysTick interrupt Handler 
  * @param  None
  * @retval None
*/  
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
    TimingDelay--;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
*/  
void assert_failed(uint8_t* file, uint32_t line)
{ 
   //User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) ;
   //Infinite loop 
  while (1);
}

#endif