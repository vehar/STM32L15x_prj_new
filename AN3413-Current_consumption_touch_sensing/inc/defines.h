#pragma once

#include "stm32l1xx.h"
#include "mcu_gpio.h"

#define VERSION "1.0"
#define DATE "03.09.13"

//#define HardwareVersion 1
#define SoftVersion 2

#define bool _Bool
#define false 0
#define true !false





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

















extern bool majorInterrupt;



//#pragma pack(push, 1)
/*
struct
{
    uint8_t tagPresent;
    RfidData data;
}	tagData;

#define tagDataSize 33
*/

typedef struct
{
    uint8_t hardwVer;
    uint8_t SoftwVer;
}	 verData_t;
#define verDataSize 2

typedef struct
{
    uint16_t   p12;   //+12
    uint16_t   p5;    //-5
    uint16_t   m5;    //+5
    uint16_t   p100;  //+100
    uint16_t   m100;  //-100
    uint16_t  temp;
} infoData_t;
#define infoDataSize 12

//160521.109,0000.0000,N,00000.0000,E
typedef struct
{
        uint8_t flags;
	uint8_t time[11];	//+1 end string
	uint8_t latid[12];	//+1
	uint8_t longit[13];	//+1
}   gpsData_t;
#define gpsDataSize 37

extern gpsData_t  gpsData;
extern verData_t  verData;
extern infoData_t infoData;


