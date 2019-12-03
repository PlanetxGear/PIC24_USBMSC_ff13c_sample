#include "diskio.h"
#include "ffconf.h"        /* FatFs lower layer API */
#include "vUSBMSC.h"
#include "vSCSI.h"

/* Definitions of physical drive number for each media */
#define DEV_USB        0
 
 
//typedef struct
//{
//    BYTE    errorCode;
//    union
//    {
//        BYTE    value;
//        struct
//        {
//            BYTE    sectorSize  : 1;
//            BYTE    maxLUN      : 1;
//        }   bits;
//    } validityFlags;
// 
//    WORD    sectorSize;
//    BYTE    maxLUN;
//} MEDIA_INFORMATION;
 
//typedef enum
//{
//    MEDIA_NO_ERROR,                     // No errors
//    MEDIA_DEVICE_NOT_PRESENT,           // The requested device is not present
//    MEDIA_CANNOT_INITIALIZE             // Cannot initialize media
//} MEDIA_ERRORS;
 
//MEDIA_INFORMATION * USBHostMSDSCSIMediaInitialize( void );
//BYTE USBHostMSDSCSIMediaDetect( void );
//BYTE USBHostMSDSCSIWriteProtectState( void );
//BYTE USBHostMSDSCSISectorRead( DWORD sectorAddress, BYTE *dataBuffer );
//BYTE USBHostMSDSCSISectorWrite( DWORD sectorAddress, BYTE *dataBuffer, BYTE allowWriteToZero );
 
 
//static BOOL isInitialized = FALSE;

/*-----------------------------------------------------------------------*/
/* Get Disk Status
*/
/*-----------------------------------------------------------------------*/
 
DSTATUS disk_status (
    BYTE pdrv        /* Physical drive nmuber (0..) */
)
{
//    DSTATUS stat;
 
    switch (pdrv) 
    {
    case DEV_USB :
        if(isUSB_DETACH()) 
        {
            return STA_NODISK | STA_NOINIT;
        } 
        else if(isUSB_NOT_INITIALIZED()) 
        {
            return STA_NOINIT;
        } 
        else
        {
            return 0;
        }
    }
    return STA_NOINIT;
}
 
 
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive
*/
/*-----------------------------------------------------------------------*/
 
DSTATUS disk_initialize (
    BYTE pdrv                /* Physical drive nmuber (0..) */
//    BYTE *buff        /* Data buffer to store read data */
)
{
    switch (pdrv) {
    case DEV_USB:
        SCSI_init();
//            SCSI_requestSense(UsbBufDAT512);
//            SCSI_readCapacity(UsbBufDAT512);
        if(isUSB_NOT_INITIALIZED()) // there is not initialized drive
        {
            return STA_NOINIT;
        }
    }
    return 0;
}
 
 
/*-----------------------------------------------------------------------*/
/* Read Sector(s)
*/
/*-----------------------------------------------------------------------*/
 
DRESULT disk_read (
    BYTE pdrv,        /* Physical drive nmuber (0..) */
    BYTE *buff,        /* Data buffer to store read data */
    DWORD sector,    /* Sector address (LBA) */
    UINT count        /* Number of sectors to read (1..128) */
)
{
    UINT i;
    WORD SectorSize;
 
    if(disk_ioctl(pdrv, GET_SECTOR_SIZE, &SectorSize) != RES_OK)
        return RES_ERROR;
 
    switch (pdrv) 
    {
    case DEV_USB:
        for(i = 0; i < count; i++) 
        {
            //if(USBHostMSDSCSISectorRead(sector + i, buff + i * SectorSize) == FALSE)
            SCSI_read((UINT8 *)buff + (i * SectorSize), sector + i);
            if(isSCSI_ERROR())    
                return RES_ERROR;
        }
 
        return RES_OK;
    }
    return RES_PARERR;
}
 
 
 
/*-----------------------------------------------------------------------*/
/* Write Sector(s)
*/
/*-----------------------------------------------------------------------*/
 
#if _USE_WRITE
DRESULT disk_write (
    BYTE pdrv,            /* Physical drive nmuber (0..) */
    const BYTE *buff,    /* Data to be written */
    DWORD sector,        /* Sector address (LBA) */
    UINT count            /* Number of sectors to write (1..128) */
)
{
    UINT i;
    WORD SectorSize;
 
    if(disk_ioctl(pdrv, GET_SECTOR_SIZE, &SectorSize) != RES_OK)
        return RES_ERROR;
 
    switch (pdrv) 
    {
    case DEV_USB :
        for(i = 0; i < count; i++) 
        {
//            if(USBHostMSDSCSISectorWrite(sector, (BYTE *)buff + i * SectorSize, TRUE) == FALSE)
//                return RES_ERROR;
            SCSI_write((UINT8 *)buff + (i * SectorSize), sector + i);
            if(isSCSI_ERROR())    
                return RES_ERROR;
        }
 
        return RES_OK;
    }
    return RES_PARERR;
}
#endif
 
 
/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions
*/
/*-----------------------------------------------------------------------*/
 
#if _USE_IOCTL
DRESULT disk_ioctl (
    BYTE pdrv,        /* Physical drive nmuber (0..) */
    BYTE cmd,        /* Control code */
    void *buff        /* Buffer to send/receive control data */
)
{
    DRESULT res;
//    int result;
 
    switch (pdrv) {
    case DEV_USB :
        switch(cmd) {
            case CTRL_SYNC:
                res = RES_OK;
                break;
            case GET_SECTOR_COUNT:    //フォーマットするときにしか使われない
                res = RES_ERROR;
                break;
            case GET_SECTOR_SIZE:
#if(FF_MAX_SS == FF_MIN_SS)
                *((WORD *)buff) = FF_MAX_SS;
                res = RES_OK;
#else
                res = RES_ERROR;
#endif
                break;
            case GET_BLOCK_SIZE:
                *((DWORD *)buff) = 1;
                res = RES_OK;
                break;
//            case CTRL_ERASE_SECTOR:
//                res = RES_OK;
//                break;
            default:
                res = RES_PARERR;
                break;
        }
        return res;
    }
    return RES_PARERR;
}
#endif


//typedef union _tagFATTIME {
//    DWORD value;
//    struct {
//        unsigned SecDiv2 : 5;
//        unsigned Min : 6;
//        unsigned Hour : 5;
//        unsigned Date : 5;
//        unsigned Month : 4;
//        unsigned YearFrom1980 : 7;
//    };
//} FATTIME;
//
//DWORD get_fattime (void)
//{
//    FATTIME time;
// 
//    time.YearFrom1980 = 34;
//    time.Month = 6;
//    time.Date = 17;
//    time.Hour = 23;
//    time.Min = 16;
//    time.SecDiv2 = 0;
// 
//    return time.value;
//}
 
void disk_detatched(void)
{
//    isInitialized = FALSE;
}

