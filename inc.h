#pragma once
#include    <stdio.h>
#include 	<iostream>

typedef signed char 		I8;	//Int8 	cValue;	为了保持统一，建议不使用ch修饰而改为c修饰
typedef signed short 		I6;	//Int16	sValue;
typedef signed int			I32;	//Int32 iValue;
typedef signed long long	I64;	//Int64	lValue;

typedef unsigned char 		U8;	//UInt8  ucValue;
typedef unsigned short 		U16;	//UInt16 usValue;
typedef unsigned int		U32;	//UInt32 uiValue;
typedef unsigned long long	U64;	//UInt64 ulValue;

#ifdef __linux__
#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif
#define msleep(X) usleep((X) * 1000)
#else	//__WIN32__ C++
#define sleep(X) Sleep((X) * 1000)
#define msleep(X) Sleep(X)
#endif

#define MAX_FRAME_LEN  65535
#define GET_BIT(v, bit) 	(((v) >> (bit)) & 0x1)	//取v的某一bit位
#define SET_BIT(v, bit) 	({(v) |= (1 << (bit));})	//设置v的某一bit位
#define CLR_BIT(v, bit)		({(v) &= (~(1 << (bit)));})	//清零v的某一bit位






