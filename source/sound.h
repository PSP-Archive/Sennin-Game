// sound.h
// original program by nem ( http://sec.pn.to/ )
// modified by MAGiC TOUCH / AND. (http://www10.big.or.jp/~and/ )

#ifndef _SOUND_H_
#define _SOUND_H_

#include "syscall.h"

//---------------------
#define WAVFILEMAX_BG	(9*1024*1024)		// 9Mbytes
#define WAVFILEMAX_SE	(1*1024*1024)		// 1Mbytes
#define SND1_MAXSLOT 	(16)				// SE Slot max

#define PGA_CHANNELS	(3)
#define PGA_SAMPLES		(256)
#define MAXVOLUME		(0x8000)

//---------------------
typedef struct wavout_wavinfo_tag
{
	unsigned long channels;
	unsigned long samplerate;
	unsigned long samplecount;
	unsigned long datalength;
	char *wavdata;
	unsigned long rateratio;		// sample rate / 44100 * 0x10000
	unsigned long playptr;
	unsigned long playptr_frac;
	int playloop;
} wavout_wavinfo_t;

//---------------------
typedef struct waveout_tag
{
	wavout_wavinfo_t *snd0_wavinfoptr;
	wavout_wavinfo_t snd1_wavinfo[SND1_MAXSLOT];

	// sound 0 / BGM
	int snd0_ready;
	int snd0_playend;
	u32 snd0_playptr;

	// sound 1 / SE
	int snd1_playing[SND1_MAXSLOT];

} WAVEOUT;

//---------------------
typedef struct pga_tag
{
	volatile int terminate;
	int		ready;
	int		handle[PGA_CHANNELS];

	// synthesize sound buffer
	short	sndbuf[PGA_CHANNELS][2][PGA_SAMPLES][2];	//	3ch x 2 x 256sample x2ch  (double buffer ?)

	// threadhandle x2
	int		threadhandle[PGA_CHANNELS];

	// callback function
	void (*channel_callback[PGA_CHANNELS])(void *buf, u32 reqn);

} PGA;

//---------------------
extern int pgaInit();

extern void BGM_LOAD();
extern void BGM_PLAY();
extern void SE_PLAY();

#endif

