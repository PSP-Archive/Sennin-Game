// utility.h
// original by super star shooter advance.
// Jun28,2005 MAGiCTOUCH(AND). http://www10.big.or.jp/~and/

#ifndef _UTILITY_H_
#define _UTILITY_H_

//----------------------------------------------------------------
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;
typedef unsigned long long int u64;

typedef signed char  s8;
typedef signed short s16;
typedef signed long  s32;
typedef signed long long int s64;

typedef unsigned char  BYTE;
typedef unsigned short HWORD;
typedef unsigned long  WORD;

typedef	signed long long int	FIX32;				// 上位32ビット整数・下位32ビット小数
typedef	signed long	int			FIX16;				// 上位16ビット整数・下位16ビット小数
typedef	signed short int		FIX8;				// 上位 8ビット整数・下位 8ビット小数

//----------------------------------------------------------------
#define	NULL	(0)
#define	TRUE	(1)
#define	FALSE	(0)

#define	ABS(_x)			( ((_x)<0) ? -(_x) : (_x) )
#define	SGN(_x)			( ((_x)==0) ? 0 : (_x)<0 ? -1 : 1 )

#define ANALOG_THRESHOLD	(22)

#define FIX_POWER		(0x10000)					// 固定少数下位8ビット

#define INT2F( _x )		( (_x) << 16 )				// FIX16用
#define FIX2I( _x )		( (_x) >> 16 )
#define	FLOAT2F(_n)		((FIX16)(((float)(_n))*FIX_POWER))

typedef struct RECT_tag
{
	FIX16	x,y;
	FIX16	w,h;
} RECT;

//----------------------------------------------------------------
extern int FixSin(int angle256);
extern int FixCos(int angle256);
extern int sprintf(char *out, const char *format, ...);
extern  int printf(const char *format, ...);
extern u8 ATAN2(FIX16 y, FIX16 x);

//----------------------------------------------------------------
extern void SetNowKey();
extern void SetOldKey();
extern u8 GetDBT_Up();
extern u8 GetPBT_Up();
extern u8 GetDBT_Down();
extern u8 GetPBT_Down();
extern u8 GetDBT_Left();
extern u8 GetPBT_Left();
extern u8 GetDBT_Right();
extern u8 GetPBT_Right();

//----------------------------------------------------------------
// KEY
extern u8 GetDBT_CIRCLE();
extern u8 GetPBT_CIRCLE();

//--------------------------
extern u8 GetDBT_CROSS();
extern u8 GetPBT_CROSS();

//--------------------------
extern u8 GetDBT_SQUARE();
extern u8 GetPBT_SQUARE();

//--------------------------
extern u8 GetDBT_TRIANGLE();
extern u8 GetPBT_TRIANGLE();

//--------------------------
extern u8 GetDBT_Start();
extern u8 GetPBT_Start();

//--------------------------
extern u8 GetDBT_Select();
extern u8 GetPBT_Select();

//--------------------------
extern u8 GetDBT_R();
extern u8 GetPBT_R();

//--------------------------
extern u8 GetDBT_L();
extern u8 GetPBT_L();

//--------------------------
extern u8 GetAnalogX();
extern u8 GetAnalogY();

//--------------------------
// 乱数（昔からのアルゴリズム版）
extern void BRandInit();
extern s32 brand();
#endif
