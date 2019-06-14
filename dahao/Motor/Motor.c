#include "PUBLIC\Std_Types.h"
#include "MOTOR\motor.h"
#include "SYSTEM\sys.h"
#include "KEY\Touch.h"
#include "Protocol\Proto_ComDef.h"
#include "FACTORY\Factory.h"


Motor_StateType Motor_State=MOTOR_STAT_IDLE;
Motor_PositType Motor_PositStatus;

Motor_DoorLockType Motor_DoorLockSt;

uint8 Motor_SubState;
uint8 Motor_DetectSw=0xff;
uint16 Motor_Timer64ms;
uint8 Motor_AutoLockTimer64ms;   /* �Զ�����*/
uint8 Motor_SwLR;      /* ���Ҽ��*/
uint8 Motor_DoorSw;    /* �Ŵż�⿪��*/

uint8 MotorLevelSw;     /* ˮƽ��⿪��*/
uint8 MotorVerticalSw; /* ��ֱ��⿪��*/
uint8 MotorBackSw;      /* ��λ��⿪��*/

#define CheckLevelSwOn()      (MotorLevelSw > MOTOR_STATE_ON)
#define ChecVerticalSwOn()    (MotorVerticalSw > MOTOR_STATE_ON)
#define ChecBackSwOn()        (MotorBackSw < MOTOR_STATE_OFF)

#define MOTOR_WORK_TIME    5000/64

unsigned char TravelSwitchLast=0;
unsigned char TravelSwitchNow=0;

extern void Tsmxx_Irq_Init(void);
extern uint8 xbug;
extern void uartSendstring(unsigned char*);;

/****************************************************************************************************
**Function:
   void Motor_Irq_Init(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Motor_Irq_Init(void)
{
//	return;
#if 1
	nrf_gpio_cfg_sense_input(TOUCH_INTT_PIN, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_LOW);	 
       nrf_gpio_cfg_sense_input(DOOR_SW, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
	NRF_GPIOTE->INTENSET = 1<< 31;
	NVIC_SetPriority(GPIOTE_IRQn, 3);
	NVIC_ClearPendingIRQ(GPIOTE_IRQn);
	NVIC_EnableIRQ(GPIOTE_IRQn);
#else
	//  1������Ӧ���ն�����IO�ܽ�����Ϊ����ģʽ
	nrf_gpio_cfg_input(DOOR_SW,NRF_GPIO_PIN_PULLUP);//���ùܽ�λ��������
	
	//  2��ͨ��CONFIG[n]����GPIOTEͨ��
    	NRF_GPIOTE->CONFIG[1] =   (GPIOTE_CONFIG_POLARITY_HiToLo<<GPIOTE_CONFIG_POLARITY_Pos)  //������ʽPOLARITY
                            | (DOOR_SW<<GPIOTE_CONFIG_PSEL_Pos)  //��������OUT[n]�����¼�IN[n]������PSEL
                            | (GPIOTE_CONFIG_MODE_Event<<GPIOTE_CONFIG_MODE_Pos); //ģʽ

	//  3��ͨ��INTENSET�Ĵ�������ʹ���ж�
	NRF_GPIOTE->INTENSET |= (GPIOTE_INTENSET_IN1_Set << GPIOTE_INTENSET_IN1_Pos);// ʹ���ж�����:
	NVIC_ClearPendingIRQ(GPIOTE_IRQn);
	NVIC_EnableIRQ(GPIOTE_IRQn);//ʹ��GPIOTE�ж�
	NVIC_SetPriority(GPIOTE_IRQn,3);//�����ж����ȼ�
#endif
}

/****************************************************************************************************
**Function:
   void Tsmxx_Irq_Disable(void)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Motor_Irq_Disable(void)
{
	//return;
	nrf_gpio_cfg_sense_input(DOOR_SW, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_LOW);	 
	NVIC_DisableIRQ(GPIOTE_IRQn);
}

/****************************************************************************************************
**Function:
   uint8 Motor_CheckLR(void)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8 Motor_CheckLR(void)
{
	uint8 i;
	uint8 LR = 0;
	uint8 ret;
	
	nrf_gpio_cfg_input(SW_LR,NRF_GPIO_PIN_PULLUP);
	Sys_GenFunDelayUs(2);
	for(i=0; i<4; i++)
	{
		if(SW_LRGetData() == 0)
		{
			LR++;
		}
	}
	if(LR>2)
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	nrf_gpio_cfg_output(SW_LR);
	return ret;
}
/****************************************************************************************************
**Function:
	void Access_MotorOpen(void)
**Author: rory
**Description:
**Input: direction:1 ˳ʱ��  0:��ʱ��
**Output: 
****************************************************************************************************/
void Access_MotorWorkOn(uint8 direction)
{
	//return;
	Vdd_PowerOn();
	nrf_gpio_cfg_output(LOCK_M_ADD_PIN);
	nrf_gpio_cfg_output(LOCK_M_MINUS_PIN);
	if(direction == 0)
	{
		nrf_gpio_pin_clear(LOCK_M_ADD_PIN); 
		nrf_gpio_pin_set(LOCK_M_MINUS_PIN);
	}
	else
	{
		nrf_gpio_pin_set(LOCK_M_ADD_PIN); 
		nrf_gpio_pin_clear(LOCK_M_MINUS_PIN);
	}
}

/****************************************************************************************************
**Function:
	void Access_MotorStop(void)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Access_MotorStop(void)
{
	nrf_gpio_cfg_output(LOCK_M_ADD_PIN);
	nrf_gpio_cfg_output(LOCK_M_MINUS_PIN);

	nrf_gpio_pin_clear(LOCK_M_ADD_PIN); 
	nrf_gpio_pin_clear(LOCK_M_MINUS_PIN);
}
/****************************************************************************************************
**Function:
   int8 Motor_Open(void *pData)
**Author:rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Motor_Open(void *pData)
{
	nrf_gpio_cfg_output(COM_COL);
	nrf_gpio_cfg_input(COM_COL1,NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(COM_COL2,NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(SW_LR,NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(REVERSAL_SW,NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(DOOR_SW,NRF_GPIO_PIN_PULLUP);
	Motor_State = MOTOR_STAT_IDLE;
	Motor_DoorLockSt = MOTOR_DOORLOCK_IDLE;
//	Motor_Irq_Init();
	COM_COLHight();
	Motor_SwLR = Motor_CheckLR();
}
	

/****************************************************************************************************
**Function:
    Std_ReturnType Motor_Write(uint8 Cmd,void *pData)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Motor_Write(void *pData)
{
	
	return E_OK;
}

/****************************************************************************************************
**Function:
    Std_ReturnType Motor_Write(uint8 Cmd,void *pData)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Motor_Read(void *pData)
{
	if(Motor_State == MOTOR_STAT_IDLE)
	{
		return E_OK;
	}
	else
	{
		return E_NOT_OK;
	}
}

/****************************************************************************************************
**Function:
    Std_ReturnType Motor_Write(uint8 Cmd,void *pData)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Motor_DetectPosit(void)
{
	nrf_gpio_cfg_input(COM_COL1, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(COM_COL2, NRF_GPIO_PIN_PULLUP);
	
	if(ComCol1GetData() == 0)
	{
		 if(MotorLevelSw<MOTOR_STATE_MAX)
		 {
			   MotorLevelSw++;
		 }
	}
	else
	{
		 if(MotorLevelSw>MOTOR_STATE_MIN)
		 {
			  MotorLevelSw--;
		 }
	}
	
	
	if(ComCol2GetData() == 0)
	{
		  if(MotorVerticalSw<MOTOR_STATE_MAX)
		  {
			   MotorVerticalSw++;
		  }
	}
	else
	{
		  if(MotorVerticalSw>MOTOR_STATE_MIN)
		  {
			   MotorVerticalSw--;
		  }
	}
	
	/*if(ReversalSwGetData() == 0)
	{
		if(MotorBackSw<MOTOR_STATE_MAX)
		{
			MotorBackSw++;
		}
	}
	else
	{
		if(MotorBackSw>MOTOR_STATE_MIN)
		{
			MotorBackSw--;
		}
	}*/

	if(Motor_SwLR == 1)
	{
		 if(CheckLevelSwOn())
		 {
			 Motor_PositStatus = MOTOR_POSIT_UNLOCK;
			 if(Sys_CheckEnableDoorLock())
			 {
				  if(Motor_DoorLockSt == MOTOR_DOORLOCK_IDLE)
				  {
					    Motor_DoorLockSt = MOTOR_DOORLOCK_OPENLOCK;
				  }
			 }
		}
		else if(ChecVerticalSwOn())
		{
			Motor_PositStatus = MOTOR_POSIT_LOCK;
		}
		else
		{
			Motor_PositStatus = MOTOR_POSIT_MIDDLE;
		}
	}
	else
	{
		if(CheckLevelSwOn())
		{
			Motor_PositStatus = MOTOR_POSIT_LOCK;
		}
		else if(ChecVerticalSwOn())
		{
			Motor_PositStatus = MOTOR_POSIT_UNLOCK;
		}
		else
		{
			Motor_PositStatus = MOTOR_POSIT_MIDDLE;
		}
	}
}


/****************************************************************************************************
**Function:
   void Motor_UnLockProc(uint8 LR)
**Author: rory
**Description:
**Input: LR = 1: Left   0:Right
**Output: 
****************************************************************************************************/
void Motor_UnLockProc(uint8 LR)
{
	switch(Motor_SubState)
	{
		case CASE_IN:
		{
       nrf_gpio_cfg_input(REVERSAL_SW, NRF_GPIO_PIN_PULLUP);
			/* �жϷ���*/
			if(LR == 1)
			{/* Left */
				if(CheckLevelSwOn())
				{
					//Access_LightStart(LIGHT_OK,LIGHT_SLOW,1,0);
					Access_BeepStart(BEEP_OPEN,1);
					Motor_SubState = CASE_OK;
					break;
				}
				else
				{
					if(xbug==1)
						 uartSendstring((unsigned char*)"mU-00\r\n\r\n");
					
					Access_MotorWorkOn(1);
					Motor_SubState = CASE_01;
					Motor_Timer64ms = MOTOR_WORK_TIME;
				}
			}
			else
			{/* Right */
				if(ChecVerticalSwOn())
				{
					//Access_LightStart(LIGHT_OK,LIGHT_SLOW,1,0);
					Access_BeepStart(BEEP_OPEN,1);
					Motor_SubState = CASE_OK;
					break;
				}
				else
				{
				  if(xbug==1)
						 uartSendstring((unsigned char*)"mU-11\r\n\r\n");
					
					Access_MotorWorkOn(0);
					Motor_SubState = CASE_01;
					Motor_Timer64ms = MOTOR_WORK_TIME;
				}
			}
			Access_BeepStart(BEEP_NORMAL,1);
			break;
		}
		case CASE_01:
		{/* 2S �ȴ�*/
			if(Motor_Timer64ms == 0)
			{
				Access_MotorStop();
				Access_LightStart(LIGHT_NG,LIGHT_SLOW,1,0);
				Access_BeepStart(BEEP_FAST,3);
				Motor_SubState = CASE_EEROR;
				break;
			}
			if(LR == 1)
			{/* Left */
				if(CheckLevelSwOn()||ComCol1GetData()==0)
				{
					Motor_SubState = CASE_02;
					Motor_Timer64ms = 1;
				}
			}
			else
			{/* Right */
				 if(ChecVerticalSwOn()||ComCol2GetData()==0)
				 {
					   Motor_SubState = CASE_02;
					   if(xbug==1)
						    uartSendstring((unsigned char*)"aaa\r\n\r\n");
						 
					   Motor_Timer64ms = 1;
				 }
			}
			break;
		}
		case CASE_02:
		{
			if(LR == 1)
			{
				 if(Motor_Timer64ms==0||ComCol1GetData()==0)
				 {
					  	 Access_MotorStop();
						   Motor_Timer64ms = 1000/64;
						   Motor_SubState = CASE_03;
						   break;
				 }
			}
			else
			{
					if(Motor_Timer64ms==0||ComCol2GetData()==0)
					{
					  	Access_MotorStop();
						  Motor_Timer64ms = 1000/64;
						  Motor_SubState = CASE_03;
						  break;
					}
		  }
			break;
		}
		case CASE_03:
		{
			if(Motor_Timer64ms != 0)
			{
				 break;
			}
			TravelSwitchLast=ReversalSwGetData();
			if(LR == 1)
			{/* Left */
				 if(xbug==1)
						uartSendstring((unsigned char*)"mU-22\r\n\r\n");
				 
				 Access_MotorWorkOn(0);
			}
			else
			{/* Right */
			   if(xbug==1)
						uartSendstring((unsigned char*)"mU-33\r\n\r\n");
				 
				 Access_MotorWorkOn(1);
			}
			Motor_Timer64ms = 500/64;
			Motor_SubState = CASE_04;
			break;
		}
		case CASE_04:
		{
			if(TravelSwitchLast != ReversalSwGetData())
				 Motor_Timer64ms=0;
			
			if(Motor_Timer64ms != 0)
			{
				 break;
			}

			Motor_Timer64ms = 1500/64;
			Motor_SubState = CASE_05;
			
			break;
		}
		
		case CASE_05:
		{
			if(Motor_Timer64ms == 0)
			{
				Access_MotorStop();
				Access_LightStart(LIGHT_NG,LIGHT_SLOW,1,0);
				Access_BeepStart(BEEP_FAST,3);
				Motor_SubState = CASE_EEROR;
				break;
			}
			
			if(ChecBackSwOn() || TravelSwitchLast != ReversalSwGetData())
			{
			#if 1
				if(LR == 1)
				{/* Left */
					Motor_Timer64ms = 500/64;
				}
				else
				{/* Right */
					Motor_Timer64ms = 0;
				}
			#else
				Motor_Timer64ms = 0;
			#endif
				
				Motor_Timer64ms = 0;
				Motor_SubState = CASE_06;
			}
			break;
		}
		case CASE_06:
		{
			if(Motor_Timer64ms == 0)
			{
				if(LR == 1)
				{/* Left */
					if(xbug==1)
						 uartSendstring((unsigned char*)"mU-44\r\n\r\n");
					
					Access_MotorWorkOn(0);
				}
				else
				{/* Right */
					 //Xiugai
					if(xbug==1)
						 uartSendstring((unsigned char*)"mU-55\r\n\r\n");
					
					Access_MotorWorkOn(1);
				}
				Motor_SubState = CASE_07;
				
				
				if(LR == 1)
					Motor_Timer64ms=4;
				else
					Motor_Timer64ms=6;
				
				break;
			}
			break;
		}
		case CASE_07:
		{
			
			if(Motor_Timer64ms == 0)
			{
				if(xbug==1)
			    uartSendstring((unsigned char*)"mu-66\r\n\r\n");
				
				Access_MotorStop();
				//Access_LightStart(LIGHT_OK,LIGHT_SLOW,1,0);
				Access_BeepStart(BEEP_OPEN,1);	
				Motor_SubState = CASE_END;
				break;
			}
			break;
		}
		case CASE_EEROR:
		case CASE_END:
		{
			break;
		}
	}
}


/****************************************************************************************************
**Function:
    Std_ReturnType Motor_Write(uint8 Cmd,void *pData)
**Author: rory
**Description:
**Input: LR = 1: Left   0:Right
**Output: 
****************************************************************************************************/
void Motor_LockProc(uint8 LR)
{
	switch(Motor_SubState)
	{
		
		case CASE_IN: /* �жϷ���*/
		{
       nrf_gpio_cfg_input(REVERSAL_SW,  NRF_GPIO_PIN_PULLUP);
			
			if(LR == 1)
			{/* Left */
					if( ChecVerticalSwOn() )
					{
						 //Access_LightStart(LIGHT_OK,LIGHT_SLOW,1,0);
						 Access_BeepStart(BEEP_OPEN,1);
						 Motor_SubState = CASE_OK;
						 break;
					}
					else
					{
						 if(xbug==1)
							 uartSendstring((unsigned char*)"mL-00\r\n\r\n");
						
						 Access_MotorWorkOn(0);
						 Motor_SubState = CASE_01;
						 Motor_Timer64ms = MOTOR_WORK_TIME;
					}
			}
			else
			{/* Right */
					if(CheckLevelSwOn())
					{
						//Access_LightStart(LIGHT_OK,LIGHT_SLOW,1,0);
						 Access_BeepStart(BEEP_OPEN,1);
						 Motor_SubState = CASE_OK;
						 break;
					}
					else
					{
						 if(xbug==1)
								uartSendstring((unsigned char*)"mL-11\r\n\r\n");
						 Access_MotorWorkOn(1);
						 Motor_SubState = CASE_01;
						 Motor_Timer64ms = MOTOR_WORK_TIME;
					}
			}
			Access_BeepStart(BEEP_NORMAL,1);
			break;
		}
		case CASE_01:
		{/* 2S �ȴ�*/
								
			if(Motor_Timer64ms == 0)
			{
				Access_MotorStop();
				Access_LightStart(LIGHT_NG,LIGHT_SLOW,1,0);
				Access_BeepStart(LIGHT_VERY_FAST,4);
				Motor_SubState = CASE_EEROR;
				break;
			}
			
			if(LR == 1)
			{/* Left */
				 if(ChecVerticalSwOn() || ComCol2GetData()==0)
				 {
				
						
					  Motor_SubState = CASE_02;
				 	  Motor_Timer64ms = 1;
				 }
			}
			else
			{/* Right */
				 if(CheckLevelSwOn() || ComCol1GetData()==0)
				 {

						
					  Motor_SubState = CASE_02;
					  Motor_Timer64ms = 2;
				 }
			}
			break;
		}
		case CASE_02:
		{
			

			
			if(ComCol1GetData()==0 || ComCol2GetData()==0)
				 Motor_Timer64ms=0;
			
			if(Motor_Timer64ms == 0)
			{
				 Access_MotorStop();
				 Motor_Timer64ms = 1000/64;
				 Motor_SubState = CASE_03;
				 break;
			}
			break;
		}
		
		case CASE_03:
		{
			
			
			if(Motor_Timer64ms != 0)
			{
				 break;
			}
			
			TravelSwitchLast=ReversalSwGetData(); //�г̿���
			if(LR == 1)/* Left */
			{
				  if(xbug==1)
			       uartSendstring((unsigned char*)"mL-33\r\n\r\n");
				  Access_MotorWorkOn(1);
			}
			else/* Right */
			{
			   if(xbug==1)
			      uartSendstring((unsigned char*)"mL-44\r\n\r\n");
				 Access_MotorWorkOn(0);
			}
			Motor_Timer64ms = 500/64;
			Motor_SubState = CASE_04;
			break;
		}
		case CASE_04:
		{
       
			 if(TravelSwitchLast != ReversalSwGetData())
				  Motor_Timer64ms=0;
			 
			 if(Motor_Timer64ms != 0)
			 {
				  break;
			 }
			 Motor_Timer64ms = 1500/64;
			 Motor_SubState = CASE_05;
			 //nrf_gpio_cfg_input(REVERSAL_SW, NRF_GPIO_PIN_PULLUP);
			 break;
		}
		case CASE_05:
		{
			if(ReversalSwGetData() == 0)
			{
				 if(MotorBackSw<MOTOR_STATE_MAX)
				 {
					  MotorBackSw++;
				 }
			}
			else
			{
				 if(MotorBackSw>MOTOR_STATE_MIN)
				 {
					 MotorBackSw--;
				 }
			}
			
			if(Motor_Timer64ms == 0)
			{
				Access_MotorStop();
				Access_LightStart(LIGHT_NG,LIGHT_SLOW,1,0);
				Access_BeepStart(BEEP_FAST,3);
				Motor_SubState = CASE_EEROR;
				break;
			}
			if( ChecBackSwOn()||(TravelSwitchLast != ReversalSwGetData()) )
			{
				Motor_SubState = CASE_06;
				if(LR == 1)
				{/* Left */
					Motor_Timer64ms = 0;//1000/32;
				}
				else
				{
					Motor_Timer64ms = 0;
				}
			}
			break;
		}
		
		case CASE_06:
		{
      if(xbug==1)
			    uartSendstring((unsigned char*)"ML-55\r\n\r\n");
			
			if(Motor_Timer64ms == 0)
			{
				if(LR == 1)
				{/* Left */
				 if(xbug==1)
			       uartSendstring((unsigned char*)"mL-66\r\n\r\n");
					Access_MotorWorkOn(1);
				}
				else
				{/* Right */
					//Xiugai
					if(xbug==1)
			       uartSendstring((unsigned char*)"mL-77\r\n\r\n");
					Access_MotorWorkOn(0);
				}
				Motor_SubState = CASE_07;
				if(LR == 1)
					 Motor_Timer64ms = 5;
				else
				  Motor_Timer64ms = 4;
				
				break;
			}
			break;
		}
		case CASE_07:
		{
				
			 //uartSendstring((unsigned char*)"ML-CASE_07\r\n\r\n");
			
			if((Motor_Timer64ms == 0) /*|| (ChecBackSwOn() == 0)*/)
			{
				if(xbug==1)
			    uartSendstring((unsigned char*)"mL-88\r\n\r\n");
				Access_MotorStop();
				//Access_LightStart(LIGHT_OK,LIGHT_SLOW,1,0);
				Access_BeepStart(BEEP_OPEN,1);	
				Motor_SubState = CASE_END;
				break;
			}
			break;
		}
		case CASE_EEROR:
		case CASE_END:
		{

			break;
		}
	}
}
/****************************************************************************************************
**Function:
    Std_ReturnType Motor_Write(uint8 Cmd,void *pData)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Motor_Proc(void)
{
	if((Motor_DetectSw>0)&&(Motor_DetectSw<4))
	{
		COM_COLHight();
		Motor_DetectPosit();
	}
	
	switch(Motor_State)
	{
		case MOTOR_STAT_IDLE:
		{
			break;
		}
		case MOTOR_STAT_LOCK:
		{
				 Motor_LockProc(Motor_SwLR);
				 if(Motor_SubState == CASE_OK)
				 {
						 Motor_State = MOTOR_STAT_IDLE;
						 break;
				 }
				 else if((Motor_SubState == CASE_END)||(Motor_SubState == CASE_EEROR))
				 {
					  Motor_State = MOTOR_STAT_IDLE;
					  TouchRstTimes = 1;
					  ble_dahao_start_advert(1);
					  break;
				 }
				 break;
		}
		case MOTOR_STAT_UNLOCK:
		{
			Motor_UnLockProc(Motor_SwLR);
			if(Motor_SubState == CASE_OK)
			{
				Motor_State = MOTOR_STAT_IDLE;
				if(Sys_CheckAutoLock())
				{
					Motor_AutoLockTimer64ms = Sys_PataCfg.OpenLockTime/64;
				}
				break;
			}
			else if(Motor_SubState == CASE_EEROR)
			{
				Motor_State = MOTOR_STAT_IDLE;
				TouchRstTimes = 1;
				break;
			}
			else if(Motor_SubState == CASE_END)
			{
				ble_dahao_start_advert(1);
				if(Factory_State != COMD_FATORY_MOTOR_TEST)
				{
					if(Sys_CheckAutoLock())
					{
						Motor_AutoLockTimer64ms = Sys_PataCfg.OpenLockTime/64;
					}
					if(Sys_CheckEnableDoorLock())
					{
						Motor_Timer64ms = 5000/64;
						Motor_State = MOTOR_STAT_WAITOPENDOOR;	
					}
					else
					{
						Motor_State = MOTOR_STAT_IDLE;
					}
					TouchRstTimes = 1;
				}
				else
				{
					Motor_State = MOTOR_STAT_IDLE;
				}
			}
			break;
		}
		case MOTOR_STAT_WAITOPENDOOR:
		{
			if(DoorIsOpen())
			{
				Motor_DoorLockSt = MOTOR_DOORLOCK_OPEN;
				Motor_State = MOTOR_STAT_IDLE;	
				break;
			}
			else if(Motor_Timer64ms == 0)
			{
				Motor_State = MOTOR_STAT_IDLE;
				Motor_DoorLockSt = MOTOR_DOORLOCK_OPENLOCK;
				break;
			}
			break;
		}
		default:
		{	
			Motor_State = MOTOR_STAT_IDLE;
		}
	}
}

/****************************************************************************************************
**Function:
    Std_ReturnType Motor_IoCtl(uint8 Cmd,void *pData)
**Author: rory
**Description:
**Input: 
**Output: 
****************************************************************************************************/
 Std_ReturnType Motor_IoCtl(uint8 Cmd,void *pData)
 {
    uint8_t  pKey[4] = {0xAA, 0xAA, 0xAA, 0xAA};

 	switch(Cmd)
 	{
		case MOTOR_CMD_SCAN:
		{
			if(Motor_Timer64ms != 0)
			{
				Motor_Timer64ms--;
			}
			if(Motor_AutoLockTimer64ms != 0)
			{
				Motor_AutoLockTimer64ms--;
				if(Motor_AutoLockTimer64ms == 0)
				{
					Access_Lock();
#if (defined(SUPPORT_RECORD_LOC_STORE)&&(SUPPORT_RECORD_LOC_STORE == STD_TRUE))
					Access_WriteRecordFlash(pKey, 0, KEY_TYPE_AUTOLOCK, ACCESS_OPEN_LOCK_TPYE);
#endif
				}
			}
			if(Motor_DetectSw == 0xff)
			{
				COM_COLHight();
				nrf_gpio_cfg_input(COM_COL1,NRF_GPIO_PIN_PULLUP);
				nrf_gpio_cfg_input(COM_COL2,NRF_GPIO_PIN_PULLUP);
			}
			else if(Motor_State !=MOTOR_STAT_IDLE)
			{
				COM_COLHight();
				nrf_gpio_cfg_input(COM_COL1,NRF_GPIO_PIN_PULLUP);
				nrf_gpio_cfg_input(COM_COL2,NRF_GPIO_PIN_PULLUP);
				Motor_DetectSw = 2;
				break;
			}
			else if(Motor_DetectSw == 4)
			{
				COM_COLLow();
			}
			else if(Motor_DetectSw > 8)
			{
				Motor_DetectSw = 0xfe;
			}
			Motor_DetectSw++;
			break;
		}
		case MOTOR_CMD_OPEN:
		{
			if(Motor_State == MOTOR_STAT_IDLE)
			{
				Motor_State = MOTOR_STAT_UNLOCK;	
				Motor_SubState = CASE_IN;	
				Motor_SwLR = Motor_CheckLR();
			}
			break;
		}
		case MOTOR_CMD_CLOSE:
		{
			if((Motor_State == MOTOR_STAT_IDLE) || (Motor_State == MOTOR_STAT_WAITOPENDOOR))
			{
				Motor_State = MOTOR_STAT_LOCK;	
				Motor_DoorLockSt = MOTOR_DOORLOCK_IDLE;
				Motor_SubState = CASE_IN;
				Motor_SwLR = Motor_CheckLR();
			}
			break;
		}
		case MOTOR_CMD_SCAN_DOOR:
		{
			nrf_gpio_cfg_input(DOOR_SW,NRF_GPIO_PIN_PULLUP);
			Sys_GenFunDelayUs(2);
			if(DoorSwGetData() == 0)
			{
				if(Motor_DoorSw < 3)
				{
					Motor_DoorSw++;
					if(Motor_DoorSw == 3)
					{
						if((Motor_DoorLockSt == MOTOR_DOORLOCK_OPEN) && (Sys_CheckEnableDoorLock())
							&& (Motor_PositStatus != MOTOR_POSIT_LOCK))
						{
							Access_Lock();
						}
					}
				}
				
#if (defined(SUPPORT_RECORD_LOC_STORE)&&(SUPPORT_RECORD_LOC_STORE == STD_TRUE))
				if(3 == Motor_DoorSw)
				{
				  Access_WriteRecordFlash(pKey, 0, KEY_TYPE_DOOR_CLOSED, ACCESS_OPEN_LOCK_TPYE);
				}
#endif
			}
			else
			{
				if(Motor_DoorSw > 0)
				{
					Motor_DoorSw--;
					if(Motor_DoorSw == 0)
					{
						if(Motor_DoorLockSt == MOTOR_DOORLOCK_OPENLOCK)
						{/* �ű���*/
							Motor_DoorLockSt = MOTOR_DOORLOCK_OPEN;
						}
#if (defined(SUPPORT_RECORD_LOC_STORE)&&(SUPPORT_RECORD_LOC_STORE == STD_TRUE))
						Access_WriteRecordFlash(pKey, 0, KEY_TYPE_DOOR_OPEN, ACCESS_OPEN_LOCK_TPYE);
#endif
					}
				}
			}
			nrf_gpio_cfg_output(DOOR_SW);
			break;
		}
		default:break;
	}
	return E_OK;
 }

/****************************************************************************************************
**Function:
   Std_ReturnType Motor_Close(void *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Motor_Close(void *pData)
{
	nrf_gpio_cfg_output(COM_COL);
	nrf_gpio_cfg_output(COM_COL1);
	nrf_gpio_cfg_output(COM_COL2);
	nrf_gpio_cfg_output(REVERSAL_SW);
	COM_COLLow();
	COM_COL1Low();
	COM_COL2Low();
	ReversalSwLow();
	//Motor_State = MOTOR_STAT_IDLE;
}

const Dr_OpertonType MotorDrive =
{
	Motor_Open, Motor_Close, Motor_Read, Motor_Write, Motor_IoCtl
};

#if (MOTOR_CHECK == STD_TRUE)
void Motor_Check(void)
{
	MotorDrive.open(NULL);
	while(1)
	{
		//Motor_DetectPosit();
	}
}
#endif
