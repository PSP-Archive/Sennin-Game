// primitive graphics for Hello World PSP
#ifndef _PG_H_
#define _PG_H_


void pgInit();
void pgWaitV();
void pgWaitVn(unsigned long count);
void pgScreenFrame(long mode,long frame);
void pgScreenFlip();
void pgScreenFlipV();
/*
void pgPrint(unsigned long x,unsigned long y,unsigned long color,const char *str);
void pgPrint2(unsigned long x,unsigned long y,unsigned long color,const char *str);
void pgPrint4(unsigned long x,unsigned long y,unsigned long color,const char *str);
*/
void pgFillvram(unsigned long color);
void pgBitBlt(unsigned long x,unsigned long y,unsigned long w,unsigned long h,unsigned long mag,const unsigned short *d);
void pgBitBltTrans0(unsigned long x,unsigned long y,unsigned long w,unsigned long h,const unsigned short *d);
//void pgPutChar(unsigned long x,unsigned long y,unsigned long color,unsigned long bgcolor,unsigned char ch,char drawfg,char drawbg,char mag);

void pgcLocate(unsigned long x, unsigned long y);
void pgcColor(unsigned long fg, unsigned long bg);
void pgcDraw(char drawfg, char drawbg);
void pgcSetmag(char mag);
void pgcPutchar(const char ch);
void pgcPutchar_nocontrol(const char ch);
void pgcPuts(const char *str);
void pgcPutsn(const char *str,unsigned long max);
void pgcPutsln(const char *str);
void pgcPuthex2(const unsigned long s);
void pgcPuthex8(const unsigned long s);
void pgcCls();
void pgcClearlines(unsigned long st, unsigned long ed);
void pgcCleareol();

extern u8 *pgGetVramAddr(unsigned long x,unsigned long y);

#define MAX_PATH 512		//temp, not confirmed

#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 272

#define		PIXELSIZE	1				//in short
#define		LINESIZE	512				//in short
#define		FRAMESIZE	0x44000			//in byte

#define MKRGB(r,g,b) ( ((b&0xf8)<<7)+((g&0xf8)<<2)+((r&0xf8)>>3)+0x8000 )

#define OF_RDONLY    0x0001 
#define OF_WRONLY    0x0002 
#define OF_RDWR      0x0003 
#define OF_NBLOCK    0x0010 
#define OF_APPEND    0x0100 
#define OF_CREAT     0x0200 
#define OF_TRUNC     0x0400 
#define OF_NOWAIT    0x8000 

#define CMAX_X 60
#define CMAX_Y 34
#define CMAX2_X 30
#define CMAX2_Y 17
#define CMAX4_X 15
#define CMAX4_Y 8

void pgExit(int n);
void pgErrorHalt(const char *str);

const char *pguGetMypath();
const char *pguGetWorkdir();
int pgfOpen(const char *filename, unsigned long flag);
void pgfClose(int fd);
int pgfRead(int fd, void *data, int size);
int pgfWrite(int fd, void *data, int size);
int pgfSeek(int fd, int offset, int whence);


#endif
