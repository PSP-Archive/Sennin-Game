// system call prototype for PSP
//
// modified by MAGiCTOUCH(http://www10.big.or.jp/~and/)		Jun29,2005
//
// http://pspdev.ofcode.com/api.php

#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#define	DEBUG	0

typedef int (*pg_threadfunc_t)(int args, void *argp);
typedef unsigned long size_t;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// sceCtrl
#define CTRL_ANALOG_X   0 
#define CTRL_ANALOG_Y   1 

// Button bit masks
#define CTRL_SQUARE		0x8000 
#define CTRL_TRIANGLE	0x1000 
#define CTRL_CIRCLE		0x2000 
#define CTRL_CROSS		0x4000 
#define CTRL_UP			0x0010 
#define CTRL_DOWN		0x0040 
#define CTRL_LEFT		0x0080 
#define CTRL_RIGHT		0x0020 
#define CTRL_START		0x0008 
#define CTRL_SELECT		0x0001 
#define CTRL_LTRIGGER	0x0100 
#define CTRL_RTRIGGER	0x0200 

// Returned control data
typedef struct _ctrl_data 
{ 
   u32 frame; 
   u32 buttons; 
   u8  analog[4]; 
   u32 unused; 
} ctrl_data_t; 


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// IoFileMgrForUser
#define O_RDONLY    0x0001 
#define O_WRONLY    0x0002 
#define O_RDWR      0x0003 
#define O_NBLOCK    0x0010 
#define O_APPEND    0x0100 
#define O_CREAT     0x0200 
#define O_TRUNC     0x0400 
#define O_NOWAIT    0x8000 

enum
{ 
    TYPE_DIR=0x10, 
    TYPE_FILE=0x20 
}; 

typedef struct dirent
{ 
    unsigned long unk0; 
    unsigned long type; 
    unsigned long size; 
    unsigned long unk[19]; 
    char name[0x108]; 
	unsigned char dmy[128];
} dirent_t;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// sceDisplay
extern void sceDisplayWaitVblankStart();
extern void sceDisplaySetMode(long,long,long);
extern void sceDisplaySetFrameBuf(char *topaddr,long linesize,long pixelsize,long);

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// ctrl
extern void  sceCtrlInit(int unknown); 
extern void  sceCtrlSetAnalogMode(int on); 
extern void  sceCtrlRead(ctrl_data_t* paddata, int unknown); 

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// file
extern int  sceIoOpen(const char* file, int mode); 
extern void sceIoClose(int fd); 
extern int  sceIoRead(int fd, void *data, int size); 
extern int  sceIoWrite(int fd, void *data, int size); 
extern int  sceIoLseek(int fd, int offset, int whence); 
extern int  sceIoRemove(const char *file); 
extern int  sceIoMkdir(const char *dir, int mode); 
extern int  sceIoRmdir(const char *dir); 
extern int  sceIoRename(const char *oldname, const char *newname); 
extern int  sceIoDopen(const char *fn); 
extern int  sceIoDread(int fd, dirent_t *de); 
extern void sceIoDclose(int fd); 
//int sceIoDevctl(const char *name int cmd, void *arg, size_t arglen, void *buf, size_t *buflen); 


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// thread
extern void sceKernelPollCallbacks(void); 
extern int  sceKernelCreateCallbacks(const char *name, void *func); 
extern int  sceKernelCreateThread(const char *name, void *func, int a,int b,int c,int d);
extern void sceKernelExitGame(void); 
extern void sceSetExitCallback(int cbid); 
extern void scePowerSetCallback(int zero, int cbid); 

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// audio
extern void sceAudioOutput();										//
extern void sceAudioOutputPanned();									//
extern long sceAudioOutputBlocking(long, long, long, void *);		//
extern long sceAudioChReserve(long, long samplecount, long);		//init buffer? returns handle, minus if error
extern void sceAudioChRelease(long handle);							//free buffer?
extern void sceAudioGetChannelRestLen();							//
extern long sceAudioSetChannelDataLen(long, long);					//
extern void sceAudioChangeChannelConfig();							//
extern void sceAudioChangeChannelVolume();							//

/*
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// AUDIO ê›íË
extern void sceAudio_1();								//
extern long sceAudio_2(long, long, long, void *);		//
extern long sceAudio_3(long, long samplecount, long);	//init buffer? returns handle, minus if error
extern void sceAudio_4(long handle);					//free buffer?
extern void sceAudio_5();								//
extern long sceAudio_6(long, long);					//
extern void sceAudio_7();								//
extern void sceAudio_8();								//
*/

// PSPE debug SETTING
#if DEBUG
	#define ADB(_s) pspeDebugWrite(_s,_strlen(_s))
#else
	#define	ADB(_s)	;
#endif


#endif
