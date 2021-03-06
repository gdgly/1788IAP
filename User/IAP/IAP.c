/*********************************************************************************************************
 *   IAP.c:  IAP C file for NXP LPC17xx Family Microprocessors
 *   Copyright(C) 2005, NXP Semiconductor
 *   All rights reserved.
 *   2011.07.10  ver 1.01
*********************************************************************************************************/
#include "IAP.h"
#define    	IAP_FCCLK       48000                                       /* 定义CCLK值大小，单位为KHz    */

uint32_t    paramin[8];                                                 /* IAP入口参数缓冲区            */
uint32_t    paramout[8];                                                /* IAP出口参数缓冲区            */
unsigned long command[5];
unsigned long result[5];
typedef void (*IAP) (unsigned int [ ] , unsigned int [ ]);             
//IAP iap_entry;
//iap_entry=(IAP) IAP_LOCATION;
//iap_entry (command , result);       /*调用IAP   */
/*********************************************************************************************************
**函数名称:  SelSector
**函数功能:  IAP操作缓冲区选择，代码为50
**入口参数:  sec1    起始扇区
**    	     sec2    终止扇区
**出口参数:  IAP 操作状态码
**    	     IAP返回值（paramout缓冲区）
*********************************************************************************************************/
uint32_t    SelSector(uint8_t    sec1,uint8_t    sec2)
{
    paramin[0] = IAP_SELECTOR;                                          /* 设置命令字                   */
    paramin[1] = sec1;                                                  /* 设置参数                     */
    paramin[2] = sec2;
    (*(void(*)())IAP_LOCATION)(paramin,paramout);                      /* 调用IAP服务程序              */
    return(paramout[0]);                                                /* 返回状态码                   */
}
/*********************************************************************************************************
**函数名称:  RamToFlash
**函数功能:  复制RAM的数据到FLASH，命令代码51
**入口参数:  dst    	目标地址，即FLASH起始地址，以256字节为分界
**    	     src    	源地址，即RAM地址，地址必须字对其
**    	     no    	    复制字节个数，为256/512/1024/4096
**出口参数:  IAP 操作状态码
**    	     IAP返回值（paramout缓冲区）
*********************************************************************************************************/
uint32_t    RamToFlash(uint32_t dst, uint32_t src, uint32_t  no)
{
    paramin[0] = IAP_RAMTOFLASH;                                        /* 设置命令字                   */
    paramin[1] = dst;                                                   /* 设置参数                     */
    paramin[2] = src;
    paramin[3] = no;
    paramin[4] = IAP_FCCLK;
    (*(void(*)())IAP_LOCATION)(paramin,paramout);                      /* 调用IAP服务程序              */
    return(paramout[0]);                                                /* 返回状态码                   */
}
/*********************************************************************************************************
**函数名称：EraseSector
**函数功能：擦除扇区，命令代码52
**入口参数：sec1    起始扇区
**    	    sec2    终止扇区
**出口参数：IAP 操作状态码
**    	    IAP返回值（paramout缓冲区）
*********************************************************************************************************/
uint32_t    EraseSector(uint32_t sec1, uint32_t sec2)
{
    paramin[0] = IAP_ERASESECTOR;                                       /* 设置命令字                   */
    paramin[1] = sec1;                                                  /* 设置参数                     */
    paramin[2] = sec2;
    paramin[3] = IAP_FCCLK;
    (*(void(*)())IAP_LOCATION)(paramin,paramout);                      /* 调用IAP服务程序              */
    return(paramout[0]);                                                /* 返回状态码                   */
}
/*********************************************************************************************************
**函数名称:  BlankCHK
**函数功能:  查空扇区，命令代码53
**入口参数:  sec1    起始扇区
**    	     sec2    终止扇区
**出口参数:  IAP 操作状态码
**    	     IAP返回值（paramout缓冲区）
*********************************************************************************************************/
uint32_t    BlankCHK(uint32_t    sec1, uint32_t  sec2)
{
    paramin[0] = IAP_BLANKCHK;                                          /* 设置命令字                   */
    paramin[1] = sec1;                                                  /* 设置参数                     */
    paramin[2] = sec2;
    paramin[3] = IAP_FCCLK;
    (*(void(*)())IAP_LOCATION)(paramin,paramout);                      /* 调用IAP服务程序              */
    return(paramout[0]);                                                /* 返回状态码                   */
}
/*********************************************************************************************************
**函数名称:  ReadParID
**函数功能:  读器件ID，命令代码54
**入口参数:  器件ID地址指针
**出口参数:  IAP 操作状态码
**    	     IAP返回值（paramout缓冲区）
*********************************************************************************************************/
uint32_t    ReadParID(uint32_t *Device_ID)
{
    paramin[0] = IAP_READPARTID;                                        /* 设置命令字                   */
    (*(void(*)())IAP_LOCATION)(paramin,paramout);                      /* 调用IAP服务程序              */
    *Device_ID = paramout[1];
    return(paramout[0]);                                                /* 返回状态码                   */
}
/*********************************************************************************************************
**函数名称:  BootCodeID
**函数功能:  读boot代码ID，命令代码55
**入口参数:  boot代码ID地址指针
**出口参数:  IAP 操作状态码
**    	     IAP返回值（paramout缓冲区）
*********************************************************************************************************/
uint32_t    BootCodeID(uint32_t *Boot_ID)    
{
    paramin[0] = IAP_BOOTCODEID;                                        /* 设置命令字                   */
    (*(void(*)())IAP_LOCATION)(paramin,paramout);                      /* 调用IAP服务程序              */
    *Boot_ID = paramout[1];
    return(paramout[0]);                                                /* 返回状态码                   */
}
/*********************************************************************************************************
**函数名称:  Compare
**函数功能:  校验数据，命令代码56
**入口参数:  dst    	目标地址，即RAM/FLASH起始地址，地址必须字对齐
**    	     src         源地址，即RAM/RAM地址，地址必须字对齐
**    	     no    	    比较字节个数，必须能被4整除
**出口参数:  IAP 操作状态码
**    	     IAP返回值（paramout缓冲区）
*********************************************************************************************************/
uint32_t    Compare(uint32_t    dst, uint32_t    src, uint32_t    no)
{
    paramin[0] = IAP_COMPARE;                                           /* 设置命令字                   */
    paramin[1] = dst;                                                   /* 设置参数                     */
    paramin[2] = src;
    paramin[3] = no;
    (*(void(*)())IAP_LOCATION)(paramin,paramout);                      /* 调用IAP服务程序              */
    return(paramout[0]);                                                /* 返回状态码                   */
}
/*********************************************************************************************************
**函数名称:  Reinvoke_ISP
**函数功能:  重新调用ISP，命令代码57。
**入口参数:  无
**出口参数:  无
*********************************************************************************************************/
void  Reinvoke_ISP(void)
{
    paramin[0] = IAP_REINVOKE_ISP;                                      /* 设置命令字                   */
    (*(void(*)())IAP_LOCATION)(paramin,paramout);                      /* 重新调用ISP                  */
} 
/*********************************************************************************************************
**函数名称:  WriteFlash
**函数功能:  向FLASH中写入数据
**入口参数:  dst    	目标地址，即FLASH起始地址，地址必须以256字节对齐，即地址的低8位必须为0
**    	     src    	源地址，即RAM地址，地址必须字对齐，即地址的低两位必须为0
**    	     no    	    写入字节数，必须为256/512/1024/4096
**出口参数:  Compare的返回值，操作成功正确返回0
*********************************************************************************************************/
uint8_t    WriteFlash(uint32_t  dst, uint32_t    src, uint32_t    no)
{
    SelSector((uint8_t)(dst/0x1000),(uint8_t)((dst+no)/0x1000));        /* 选择扇区                     */
    EraseSector((dst/0x1000),(dst+no)/0x1000);                          /* 擦除扇区                     */
    BlankCHK((dst/0x1000),(dst+no)/0x1000);                             /* 查空扇区                     */
    SelSector((uint8_t)(dst/0x1000),(uint8_t)((dst+no)/0x1000));        /* 选择扇区                     */
    RamToFlash(dst,src,no);                                             /* 写数据到FLASH                */
    return((uint8_t)(Compare(dst,src,no)));                             /* 比较数据                     */
}

__asm void ExceuteApplication(void)
{
		/* Load main stack pointer with application stack pointer initial value,
		   stored at first location of application area */
		ldr r0, =0x018000
		ldr r0, [r0]
		mov sp, r0
		
		/* Load program counter with application reset vector address, located at
		   second word of application area. */
		ldr r0, =0x018004
		ldr r0, [r0]
        BX  r0
		NOP
}