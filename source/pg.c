// primitive graphics for Hello World PSP

#include "utility.h"
#include "pg.h"
#include "syscall.h"

//system call
void sceDisplayWaitVblankStart();
void sceDisplaySetMode(long,long,long);
void sceDisplaySetFrameBuf(char *topaddr,long linesize,long pixelsize,long);



//variables
char *pg_vramtop=(char *)0x04000000;
long pg_screenmode;
long pg_showframe;
long pg_drawframe;
unsigned long pgc_csr_x[2], pgc_csr_y[2];
unsigned long pgc_fgcolor[2], pgc_bgcolor[2];
char pgc_fgdraw[2], pgc_bgdraw[2];
char pgc_mag[2];

char pg_mypath[MAX_PATH];
char pg_workdir[MAX_PATH];



void pgWaitVn(unsigned long count)
{
	for (; count>0; --count) {
		sceDisplayWaitVblankStart();
	}
}


void pgWaitV()
{
	sceDisplayWaitVblankStart();
}


void pgInit()
{
	sceDisplaySetMode(0,SCREEN_WIDTH,SCREEN_HEIGHT);
	pgScreenFrame(0,0);
}

u8 *pgGetVramAddr(unsigned long x,unsigned long y)
{
	return pg_vramtop+(pg_drawframe?FRAMESIZE:0)+x*PIXELSIZE*2+y*LINESIZE*2+0x40000000;
}


/*
void pgPrint(unsigned long x,unsigned long y,unsigned long color,const char *str)
{
	while (*str!=0 && x<CMAX_X && y<CMAX_Y) {
		pgPutChar(x*8,y*8,color,0,*str,1,0,1);
		str++;
		x++;
		if (x>=CMAX_X) {
			x=0;
			y++;
		}
	}
}


void pgPrint2(unsigned long x,unsigned long y,unsigned long color,const char *str)
{
	while (*str!=0 && x<CMAX2_X && y<CMAX2_Y) {
		pgPutChar(x*16,y*16,color,0,*str,1,0,2);
		str++;
		x++;
		if (x>=CMAX2_X) {
			x=0;
			y++;
		}
	}
}


void pgPrint4(unsigned long x,unsigned long y,unsigned long color,const char *str)
{
	while (*str!=0 && x<CMAX4_X && y<CMAX4_Y) {
		pgPutChar(x*32,y*32,color,0,*str,1,0,4);
		str++;
		x++;
		if (x>=CMAX4_X) {
			x=0;
			y++;
		}
	}
}
*/

void pgFillvram(unsigned long color)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned long i;

	vptr0=pgGetVramAddr(0,0);
	for (i=0; i<FRAMESIZE/2; i++) {
		*(unsigned short *)vptr0=color;
		vptr0+=PIXELSIZE*2;
	}
}

void pgBitBlt(unsigned long x,unsigned long y,unsigned long w,unsigned long h,unsigned long mag,const unsigned short *d)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned char *vptr;		//pointer to vram
	unsigned long xx,yy,mx,my;
	const unsigned short *dd;
	
	unsigned char *vptrMin =pgGetVramAddr(0,0);
	unsigned char *vptrMax =pgGetVramAddr(479,271);

	vptr0=pgGetVramAddr(x,y);
	for (yy=0; yy<h; yy++)
	{
		for (my=0; my<mag; my++)
		{
			vptr=vptr0;
			dd=d;
			for (xx=0; xx<w; xx++)
			{
				if (vptr<vptrMin || vptr>vptrMax)
				{
					continue;
				}

				for (mx=0; mx<mag; mx++)
				{
					*(unsigned short *)vptr=*dd;
					vptr+=PIXELSIZE*2;
				}
				dd++;
			}
			vptr0+=LINESIZE*2;
		}
		d+=w;
	}
	
}

void pgBitBltTrans0(unsigned long x,unsigned long y,unsigned long w,unsigned long h,const unsigned short *d)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned char *vptr;		//pointer to vram
	unsigned long xx,yy,mx,my;
	const unsigned short *dd;
	
	unsigned char *vptrMin =pgGetVramAddr(0,0);
	unsigned char *vptrMax =pgGetVramAddr(479,271);

	vptr0=pgGetVramAddr(x,y);
	for (yy=0; yy<h; yy++)
	{
		vptr=vptr0;
		dd=d;
		for (xx=0; xx<w; xx++)
		{
			if (vptr<vptrMin || vptr>vptrMax)
			{
				continue;
			}
			if (*dd != 0x7c1f/*MKRGB(0xFF,0,0xFF)*/)		// “§‰ßF:ƒ}ƒ[ƒ“ƒ^
			{
				*(unsigned short *)vptr=*dd;
			}
			vptr+=PIXELSIZE*2;
			dd++;
		}
		vptr0+=LINESIZE*2;
		d+=w;
	}
	
}

/*
void pgPutChar(unsigned long x,unsigned long y,unsigned long color,unsigned long bgcolor,unsigned char ch,char drawfg,char drawbg,char mag)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned char *vptr;		//pointer to vram
	const unsigned char *cfont;		//pointer to font
	unsigned long cx,cy;
	unsigned long b;
	char mx,my;

	if (ch>255) return;
	cfont=font+ch*8;
	vptr0=pgGetVramAddr(x,y);
	for (cy=0; cy<8; cy++) {
		for (my=0; my<mag; my++) {
			vptr=vptr0;
			b=0x80;
			for (cx=0; cx<8; cx++) {
				for (mx=0; mx<mag; mx++) {
					if ((*cfont&b)!=0) {
						if (drawfg) *(unsigned short *)vptr=color;
					} else {
						if (drawbg) *(unsigned short *)vptr=bgcolor;
					}
					vptr+=PIXELSIZE*2;
				}
				b=b>>1;
			}
			vptr0+=LINESIZE*2;
		}
		cfont++;
	}
}
*/

void pgScreenFrame(long mode,long frame)
{
	pg_screenmode=mode;
	frame=(frame?1:0);
	pg_showframe=frame;
	if (mode==0) {
		//screen off
		pg_drawframe=frame;
		sceDisplaySetFrameBuf(0,0,0,1);
	} else if (mode==1) {
		//show/draw same
		pg_drawframe=frame;
		sceDisplaySetFrameBuf(pg_vramtop+(pg_showframe?FRAMESIZE:0),LINESIZE,PIXELSIZE,1);
	} else if (mode==2) {
		//show/draw different
		pg_drawframe=(frame?0:1);
		sceDisplaySetFrameBuf(pg_vramtop+(pg_showframe?FRAMESIZE:0),LINESIZE,PIXELSIZE,1);
	}
}


void pgScreenFlip()
{
	pg_showframe=(pg_showframe?0:1);
	pg_drawframe=(pg_drawframe?0:1);
	sceDisplaySetFrameBuf(pg_vramtop+(pg_showframe?FRAMESIZE:0),LINESIZE,PIXELSIZE,0);
}


void pgScreenFlipV()
{
	pgWaitV();
	pgScreenFlip();
}

/******************************************************************************/

volatile int pg_terminate=0;


void pgErrorHalt(const char *str)
{
#if 0	
	pgScreenFrame(1,0);
	pgcLocate(0,0);
	pgcColor(0xffff,0x0000);
	pgcDraw(1,1);
	pgcSetmag(1);
	pgcPuts(str);
	while (1) { pgWaitV(); }
#endif
}


#if 0

void pgMain(unsigned long args, void *argp)
{
#if 0	
	int ret;
	int n;
	n=args;
	if (n>sizeof(pg_mypath)-1) n=sizeof(pg_mypath)-1;
	_memcpy(pg_mypath,argp,n);
	pg_mypath[sizeof(pg_mypath)-1]=0;
	_strcpy(pg_workdir,pg_mypath);
	for (n=_strlen(pg_workdir); n>0 && pg_workdir[n-1]!='/'; --n) pg_workdir[n-1]=0;
	
	sceDisplaySetMode(0,SCREEN_WIDTH,SCREEN_HEIGHT);
	
	pgScreenFrame(0,1);
	pgcLocate(0,0);
	pgcColor(0xffff,0x0000);
	pgcDraw(1,1);
	pgcSetmag(1);
	pgScreenFrame(0,0);
	pgcLocate(0,0);
	pgcColor(0xffff,0x0000);
	pgcDraw(1,1);
	pgcSetmag(1);
	
//	pgiInit();
	
	ret=pgaInit();
	if (ret) pgErrorHalt("pga subsystem initialization failed.");

	ret=xmain();
	pgExit(ret);
#endif
}


void pgExit(int n)
{
#if 0
	pg_terminate=1;
	
	// terminate subsystem preprocess
	pgaTermPre();
	
	// terminate subsystem
	pgaTerm();
	
	__exit();

#endif
}

#endif

const char *pguGetMypath() 
{ 
	return pg_mypath; 
}

const char *pguGetWorkdir() 
{ 
	return pg_workdir; 
}



int pgfOpen(const char *filename, unsigned long flag)
{
	char fn[MAX_PATH*2];
	if (_strchr(filename,':')!=NULL || *filename=='/' || *filename=='\\')
	{
		return sceIoOpen(filename,flag);
	}
	else
	{
		_strcpy(fn,pg_workdir);
		_strcat(fn,filename);
		return sceIoOpen(fn,flag);
	}
}

void pgfClose(int fd)
{
	sceIoClose(fd);
}

int pgfRead(int fd, void *data, int size)
{
	return sceIoRead(fd,data,size);
}

int pgfWrite(int fd, void *data, int size)
{
	return sceIoWrite(fd,data,size);
}

int pgfSeek(int fd, int offset, int whence)
{
	return sceIoLseek(fd,offset,whence);
}


