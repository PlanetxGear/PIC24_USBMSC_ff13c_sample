/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.125
        Device            :  PIC24FJ64GB002
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36B
        MPLAB 	          :  MPLAB X v5.20
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
#include <string.h>

#include "integer.h"
#include "xprintf.h"

#include "vTMR1.h"
#include "vUSBMSC.h"
#include "vSCSI.h"

#include "uart_pic24f.h"
#include "ff.h"
#include "diskio.h"
#include "ff_timer.h"

/**	
 * @brief	variables for FatFs sample .
 */
DWORD AccSize;			/* Work register for fs command */
WORD AccFiles, AccDirs;
FILINFO Finfo;

char Line[256];			/* Console input buffer */

FATFS FatFs;			/* File system object */
FIL File[2];			/* File objects */
BYTE Buff[4096];		/* Working buffer */



/*---------------------------------------------------------*/
/* User Provided RTC Function for FatFs module             */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support an RTC.                     */
/* This function is not required in read-only cfg.         */




/*--------------------------------------------------------------------------*/
/* Monitor                                                                  */

static
void put_rc (FRESULT rc)
{
	const char *str =
		"OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
		"INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
		"INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
		"LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
	FRESULT i;

	for (i = 0; i != rc && *str; i++) {
		while (*str++) ;
	}
	xprintf("rc=%u FR_%s\n", (UINT)rc, str);
}

static
FRESULT scan_files (
	char* path		/* Pointer to the path name working buffer */
)
{
	DIR dirs;
	FRESULT res;
	int i;


	if ((res = f_opendir(&dirs, path)) == FR_OK) {
		while (((res = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
			if (Finfo.fattrib & AM_DIR) {
				i = strlen(path);
				AccDirs++;
				path[i] = '/'; strcpy(&path[i+1], Finfo.fname);
				res = scan_files(path);
				path[i] = 0;
				if (res != FR_OK) break;
			} else {
#if 0
				xprintf("%s/%s\n", path, fn);
#endif
				AccFiles++;
				AccSize += Finfo.fsize;
			}
		}
	}

	return res;
}

/*-----------------------------------------------------------------------*/
/* FatFs 13 Main                                                         */
void ff13_main (void)
{
	char *ptr, *ptr2;
	long p1, p2, p3;
	BYTE b, drv = 0;
	const BYTE ft[] = {0,12,16,32};
	UINT s1, s2, cnt;
	DWORD ofs = 0, sect = 0;
	FRESULT res;
	FATFS *fs;				/* Pointer to file system object */
	DIR dir;				/* Directory object */


//	IoInit();

//	xdev_in(uart_getc);		/* Join UART and console */
//	xdev_out(uart_putc);
//	xputs("\nFatFs module test monitor for PIC24F\n");
//	xprintf("LFN=%s, CP= %u\n", FF_USE_LFN ? "Enabled" : "Disabled", FF_CODE_PAGE);

//	for (;;) {
		xputc('>');
		xgets(Line, sizeof Line);

		ptr = Line;
		switch (*ptr++) {
		case 'd' :
			switch (*ptr++) {
			case 'd' :	/* dd [<pd#> <sector>] - Dump secrtor */
				if (xatoi(&ptr, &p1)) {
					if (!xatoi(&ptr, &p2)) break;
				} else {
					p1 = drv; p2 = sect;
				}
				b = disk_read((BYTE)p1, Buff, p2, 1);
				if (b) { xprintf("rc=%d\n", b); break; }
				drv = (BYTE)p1; sect = p2 + 1;
				xprintf("PD#:%u, Sector:%lu\n", (BYTE)p1, p2);
				for (ptr=(char*)Buff, ofs = 0; ofs < 0x200; ptr += 16, ofs += 16)
					put_dump((BYTE*)ptr, ofs, 16, DW_CHAR);
				break;

			case 'i' :	/* di <pd#> - Initialize physical drive */
				if (!xatoi(&ptr, &p1)) break;
				xprintf("rc=%d\n", disk_initialize((BYTE)p1));
				break;

			case 's' :	/* ds <pd#> - Show disk status */
				if (!xatoi(&ptr, &p1)) break;
				if (disk_ioctl((BYTE)p1, GET_SECTOR_COUNT, &p2) == RES_OK)
					{ xprintf("Drive size: %lu sectors\n", p2); }
				if (disk_ioctl((BYTE)p1, GET_BLOCK_SIZE, &p2) == RES_OK)
					{ xprintf("Erase block: %lu sectors\n", p2); }
				if (disk_ioctl((BYTE)p1, MMC_GET_TYPE, &b) == RES_OK)
					{ xprintf("MMC/SDC type: %u\n", b); }
				if (disk_ioctl((BYTE)p1, MMC_GET_CSD, Buff) == RES_OK)
					{ xputs("CSD:\n"); put_dump(Buff, 0, 16, DW_CHAR); }
				if (disk_ioctl((BYTE)p1, MMC_GET_CID, Buff) == RES_OK)
					{ xputs("CID:\n"); put_dump(Buff, 0, 16, DW_CHAR); }
				if (disk_ioctl((BYTE)p1, MMC_GET_OCR, Buff) == RES_OK)
					{ xputs("OCR:\n"); put_dump(Buff, 0, 4, DW_CHAR); }
				if (disk_ioctl((BYTE)p1, MMC_GET_SDSTAT, Buff) == RES_OK) {
					xputs("SD Status:\n");
					for (s1 = 0; s1 < 64; s1 += 16) put_dump(Buff+s1, s1, 16, DW_CHAR);
				}
				break;
			}
			break;

		case 'b' :
			switch (*ptr++) {
			case 'd' :	/* bd <ofs> - Dump R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				for (ptr=(char*)&Buff[p1], ofs = p1, cnt = 32; cnt; cnt--, ptr += 16, ofs += 16)
					put_dump((BYTE*)ptr, ofs, 16, DW_CHAR);
				break;

			case 'e' :	/* be <ofs> [<data>] ... - Edit R/W buffer */
				if (!xatoi(&ptr, &p1)) break;
				if (xatoi(&ptr, &p2)) {
					do {
						Buff[p1++] = (BYTE)p2;
					} while (xatoi(&ptr, &p2));
					break;
				}
				for (;;) {
					xprintf("%04X %02X-", (WORD)p1, Buff[p1]);
					xgets(Line, sizeof Line);
					ptr = Line;
					if (*ptr == '.') break;
					if (*ptr < ' ') { p1++; continue; }
					if (xatoi(&ptr, &p2))
						Buff[p1++] = (BYTE)p2;
					else
						xputs("???\n");
				}
				break;

			case 'r' :	/* br <pd#> <sector> <count> - Read disk into R/W buffer */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				xprintf("rc=%u\n", disk_read((BYTE)p1, Buff, p2, (BYTE)p3));
				break;

			case 'w' :	/* bw <pd#> <sector> <count> - Write R/W buffer into disk */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				xprintf("rc=%u\n", disk_write((BYTE)p1, Buff, p2, (BYTE)p3));
				break;

			case 'f' :	/* bf <n> - Fill working buffer */
				if (!xatoi(&ptr, &p1)) break;
				memset(Buff, (int)p1, sizeof Buff);
				break;

			}
			break;

		case 'f' :
			switch (*ptr++) {

			case 'i' :	/* fi [<mount>] - Force initialized the logical drive */
				if (!xatoi(&ptr, &p2)) p2 = 0;
				put_rc(f_mount(&FatFs, "", (BYTE)p2));
				break;

			case 's' :	/* fs [<path>] - Show logical drive status */
				while (*ptr == ' ') ptr++;
				res = f_getfree(ptr, (DWORD*)&p2, &fs);
				if (res) { put_rc(res); break; }
				xprintf("FAT type = FAT%u\nBytes/Cluster = %lu\nNumber of FATs = %u\n"
						"Root DIR entries = %u\nSectors/FAT = %lu\nNumber of clusters = %lu\n"
						"Volume start (lba) = %lu\nFAT start (lba) = %lu\nDIR start (lba,clustor) = %lu\nData start (lba) = %lu\n\n...",
						ft[fs->fs_type & 3], fs->csize * 512UL, fs->n_fats,
						fs->n_rootdir, fs->fsize, fs->n_fatent - 2,
						fs->volbase, fs->fatbase, fs->dirbase, fs->database
				);
				AccSize = AccFiles = AccDirs = 0;
				res = scan_files(ptr);
				if (res) { put_rc(res); break; }
				xprintf("\r%u files, %lu bytes.\n%u folders.\n"
						"%lu KiB total disk space.\n%lu KiB available.\n",
						AccFiles, AccSize, AccDirs,
						(fs->n_fatent - 2) * (fs->csize / 2), p2 * (fs->csize / 2)
				);
				break;

			case 'l' :	/* fl [<path>] - Directory listing */
				while (*ptr == ' ') ptr++;
				res = f_opendir(&dir, ptr);
				if (res) { put_rc(res); break; }
				p1 = s1 = s2 = 0;
				for(;;) {
					res = f_readdir(&dir, &Finfo);
					if ((res != FR_OK) || !Finfo.fname[0]) break;
					if (Finfo.fattrib & AM_DIR) {
						s2++;
					} else {
						s1++; p1 += Finfo.fsize;
					}
					xprintf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s\n",
							(Finfo.fattrib & AM_DIR) ? 'D' : '-',
							(Finfo.fattrib & AM_RDO) ? 'R' : '-',
							(Finfo.fattrib & AM_HID) ? 'H' : '-',
							(Finfo.fattrib & AM_SYS) ? 'S' : '-',
							(Finfo.fattrib & AM_ARC) ? 'A' : '-',
							(Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
							(Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63, Finfo.fsize, Finfo.fname);
				}
				xprintf("%4u File(s),%10lu bytes total\n%4u Dir(s)", s1, p1, s2);
				if (f_getfree(ptr, (DWORD*)&p1, &fs) == FR_OK)
					xprintf(", %10lu bytes free\n", p1 * fs->csize * 512);
				break;
#if FF_USE_FIND
			case 'L' :	/* fL <path> <pattern> - Directory search */
				while (*ptr == ' ') ptr++;
				ptr2 = ptr;
				while (*ptr != ' ') ptr++;
				*ptr++ = 0;
				res = f_findfirst(&dir, &Finfo, ptr2, ptr);
				while (res == FR_OK && Finfo.fname[0]) {
					xprintf("%s\n", Finfo.fname);
					res = f_findnext(&dir, &Finfo);
				}
				if (res) put_rc(res);
				f_closedir(&dir);
				break;
#endif
                case 'o' :	/* fo <mode> <file> - Open a file / mode 1:read, 2:write*/
				if (!xatoi(&ptr, &p1)) 
                {
                    xputs("MODE_ERROR\n");  
                    break;
                }
				while (*ptr == ' ') ptr++;
				put_rc(f_open(&File[0], ptr, (BYTE)p1));
				break;

			case 'c' :	/* fc - Close a file */
				put_rc(f_close(&File[0]));
				break;

			case 'e' :	/* fe <ofs> - Seek file pointer */
				if (!xatoi(&ptr, &p1)) break;
				res = f_lseek(&File[0], p1);
				put_rc(res);
				if (res == FR_OK)
					xprintf("fptr = %lu(0x%lX)\n", f_tell(&File[0]), f_tell(&File[0]));
				break;

			case 'r' :	/* fr <len> - read file */
				if (!xatoi(&ptr, &p1)) break;
				p2 = 0;
				Timer = 0;
				while (p1) {
					if ((DWORD)p1 >= sizeof Buff) {
						cnt = sizeof Buff; p1 -= sizeof Buff;
					} else {
						cnt = p1; p1 = 0;
					}
					res = f_read(&File[0], Buff, cnt, &s2);
					if (res != FR_OK) { put_rc(res); break; }
					p2 += s2;
					if (cnt != s2) break;
				}
				xprintf("%lu bytes read with %lu kB/sec.\n", p2, Timer ? (p2 / Timer) : 0);
				break;

			case 'd' :	/* fd <len> - read and dump file from current fp */
				if (!xatoi(&ptr, &p1)) break;
				ofs = f_tell(&File[0]);
				while (p1) {
					if ((UINT)p1 >= 16) { cnt = 16; p1 -= 16; }
					else 				{ cnt = p1; p1 = 0; }
					res = f_read(&File[0], Buff, cnt, &cnt);
					if (res != FR_OK) { put_rc(res); break; }
					if (!cnt) break;
					put_dump(Buff, ofs, cnt, DW_CHAR);
					ofs += 16;
				}
				break;

			case 'w' :	/* fw <len> <val> - write file */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				memset(Buff, (int)p2, sizeof Buff);
				p2 = 0;
				Timer = 0;
				while (p1) {
					if ((DWORD)p1 >= sizeof Buff) {
						cnt = sizeof Buff; p1 -= sizeof Buff;
					} else {
						cnt = p1; p1 = 0;
					}
					res = f_write(&File[0], Buff, cnt, &s2);
					if (res != FR_OK) { put_rc(res); break; }
					p2 += s2;
					if (cnt != s2) break;
				}
				xprintf("%lu bytes written with %lu kB/sec.\n", p2, Timer ? (p2 / Timer) : 0);
				break;

			case 'n' :	/* fn <old_name> <new_name> - Change file/dir name */
				while (*ptr == ' ') ptr++;
				ptr2 = strchr(ptr, ' ');
				if (!ptr2) break;
				*ptr2++ = 0;
				while (*ptr2 == ' ') ptr2++;
				put_rc(f_rename(ptr, ptr2));
				break;

			case 'u' :	/* fu <path> - Unlink a file or dir */
				while (*ptr == ' ') ptr++;
				put_rc(f_unlink(ptr));
				break;

			case 'v' :	/* fv - Truncate file */
				put_rc(f_truncate(&File[0]));
				break;

			case 'k' :	/* fk <path> - Create a directory */
				while (*ptr == ' ') ptr++;
				put_rc(f_mkdir(ptr));
				break;
#if FF_USE_CHMOD
			case 'a' :	/* fa <atrr> <mask> <name> - Change file/dir attribute */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
				while (*ptr == ' ') ptr++;
				put_rc(f_chmod(ptr, p1, p2));
				break;

			case 't' :	/* ft <year> <month> <day> <hour> <min> <sec> <name> - Change timestamp */
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				Finfo.fdate = (WORD)(((p1 - 1980) << 9) | ((p2 & 15) << 5) | (p3 & 31));
				if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				Finfo.ftime = (WORD)(((p1 & 31) << 11) | ((p1 & 63) << 5) | ((p1 >> 1) & 31));
				put_rc(f_utime(ptr, &Finfo));
				break;
#endif
			case 'x' : /* fx <src_name> <dst_name> - Copy file */
				while (*ptr == ' ') ptr++;
				ptr2 = strchr(ptr, ' ');
				if (!ptr2) break;
				*ptr2++ = 0;
				while (*ptr2 == ' ') ptr2++;
				xprintf("Opening \"%s\"", ptr);
				res = f_open(&File[0], ptr, FA_OPEN_EXISTING | FA_READ);
				xputc('\n');
				if (res) {
					put_rc(res);
					break;
				}
				xprintf("Creating \"%s\"", ptr2);
				res = f_open(&File[1], ptr2, FA_CREATE_ALWAYS | FA_WRITE);
				xputc('\n');
				if (res) {
					put_rc(res);
					f_close(&File[0]);
					break;
				}
				xprintf("Copying...");
				p1 = 0;
				for (;;) {
					res = f_read(&File[0], Buff, sizeof Buff, &s1);
					if (res || s1 == 0) break;   /* error or eof */
					res = f_write(&File[1], Buff, s1, &s2);
					p1 += s2;
					if (res || s2 < s1) break;   /* error or disk full */
				}
				xprintf("\n%lu bytes copied.\n", p1);
				f_close(&File[0]);
				f_close(&File[1]);
				break;
#if FF_FS_RPATH
			case 'g' :	/* fg <path> - Change current directory */
				while (*ptr == ' ') ptr++;
				put_rc(f_chdir(ptr));
				break;
#if FF_FS_RPATH >= 2
			case 'q' :	/* fq - Show current dir path */
				res = f_getcwd(Line, sizeof Line);
				if (res)
					put_rc(res);
				else
					xprintf("%s\n", Line);
				break;
#endif
#endif
#if FF_USE_MKFS
			case 'm' :	/* fm <partition rule> <sect/clust> - Create file system */
				if (!xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) break;
				xprintf("The memory card will be formatted. Are you sure? (Y/n)=", p1);
				xgets(Line, sizeof Line);
				if (Line[0] == 'Y')
					put_rc(f_mkfs("", (BYTE)p2, (DWORD)p3, Buff, sizeof Buff));
				break;
#endif
			}
			break;

		case 't' :		/* t [<year> <mon> <mday> <hour> <min> <sec>] */
			if (xatoi(&ptr, &p1)) {
				rtcYear = p1;
				xatoi(&ptr, &p1); rtcMon = p1;
				xatoi(&ptr, &p1); rtcMday = p1;
				xatoi(&ptr, &p1); rtcHour = p1;
				xatoi(&ptr, &p1); rtcMin = p1;
				if(!xatoi(&ptr, &p1)) break;
				rtcSec = p1;
			}
			xprintf("%u/%u/%u %02u:%02u:%02u\n", rtcYear, rtcMon, rtcMday, rtcHour, rtcMin, rtcSec);
			break;

		case 'u' :		/* usb command */
			switch (*ptr++) {
            case 'a':
                SCSI_init();
                break;
            case 'b':
                SCSI_requestSense(Buff);
                break;
            case 'c':
                SCSI_readCapacity(Buff);
                break;
            case 'd' :	/* bd <ofs> - Dump R/W buffer */
				if (!xatoi(&ptr, &p1))   //Get a offset
                {
                    p1 = 0;
                }
                for (ptr=(char*)&Buff[p1], ofs = p1, cnt = 32; cnt; cnt--, ptr += 16, ofs += 16)
                    put_dump((BYTE*)ptr, ofs, 16, DW_CHAR);
                break;
            case 's':
                xprintf("SCSIstatus:%d USBstatus:%d \n",SCSIcondition.Status, USBcondition.Status);
                break;
            case 'r':
                SCSI_read(Buff, SCSIcondition.MscTotal);
                for (ptr=(char*)Buff, ofs = 0; ofs < 512; ptr += 16, ofs += 16)
                {
                    put_dump((BYTE*)ptr, ofs, 16, DW_CHAR);
                }
                break;
//            case 'w':	/* uw <No.> - Fill the last sector with a No. */
//                pCmdbuf = (char*)&cCmdBuf[3];   //Get a No.
//                if (!xatoi(&pCmdbuf, &num1))
//                {
//                    vXputs_FormatError();
//                } else if (SCSIcondition.MscTotal == 0)
//                {
//                        xputs("You should get Max sector No. by 'UC' command.\n");
//                } else
//                {
//                    // set data buffer by input Number
//                    for (ptr=Buff, i = 0; i < 512; ptr += 1, i += 1)
//                    {
//                        *ptr = num1;
//                    }
//                    SCSI_write(Buff, SCSIcondition.MscTotal);
//                }
//                break;
    		}
			break;
		}
//	}

}




/***************************************************************************//**
 *                       Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    TMR1_SetInterruptHandler(&Intrrupt_TMR1);
    UART1_SetRxInterruptHandler(&Interrupt_U1RX);
    UART1_SetTxInterruptHandler(&Interrupt_U1TX);
	xdev_in(uart_getc);		/* Join UART and console */
	xdev_out(uart_putc);
    
	setUSB_INITIALIZE();
    setSCSI_NOT_INITIALIZED();      // instead of SCSI initialize

    INTERRUPT_GlobalEnable();

	xputs("\nFatFs module test monitor for PIC24F\n");
	xprintf("LFN=%s, CP= %u\n", FF_USE_LFN ? "Enabled" : "Disabled", FF_CODE_PAGE);
    
    while (1)
    {
        // Add your application code

        // USBMSC_initialize
        if(USBcondition.Status < eUSB_IDLE )
        {
            USBMSC_statusControl();
        }
        else
        {
            ff13_main();
        }
        
        if(IS_uiTMR007_FINISH)
        {
            uiTMR007 = 500;     //wait 500msec
            LED1_Toggle();
        }
    }

    return 1;
}
/**
 End of File
*/

