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

#include "defines.h"
    



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


void Button_init_vs_irq (void);



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



typedef struct
{
        uint8_t  flags;
	uint16_t  btn_state;	
	uint16_t encoder_state;	
	uint16_t voltage_state;	
    uint16_t current_state;
}   OutData_t;

OutData_t OutData;



#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
