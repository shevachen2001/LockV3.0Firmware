/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/

#include "Tsmxx.h"
#include  "SYSTEM\Sys_GenFun.h"
#include  "I2C\I2c_Soft.h"
#include  "KEY\key.h"
#include  "KEY\TSMxx.h"
#include "Access\Access.h"
#include "Light\light.h"
#include  "KEY\Touch.h"

extern Std_ReturnType power_out_sleep(void);
/****************************************************************************************************
**Function:
   void Tsmxx_Irq_Init(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Tsmxx_Irq_Init(void)
{
	// 配置BUTTON_1管脚为DETECT信号输出，该句很重要，是CPU被GPIO唤醒的必要条件  
	nrf_gpio_cfg_sense_input(TOUCH_INTT_PIN, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_LOW);	 
	NRF_GPIOTE->INTENSET = 1 << 31;
	NVIC_SetPriority(GPIOTE_IRQn, 3);
	NVIC_ClearPendingIRQ(GPIOTE_IRQn);
	NVIC_EnableIRQ(GPIOTE_IRQn);
}
/****************************************************************************************************
**Function:
   void Tsmxx_Irq_Disable(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Tsmxx_Irq_Disable(void)
{
	nrf_gpio_cfg_sense_input(TOUCH_INTT_PIN, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_LOW);	 
	NVIC_DisableIRQ(GPIOTE_IRQn);
}

