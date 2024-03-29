/***************************************************************************//**
 * @file vUSBMSC.c
 * @brief	USB HOST MSC driver.
 *			It's a low level driver of USB module
 * @author hiroshi murakami
 * @date	20190728
 *
 * This software is released under the MIT License, see LICENSE.md
 ******************************************************************************/
#define _vUSBMSC_C

#include <string.h>		// for memset

//#include "mcc_generated_files/system.h"
#include "p24FJ64GB002.h"
#include "xprintf.h"

#include "v.h"
#include "vUSBMSC.h"
#include "vTMR1.h"

#define USB_ADDRESS_1               1           // ADDRESS = 1
#define U1TOK_PID_TOKEN_SETUP		((UINT16)0b11010000)
#define U1TOK_PID_TOKEN_IN			((UINT16)0b10010000)
#define U1TOK_PID_TOKEN_OUT			((UINT16)0b00010000)
#define U1TOK_EP_EP0				((UINT16)0x00)
#define U1TOK_EP_EP1_IN				((UINT16)0x01)
#define U1TOK_EP_EP2_OUT			((UINT16)0x02)

BDT_ENTRY __attribute__ ((aligned(512)))    BDT[2];
//BDT_ENTRY   *pBDT;

UINT16	*BD;        // BD array access pointer
UINT8 UsbBufCMD64[64];	// Usb buffer for COMMAND
//UINT8 UsbBufDAT512[512];	// Usb buffer for DATA

// USB condition
USB_CONDITION USBcondition;
UINT16	EP0_DATA01, EP1_DATA01, EP2_DATA01;

const UINT8 USBMSC_SetAdrsCommand[]   = {0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};	// SET_ADDRESS contents / address = 1
const UINT8 USBMSC_SetConfigCommand[] = {0x00, 0x09, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};	// SET_CONFIG contents  / config  = 1

//******************************************************************************
/**	
 * @brief	USB interrupts handler (Handle SOF, ATTACH, DETACH, interrupts).
 * @param[in]	U1IRbits.SOFIF
 * @param[in]	U1IRbits.ATTACHIF
 * @param[in]	U1IRbits.DETACHIF
 * @param[out]	USBcondition.SOFCount
 * @param[out]	USBcondition.IsAttach
 * @param[out]	USBcondition.IsLowSpeed
 *   	
 */
void __attribute__((__interrupt__,no_auto_psv)) _USB1Interrupt(void)
{
	if(U1IRbits.SOFIF)	// SOF interrupts? 
    {
		USBcondition.SOFCount++;	// Just increment SOF counter
    } 
    else if(U1IRbits.ATTACHIF && U1IEbits.ATTACHIE)	// Check attache flag 
    {
        U1IEbits.ATTACHIE = 0;	// Disable ATTACH interrupts
        U1IEbits.DETACHIE = 1;	// Enable DETACH interrupts
        USBcondition.IsAttach = 1;	// Set attache flag
        USBcondition.Status = eUSB_AttachWait;
        if(!U1CONbits.JSTATE)	// Connected device is low speed?
        {
            DEBUG_PUTS("LowSpeed!\n");
            USBcondition.IsLowSpeed = 1;	// Set low speed flag
        }
        U1ADDR = USBcondition.IsLowSpeed? 0x80: 0x00;	// Reset USB address (Set MSB if low speed device)
        U1EP0 = USBcondition.IsLowSpeed? 0xcd: 0x4d;	// Enable EP0 & Auto retry off (Set MSB if low speed device)
        DEBUG_PUTS("Attached1\n");	// Show attached message
    } 
    else if(U1IRbits.DETACHIF && U1IEbits.DETACHIE)	// Detach interrupt? 
    {
        U1IEbits.ATTACHIE = 1;	// Disable ATTACH interrupts
        U1IEbits.DETACHIE = 0;	// Enable DETACH interrupts
        USBcondition.IsAttach = 0;	// Set attache flag
        USBcondition.Status = eUSB_initRegister;
        DEBUG_PUTS("Detached1\n");	// Show detached message
    } 
    
    // when you clearing interrupt flags, you should do it following order. 
    // U1EIR->U1IR->U1OTGIR->USB1IF
    U1EIR = 0xff;           // Clear error interrupt flag
    U1IR = 0xff;            // Clear USB status interrupt flag 
    U1OTGIR = 0xff;         // Clear OTG status interrupt flag
	IFS5bits.USB1IF = 0;	// Clear USB1 interrupt flag
    
}

//******************************************************************************
/**	
 * @brief	USB HOST CONTROL initializing.
 *   	
 */
void USBMSC_initRegisters(void)
{
    DEBUG_USB1PUTS("USBMSC_initRegisters\n");
	USBcondition.IsAttach = 0;	// Clear attach flag
	USBcondition.IsLowSpeed = 0;	// Reset USB bus speed
	memset((void*)BDT, 0, 8);	// Clear BDT tables
//	BDT_IN.ADR = (UINT16)UsbBufDAT512;	// UsbBufDAT512 is data buffer
	BDT_IN.ADR = (UINT16)UsbBufCMD64;	// Setup IN BD buffer address (max 64 bytes). *Command buffer.
	BDT_OUT.ADR = (UINT16)UsbBufCMD64;	// Setup SETUP/OUT BD buffer address (max 64 bytes) *Command buffer.
	EP0_DATA01 = 0;		//set DATA0/1 bits, at first = DATA0 
	EP1_DATA01 = 0;		//set DATA0/1 bits, at first = DATA0
	EP2_DATA01 = 0;		//set DATA0/1 bits, at first = DATA0

   	USBcondition.Status = eUSB_initRegister;
	
	U1BDTP1 = (UINT16)BDT >> 8;	// Set BDT address 
    
	U1EIE = 0;      // Disable error interrupt
	U1IE = 0;       // Disable all interrupt
	U1OTGIE = 0;    // Disable USB bus change(OTG check) interrupt
	
	U1EIR = 0xff;   // Clear all USB error interrupts
	U1IR = 0xff;	// Clear all USB interrupts
	U1OTGIR = 0xff;	// Clear all USB interrupts
	memset((void*)&U1EP0, 0, 32);	// Clear all UEP register

	U1PWRCbits.USBPWR = 1;	// Turn power on
	U1ADDR = 0;     // Clear USB address register
	U1SOF = 0x4A;	// Threth hold is 64 bytes
	U1OTGCON = 0x30; // Pull down D+ and D- & VBUS off
	U1CON = 0x08;	// Host mode enable

	U1IEbits.ATTACHIE = 1;	// Enable ATTACH interrupt
	U1IEbits.SOFIE = 1;	// Enable SOFIE interrupt
	U1EIE = 0xbf;	// Enable all error interrupts but U1IEbits.UERRIE is off

	IPC21bits.USB1IP = 1;	// USB interrupt priority is one
	IEC5bits.USB1IE = 1;	// USB interrupt enable
}

//******************************************************************************
/**	
 * @brief	USB transaction 1msec wait for NAK.
 * @param[in,out]	USBcondition	for next step.
 * @details
 * when NAK occur, retry transmission, but wait SOF for 1ms past.
 *   	
 */
void USBMSC_wait1msForNAK(void)
{
    if(USBcondition.SOFCountEx != USBcondition.SOFCount)	// 1ms past?
    {
        USBcondition.Status++;          //next step
    }
}

//******************************************************************************
/**	
 * @brief	USB Transaction Return.
 * @return		eUSB_STATE	is USB status for next step.
 * @param[in]	*pBDT		for checking the return Handshake packet.
 * @param[in,out]	USBcondition	for update status.
 * @details	
 * wait until transaction return.
 * when it return, check U1IRbits for STALL, ERROS or compleated transmission. 
 * when transmission is done, check the return PID for ACK, NAK or other Errors.
 * if ACK then next step, if NACK then retry after SOF changing, mean go back 2 status.
 * and it also check the time out error.
 *   	
 */
enum eUSB_STATE eUSBMSC_checkTransactionReturn(BDT_ENTRY *pBDT)
{
    if(USBcondition.SOFCountEx != USBcondition.SOFCount)	// 1mS past?
    {
        USBcondition.SOFCountEx = USBcondition.SOFCount;	// Update new tick value
    }
// Handle Stall
    if(U1IRbits.STALLIF)	// Check STALL bit 
    {
        U1IRbits.STALLIF = 1;	// Clear STALL bit 
		USBcondition.Status = eUSB_ERR_STALL;
        DEBUG_PUTS( "USB_STALL\n");
        return USBcondition.Status;	// STALL return
    }
// Handle Errors
    if(U1IRbits.UERRIF)	// Check error bit 
    {
		USBcondition.Status = eUSB_ERR_ANY;	// ERROR Return
        DEBUG_PRINTF( "USB_ERR_ANY. U1EIR:0x%x\n", U1EIR);
        return USBcondition.Status;
    }
// Handle data
    else if(U1IRbits.TRNIF)	// Data transfer bit set? 
    {
        U1IRbits.TRNIF = 1;	// Clear data transfer bit 
        USBcondition.BDpid = pBDT->STAT.PID;	// Get PID
        USBcondition.BDbyteCount = pBDT->count;	// Get data count
		DEBUG_USB1PRINTF( "PID:0x%02x\n", USBcondition.BDpid);
        if(USBcondition.BDpid == 0x02 || USBcondition.BDpid == 0x03 || USBcondition.BDpid == 0x0b)	// ACK/DATA0/DATA1 PID means success of transaction
		{
            uiTMR001 = 4;		//clear set 4ms time out trap timer for USB write, it's needed! 
            USBcondition.Status ++;		// Next Step
			return USBcondition.Status;	// return Next Steatus
		} 
        else if(USBcondition.BDpid == 0x0a)	// NAK PID means USB device is not ready
        {
    		USBcondition.Status = USBcondition.Status -2;		// retray but wait SOF change
			return USBcondition.Status;	// return retray with waitting SOF change for NAK
        }
        else if(USBcondition.BDpid == 0x0e)	// STALL PID?
        {
			USBcondition.Status = eUSB_ERR_STALL;	// Return as STALL result
            DEBUG_PUTS( "USB_STALL\n");
			return USBcondition.Status;
        }
        else	// Unknown pid has come
			USBcondition.Status = eUSB_ERR_UnknownPID;	// Return as ERROR result
			DEBUG_PRINTF( "USB_PID_ERROR. Unknow PID:0x%02x\n", USBcondition.BDpid);
			return USBcondition.Status;
    }
    
    if(IS_uiTMR001_FINISH)  //time out trap
    {
        USBcondition.Status = eUSB_ERR_TIMEOUT;
        DEBUG_PUTS( "USB_TIMEOUT_ERROR\n");
        return USBcondition.Status;	// Time Out Error return
    }
	
    return 0;	// Wait return
}

//******************************************************************************
/**	
 * @brief	USB transaction status control.
 * @param[in,out]	USBcondition.
 * @details	
 * this is main program of USB MSC module. it consists of 4 stages.
 * 1.USB module initialize, USB attach, reset USB bus
 * 2.USB set Address, set config, they are CONTROL transfer. it use EP0
 * 3.data in, data out, they are BULK transfer. they use EP1, EP2.
 * 4.Note that, when errors happen the status stops in ERRORs. 
 *   you should initialize the USB status.
 *   	
 */
void USBMSC_statusControl(void)
{
	switch (USBcondition.Status) 
    {
	case  	eUSB_initRegister :
		USBMSC_initRegisters();
		USBcondition.Status++;		//next step
		break;
	case  	eUSB_AttachWait :
		if (USBcondition.IsAttach) 
        {
	        uiTMR001 = 100;		// wait timer set for **ms
			USBcondition.Status++;		//next step
        }
 		break;
	case  	eUSB_AttachStable :
        if(IS_uiTMR001_FINISH)
        {
            DEBUG_USB1PUTS("Attached Stable\n");
			USBcondition.Status++;		//next step
		}
 		break;
	case  	eUSB_resetUsbBus:
		U1CONbits.SOFEN = 0;	// Turn off SOF's, SOF packet start
        U1CONbits.USBRST = 1;	// Invoke reset
        uiTMR001 = 50;          // wait timer set for 50ms
		USBcondition.Status++;          //next step
		break;
	case  	eUSB_resetUsbBusWait50ms:
        if(IS_uiTMR001_FINISH)
        {
            U1CONbits.USBRST = 0;	// Release reset
            U1CONbits.SOFEN = 1;	// Turn on SOF's, SOF packet start
            DEBUG_USB1PUTS("Reset USB Bus\n");
	        uiTMR001 = 10;          // wait timer set for 10ms
            USBcondition.Status++;          //next step
        }
		break;
	case  	eUSB_waitUsbBusStable:
        if(IS_uiTMR001_FINISH)
        {
           USBcondition.Status++;          //next step
        }
		break;
		
    ////////////////////////////////////////////////////////////////////////////
	case  	eUSB_SETUP_setAddress_start:
        DEBUG_USB1PUTS("Set USB Address start\n");
        memcpy(UsbBufCMD64, USBMSC_SetAdrsCommand, 8);	// Copy SET_ADDRESS request packet to send buffer, with address 1.
        USBcondition.Command = eUSB_SETUP_setAddress_start;
        USBcondition.Status = eUSB_SETUP_start;          //SETUP!
		break;
	case  	eUSB_SETUP_setAddress_END:
        U1ADDR = (USBcondition.IsLowSpeed? 0x80: 0x00) + USB_ADDRESS_1;	// Set new address
		xprintf( "Set USB Address. U1ADDR:0x%02x\n", U1ADDR);
        USBcondition.Status++;          //next step
		break;

	case  	eUSB_SETUP_setConfig_start:
        DEBUG_USB1PUTS("Set USB Config start\n");
        memcpy(UsbBufCMD64, USBMSC_SetConfigCommand, 8);	// Copy SET_CONFIG request packet to send buffer.
        USBcondition.Command = eUSB_SETUP_setConfig_start;
        USBcondition.Status = eUSB_SETUP_start;          //SETUP!
		break;
	case  	eUSB_SETUP_setConfig_END:
        xputs("Set USB Config END\n");
        USBcondition.Command = eUSB_IDLE;
        USBcondition.Status = eUSB_IDLE;          //Read & Write command waiting
		break;
    
    ////////////////////////////////////////////////////////////////////////////
	case  	eUSB_SETUP_retryAfterSOF:
        USBMSC_wait1msForNAK();
		break;
	case  	eUSB_SETUP_start:
        DEBUG_USB1PUTS("SETUP start\n");
        BDT_OUT.ADR = (UINT16)UsbBufCMD64;	// SET_CONFIG request packet address to BDT_OUT buffer address.
        BDT_OUT.UOWN_COUNT = 0x8008;	// Setup BD to send 8 byte
        BDT_OUT.STAT.DTS  = 0;		//DATA0/1 bits = 0 at first
        EP0_DATA01 = 1;	// Flip DATA0/1 bits for next
        U1TOK = U1TOK_PID_TOKEN_SETUP | U1TOK_EP_EP0;	//start transaction
        uiTMR001 = 10;     //time out trap
        USBcondition.Status++;          //next step
		break;
	case  	eUSB_SETUP_waitTransactionReturn:
		eUSBMSC_checkTransactionReturn(&BDT_OUT);
		break;
	case  	eUSB_SETUP_checkBusy_retryAfterSOF:
        USBMSC_wait1msForNAK();
		break;
	case  	eUSB_SETUP_checkBusy:
        DEBUG_USB1PUTS("Check Busy\n");
        //next, check the device busy.
        BDT_IN.UOWN_COUNT = 0x80FF;	// Setup BD to get 255 byte
        BDT_IN.STAT.DTS  = 1;
        U1TOK = U1TOK_PID_TOKEN_IN | U1TOK_EP_EP0;	//start transaction
        uiTMR001 = 10;     //time out trap
        USBcondition.Status++;          //next step
		break;
	case  	eUSB_SETUP_waitBusy:
		eUSBMSC_checkTransactionReturn(&BDT_IN);
		break;
	case  	eUSB_SETUP_END:
        DEBUG_USB1PUTS("SETUP END\n");
        USBcondition.Status = ++USBcondition.Command;    //return to command & next step
		break;

    ////////////////////////////////////////////////////////////////////////////
	case  	eUSB_BULK_dataIN_retryAfterSOF:
        USBMSC_wait1msForNAK();
		break;
	case  	eUSB_BULK_dataIN_start:
        DEBUG_USB1PRINTF("DATA IN START,SOF:%d\n",USBcondition.SOFCount);
        BDT_IN.UOWN_COUNT = 0x8040;			//Setup BD to get 64 byte
        BDT_IN.STAT.DTS  = EP1_DATA01;		//set DATA0/1 bits 
        U1TOK = U1TOK_PID_TOKEN_IN | U1TOK_EP_EP1_IN;	//start transaction
        uiTMR001 = 10;		//time out trap
        USBcondition.Status++;		//next step
		break;
	case  	eUSB_BULK_dataIN_waitTransactionReturn:
		eUSBMSC_checkTransactionReturn(&BDT_IN);   //when it gets NACK, then retry.
		break;
	case  	eUSB_BULK_dataIN_END:
        DEBUG_USB1PUTS("DATA IN END \n");
		EP1_DATA01 ^= 1;	// Flip DATA0/1 bits for next
        USBcondition.Status = eUSB_IDLE;          //return to wait
		break;

		
	case  	eUSB_BULK_dataOUT_retryAfterSOF:
        DEBUG_USB1PUTS("DATA OUT waitSOF\n");
        uiTMR001 = 3;		//time wait trap
        USBcondition.Status++;		//next step
        break;
	case  	eUSB_BULK_dataOUT_start:
        if(uiTMR001)		//time wait trap for snail USB writing
        {
            break;
        }
        DEBUG_USB1PRINTF("DATA OUT START,SOF:%u\n",USBcondition.SOFCount);
        BDT_OUT.STAT.DTS  = EP2_DATA01;		//set DATA0/1 bits 
        U1TOK = U1TOK_PID_TOKEN_OUT | U1TOK_EP_EP2_OUT;		//start transaction
        uiTMR001 = 10;		//time out trap
        USBcondition.Status++;		//next step
        break;
	case  	eUSB_BULK_dataOUT_waitTransactionReturn:
		eUSBMSC_checkTransactionReturn(&BDT_OUT);		//when it gets NACK, then retry.
		break;
	case  	eUSB_BULK_dataOUT_END:
        DEBUG_USB1PUTS("DATA OUT END \n");
		EP2_DATA01 ^= 1;		// flip DATA0/1 bits for next
        USBcondition.Status = eUSB_IDLE;		//return to wait
		break;

    ////////////////////////////////////////////////////////////////////////////
	case  	eUSB_IDLE:
		break;

    ////////////////////////////////////////////////////////////////////////////
	case  	eUSB_ERRORS:
		break;
	case  	eUSB_ERR_TIMEOUT:
		break;
	case  	eUSB_ERR_STALL:
		break;
    case	eUSB_ERR_UnknownPID:	
		break;
	case	eUSB_ERR_ANY:	
    	IEC5bits.USB1IE = 0;	// stop USB interrupt
        USBcondition.Status = eUSB_ERR_END;
        xputs("USB_ERR_ANY\n");	// Show Error message
        xprintf( "U1OTGIR:0x%x\n", U1OTGIR);
        xprintf( "U1IR:0x%x\n", U1IR);
        xprintf( "U1EIR:0x%x\n", U1EIR);
		break;
	case	eUSB_ERR_END:	
		break;

	default:
		DEBUG_PRINTF( "USB_DEFAULT!! status:%d\n", USBcondition.Status);
	}

}

