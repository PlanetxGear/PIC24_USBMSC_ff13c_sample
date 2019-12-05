/***************************************************************************//**
 * @file vUSBMSC.h
 * @brief	USB HOST MSC driver.
 *			It's a low level driver of USB module
 * @author hiroshi murakami
 * @date	20190728
 *
 * This software is released under the MIT License, see LICENSE.md
 ******************************************************************************/
#ifndef _vUSBMSC_H
#define _vUSBMSC_H

#include "integer.h"

/*****************************
 * STATE
 *****************************/
enum eUSB_STATE {
    eUSB_initRegister = 0,
// Attach
    eUSB_AttachWait,
    eUSB_AttachStable,
// rest USB Bus
    eUSB_resetUsbBus,
    eUSB_resetUsbBusWait50ms,
    eUSB_waitUsbBusStable,
// setAddress,
    eUSB_SETUP_setAddress_start,
    eUSB_SETUP_setAddress_END,
// setConfig,
    eUSB_SETUP_setConfig_start,
    eUSB_SETUP_setConfig_END,

// CONTROL transfer
    eUSB_SETUP_retryAfterSOF,
        eUSB_SETUP_start,
        eUSB_SETUP_waitTransactionReturn,
        eUSB_SETUP_checkBusy_retryAfterSOF,
        eUSB_SETUP_checkBusy,
        eUSB_SETUP_waitBusy,
        eUSB_SETUP_END,

// BULK transfer
    eUSB_BULK_dataIN_retryAfterSOF,
        eUSB_BULK_dataIN_start,
        eUSB_BULK_dataIN_waitTransactionReturn,
        eUSB_BULK_dataIN_END,

    eUSB_BULK_dataOUT_retryAfterSOF,
        eUSB_BULK_dataOUT_start,
        eUSB_BULK_dataOUT_waitTransactionReturn,
        eUSB_BULK_dataOUT_END,

// IDLE or END or wait next data.
    eUSB_IDLE,

// Errors. you should initialize USB status.
    eUSB_ERRORS,
        eUSB_ERR_NODEVICE,
        eUSB_ERR_TIMEOUT,
        eUSB_ERR_STALL,
        eUSB_ERR_UnknownPID,
        eUSB_ERR_ANY,
        eUSB_ERR_END,

// when it happened, this code has bug.        
    eUSB_defaut
};

/*****************************
 * DEFINES
 *****************************/
#define setUSB_INITIALIZE()     (USBcondition.Status = eUSB_initRegister)
#define setUSB_DATAIN()         (USBcondition.Status = eUSB_BULK_dataIN_start)
#define setUSB_DATAOUT() 		(USBcondition.Status = eUSB_BULK_dataOUT_start)
#define isUSB_DETACH()          (USBcondition.Status <= eUSB_AttachWait)
#define isUSB_IDLE()            (USBcondition.Status == eUSB_IDLE)
#define isUSB_INITIALIZED()     (eUSB_SETUP_END < USBcondition.Status && USBcondition.Status <= eUSB_IDLE)
#define isUSB_NOT_INITIALIZED() ((USBcondition.Status <= eUSB_SETUP_END) || (eUSB_ERRORS <= USBcondition.Status))
#define BDT_IN				BDT[0]           // EP0 IN Buffer Descriptor
#define BDT_OUT				BDT[1]           // EP0 OUT Buffer Descriptor

// Buffer Descriptor Status Register layout.
typedef union _BD_STAT
{
    struct{
        unsigned            :2;     //Byte count
        unsigned    BSTALL  :1;     //Buffer Stall Enable
        unsigned    DTSEN   :1;     //Data Toggle Synch Enable
        unsigned            :2;     //Reserved - write as 00
        unsigned    DTS     :1;     //Data Toggle Synch Value
        unsigned    UOWN    :1;     //USB Ownership
    };
    struct{
        unsigned            :2;
        unsigned    PID0    :1;
        unsigned    PID1    :1;
        unsigned    PID2    :1;
        unsigned    PID3    :1;
    };
    struct{
        unsigned            :2;
        unsigned    PID     :4;     // Packet Identifier
    };
    UINT8            Val;
} BD_STAT;                      //Buffer Descriptor Status Register


// BDT Entry Layout
typedef union __BDT
{
    union
    {
        struct
        {
            UINT8       CNT;
            BD_STAT     STAT __attribute__ ((packed));
        };
        struct
        {
            UINT16        count:10;   //test
            UINT8        :6;
            UINT16        ADR; //Buffer Address
        };
        struct
        {
            UINT16        UOWN_COUNT;   //test
            UINT16        ADR_; //Buffer Address
        };
    };
    UINT32              Val;
    UINT16              v[2];
} BDT_ENTRY;

typedef struct __USB_CONDITION
{
 	enum eUSB_STATE		Status;
 	enum eUSB_STATE		Command;
	UINT16	IsLowSpeed;     //Low speed device flag. 1:Low Speed 0:Full Speed,  
	UINT16	IsAttach;       //Attache flag. 1:attach 0:detach
	UINT16	SOFCount;       // SOF freame counter
	UINT16	SOFCountEx;     // Ex SOF freame counter (prior counter value)
	UINT16	BDpid;          // BD PID save
	UINT16	BDbyteCount;    // BD transfar byte count save
#ifdef _V_DEBUG_SCSI2
	UINT16	SOFCountSt;     // SOF freame counter save for SCSI DEBUG
#endif
} USB_CONDITION;

/*****************************
 * VARIABLES
 *****************************/
extern UINT8 UsbBufCMD64[64];	// Usb buffer for COMMAND
//extern UINT8 UsbBufDAT512[512];	// Usb buffer for DATA

extern USB_CONDITION	USBcondition;
extern BDT_ENTRY		BDT[2];


/*****************************
 * PROTOTYPES
 *****************************/
void USBMSC_initRegisters(void);
void USBMSC_statusControl(void);


#endif
