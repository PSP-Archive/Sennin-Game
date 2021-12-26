#include "utility.h"
#include "syscall.h"
#include "pg.h"


// 0～45度(実際には0～32)用の数値、これを場合分けで0～360度(実際には0～255)にするためのテーブル
static const u8 atan256tbl[] =								// 258個
{
	 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3,
	 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5,
	 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8,
	 8, 8, 8, 9, 9, 9, 9, 9, 9, 9,10,10,10,10,10,10,
	10,11,11,11,11,11,11,12,12,12,12,12,12,12,13,13,
	13,13,13,13,13,14,14,14,14,14,14,14,15,15,15,15,
	15,15,15,16,16,16,16,16,16,16,16,17,17,17,17,17,
	17,17,18,18,18,18,18,18,18,18,19,19,19,19,19,19,
	19,20,20,20,20,20,20,20,20,21,21,21,21,21,21,21,
	21,21,22,22,22,22,22,22,22,22,23,23,23,23,23,23,
	23,23,23,24,24,24,24,24,24,24,24,24,25,25,25,25,
	25,25,25,25,25,26,26,26,26,26,26,26,26,26,27,27,
	27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,28,
	28,28,28,29,29,29,29,29,29,29,29,29,29,30,30,30,
	30,30,30,30,30,30,30,30,30,31,31,31,31,31,31,31,
	31,31,31,31,32,32,32,32,32,32,32,32,32,32,32,32,
	32,255												// <-この256個めが肝。257個目は穴埋め用（ダミー）
};

//--------------------------------------------------------------------
//
//	u8 ATAN2(FIX16 y,FIX16 y)
//
//		Yが先なので注意ね。
//		戻り値は、角度ね。 0-255
// ---------------------------------------------------------------------
u8 ATAN2(FIX16 y, FIX16 x)
{
	u8	result;

	if (x == 0 && y == 0)			// 位置が同じ
	{
		return(0);					// 本当は答えが無限になっちゃうから、zero返す
	}

	// -------------------------------------------------------
	// 場合わけをして戻り値を得る
	if (x > 0)
	{
		if (y > 0)
		{
			if (x > y)
			{
				result =     atan256tbl[(y<<8) / x];		// <<8 = *256
			}
			else
			{
				result = 64 -atan256tbl[(x<<8) / y];
			}
		}
		else
		{
			y = -y;
			if (x > y)
			{
				result = 256 -atan256tbl[(y<<8) / x];
			}
			else
			{
				result = 192 +atan256tbl[(x<<8) / y];
			}
		}
	}
	else
	{
		if (y > 0)
		{
			x = -x;
			if (x > y)
			{
				result = 128 -atan256tbl[(y<<8) / x];
			}
			else
			{
				result =  64 +atan256tbl[(x<<8) / y];
			}
		}
		else
		{
			x = -x;
			y = -y;
			if (x > y)
			{
				result = 128 +atan256tbl[(y<<8) / x];
			}
			else
			{
				result = 192 -atan256tbl[(x<<8) / y];
			}
		}
	}
	return(result & 0xff);
}


// SIN256TABLE
const int SINTBL256[] =
{
	     0,  1608,  3216,  4821,  6424,  8022,  9616, 11204,
	 12785, 14359, 15924, 17479, 19024, 20557, 22078, 23586,
	 25080, 26558, 28020, 29466, 30893, 32303, 33692, 35062,
	 36410, 37736, 39040, 40320, 41576, 42806, 44011, 45190,
	 46341, 47464, 48559, 49624, 50660, 51665, 52639, 53581,
	 54491, 55368, 56212, 57022, 57798, 58538, 59244, 59914,
	 60547, 61145, 61705, 62228, 62714, 63162, 63572, 63944,
	 64277, 64571, 64827, 65043, 65220, 65358, 65457, 65516,
	 65536, 65516, 65457, 65358, 65220, 65043, 64827, 64571,
	 64277, 63944, 63572, 63162, 62714, 62228, 61705, 61145,
	 60547, 59914, 59244, 58538, 57798, 57022, 56212, 55368,
	 54491, 53581, 52639, 51665, 50660, 49624, 48559, 47464,
	 46341, 45190, 44011, 42806, 41576, 40320, 39040, 37736,
	 36410, 35062, 33692, 32303, 30893, 29466, 28020, 26558,
	 25080, 23586, 22078, 20557, 19024, 17479, 15924, 14359,
	 12785, 11204,  9616,  8022,  6424,  4821,  3216,  1608,
	     0, -1608, -3216, -4821, -6424, -8022, -9616,-11204,
	-12785,-14359,-15924,-17479,-19024,-20557,-22078,-23586,
	-25080,-26558,-28020,-29466,-30893,-32303,-33692,-35062,
	-36410,-37736,-39040,-40320,-41576,-42806,-44011,-45190,
	-46341,-47464,-48559,-49624,-50660,-51665,-52639,-53581,
	-54491,-55368,-56212,-57022,-57798,-58538,-59244,-59914,
	-60547,-61145,-61705,-62228,-62714,-63162,-63572,-63944,
	-64277,-64571,-64827,-65043,-65220,-65358,-65457,-65516,
	-65536,-65516,-65457,-65358,-65220,-65043,-64827,-64571,
	-64277,-63944,-63572,-63162,-62714,-62228,-61705,-61145,
	-60547,-59914,-59244,-58538,-57798,-57022,-56212,-55368,
	-54491,-53581,-52639,-51665,-50660,-49624,-48559,-47464,
	-46341,-45190,-44011,-42806,-41576,-40320,-39040,-37736,
	-36410,-35062,-33692,-32303,-30893,-29466,-28020,-26558,
	-25080,-23586,-22078,-20557,-19024,-17479,-15924,-14359,
	-12785,-11204, -9616, -8022, -6424, -4821, -3216, -1608
};

// sin (256 angle) -65536～65536
int FixSin(int angle256)
{
	return SINTBL256[(u8)((angle256) & 0xFF)];
}

// cos (256 angle) -65536～65536
int FixCos(int angle256)
{
	return SINTBL256[(u8)((angle256+64) & 0xFF)];
}

//----------------------------------------------------------------
static ctrl_data_t padnow;
static ctrl_data_t padold;

//----------------------------------------------------------------
void SetNowKey()
{
	sceCtrlRead(&padnow, 1);
}

//----------------------------------------------------------------
// 直接
//u32 GetKeyStatDirect()
//{
//	volatile u32 *KEYS		= (u32*)0x4000130;
//	return(~*KEYS);				// bit 反転させてつかってたもれ
//}

//----------------------------------------------------------------
void SetOldKey()
{
	// backup
	padold = padnow;
}

//--------------------------
u8 GetDBT_Up()
{
	return ( (padnow.buttons & CTRL_UP) ? 1 : 0);
}
u8 GetPBT_Up()
{
	return( ((~padold.buttons) & CTRL_UP) &&  ( (padnow.buttons & CTRL_UP) ? 1 : 0) );
}

u8 GetDBT_Down()
{
	return ( (padnow.buttons & CTRL_DOWN) ? 1 : 0 );
}

u8 GetPBT_Down()
{
	return( ((~padold.buttons) & CTRL_DOWN) &&  ( (padnow.buttons & CTRL_DOWN) ? 1 : 0) );
}

u8 GetDBT_Left()
{
	return ( (padnow.buttons & CTRL_LEFT) ? 1 : 0);
}

u8 GetPBT_Left()
{
	return( ((~padold.buttons) & CTRL_LEFT) &&  ( (padnow.buttons & CTRL_LEFT) ? 1 : 0) );
}

u8 GetDBT_Right()
{
	return ((padnow.buttons & CTRL_RIGHT) ? 1 : 0);
}

u8 GetPBT_Right()
{
	return( ((~padold.buttons) & CTRL_RIGHT) &&  ( (padnow.buttons & CTRL_RIGHT) ? 1 : 0) );
}


//---------------------------------------------------------------- KEYかんけー
u8 GetDBT_CIRCLE()
{
	return ( (padnow.buttons & CTRL_CIRCLE) ? 1 : 0);
}

u8 GetPBT_CIRCLE()
{
	return( ((~padold.buttons) & CTRL_CIRCLE) &&  ( (padnow.buttons & CTRL_CIRCLE) ? 1 : 0) );
}

//--------------------------
u8 GetDBT_CROSS()
{
	return ( (padnow.buttons & CTRL_CROSS) ? 1 : 0 );
}

u8 GetPBT_CROSS()
{
	return( ((~padold.buttons) & CTRL_CROSS) &&  (padnow.buttons & CTRL_CROSS) ? 1 : 0) ;
}

//--------------------------
u8 GetDBT_SQUARE()
{
	return ( (padnow.buttons & CTRL_SQUARE) ? 1 : 0);
}

u8 GetPBT_SQUARE()
{
	return( ((~padold.buttons) & CTRL_SQUARE) &&  ( (padnow.buttons & CTRL_SQUARE) ? 1 : 0) );
}

//--------------------------
u8 GetDBT_TRIANGLE()
{
	return ( (padnow.buttons & CTRL_TRIANGLE) ? 1 : 0 );
}

u8 GetPBT_TRIANGLE()
{
	return( ((~padold.buttons) & CTRL_TRIANGLE) &&  (padnow.buttons & CTRL_TRIANGLE) ? 1 : 0) ;
}

//--------------------------
u8 GetDBT_Start()
{
	return ( (padnow.buttons & CTRL_START) ? 1 : 0);
}

u8 GetPBT_Start()
{
	return( ((~padold.buttons) & CTRL_START) &&  (padnow.buttons & CTRL_START) ? 1 : 0);
}

//--------------------------
u8 GetDBT_Select()
{
	return ((padnow.buttons & CTRL_SELECT) ? 1 : 0);
}

u8 GetPBT_Select()
{
	return( ((~padold.buttons) & CTRL_SELECT) && (padnow.buttons & CTRL_SELECT) ? 1 : 0);
}

//--------------------------
u8 GetDBT_R()
{
	return ((padnow.buttons & CTRL_RTRIGGER) ? 1 : 0);
}

u8 GetPBT_R()
{
	return( ((~padold.buttons) & CTRL_RTRIGGER) &&  (padnow.buttons & CTRL_RTRIGGER) ? 1 : 0) ;
}

//--------------------------
u8 GetDBT_L()
{
	return ((padnow.buttons & CTRL_LTRIGGER) ? 1 : 0);
}

u8 GetPBT_L()
{
	return( ((~padold.buttons) & CTRL_LTRIGGER) &&  (padnow.buttons & CTRL_LTRIGGER) ? 1 : 0) ;
}



u8 GetAnalogX()
{
	return padnow.analog[CTRL_ANALOG_X];
}
u8 GetAnalogY()
{
	return padnow.analog[CTRL_ANALOG_Y];
}

// -----------------------------------------
//	srand init(arand)
// -----------------------------------------
static s32 s=14624626,count=0;
void BRandInit()
{
	brand();
}

// -----------------------------------------
//	srand(arand)
// -----------------------------------------
s32 brand()
{
	s=s*1103515245+12345;
	count++;
	return (u16)((s>>16)&0xFFFF);
}

// ---------------------------------------------------------------------------------
#define POWER_CB_POWER      0x80000000 
#define POWER_CB_HOLDON      0x40000000 
#define POWER_CB_STANDBY   0x00080000 
#define POWER_CB_RESCOMP   0x00040000 
#define POWER_CB_RESUME      0x00020000 
#define POWER_CB_SUSPEND   0x00010000 
#define POWER_CB_EXT      0x00001000 
#define POWER_CB_BATLOW      0x00000100 
#define POWER_CB_BATTERY    0x00000080 
#define POWER_CB_BATTPOWER   0x0000007F 

// ---------------------------------------------------------------------------------
// HOME/POWER コールバック設定
// ---------------------------------------------------------------------------------
static int exit_callback(void) 
{ 
	// Exit game 
   sceKernelExitGame(); 

   return 0; 
} 

static void power_callback(int unknown, int pwrflags) 
{
   // Combine pwrflags and the above defined masks 
} 

// Thread to create the callbacks and then begin polling 
int CallbackThread(void *arg) 
{ 
	int cbid; 

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback); 
	sceSetExitCallback(cbid); 

	cbid = sceKernelCreateCallback("Power Callback", power_callback); 
	scePowerSetCallback(0, cbid); 

	sceKernelPollCallbacks(); 
} 

/* Sets up the callback thread and returns its thread id */ 
int SetupCallbacks(void) 
{ 
   int thid = 0; 

   //thid = sceKernelCreateThread("update_thread", (pg_threadfunc_t)&CallbackThread, 0x11, 0xFA0, 0, 0); 
   thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0); 
   if(thid >= 0) 
   {
      sceKernelStartThread(thid, 0, 0); 
   }

   return thid; 
} 

