/*****************************************************************************
|  File Name: Usart_bsp.h
|
|  Description: Header File defined
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     -------------------------------------
| LXW          LXW               
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date        Ver     Author  Description
| ---------   ----    ------  ------------------------------------------------
| 2012-09      LXW     Initial version 
|****************************************************************************/
/* Check if information is already included */
#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H


#include <stdint.h>
#include "nrf_gpio.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_gpiote.h"
//#include "bsp.h"
//#include "app_error.h"
#include "nrf.h"
#include "Config.h"


/****************************************************************************/
/* macros                                                                   */
/****************************************************************************/

#define EI()  __enable_irq()//__asm("ei")
#define	DI()  __disable_irq()//__asm("di")
#define	NOP()  __asm("nop")
#define	HALT()  __asm("halt")

/****************************************************************************/
/* data types                                                               */
/****************************************************************************/
typedef 	signed char 			int8;		
typedef 	unsigned	short			uint16; 	
typedef 	short 					int16;		
typedef 	unsigned	long		uint32; 
typedef 	long					int32;		
typedef 	float					fp32;		
typedef 	double					fp64;	

typedef 	unsigned char			uint8;
typedef 	unsigned char			BYTE;


/****************************************************************************/
/* data types                                                               */
/****************************************************************************/

/****************************************************************************/
/* IO Ddfeine                                                             */
/****************************************************************************/

/****************************************************************************/
/* RF                                               */
/****************************************************************************/

/****************************************************************************/
/* ADC                                              */
/****************************************************************************/
#define ADC_PIN			 3


/****************************************************************************/
/* USART                                               */
/****************************************************************************/
#define UART1_GPIO							GPIOA  
#define UART1_TXD_PIN						GPIO_Pin_9
#define UART1_RXD_PIN						GPIO_Pin_10

#define UART1_485_PIN						GPIO_Pin_11 
#define UART1_485_GPIO						GPIOA

#define UART1_485Low()			      //GPIO_WriteBit(UART1_485_GPIO,UART1_485_PIN,Bit_RESET)
#define UART1_485High()			      //GPIO_WriteBit(UART1_485_GPIO,UART1_485_PIN,Bit_SET)




#define UART2_GPIO							GPIOA  
#define UART2_TXD_PIN						GPIO_Pin_2
#define UART2_RXD_PIN						GPIO_Pin_3


#define UART2_485_PIN						GPIO_Pin_13 
#define UART2_485_GPIO						GPIOC

#define UART2_485Low()			      	//GPIO_WriteBit(UART2_485_GPIO,UART2_485_PIN,Bit_RESET)
#define UART2_485High()			      	//GPIO_WriteBit(UART2_485_GPIO,UART2_485_PIN,Bit_SET)




#define UART6_GPIO							GPIOC 
#define UART6_TXD_PIN						GPIO_Pin_6
#define UART6_RXD_PIN						GPIO_Pin_7

#define UART6_485_PIN						GPIO_Pin_8 
#define UART6_485_GPIO						GPIOC

#define UART6_485Low()			      GPIO_WriteBit(UART6_485_GPIO,UART6_485_PIN,Bit_RESET)
#define UART6_485High()			      GPIO_WriteBit(UART6_485_GPIO,UART6_485_PIN,Bit_SET)

#define UART3_GPIO							GPIOB  
#define UART3_TXD_PIN						GPIO_Pin_10
#define UART3_RXD_PIN						GPIO_Pin_11


/****************************************************************************/
/* led                                               */
/****************************************************************************/

#define DET_PIN						GPIO_Pin_5
#define DET_GPIO						GPIOB

#define GetDetPin()			      GPIO_ReadInputDataBit(DET_GPIO,DET_PIN)

/****************************************************************************/
/* LED                                               */
/****************************************************************************/
#define RUN_PIN						GPIO_Pin_8
#define RUN_GPIO						GPIOB
#define RunLow()			      	GPIO_WriteBit(RUN_GPIO,RUN_PIN,Bit_RESET)
#define RunHigh()			  		GPIO_WriteBit(RUN_GPIO,RUN_PIN,Bit_SET)


/* Lock Relay */
#define LOCK_M_ADD_PIN			1
#define LOCK_M_MINUS_PIN		2


/* Door Bell */
#define DOOR_BELL_PIN			 GPIO_Pin_3
#define DOOR_BELL_GPIO		        GPIOB

#define DoorBell_Open()                 GPIO_WriteBit(DOOR_BELL_GPIO,DOOR_BELL_PIN,Bit_SET)
#define DoorBell_Close()                 GPIO_WriteBit(DOOR_BELL_GPIO,DOOR_BELL_PIN,Bit_RESET)


/****************************************************************************/
/* Alarm                                                           */
/****************************************************************************/
#define ALARM_PIN						GPIO_Pin_0
#define ALARM_GPIO						GPIOA

#define Alarm_GetData()			      GPIO_ReadInputDataBit(ALARM_GPIO, ALARM_PIN)

/****************************************************************************/
/*Reset CC2540                                                            */
/****************************************************************************/
#define RESET_CC2540_GPIO			GPIOC 
#define RESET_CC2540_PIN			GPIO_Pin_15
#define ResetCc2540Low()				GPIO_WriteBit(RESET_CC2540_GPIO, RESET_CC2540_PIN, Bit_RESET)
#define ResetCc2540High()				GPIO_WriteBit(RESET_CC2540_GPIO, RESET_CC2540_PIN, Bit_SET)


#define ResetWifiLow()				 	ResetCc2540Low()				
#define ResetWifiHigh()					ResetCc2540High()				


#define GprsWkupLow()				
#define GprsWkup()				


#define PWM125K_GPIO				GPIOB //	GPIOA 
#define PWM125K_PIN					GPIO_Pin_14//	GPIO_Pin_11
#define Pwm125kLow()				GPIO_WriteBit(PWM125K_GPIO, PWM125K_PIN, Bit_RESET)
#define Pwm125kHigh()				GPIO_WriteBit(PWM125K_GPIO, PWM125K_PIN, Bit_SET)

#define T5557DAT_GPIO			GPIOA 
#define T5557DAT_PIN				GPIO_Pin_1
#define T557Dat_GetData()			      GPIO_ReadInputDataBit(T5557DAT_GPIO, T5557DAT_PIN)




#define I2C_SOFT_CLK_PIN			17


#define I2c_SoftClockSetOutput() nrf_gpio_cfg_output(I2C_SOFT_CLK_PIN) 

#define I2c_SoftClockSetInput()   nrf_gpio_cfg_input(I2C_SOFT_CLK_PIN,NRF_GPIO_PIN_NOPULL)

								 
#define I2c_SoftClockGetData()     nrf_gpio_pin_read(I2C_SOFT_CLK_PIN)

#define I2c_SoftClockLow()           nrf_gpio_pin_clear(I2C_SOFT_CLK_PIN) 
#define I2c_SoftClockhigh()          nrf_gpio_pin_set(I2C_SOFT_CLK_PIN) 

#define I2C_SOFT_DAT_PIN			18


#define I2c_SoftDatSetOutput()   	nrf_gpio_cfg_output(I2C_SOFT_DAT_PIN) 
#define I2c_SoftDatSetInput()  		nrf_gpio_cfg_input(I2C_SOFT_DAT_PIN,NRF_GPIO_PIN_NOPULL) 


#define I2c_SoftDatGetData()        nrf_gpio_pin_read(I2C_SOFT_DAT_PIN)
#define I2c_SoftDatLow()           	nrf_gpio_pin_clear(I2C_SOFT_DAT_PIN)
#define I2c_SoftDathigh()          	nrf_gpio_pin_set(I2C_SOFT_DAT_PIN) 


#define BEEP_PIN       24

#define Beep_OutPutHigh()               nrf_gpio_pin_set(BEEP_PIN) 
#define Beep_OutPutLow()                nrf_gpio_pin_clear(BEEP_PIN)


#define TOUCH_SET_RST_PIN       16
#define Touch_SetRstLow()           	nrf_gpio_pin_clear(TOUCH_SET_RST_PIN)
#define Touch_SetRstHigh()          	nrf_gpio_pin_set(TOUCH_SET_RST_PIN) 


#define TOUCH_INTT_PIN       19
#define TouchIntatGetData()        		nrf_gpio_pin_read(TOUCH_INTT_PIN)

#define TOUCH_I2CEN_PIN       20
#define Touch_I2cEnLow()           	nrf_gpio_pin_clear(TOUCH_I2CEN_PIN)
#define Touch_I2cEnHigh()          	nrf_gpio_pin_set(TOUCH_I2CEN_PIN) 


/* RC522 RST  */
#define RC522_RST_PIN			 15

#define RC522_SetRstHigh()         nrf_gpio_pin_set(RC522_RST_PIN)
#define RC522_SetRstLow()          nrf_gpio_pin_clear(RC522_RST_PIN)


/* LORA CS  */

#define LORA_CS_PIN			 8

#define LORA_SetCsHigh()       nrf_gpio_pin_set(LORA_CS_PIN)
#define LORA_SetCsLow()        nrf_gpio_pin_clear(LORA_CS_PIN)


/* RC522 CS  */
#define RC522_CS_PIN			 13

#define RC522_SetCsHigh()       nrf_gpio_pin_set(RC522_CS_PIN)
#define RC522_SetCsLow()        nrf_gpio_pin_clear(RC522_CS_PIN)

#define FLASH_CS_PIN			 14

#define Flash_SpiCsHigh()       nrf_gpio_pin_set(FLASH_CS_PIN)
#define Flash_SpiCsLow()        nrf_gpio_pin_clear(FLASH_CS_PIN)

/* MF522_MSCK  */
#define RC522_MSCK_PIN			 12

#define RC522_ClkHigh()       NRF_GPIO->OUTSET = 0x1000//(1UL << RC522_MSCK_PIN);
//nrf_gpio_pin_set(RC522_MSCK_PIN)
#define RC522_ClkLow()        NRF_GPIO->OUTCLR = 0x1000//(1UL << RC522_MSCK_PIN);
//nrf_gpio_pin_clear(RC522_MSCK_PIN)


/* MF522_MISO  */
#define RC522_MISO_PIN			 10
#define RC522_GetMisoDat()      nrf_gpio_pin_read(RC522_MISO_PIN)
#define RC522_MisoHigh()        nrf_gpio_pin_set(RC522_MISO_PIN)
#define RC522_MisoLow()         nrf_gpio_pin_clear(RC522_MISO_PIN)


/* MF522_MOSI  */
#define RC522_MOSI_PIN			 11
#define RC522_MosiHigh()       NRF_GPIO->OUTSET =0x800 //(1UL << RC522_MOSI_PIN);
//nrf_gpio_pin_set(RC522_MOSI_PIN)
#define RC522_MosiLow()        NRF_GPIO->OUTCLR = 0x800//(1UL << RC522_MOSI_PIN);
//nrf_gpio_pin_clear(RC522_MOSI_PIN)





#define HC595_DAT			 	RC522_MISO_PIN
#define Hc595_Dat_High()       nrf_gpio_pin_set(HC595_DAT)
#define Hc595_Dat_Low()        nrf_gpio_pin_clear(HC595_DAT)

#define HC595_SCLK			 	RC522_MSCK_PIN
#define Hc595_Sclk_High()       nrf_gpio_pin_set(HC595_SCLK)
#define Hc595_Sclk_Low()        nrf_gpio_pin_clear(HC595_SCLK)

#define HC595_LCLK			 	21
#define Hc595_Lclk_High()       nrf_gpio_pin_set(HC595_LCLK)
#define Hc595_Lclk_Low()        nrf_gpio_pin_clear(HC595_LCLK)


//#define LIGHT_BACKUP	 		0
//#define Light_BackupOn()       nrf_gpio_pin_set(LIGHT_BACKUP)
//#define Light_BackupOff()        nrf_gpio_pin_clear(LIGHT_BACKUP)


#define HC595_POWER	 			0
#define Hc595_PowerOn()         nrf_gpio_pin_clear(HC595_POWER)
#define Hc595_PowerOff()         nrf_gpio_pin_set(HC595_POWER)

#define VDD_POWER	 			25
#define Vdd_PowerOn()         nrf_gpio_pin_set(VDD_POWER)
//#define Vdd_PowerOff()        


#define CFG_KEY_PIN       30
#define CfgKeyGetData()        		nrf_gpio_pin_read(CFG_KEY_PIN)



#define LORA_INTT_PIN       9
#define LoraIntatGetData()        		nrf_gpio_pin_read(LORA_INTT_PIN)


#define LORA_RST	 			7
#define LORA_RstHigh()         nrf_gpio_pin_set(LORA_RST)
#define LORA_RstLow()        	nrf_gpio_pin_clear(LORA_RST) 

#define COM_COL                             14
#define COM_COLHight()		nrf_gpio_pin_set(COM_COL)
#define COM_COLLow()		nrf_gpio_pin_clear(COM_COL)

#define COM_COL1                             22
#define ComCol1GetData()       nrf_gpio_pin_read(COM_COL1)
#define COM_COL1Hight()		nrf_gpio_pin_set(COM_COL1)
#define COM_COL1Low()		nrf_gpio_pin_clear(COM_COL1)

#define COM_COL2                             23
#define ComCol2GetData()        nrf_gpio_pin_read(COM_COL2)
#define COM_COL2Hight()		nrf_gpio_pin_set(COM_COL2)
#define COM_COL2Low()		nrf_gpio_pin_clear(COM_COL2)

#define SW_LR                            4
#define SW_LRGetData()    nrf_gpio_pin_read(SW_LR)
#define SW_LRHight()		nrf_gpio_pin_set(SW_LR)
#define SW_LRLow()		nrf_gpio_pin_clear(SW_LR)

#define DOOR_SW                      5
#define DoorSwGetData()    nrf_gpio_pin_read(DOOR_SW)
#define DoorSwHight()	  nrf_gpio_pin_set(DOOR_SW)
#define DoorSwLow()		  nrf_gpio_pin_clear(DOOR_SW)

#define REVERSAL_SW                      6
#define ReversalSwGetData()    nrf_gpio_pin_read(REVERSAL_SW)
#define ReversalSwHight()	        nrf_gpio_pin_set(REVERSAL_SW)
#define ReversalSwLow()		 nrf_gpio_pin_clear(REVERSAL_SW)
#endif /* ifndef PLATFORM_TYPES_H */
/************************** end of file *************************************/
