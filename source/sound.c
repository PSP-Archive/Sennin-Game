// sound.c
// original program by nem ( http://sec.pn.to/ )
// modified by MAGiCTOUCH / AND. (http://www10.big.or.jp/~and/ )

#include "utility.h"
#include "syscall.h"
#include "pg.h"
#include "sound.h"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// audio wave buffer
static char wavdata_bg[WAVFILEMAX_BG];	
static char wavdata_se0[WAVFILEMAX_SE];
static char wavdata_se1[WAVFILEMAX_SE];
static char wavdata_se2[WAVFILEMAX_SE];
static char wavdata_se3[WAVFILEMAX_SE];

// audio wave infomation
static wavout_wavinfo_t wavinfo_bg;
static wavout_wavinfo_t wavinfo_se0;
static wavout_wavinfo_t wavinfo_se1;
static wavout_wavinfo_t wavinfo_se2;
static wavout_wavinfo_t wavinfo_se3;


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
static WAVEOUT wavout;
static PGA pga;

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// proto type
void pgaSetChannelCallback(int channel, void *callback);
void wavout_snd0_callback(short *_buf, u32 _reqn);
void wavout_snd1_callback(short *_buf, u32 _reqn);

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void halt(char *str)
{
	//ADB("ERROR:");
	//ADB(str);
	while(1);
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// wave out initalize.
// return 0 if success
int wavoutInit()
{
	int i;

	// pointer Null clear
	wavout.snd0_wavinfoptr= NULL;

	// play mode initialize
	for (i=0; i<SND1_MAXSLOT; i++)
	{
		wavout.snd1_playing[i]= FALSE;
	}

	// sound callback set
	pgaSetChannelCallback(0,wavout_snd0_callback);
	pgaSetChannelCallback(1,wavout_snd1_callback);

	return 0;
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// for bgm.
void wavout_snd0_callback(short *_buf, u32 _reqn)
{
	s8 *src;
	s32 channels;
	u32 i;
	u32 long ptr,frac,rr,max;

	u32 reqn= _reqn;
	s16 *buf= _buf;
	
	wavout_wavinfo_t *wi = wavout.snd0_wavinfoptr;

	if (wi == NULL)
	{
		wavout.snd0_ready= TRUE;
		_memset(buf,0,reqn<2);				// zero clear(buffer clear)
		return;
	}
	
	ptr=		wi->playptr;
	frac=		wi->playptr_frac;
	rr=			wi->rateratio;
	max=		wi->samplecount;
	channels=	wi->channels;
	src=		wi->wavdata;

	wavout.snd0_ready= FALSE;

	for (; reqn>0; --reqn)
	{
		frac += rr;
		ptr += (frac>>16);
		frac &= 0xffff;

		if (ptr >= max)
		{
			if (wi->playloop)
			{
				ptr= NULL;
			}
			else
			{
				for (; reqn>0; --reqn)
				{
					// clear
					*(buf++)= 0;
					*(buf++)= 0;
				}
				goto playend;
			}
		}

		// channel check
		if (channels == 1)		
		{
			// mono.
			buf[0]= buf[1]= *(short *)(src + (ptr<<1));
			buf+= 2;
		}
		else
		{
			// stereo.
			buf[0]= *(short *)(src + (ptr<<2));
			buf[1]= *(short *)(src + (ptr<<2)+2);
			buf+= 2;
		}
	}

	wavout.snd0_playptr=	ptr;
	wi->playptr=			ptr;
	wi->playptr_frac=		frac;

	return;

playend:
	wavout.snd0_playend= TRUE;
	return;
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// SE
//		wave synthesize.
//
void wavout_snd1_callback(short *_buf, u32 _reqn)
{
	u32 i,slot;
	u32 ptr,frac;
	wavout_wavinfo_t *wi;
	s16 *buf= _buf;
	
	for (i=0; i<_reqn; i++)
	{
		int outr=0,outl=0;
		for (slot=0; slot<SND1_MAXSLOT; slot++)
		{
			// is playing?
			if (!wavout.snd1_playing[slot])
			{
				// to next slot.
				continue;
			}

			//target
			wi=					&wavout.snd1_wavinfo[slot];
			frac=				wi->playptr_frac + wi->rateratio;
			wi->playptr= ptr=	wi->playptr + (frac>>16);
			wi->playptr_frac=	(frac&0xffff);

			if (ptr >= wi->samplecount)
			{
				wavout.snd1_playing[slot]= FALSE;
				break;
			}

			// wave synthesize
			short *src= (short *)wi->wavdata;
			if (wi->channels == 1)
			{
				outl+= src[ptr];
				outr+= src[ptr];
			}
			else
			{
				outl+= src[ ptr<<1];
				outr+= src[(ptr<<1)+1];
			}
		}

		// output min/max limit check
		if (outl<-32768)
		{
			outl=-32768;
		} 
		else if (outl>32767)
		{
			 outl=32767;
		}

		if (outr<-32768)
		{
			outr=-32768;
		}
		else if (outr>32767)
		{
			outr=32767;
		}

		// write buffer
		*(buf++)= outl;			// left sound buffer
		*(buf++)= outr;			// right sound buffer
	}
}




// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void wavoutStopPlay0()
{
	if (wavout.snd0_wavinfoptr != NULL)
	{
		// wait.
		while (wavout.snd0_ready)
		{
			pgWaitV();
		}

		// paramater clear
		wavout.snd0_wavinfoptr= NULL;

		// wait.
		while (!wavout.snd0_ready)
		{
			pgWaitV();
		}
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void wavoutStartPlay0(wavout_wavinfo_t *wi)
{
	wavoutStopPlay0();

	// wait.
	while (!wavout.snd0_ready)
	{
		pgWaitV();
	}
	
	// paramater clear
	wavout.snd0_playptr=	0;
	wavout.snd0_playend=	0;
	wavout.snd0_wavinfoptr=	wi;

	// wait.
	while (wavout.snd0_ready)
	{
		pgWaitV();
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int wavoutWaitEnd0()
{
	if (wavout.snd0_wavinfoptr == NULL)
	{
		return -1;
	}

	if (wavout.snd0_wavinfoptr->playloop)
	{
		return -1;
	}

	// wait.
	while (!wavout.snd0_playend)
	{
		 pgWaitV();
	}
	return 0;
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//stop all
void wavoutStopPlay1()
{
	int i;
	for (i=0; i<SND1_MAXSLOT; i++)
	{
		wavout.snd1_playing[i]= FALSE;
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//return 0 if success
int wavoutStartPlay1(wavout_wavinfo_t *wi)
{
	int i;
	wavout_wavinfo_t *wid;

	for (i=0; i<SND1_MAXSLOT; i++)
	{
		if (wavout.snd1_playing[i] == FALSE)
		{
			break;
		}
	}

	if (i == SND1_MAXSLOT)
	{
		return -1;
	}

	wid= &wavout.snd1_wavinfo[i];
	wid->channels=		wi->channels;
	wid->samplerate=	wi->samplerate;
	wid->samplecount=	wi->samplecount;
	wid->datalength=	wi->datalength;
	wid->wavdata=		wi->wavdata;
	wid->rateratio=		wi->rateratio;
	wid->playptr=		0;
	wid->playptr_frac=	0;
	wid->playloop=		0;

	wavout.snd1_playing[i]= TRUE;

	return 0;
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void wavoutClearWavinfoPtr(wavout_wavinfo_t *wi)
{
	wi->playptr= 0;
	wi->playptr_frac= 0;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//return 0 if success
int wavoutLoadWav(const char *filename, wavout_wavinfo_t *wi, void *buf, u32 buflen)
{
	int fd;

	u32 filelen;
	u32 channels;
	u32 samplerate;
	u32 blocksize;
	u32 bitpersample;
	u32 datalength;
	u32 samplecount;
	u32 i;
	
	char *wavfile= buf;

	wi->wavdata= NULL;

	// wave data file open
	fd= pgfOpen(filename,OF_RDONLY);
	if (fd < 0)
	{
		// can't open
		return -1;
	}
	
	filelen= pgfRead(fd,wavfile,buflen);
	pgfClose(fd);

	if (filelen >= buflen)
	{
		//too long
		return -1;
	}
	
	if (_memcmp(wavfile,"RIFF",4) != 0)
	{
		// pgcPuts("format err");
		return -1;
	}
	
	if (_memcmp(wavfile+8,"WAVEfmt \x10\x00\x00\x00\x01\x00",14)!=0)
	{
		// pgcPuts("format err");
		return -1;
	}
	
	// infomation setting
	channels=		*(short*)(wavfile+0x16);
	samplerate=		*(long* )(wavfile+0x18);
	blocksize=		*(short*)(wavfile+0x20);
	bitpersample=	*(short*)(wavfile+0x22);
	
	if (_memcmp(wavfile+0x24,"data",4) != 0)
	{
		// pgcPuts("format err");
		return -1;
	}
	
	datalength= *(u32 *)(wavfile+0x28);
	
	if (datalength + 0x2c > filelen)
	{
		// pgcPuts("format err");
		return -1;
	}
	
	if (channels!=2 && channels!=1)
	{
		// pgcPuts("format err, channel");
		return -1;
	}
	
	//	if (samplerate!=44100 && samplerate!=22050 && samplerate!=11025) {
	if (samplerate>100000 || samplerate<2000)
	{
		// pgcPuts("format err, samplerate");
		return -1;
	}
	
	if (blocksize!=channels*2)
	{
		// pgcPuts("format err, blocksize");
		return -1;
	}
	
	if (bitpersample!=16)
	{
		// pgcPuts("format err, bitpersample");
		return -1;
	}
	
	if (channels==2)
	{
		samplecount= datalength>>2;
	}
	else
	{
		samplecount= datalength>>1;
	}
	
	if (samplecount<=0)
	{
		// pgcPuts("format err, samplecount");
		return -1;
	}
	
	// infomation setting
	wi->channels=		channels;
	wi->samplerate=		samplerate;
	wi->samplecount=	samplecount;
	wi->datalength=		datalength;
	wi->wavdata=		wavfile + 0x2c;
	wi->rateratio=		(samplerate*0x4000)/11025;
	wi->playptr=		0;
	wi->playptr_frac=	0;
	wi->playloop=		0;
	
	return 0;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void BGM_LOAD()
{
	// BGM
	int ret=wavoutLoadWav("bg0.wav",&wavinfo_bg,wavdata_bg,sizeof(wavdata_bg));
	if (ret!=0)
	{
		// error
		halt("op.wav load error");
	}
	
	// SE
	ret=wavoutLoadWav("se0.wav",&wavinfo_se0,wavdata_se0,sizeof(wavdata_se0));
	if (ret!=0)
	{
		// error
		halt("se0.wav load error");
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void BGM_PLAY()
{
	wavinfo_bg.playloop= TRUE;				// loop mode true
	wavoutStartPlay0(&wavinfo_bg);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void SE_PLAY()
{
	wavoutStartPlay1(&wavinfo_se0);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// callback from thread.
static int pga_channel_thread(int args, void *argp)
{
	volatile int bufidx=0;

	int channel= *(int *)argp;
	
	while (pga.terminate == FALSE)
	{
		void *bufptr= &pga.sndbuf[channel][bufidx];
		void (*callback)(void *buf, u32 reqn);

		callback= pga.channel_callback[channel];
		if (callback)
		{
			// callback execute.
			callback(bufptr,PGA_SAMPLES);
		}
		else
		{
			// no callback,clear sound buffer.
			int i;
			u32 *ptr= bufptr;
			for (i=0; i<PGA_SAMPLES; ++i)
			{
				*(ptr++)= 0;
			}
		}

		pgaOutBlocking(channel,0x8000,0x8000,bufptr);

		// swap double sound buffer
		bufidx= (bufidx?0:1);
	}

	sceKernelExitThread(0);
	return 0;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// callback setting
void pgaSetChannelCallback(int channel, void *callback)
{
	pga.channel_callback[channel]= callback;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// 'pga' initalize
int pgaInit()
{
	char str[32];
	int i,ret;

	// initialize
	pga.terminate= FALSE;
	pga.ready= FALSE;

	// clear infomation.
	for (i=0; i<PGA_CHANNELS; i++)
	{
		pga.handle[i]= -1;
		pga.threadhandle[i]= -1;
		pga.channel_callback[i]= 0;
	}

	// handle get
	int failed= FALSE;
	for (i=0; i<PGA_CHANNELS; i++)
	{
		if ( (pga.handle[i]= sceAudioChReserve(-1,PGA_SAMPLES,0)) < 0 )
		{
			// faild.
			failed=1;
		}
	}

	// failed?  ... yes,release audio and quit.
	if (failed)
	{
		for (i=0; i<PGA_CHANNELS; i++)
		{
			if (pga.handle[i] != -1)
			{
				sceAudioChRelease(pga.handle[i]);
			}
			pga.handle[i]= -1;
		}
		return -1;
	}
	
	// ready.
	pga.ready= TRUE;

	// makeing thread.
	strcpy(str,"pgasnd0");					// base thread name.
	for (i=0; i<PGA_CHANNELS; i++)
	{
		// create thread
		str[6]= '0'+i;		// ascii code change
		pga.threadhandle[i]= sceKernelCreateThread(str,(pg_threadfunc_t)&pga_channel_thread,0x12,0x10000,0,NULL);
		if (pga.threadhandle[i] < 0)
		{
			// error
			pga.threadhandle[i]= -1;
			failed= TRUE;
			break;
		}
		
		// start thread.
		ret= sceKernelStartThread(pga.threadhandle[i],sizeof(i),&i);
		if (ret != 0)
		{
			// error
			failed= TRUE;
			break;
		}
	}

	// failed?  ... yes,release tharead and quit.
	if (failed)
	{
		pga.terminate= TRUE;
		for (i=0; i<PGA_CHANNELS; i++)
		{
			if (pga.threadhandle[i] != -1)
			{
				sceKernelWaitThreadEnd(pga.threadhandle[i],NULL);
				sceKernelDeleteThread(pga.threadhandle[i]);
			}
			pga.threadhandle[i]= -1;
		}

		pga.ready= FALSE;

		return -1;
	}
	return 0;
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// initialize
void pgaTermPre()
{
	pga.ready= FALSE;
	pga.terminate= TRUE;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// terminate sound
void pgaTerm()
{
	int i;
	pga.ready= FALSE;
	pga.terminate= TRUE;

	// release tharead
	for (i=0; i<PGA_CHANNELS; i++)
	{
		if (pga.threadhandle[i]!= -1)
		{
			sceKernelWaitThreadEnd(pga.threadhandle[i],NULL);
			sceKernelDeleteThread(pga.threadhandle[i]);
		}
		pga.threadhandle[i]= -1;
	}

	for (i=0; i<PGA_CHANNELS; i++)
	{
		if (pga.handle[i] != -1)
		{
			sceAudioChRelease(pga.handle[i]);
			pga.handle[i] != -1;
		}
	}
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int pgaOutBlocking(u32 channel,u32 vol1,u32 vol2,void *buf)
{
	if (pga.ready==FALSE)					// is ready?
	{
		return -1;
	}

	if (channel>=PGA_CHANNELS) 		// channel ok?
	{
		return -1;
	}
	
	// maximum volume limmit check
	// overflow? yes...over volume cut.
	if (vol1>MAXVOLUME)
	{
		vol1= MAXVOLUME;
	}

	if (vol2>MAXVOLUME)
	{
		vol2= MAXVOLUME;
	}

	return sceAudioOutputPannedBlocking(pga.handle[channel],vol1,vol2,buf);
}
