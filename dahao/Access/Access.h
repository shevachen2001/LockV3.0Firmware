/*****************************************************************************
|  File Name: DataUp.h
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
| 2016-05      Rory     Initial version 
|****************************************************************************/

#ifndef __ACCESS_H
#define __ACCESS_H
#include "PUBLIC\Std_Types.h"
#include "Card\Rc522.h"
#include "Beep\Beep.h"
#include "Light\light.h"

#define ACCESS_CARDBUILDID_MAX    			4

#define ACCESS_RECORD_ONCE_MAX				14
#define ACCESS_RECORD_MAX				80 //(512-1)
#define ACCESS_RECORD_RDMAX				256

#define ACCESS_LEARN_RECORD_MAX				100
#define ACCESS_RECORD_SEND_ONCE_MAX			16


#define ACCESS_OPEN_LOCK_TPYE	       1
#define ACCESS_CLOSE_LOCK_TPYE	0
#define ACCESS_NG_TPYE	                   0x02


#define ACCESS_FLASH_INDEX_NULL	0xffff



#define ACCESS_USERID_CARD		       0x88888888
#define ACCESS_USERID_FOREVER		0x66666666

#define FLASH_ONCEBLOCK_SIZE							128*1024




#define ACCESS_SYNC_PASWD_MAX    10



#define ACCESS_PARA_OFFSET				0		
#define ACCESS_SYS_PARA_OFFSET			FLASH_ONCEBLOCK_SIZE*1		



#define ACCESS_USERID_APP_OFFSET 					FLASH_ONCEBLOCK_SIZE*2
#define ACCESS_USERID_PSWD_OFFSET 					FLASH_ONCEBLOCK_SIZE*3
#define ACCESS_USERID_CARD_OFFSET 					FLASH_ONCEBLOCK_SIZE*4
#define ACCESS_USERID_FINGERPRINT_OFFSET 			FLASH_ONCEBLOCK_SIZE*5
#define ACCESS_USERID_IDENTITY_OFFSET 				FLASH_ONCEBLOCK_SIZE*6
#define ACCESS_USERID_QRCODE_OFFSET 				FLASH_ONCEBLOCK_SIZE*7

#define ACCESS_BLACK_LIST_OFFSET                 FLASH_ONCEBLOCK_SIZE*2
#define ACCESS_SERVE_KEY_OFFSET                  FLASH_ONCEBLOCK_SIZE*3
#define ACCESS_APP_KEY_OFFSET                      FLASH_ONCEBLOCK_SIZE*4



#define ACCESS_ATTTIME_APP_OFFSET 					FLASH_ONCEBLOCK_SIZE*10
#define ACCESS_ATTTIME_PSWD_OFFSET 					FLASH_ONCEBLOCK_SIZE*11
#define ACCESS_ATTTIME_CARD_OFFSET 					FLASH_ONCEBLOCK_SIZE*12
#define ACCESS_ATTTIME_FINGERPRINT_OFFSET 			FLASH_ONCEBLOCK_SIZE*13
#define ACCESS_ATTTIME_IDENTITY_OFFSET 				FLASH_ONCEBLOCK_SIZE*14
#define ACCESS_ATTTIME_QRCODE_OFFSET 				FLASH_ONCEBLOCK_SIZE*15


#define ACCESS_USERID_MAX 							100
#define ACCESS_VISITID_MAX 							50
#define ACCESS_USERID_OFFSET 					0


#define ACCESS_APP_OFFSET 				FLASH_ONCEBLOCK_SIZE*20
#define ACCESS_PASSWORD_OFFSET			FLASH_ONCEBLOCK_SIZE*21
#define ACCESS_CARD_OFFSET				FLASH_ONCEBLOCK_SIZE*22
#define ACCESS_FINGERPRINT_OFFSET		FLASH_ONCEBLOCK_SIZE*23
#define ACCESS_IDENTITY_CARD_OFFSET		FLASH_ONCEBLOCK_SIZE*24
#define ACCESS_IDENTITY_QRCODE_OFFSET	FLASH_ONCEBLOCK_SIZE*25


#define ACCESS_RECORD_OPEN_OFFSET		FLASH_ONCEBLOCK_SIZE*28	
#define ACCESS_RECORD_LEARN_OFFSET		FLASH_ONCEBLOCK_SIZE*30

#define LOW_BATTERY_VOLTAGE_MILLIVOLT (4800)


/******************************************************************************
**********************Macro definition*******************************************
******************************************************************************/
enum
{
	WORK_NORMAL, /* 关*/
	WORK_NORMALLY_OPEN,   /* 常开*/
	WORK_LEARN_CARD,
	WORK_TEST,
};

enum
{
	TYPE_LOCK_HOME = 0, /* 家用门锁*/
	TYPE_LOCK_FLAT, /* 公寓锁*/ 
	TYPE_LOCK_CAR, /* 车位锁*/
	TYPE_KEY, /* 蓝牙电子钥匙*/
	TYPE_LOCK_HANDUP, /* 电子挂锁*/
	TYPE_LOCK_ACCESS , /* 门禁*/
	TYPE_LOCK_ACCESS_WIG , /* 门禁韦根输出*/
	TYPE_LOCK_ACCESS_REMOTE, /* 远程控制门禁*/
	TYPE_LOCK_VIDEO, /* 可视对讲门禁*/
	TYPE_LOCK_BIKE, /* 远程控制门禁*/
	TYPE_LOCK_BIKE_REMOTE, /* 远程控制门禁*/
	TYPE_LOCK_HOTEL_REMOTE = 11,  /* 远程控制酒店公寓门锁*/
	TYPE_LOCK_HOTEL = 12,  /* 酒店公寓门锁*/
	TYPE_LOCK_FIGNERPRINT , /* 指纹锁*/
	TyPE_LOCK_LORA_GATEWAY = 16, /* Lora网关*/
	TyPE_LOCK_SEND_CARD = 31, /* 发卡机*/
	TYPE_LAB = 0x20, /* 蓝牙电子标签*/
};


enum
{
	KEY_TYPE_ALL = 0,
	KEY_TYPE_APP,
	KEY_TYPE_PASSWORD,
	KEY_TYPE_CARD,
	KEY_TYPE_57CARD = KEY_TYPE_CARD,
	KEY_TYPE_FINGERPRINT,
	KEY_TYPE_IDENTITY_CARD, // 5
	KEY_TYPE_QRCODE , // 二维码透传
	KEY_TYPE_USERID,
	KEY_TYPE_CLEAR_ALL,// 清除所有的钥匙
	KEY_TYPE_LONG_KEY_PRESS_LOCK = 10,
	KEY_TYPE_TIME_ONE, /*6位单个访客密码*/
	KEY_TYPE_AUTOLOCK,
	KEY_TYPE_TIME_INDEX, /*6位代索引访客密码*/
	KEY_TYPE_TIME_SLOT, /*6位时间段访客密码*/
	KEY_TYPE_TIME_DATE,  // 15 /*8位开门密码*/
	KEY_TYPE_DOOR_OPEN,
	KEY_TYPE_DOOR_CLOSED,
	KEY_TYPE_MECHANICAL_KEY,  /* 机械钥匙开门*/
	RECORD_EMERGENCY_CARD, /* 应急卡*/
	RECORD_TOTAL_CARD, // 20/* 总卡*/
	RECORD_STAFF_CARD, /* 员工卡限时*/
	RECORD_STAFF_LOOP_CARD, /* 员工循环*/
	RECORD_CUSTOM_CARD, /* 客人卡*/
  RECORD_INSPECTION_CARD, /* 巡检卡*/
	KEY_TYPE_ONE_TIME, // 25// 一次性密码
	KEY_TYPE_REBOOT,
	KEY_TYPE_LOW_BATTERY,
	KEY_TYPE_FOREVER, // 永久密码
	KEY_TYPE_TIME_SLOT_7, //7位 时间点
	KEY_TYPE_CLEAR, // 30 // 清空密码
	KEY_TYPE_TIME_SLOT_8, //8位 时间点
	ACCESS_INFO_LOCK_STATUS, /*  门状态信息推送:  act:   bit0:门状态(1:开 0:关) bit1:反锁(1:反锁)*/
	ACCESS_FALSE_LOCK_ALARM, /*  假锁报警*/
	ACCESS_LEARN_IDENTITY_CARD_TPYE, /* 身份证*/
	ACCESS_LEARN_CARD_TPYE, // 35
	
	KEY_TYPE_FAST_APP_NOID_OPEN = 0xf1,  //APP快速开门
	KEY_TYPE_FAST_PSWD_OPEN,//访客码快速开门
	KEY_TYPE_FAST_CARD_OPEN,//卡片快速开门
	KEY_TYPE_FAST_QRCODE_OPEN,//二维码快速开门
};

enum
{
	ACCESS_DET_WAIT = 0,
	ACCESS_DET_M1,
	ACCESS_DET_T5557ST,
	ACCESS_DET_T5557STOP,
};

enum
{
	CARD_TYPE_STAFF_LOOP = 0,  /* 员工卡限制时间点:每日*/
	CARD_TYPE_STAFF,  /* 员工卡 有效期*/
	CARD_TYPE_LOSS, /* 挂失卡*/
	CARD_TYPE_EMERGENCY, /* 应急卡 */
	CARD_TYPE_ALL, /* 总卡*/
	CARD_TYPE_INSPECTION, /* 巡检卡*/
	CARD_TYPE_TESK, /* 测试卡 */
	CARD_TYPE_GUEST, /* 客人卡*/
	CARD_TYPE_NULL, /* 无效卡*/
};


enum
{
	ACCESS_AUTO_TEST_IDLE= 0,
	ACCESS_AUTO_TEST_CLOCK,
	ACCESS_AUTO_TEST_BLE,
	ACCESS_AUTO_TEST_IC_ID_IDENTITY,	
	ACCESS_AUTO_TEST_KEYTABLE,
	ACCESS_AUTO_TEST_WIFI,
	ACCESS_AUTO_TEST_GPRS,
};

#define LOCK_HOME   0
#define LOCK_FLAT    1
#define LOCK_HOTEL_FLAT    2
#define LOCK_HOTEL_FLAT_REMOTE    3

#if (defined LORA_ENABLE) && (LORA_ENABLE == STD_TRUE)	
#define LOCK_TYPE   LOCK_HOTEL_FLAT_REMOTE
#else
#define LOCK_TYPE   LOCK_HOTEL_FLAT
#endif


#define LOCK_TOUCH_REMOTE_OPEN_TIME    20000/64

//#define ACCESS_FASTCMPMAX	1024
//#define ACCESS_FASTCMPMIN	512
#define OPEN_REVERSE_DOOR			0x01   /* 可以开反锁门*/
#define OPEN_COMMOM_DOOR			0x02   /* 可以公共门*/
#define LOSS_OLD_CARD			       0x03   /* 挂失先前卡*/

/******************************************************************************
**********************Type statement*******************************************
******************************************************************************/
typedef struct {
	uint8 WegeMode; /* 0: 34; 1:26 */
	uint8 ErrorTimes;
	//uint8 State;
	uint8 DefaultTimes;
	uint8 UserIndex;
}Access_ParaType;	

typedef struct {
	uint16 UserIndex; 
	uint16 FlashIndex;
}Access_IdIndxType;	

typedef struct{
	uint8 keyId;//[ACCESS_SYNC_PASWD_MAX];
	uint8 KeyIndex;
	//uint8 KeyMax;
	//uint8 Year;
	//uint8 Month;
	//uint8 Day;
}Access_SyncPaswdType;

typedef struct {
	uint8 	FlashInitFlag;
}Access_flahInfoType;

typedef struct {
	uint8  cardtype;
	uint16 hotelId;
	uint32 deviceId;
	uint32 devicePaswd;
	uint16 HotelPaswd;
	uint32 Timest;
	uint32 Timeend;
	//uint8 build[ACCESS_CARDBUILDID_MAX];
}Access_CardDatType;

typedef struct {
	uint8 TypeResult;//操作类似| 操作结果
	uint32 Id;//钥匙ID
	uint32 Time;//操作时间
}AccRcordDataType;

typedef struct {
	uint16 Wrecordindex;//写记录索引0~
	uint16 Srecordindex;//发送记录索引 0~
	uint16 recordnum;//总记录个数
	uint32 TimerBk;//时间
	//uint8 BatteryBk[5];
	uint32 CustomTimerBk; /* 客人卡的挂失时间*/
	uint32 StaffTimerBk; /* 员工卡的挂失时间*/
	uint32 AllTimerBk; /* 总卡的挂失时间*/
	uint32 EmergencyTimerBk; /* 应急卡的挂失时间*/
	uint32 AppTimerBk; /* APP开锁时间*/
	AccRcordDataType RecordList[ACCESS_RECORD_MAX+1];
	uint32 KeyWord;
}AccRcordParaType;

/******************************************************************************
**********************Variable definition*******************************************
******************************************************************************/
extern Access_ParaType Access_Globle;
extern uint16 Access_ResetDeviceTimer64;
extern uint16 Access_OpenRedIndex ;
extern uint16 Access_OpenWriteaIndex ;
extern uint16 Access_Store_Recordx64ms;
extern uint8 protoAnaly_netCommType;

extern uint8 Access_MotorTimer64;
extern uint16 Access_LockTimer64;
extern uint16 Access_DefaultTimer64;
extern uint16 Access_CardLearnTimer64;
extern uint16 Access_BatteryData;
extern AccRcordParaType AccRcord;
extern uint32 Access_UserId_Temp;
extern uint16 Access_LockDeviceTimer64ms;
extern uint8 Access_LockStatus;
/****************************************************************************/
/* Function                                                                 */
/****************************************************************************/
extern void Access_Init(void);
extern void Access_Unlock(void);
extern void Access_Lock(void);
extern void Access_OpenError(void);


extern Std_ReturnType Access_ComTime(uint32 startTime, uint32 endTime);
extern void Access_LearnCardOk(uint8 idtpye,uint8*cardid);
extern void Access_CardProcess(uint8 idtpye, uint8* pUid,Access_CardDatType *CardDat);

extern void Access_FlashArrang(void);
extern Std_ReturnType Access_GetSupportType(uint8 idtype);
extern void Access_TimerProc(void);

extern void Access_EraseAllKey(uint8 idtpye);
extern void Access_DefaultPaswd(void);
extern void Access_SendRecordProc(void);
extern void Access_WriteRecordFlash(uint8* pKeyId,uint32 time ,uint8 type, uint8 action);
extern void Access_BeepStart(Beep_StateType mode, uint8 times);
extern void Access_LightStart(Light_NameType index,Light_StateType mode, uint8 times,uint16 data);
//extern void Access_MotorOpen(void);
//extern void Access_MotorClose(void);
//extern void Access_MotorStop(void);
extern void Vdd_PowerOff(void);

extern uint8 Access_CardDatProc(Card_ParmType *pdat, Access_CardDatType *pAccess_CardDat);
extern uint8  Access_GetRecord(uint16 index,uint8 *poutdat);
#define Access_DelayReset(x)   Access_ResetDeviceTimer64 = x/64

extern void Access_DetCardProc(void);
extern void Access_UpdateStatus(void);
#endif	
/************************ (C) COPYRIGHT DaHao electronics *****END OF FILE****/

