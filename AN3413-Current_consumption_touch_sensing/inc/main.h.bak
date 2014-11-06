/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Template/main.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    13-September-2011
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "discover_board.h"
#include <stdio.h>
#include <stdint.h>


#include "macros.h"
#include "stm32l1xx_it.h" //for interrupt hndls
//#include "stm32l1xx_gpio.h" //for interrupt hndls
//#include "stm32l1xx_tim.h"
//#include "core_cm3.h"
#include "mcu_gpio.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);




volatile uint16_t temp, send_data = 0x01;
#define ADC_RATIO                   ((uint16_t) 806) /*ADC_RATIO = ( 3.3 * 1000 * 1000)/4095 */


//uint16_t button_port = 0;

volatile bool KeyPressed = FALSE;

void All_clk_On(void);

void Info();
void GPS_get_data(void);
bool ParseGpsData();

void Current_monitor_get_data(void);
void delay (int a);
void HSI_on_16MHz (void);



void All_init(void);

void ADC_init(void);

void i2c1_init();
void i2c1_rx (int adress);
void i2c1_rx2 (void);
void i2c1_tx (int adress, int data);

uint16_t Get_current (void); //Read current monitor data
uint16_t Get_time (void); //Read RTC data

void Timer2_init_vs_irq(void);
void Timer3_init_encoder_mode(void);

void Encoder_btn_init_vs_irq (void);
void encoder_init(void);
//new experiment ver
void Enc_Init_2ver(void);
void Enc_Init_3ver(void);




void Button_init_vs_irq (void);

void Matrix_kbd_init (void);
char Get_key(void);

void Usart_init_vs_irq (void);
void send_to_usart(uint8_t data);
void send_str_u(char * string);
void Spi2_init_vs_irq(void);
void recieve_from_spi(void);
void spi_exchange(uint8_t send_data);

void ADC_measure(char ch);
void DM_spi_test(void);

//void lcd_init();

char Get_key(void);


#define BTN_PRESSED 1
#define ENCODER_NEW_STATE 2
#define OUT_DATA_SENDED 4

#define ANY_DATA (BTN_PRESSED||ENCODER_NEW_STATE) //TODO дополнить

#define LED_GREEN	B, 7, HIGH, MODE_OUTPUT_PUSH_PULL, SPEED_400KHZ, AF_NO
#define LED_BLUE	B, 6, HIGH, MODE_OUTPUT_PUSH_PULL, SPEED_400KHZ, AF_NO

/****BUTTON SECTION_START*****************/
#define USER_BUTTON	A, 0, HIGH, MODE_INPUT_FLOATING, SPEED_400KHZ, AF_NO
#define ENCODER_BUTTON	A, 5, HIGH, MODE_INPUT_FLOATING, SPEED_400KHZ, AF_NO

#define IN_1	C, 0, HIGH, MODE_INPUT_PULL_UP, SPEED_40MHZ, AF_NO //IN
#define IN_2	C, 1, HIGH, MODE_INPUT_PULL_UP, SPEED_40MHZ, AF_NO //IN
#define IN_3	C, 2, HIGH, MODE_INPUT_PULL_UP, SPEED_40MHZ, AF_NO //IN
#define IN_4	C, 3, HIGH, MODE_INPUT_PULL_UP, SPEED_40MHZ, AF_NO //IN
#define IN_5	C, 4, HIGH, MODE_INPUT_PULL_UP, SPEED_40MHZ, AF_NO //IN
#define IN_6	C, 5, HIGH, MODE_INPUT_PULL_UP, SPEED_40MHZ, AF_NO //IN


#define OUT_1	C, 6, HIGH, MODE_OUTPUT_OPEN_DRAIN, SPEED_40MHZ, AF_NO //OUT
#define OUT_2	C, 7, HIGH, MODE_OUTPUT_OPEN_DRAIN, SPEED_40MHZ, AF_NO //OUT

/****BUTTON SECTION_END*****************/

#define USART1_RX	A, 10,HIGH, MODE_AF_PUSH_PULL, SPEED_2MHZ, AF7
#define USART1_TX	A, 9, HIGH, MODE_AF_PUSH_PULL, SPEED_2MHZ, AF7

#define PIN_I2C1_SCL		B, 6, HIGH, MODE_AF_OPEN_DRAIN_PULL_UP, SPEED_40MHZ, AF4
#define PIN_I2C1_SDA		B, 7, HIGH, MODE_AF_OPEN_DRAIN_PULL_UP, SPEED_40MHZ, AF4

//#define ENCODER_CH1	B, 4, HIGH, MODE_AF_PUSH_PULL_PULL_UP, SPEED_40MHZ, AF2 // 
//#define ENCODER_CH2	B, 5, HIGH, MODE_AF_PUSH_PULL_PULL_UP, SPEED_40MHZ, AF2 // 
   
#define ENCODER_CH1	A, 6, HIGH, MODE_AF_PUSH_PULL_PULL_DOWN, SPEED_40MHZ, AF2 // 
#define ENCODER_CH2	A, 7, HIGH, MODE_AF_PUSH_PULL_PULL_DOWN, SPEED_40MHZ, AF2 // 

//#define ADC_CH_10       C, 0, HIGH, MODE_ANALOG, SPEED_40MHZ, AF_NO // 
 /* Configure PB.12 (ADC Channel18) as analog input -------------------------*/
//#define ADC_CH_11       C, 1, HIGH, MODE_ANALOG, SPEED_40MHZ, AF_NO // 
 /* Configure PB.12 (ADC Channel20) as analog input -------------------------*/

#define ADC_CH_3       A, 2, HIGH, MODE_ANALOG, SPEED_40MHZ, AF_NO //   
#define ADC_CH_4       A, 3, HIGH, MODE_ANALOG, SPEED_40MHZ, AF_NO //   
  


#define PIN_MCO		A, 8, HIGH, MODE_AF_PUSH_PULL, SPEED_40MHZ, AF0


//button pins define
//----------------------------------------

//----------------------------------------
//

//button flags
uint8_t A_flag = 0;
uint8_t B_flag = 0;
uint8_t C_flag = 0;
uint8_t D_flag = 0;
uint8_t E_flag = 0;
uint8_t F_flag = 0;
uint8_t G_flag = 0;
uint8_t H_flag = 0;
uint8_t I_flag = 0;
uint8_t J_flag = 0;


char pressed_key;
uint16_t button_port  = 0;

uint8_t ENKODER_flag = 0;
uint8_t ADC_start_flag = 0;
uint8_t ADC_data_rdy_flag = 0;
uint8_t Get_current_flag = 0;
uint8_t Get_RTC_flag = 0;


#define USART_RX_BUFF_SIZE (50)
#define SPI1_BUFF_SIZE (50)
#define I2C_RX_BUFF_SIZE (50)

uint8_t t=1;

uint8_t usart_data=0;
uint8_t UsartRxBuffer[USART_RX_BUFF_SIZE];
uint8_t usart1_rx_bit=0;

uint8_t spi1_data=0;
uint8_t spi1_buf[SPI1_BUFF_SIZE];
uint8_t spi1_bit=0;

uint8_t i2c_data=0;
uint8_t i2c_rx_buf[I2C_RX_BUFF_SIZE];
uint8_t i2c_rx_bit=0;

//char tmp=0;
char rv3voltage=0;
char ADC_data=0;

//160521.109,0000.0000,N,00000.0000,E
typedef struct
{
        uint8_t flags;
	uint8_t time[11];	//+1 end string
	uint8_t latid[12];	//+1
	uint8_t longit[13];	//+1
}   gpsData_t;

#define gpsDataSize 37

//extern gpsData_t  gpsData;
gpsData_t  gpsData;
//extern verData_t  verData;
//extern infoData_t infoData;


typedef struct
{
        uint8_t  flags;
	uint8_t  btn_state;	
	uint16_t encoder_state;	
	uint16_t voltage_state;	
        uint16_t current_state;
}   OutData_t;

OutData_t OutData;



#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
