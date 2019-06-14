/*****************************************************************************
|  File Name: Flash.c
|
|  Description: 
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2002-2012 by DAHAO TECHNOLOGY Co., Ltd. All rights reserved.
|
|               This software is copyright protected and proprietary 
|               to DAHAO TECHNOLOGY Co., Ltd. DAHAO TECHNOLOGY Co., Ltd 
|               grants to you only those rights as set out in the 
|               license conditions. All other rights remain with 
|               DAHAO TECHNOLOGY Co., Ltd.
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     -------------------------------------
| LXW          LXW               DAHAO TECHNOLOGY Co., Ltd
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date        Ver     Author  Description
| ---------   ----    ------  ------------------------------------------------
| 2013-10 1.00    lxw     Initial version 
|****************************************************************************/

/****************************************************************************/ 
/* Include file                                                             */                                                 
/****************************************************************************/

#include "Flash\Flash.h"
//#include "WatchDog\WatchDog.h"
#include"SYSTEM\Sys_GenFun.h"
#include "nrf_drv_common.h"
#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "Card\Rc522.h"



static Std_ReturnType Flash_SpiSectorErase(void *SectorAddr);
static Std_ReturnType Flash_SpiBulkErase(void *SectorAddr);
static void Flash_SpiPageWrite(uint8* pBuffer, uint32 WriteAddr, uint16 NumByteToWrite);
static void Flash_SpiBufferWrite(uint8* pBuffer, uint32 WriteAddr, uint16 NumByteToWrite);
static void Flash_SpiBufferRead(uint8* pBuffer, uint32 ReadAddr, uint16 NumByteToRead);
//static uint32 Flash_SpiReadID(void);
//static void Flash_SpiStartReadSequence(uint32 ReadAddr);
//static uint8 Flash_SpiReadByte(void);
extern uint8 Flash_SpiSendByte(uint8 byte);
static void Flash_SpiWriteEnable(void);
//static void Flash_SpiWriteStatus(uint8 Data);
static void Flash_SpiWaitForWriteEnd(void);
static void Flash_SpiWaitForWriteEndEx(void);

extern uint8  Sys_McuRestFlag;
extern bool spi_xfer_done;  //SPI数据传输完成标志
extern const nrf_drv_spi_t spi;/// = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
uint8 flash_openflag= 0;

/****************************************************************************************************
**Function:
void Flash_SpiSectorErase(uint32 SectorAddr)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Flash_SpiSectorErase(void *SectorAddr)
{
#if (defined WATCHDOG_EN) && (WATCHDOG_EN == STD_TRUE)
	//WatchDogDrive.write(NULL);
#endif
	/* Send write enable instruction */
	Flash_SpiWriteEnable();
	/* Sector Erase */
	/* Select the FLASH: Chip Select low */
	Flash_SpiCsLow();
	/* Send Sector Erase instruction */
	Flash_SpiSendByte(FLASH_CMD_SE);
	/* Send SectorAddr high nibble address byte */
	Flash_SpiSendByte(((*((uint32*)SectorAddr)) & 0xFF0000) >> 16);
	/* Send SectorAddr medium nibble address byte */
	Flash_SpiSendByte(((*((uint32*)SectorAddr)) & 0xFF00) >> 8);
	/* Send SectorAddr low nibble address byte */
	Flash_SpiSendByte((*((uint32*)SectorAddr)) & 0xFF);
	/* Deselect the FLASH: Chip Select high */
	Flash_SpiCsHigh();
	/* Wait the end of Flash writing */
	Flash_SpiWaitForWriteEndEx();
	return E_OK;
}
/****************************************************************************************************
**Function:
void Flash_SpiSectorErase(uint32 SectorAddr)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Flash_SpiBlockErase(void *BlockAddr)
{
#if (defined WATCHDOG_EN) && (WATCHDOG_EN == STD_TRUE)
	//WatchDogDrive.write(NULL);
#endif
	/* Send write enable instruction */
	Flash_SpiWriteEnable();
	/* Sector Erase */
	/* Select the FLASH: Chip Select low */
	Flash_SpiCsLow();
	/* Send Sector Erase instruction */
	Flash_SpiSendByte(FLASH_CMD_BL);
	/* Send SectorAddr high nibble address byte */
	Flash_SpiSendByte(((*((uint32*)BlockAddr)) & 0xFF0000) >> 16);
	/* Send SectorAddr medium nibble address byte */
	Flash_SpiSendByte(((*((uint32*)BlockAddr)) & 0xFF00) >> 8);
	/* Send SectorAddr low nibble address byte */
	Flash_SpiSendByte((*((uint32*)BlockAddr)) & 0xFF);
	/* Deselect the FLASH: Chip Select high */
	Flash_SpiCsHigh();
	/* Wait the end of Flash writing */
	Flash_SpiWaitForWriteEndEx();
	return E_OK;
}

/****************************************************************************************************
**Function:
void Flash_SpiBulkErase(void)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Flash_SpiBulkErase(void* SectorAddr)
{
#if (defined WATCHDOG_EN) && (WATCHDOG_EN == STD_TRUE)
				//WatchDogDrive.write(NULL);
#endif
	/* Send write enable instruction */
	Flash_SpiWriteEnable();
	/* Bulk Erase */
	/* Select the FLASH: Chip Select low */
	Flash_SpiCsLow();
	/* Send Bulk Erase instruction  */
	Flash_SpiSendByte(FLASH_CMD_BE);
	/* Deselect the FLASH: Chip Select high */
	Flash_SpiCsHigh();
	/* Wait the end of Flash writing */
	Flash_SpiWaitForWriteEndEx();
	return E_OK;
}
/****************************************************************************************************
**Function:
void Flash_SpiBulkErase(void)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Flash_SpiPageWrite(uint8* pBuffer, uint32 WriteAddr, uint16 NumByteToWrite)
{

#if 0
	uint32 i;
	if (NumByteToWrite < 2)
	{
		return ;
	}
	if (NumByteToWrite % 2)
	{
		NumByteToWrite--;
	}
	/* Enable the write access to the FLASH */
	Flash_SpiWriteEnable();
	/* Select the FLASH: Chip Select low */
	Flash_SpiCsLow();
	/* Send "Write to Memory " instruction */
	Flash_SpiSendByte(FLASH_CMD_AAI);
	/* Send WriteAddr high nibble address byte to write to */
	Flash_SpiSendByte((WriteAddr & 0xFF0000) >> 16);
	/* Send WriteAddr medium nibble address byte to write to */
	Flash_SpiSendByte((WriteAddr & 0xFF00) >> 8);
	/* Send WriteAddr low nibble address byte to write to */
	Flash_SpiSendByte(WriteAddr & 0xFF);
	Flash_SpiSendByte(*pBuffer++);
	Flash_SpiSendByte(*pBuffer++);
	/* Deselect the FLASH: Chip Select high */
	Flash_SpiCsHigh();
	Flash_SpiWaitForWriteEnd();
	NumByteToWrite -= 2;
	for (i = 0; i < NumByteToWrite / 2; i++)
	{
		Flash_SpiCsLow();
		Flash_SpiSendByte(FLASH_CMD_AAI);
		Flash_SpiSendByte(*pBuffer++);
		Flash_SpiSendByte(*pBuffer++);
		Flash_SpiCsHigh();
		/* Wait the end of Flash writing */
		Flash_SpiWaitForWriteEnd();
	}
	Flash_SpiCsLow();
	Flash_SpiSendByte(FLASH_CMD_DISWR);
	Flash_SpiCsHigh();
	Flash_SpiWaitForWriteEnd();
#else
#if (defined WATCHDOG_EN) && (WATCHDOG_EN == STD_TRUE)
			  //WatchDogDrive.write(NULL);
#endif
  /* Enable the write access to the FLASH */
  Flash_SpiWriteEnable();
  /* Select the FLASH: Chip Select low */
  Flash_SpiCsLow();
  /* Send "Write to Memory " instruction */
  Flash_SpiSendByte(FLASH_CMD_WRITE);
  /* Send WriteAddr high nibble address byte to write to */
  Flash_SpiSendByte((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  Flash_SpiSendByte((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  Flash_SpiSendByte(WriteAddr & 0xFF);
  /* while there is Data to be written on the FLASH */



    while(NumByteToWrite!= 0)
	{

		if(NumByteToWrite>128)
		{
			APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, pBuffer, 128, NULL, 0));
			NumByteToWrite-= 128;
			pBuffer+=128;
		}
		else 
		{
			APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, pBuffer, NumByteToWrite, NULL, 0));
			NumByteToWrite-= NumByteToWrite;
			pBuffer+=NumByteToWrite;

		}
	}
/*  while (NumByteToWrite--)
  {
	Flash_SpiSendByte(*pBuffer);
	pBuffer++;
  }
  
  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, pBuffer, NumByteToWrite, NULL, 0));
  */
  /* Deselect the FLASH: Chip Select high */
  Flash_SpiCsHigh();
  /* Wait the end of Flash writing */
  Flash_SpiWaitForWriteEnd();
#endif
}
/****************************************************************************************************
**Function:
void Flash_SpiBufferWrite(uint8* pBuffer, uint32 WriteAddr, uint16 NumByteToWrite)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Flash_SpiBufferWrite(uint8* pBuffer, uint32 WriteAddr, uint16 NumByteToWrite)
{
	uint8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

	Addr = WriteAddr % SPI_FLASH_PAGESIZE;
	count = SPI_FLASH_PAGESIZE - Addr;
	NumOfPage =  NumByteToWrite / SPI_FLASH_PAGESIZE;
	NumOfSingle = NumByteToWrite % SPI_FLASH_PAGESIZE;
	if (Addr == 0) /* WriteAddr is SPI_FLASH_PAGESIZE aligned  */
	{
		if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PAGESIZE */
		{
			Flash_SpiPageWrite(pBuffer, WriteAddr, NumByteToWrite);
		}
		else /* NumByteToWrite > SPI_FLASH_PAGESIZE */
		{
			while (NumOfPage--)
			{
				Flash_SpiPageWrite(pBuffer, WriteAddr, SPI_FLASH_PAGESIZE);
				WriteAddr +=  SPI_FLASH_PAGESIZE;
				pBuffer += SPI_FLASH_PAGESIZE;
			}
			Flash_SpiPageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	else /* WriteAddr is not SPI_FLASH_PAGESIZE aligned  */
	{
		if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PAGESIZE */
		{
			if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PAGESIZE */
			{
				temp = NumOfSingle - count;

				Flash_SpiPageWrite(pBuffer, WriteAddr, count);
				WriteAddr +=  count;
				pBuffer += count;
				Flash_SpiPageWrite(pBuffer, WriteAddr, temp);
			}
			else
			{
				Flash_SpiPageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else /* NumByteToWrite > SPI_FLASH_PAGESIZE */
		{
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / SPI_FLASH_PAGESIZE;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PAGESIZE;
			Flash_SpiPageWrite(pBuffer, WriteAddr, count);
			WriteAddr +=  count;
			pBuffer += count;
			while (NumOfPage--)
			{
				Flash_SpiPageWrite(pBuffer, WriteAddr, SPI_FLASH_PAGESIZE);
				WriteAddr +=  SPI_FLASH_PAGESIZE;
				pBuffer += SPI_FLASH_PAGESIZE;
			}
			if (NumOfSingle != 0)
			{
				Flash_SpiPageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}

/****************************************************************************************************
**Function:
void Flash_SpiBufferRead(uint8* pBuffer, uint32 ReadAddr, uint16 NumByteToRead)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Flash_SpiBufferRead(uint8* pBuffer, uint32 ReadAddr, uint16 NumByteToRead)
{
	/* Select the FLASH: Chip Select low */
	Flash_SpiCsLow();
	/* Send "Read from Memory " instruction */
	Flash_SpiSendByte(FLASH_CMD_READ);
	/* Send ReadAddr high nibble address byte to read from */
	Flash_SpiSendByte((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte to read from */
	Flash_SpiSendByte((ReadAddr& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte to read from */
	Flash_SpiSendByte(ReadAddr & 0xFF);

	while(NumByteToRead != 0)
	{
		if(NumByteToRead> 128)
		{
			APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, NULL, 0, pBuffer, 128));
			NumByteToRead-=128;
			pBuffer+= 128;
		}
		else 
		{
			APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, NULL, 0, pBuffer, NumByteToRead));
			NumByteToRead-=NumByteToRead;
			pBuffer+= NumByteToRead;
		}
	}
/*	while (NumByteToRead--)
	{
		*pBuffer = Flash_SpiSendByte(DUMMY_BYTE);
		pBuffer++;
	}*/
	
//	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, NULL, 0, pBuffer, NumByteToRead));
	/* Deselect the FLASH: Chip Select high */
	Flash_SpiCsHigh();
}
#if 0
/****************************************************************************************************
**Function:
uint32 Flash_SpiReadID(void)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint32 Flash_SpiReadID(void)
{
	uint32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	/* Select the FLASH: Chip Select low */
	Flash_SpiCsLow();
	/* Send "RDID " instruction */
	Flash_SpiSendByte(0x9F);
	/* Read a byte from the FLASH */
	Temp0 = Flash_SpiSendByte(DUMMY_BYTE);
	/* Read a byte from the FLASH */
	Temp1 = Flash_SpiSendByte(DUMMY_BYTE);
	/* Read a byte from the FLASH */
	Temp2 = Flash_SpiSendByte(DUMMY_BYTE);
	/* Deselect the FLASH: Chip Select high */
	Flash_SpiCsHigh();
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	return Temp;
}
#endif
#if 0
/****************************************************************************************************
**Function:
void Flash_SpiStartReadSequence(uint32 ReadAddr)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Flash_SpiStartReadSequence(uint32 ReadAddr)
{
	/* Select the FLASH: Chip Select low */
	Flash_SpiCsLow();
	/* Send "Read from Memory " instruction */
	Flash_SpiSendByte(FLASH_CMD_READ);
	/* Send the 24-bit address of the address to read from ---------------------*/
	/* Send ReadAddr high nibble address byte */
	Flash_SpiSendByte((ReadAddr & 0xFF0000) >> 16);
	/* Send ReadAddr medium nibble address byte */
	Flash_SpiSendByte((ReadAddr& 0xFF00) >> 8);
	/* Send ReadAddr low nibble address byte */
	Flash_SpiSendByte(ReadAddr & 0xFF);
}
#endif
#if 0
/****************************************************************************************************
**Function:
uint8 Flash_SpiReadByte(void)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8 Flash_SpiReadByte(void)
{
  return (Flash_SpiSendByte(DUMMY_BYTE));
}
#endif
/****************************************************************************************************
**Function:
uint8 Flash_SpiSendByte(uint8 byte)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
uint8 Flash_SpiSendByte(uint8 byte)
{
#if 0
	uint8 i;
	uint8 dattemp =  0;
	RC522_MosiLow();
	for(i= 0;i< 8;i++)
	{
		dattemp<<= 1;
		RC522_ClkLow(); 		
		if(byte&0x80)
		{
			RC522_MosiHigh();
		}
		else 
		{
			RC522_MosiLow();
		}		
		byte<<=1;
		RC522_ClkHigh();					
		if(RC522_GetMisoDat())
		{
			dattemp|= 0x01;
		}								
	}
	return dattemp;
#else 
	uint8 spi_rx_buf = 0xff;
//	spi_xfer_done = false;
	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, &byte, 1, &spi_rx_buf, 1));
//	while(spi_xfer_done);
	return (spi_rx_buf);
#endif
}
/****************************************************************************************************
**Function:
void Flash_SpiWriteEnable(void)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Flash_SpiWriteEnable(void)
{
	/* Select the FLASH: Chip Select low */
	Flash_SpiCsLow();
	/* Send "Write Enable" instruction */
	Flash_SpiSendByte(FLASH_CMD_WREN);
	/* Deselect the FLASH: Chip Select high */
	Flash_SpiCsHigh();
}
#if 0
/****************************************************************************************************
**Function:
void Flash_SpiWriteEnable(void)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Flash_SpiWriteStatus(uint8 Data)
{
	/* Select the FLASH: Chip Select low */
	Flash_SpiCsLow();
//	Flash_SpiSendByte(FLASH_CMD_EWRSR);
	/* Deselect the FLASH: Chip Select high */
	Flash_SpiCsHigh();
	/* Select the FLASH: Chip Select low */
	Flash_SpiCsLow();
	Flash_SpiSendByte(FLASH_CMD_WRSR);
	Flash_SpiSendByte(Data);
	/* Deselect the FLASH: Chip Select high */
	Flash_SpiCsHigh();
}
#endif
/****************************************************************************************************
**Function:
void Flash_SpiWaitForWriteEnd(void)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Flash_SpiWaitForWriteEnd(void)
{
	#if 1
	uint32 timeout=10000;
	uint8 flashstatus = 0;
	/* Select the FLASH: Chip Select low */
	Flash_SpiCsLow();
	/* Send "Read Status Register" instruction */
	Flash_SpiSendByte(FLASH_CMD_RDSR);
	/* Loop as long as the memory is busy with a write cycle */
	do
	{
		/* Send a dummy byte to generate the clock needed by the FLASH
		and put the value of the status register in FLASH_Status variable */
		flashstatus = Flash_SpiSendByte(DUMMY_BYTE);
	}
	while (((flashstatus & WIP_FLAG) == SET)&&(timeout--!= 0)); /* Write in progress */
	if(timeout == 0)
	{
		Sys_McuRestFlag = STD_TRUE;
	}
	/* Deselect the FLASH: Chip Select high */
	Flash_SpiCsHigh();
	#endif 
}


/****************************************************************************************************
**Function:
void Flash_SpiWaitForWriteEnd(void)
**Author: lxw 
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Flash_SpiWaitForWriteEndEx(void)
{
	#if 1
	uint32 timeout=50000;
	uint8 flashstatus = 0;
	/* Select the FLASH: Chip Select low */
	Flash_SpiCsLow();
	/* Send "Read Status Register" instruction */
	Flash_SpiSendByte(FLASH_CMD_RDSR);
	/* Loop as long as the memory is busy with a write cycle */
	do
	{
		/* Send a dummy byte to generate the clock needed by the FLASH
		and put the value of the status register in FLASH_Status variable */
		flashstatus = Flash_SpiSendByte(DUMMY_BYTE);
	}
	while (((flashstatus & WIP_FLAG) == SET)&&(timeout--!= 0)); /* Write in progress */
	if(timeout == 0)
	{
		Sys_McuRestFlag = STD_TRUE;
	}
	/* Deselect the FLASH: Chip Select high */
	Flash_SpiCsHigh();
	#endif 
}

/****************************************************************************************************
**Function:
   int8 Flash_Open(void *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Flash_Open(void *pData)
{
	if(flash_openflag == STD_TRUE)
	{
		return ;
	}
	Hc595_PowerOn();
	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG(SPI_INSTANCE);

	nrf_gpio_cfg_output(RC522_CS_PIN);
	nrf_gpio_cfg_output(RC522_RST_PIN);
#if (defined LORA_ENABLE) && (LORA_ENABLE == STD_TRUE)
	nrf_gpio_cfg_output(LORA_CS_PIN);
#endif
	nrf_gpio_cfg_output(FLASH_CS_PIN);	
/*	spi_config.miso_pin = RC522_MISO_PIN;
	spi_config.mosi_pin = RC522_MOSI_PIN;
	spi_config.sck_pin = RC522_MSCK_PIN;
	nrf_drv_spi_init(&spi, &spi_config, NULL);
*/
	nrf_gpio_cfg_output(RC522_MSCK_PIN);
	nrf_gpio_cfg_output(RC522_MOSI_PIN);
	nrf_gpio_cfg_input(RC522_MISO_PIN,NRF_GPIO_PIN_PULLUP);	
	nrf_gpio_cfg_output(FLASH_CS_PIN);	
	
	RC522_SetCsHigh();
	Flash_SpiCsHigh();
#if (defined LORA_ENABLE) && (LORA_ENABLE == STD_TRUE)
	LORA_SetCsHigh();
#endif
	flash_openflag = STD_TRUE;
}
/****************************************************************************************************
**Function:
   Std_ReturnType Flash_Close(void *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
void Flash_Close(void *pData)
{
	if(flash_openflag != STD_TRUE)
	{
		return ;
	}
	nrf_gpio_cfg_output(RC522_MSCK_PIN);
	nrf_gpio_cfg_output(RC522_MOSI_PIN);
	nrf_gpio_cfg_output(RC522_MISO_PIN);	
	nrf_gpio_cfg_output(FLASH_CS_PIN);	

//	nrf_drv_spi_uninit(&spi);

	RC522_ClkLow();
	RC522_MosiLow();
	RC522_MisoLow();
	RC522_SetCsLow();
	Flash_SpiCsLow();
	//  flash  power
	Hc595_PowerOff();
	flash_openflag = STD_FALSE;
}
/****************************************************************************************************
**Function:
   Std_ReturnType Flash_Read(void *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Flash_Read(void *pData)
{
	Std_ReturnType Err;
	if(((Flash_ComType *)pData)->pData == NULL)
	{
		
		Err = E_INPAR_ERR;
	}
	else 
	{
#if (defined WATCHDOG_EN) && (WATCHDOG_EN == STD_TRUE)
		//WatchDogDrive.write(NULL);
#endif
		Flash_SpiBufferRead(((Flash_ComType *)pData)->pData,((Flash_ComType *)pData)->Add,((Flash_ComType *)pData)->Lenth);
		Err = E_OK;
	}
	return Err;
}
/****************************************************************************************************
**Function:
   Std_ReturnType Flash_Write(void *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Flash_Write(void *pData)
{
	Std_ReturnType Err;
	if(((Flash_ComType *)pData)->pData == NULL)
	{
		
		Err = E_INPAR_ERR;
	}
	else 
	{
#if (defined WATCHDOG_EN) && (WATCHDOG_EN == STD_TRUE)
		//WatchDogDrive.write(NULL);
#endif
		Flash_SpiBufferWrite(((Flash_ComType *)pData)->pData,((Flash_ComType *)pData)->Add,((Flash_ComType *)pData)->Lenth);
		Err = E_OK;
	}
	return Err;
}
/****************************************************************************************************
**Function:
   int8 Canuser_Write(void *pData)
**Author: lxw
**Description:
**Input: 
**Output: 
****************************************************************************************************/
Std_ReturnType Flash_IoCtl(uint8 Cmd,void *pData)
{
#if (defined WATCHDOG_EN) && (WATCHDOG_EN == STD_TRUE)
	//WatchDogDrive.write(NULL);
#endif
	switch(Cmd )
	{
		case FLASH_ERASE_SECT:
		{
			return Flash_SpiSectorErase(pData);
		};
		case FLASH_ERASE_BLOCK:
		{
			return Flash_SpiBlockErase(pData);
		};
		case FLASH_ERASE_CHIP:		
		{
			return	Flash_SpiBulkErase(pData);
		};
		default: return E_NOT_OK;
	}
}

const Dr_OpertonType FlashDrive =
{
	//Flash_Open,Flash_Close, Flash_Read, Flash_Write,Flash_IoCtl
	Flash_Open,Flash_Close, NULL, NULL,NULL
};

#if (FLASH_CHECK == STD_TRUE)
/*******************************************************************************
* Function Name  : 
void Can_Check(void)
* Description    : 
*                  reset values.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Flash_Check(void)
{
	uint16 count;
	
	static uint8 Databuffer[512];
	Flash_ComType Message;
	Message.pData= Databuffer;
	Message.Add = 0;
	Message.Lenth = 100;
	FlashDrive.open(NULL);

	
	FlashDrive.read(&Message);
	for(count = 0 ;count < 512;count++)
	{
		Databuffer[count] = count;
	}
	Message.Add = 128*1024;//FLASH_ONCEBLOCK_SIZE;
	for(;;)
	{
//		Num = Flash_SpiReadID();
		FlashDrive.ioctl(FLASH_ERASE_SECT,&(Message.Add));
		FlashDrive.write(&Message);
		memset(Databuffer,0,100);
		Message.Add = 0;
		Message.Lenth = 100;
		FlashDrive.read(&Message);
		#if 1
	//	TxMessage.Lenth= 10;
	//	FlashDrive.write((void *)&TxMessage);
		#else
		FlashDrive.read((void *)&TxMessage);
		#endif
		while(1)
		{
			;
		}
	}
}
#endif 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/




