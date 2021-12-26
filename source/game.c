// GAME
#include "utility.h"
#include "game.h"
#include "syscall.h"
#include "sennindata.h"
#include "pg.h"
#include "enemy.h"

//----------------------------------------------------------
BGSCREEN bg[BGSCREEN_MAX];


//----------------------------------------------------------
const BGDATA bgmapdata[]=	// ypos
{
	 0,	32,FLOAT2F(0.5f),
	32,	32,FLOAT2F(1.3f),
	64,	32,FLOAT2F(2.6f),
	96,	48,FLOAT2F(3.8f),
	144,40,FLOAT2F(4.2f),
};

//----------------------------------------------------------
// 背景スクロール用の各種パラメータ初期化
void InitBG()
{
	const int bias = 272-184;

	int i;
	for (i=0;i<BGSCREEN_MAX; i++)
	{
		bg[i].x = 0;
		bg[i].y = INT2F(bgmapdata[i].offsety + bias);
		bg[i].height = bgmapdata[i].height;
		bg[i].offsety = bgmapdata[i].offsety*BG_WIDTHPITCH;
		bg[i].speed = bgmapdata[i].speed;
	}
}

//----------------------------------------------------------
void BitBltBG(u32 x,u32 y,u32 w,u32 h,const u16 *pSrc,int pitch)
{
	u8 *pVRAMTOP;		//pointer to VRAM
	u8 *pVRAM;			//pointer to VRAM
	u32 xx,yy,mx,my;
	const u16 *pDest;
	
	// 範囲外描画をさせないように（Y限定）
	u8 *pVRAMMin = pgGetVramAddr(0,0);			// min limit
	u8 *pVRAMMax = pgGetVramAddr(479,271);		// max limit

	pVRAMTOP=pgGetVramAddr(x,y);
	for (yy=0; yy<h; yy++)
	{
		pVRAM= pVRAMTOP;
		pDest= pSrc;
		for (xx=0; xx<w; xx++)
		{
			if (pVRAM < pVRAMMin || pVRAM > pVRAMMax)
			{
				// 描画が範囲外ならばこの列は描画しない
				continue;
			}

			*(u16*)pVRAM= *pDest;
			pVRAM+=PIXELSIZE*2;
			pDest++;
		}
		pVRAMTOP += LINESIZE*2;
		pSrc+=pitch;
	}
}

void DrawBG()
{
	int	i;
	for(i=0;i<BGSCREEN_MAX; i++)
	{
		// その１メイン
		int len = 480-FIX2I(bg[i].x);
		BitBltBG(FIX2I(bg[i].x), FIX2I(bg[i].y), len,bg[i].height,screenbgdata+bg[i].offsety ,BG_WIDTHPITCH);

		// その２端数
		len = FIX2I(bg[i].x);
		BitBltBG(FIX2I(      0), FIX2I(bg[i].y), len,bg[i].height,screenbgdata+bg[i].offsety +(480-len),BG_WIDTHPITCH);

		bg[i].x-=bg[i].speed;
		if (bg[i].x<0){bg[i].x = INT2F(480)+bg[i].x;}
	}

}

static int amplitude = 0;	// 振幅
#define	BGPNG_HEIGHT	184

void SinRasterTest()
{
	int bias = 272-BGPNG_HEIGHT;
		
	int i;
	for(i=0;i<BGPNG_HEIGHT; i++)
	{
		// その１メイン
		int x = (1<<7) + FIX2I(FixSin(amplitude+i)<<6);		// ±64
		x= (480+x)%480;

		int len = 480-x;
		BitBltBG(x, bias+i, len,1,screenbgdata+i*BG_WIDTHPITCH ,BG_WIDTHPITCH);

		// その２端数
		len = x;
		BitBltBG(0, bias+i, len,1,screenbgdata+i*BG_WIDTHPITCH +(480-len),BG_WIDTHPITCH);

	}

	amplitude++;
}


// 手抜き判定判定
int Judge(PLAYER *player)
{
	// プレイヤの中心
	int i;
	for (i=0;i<ENEMYMAX; i++)
	{
		if (enemy[i].isUse)	
		{
			if ( (FIX2I(enemy[i].fx)+enemy[i].hitRect.x)                    <= (player->fx+player->hitRect.x+player->hitRect.w)
			 &&  (FIX2I(enemy[i].fy)+enemy[i].hitRect.y)                    <= (player->fy+player->hitRect.y+player->hitRect.h)
			 &&  (FIX2I(enemy[i].fx)+enemy[i].hitRect.x+enemy[i].hitRect.w) >= (player->fx+player->hitRect.x)
			 &&  (FIX2I(enemy[i].fy)+enemy[i].hitRect.y+enemy[i].hitRect.h) >= (player->fy+player->hitRect.y) )
			{
				// あたり
				SE_PLAY();

				//SetAllEnemyToPlayer(player->fx,player->fy);
				return 1;
			}
		}
	}
	
	// あたらない
	return 0;
}











void PlayerMove(FIX16 *mx,FIX16 *my)
{
	int ax = GetAnalogX();
	int ay = GetAnalogY();

/*
	char str[10];
	_ustoa(ax, str);
	mh_print(10,10,str,0,0,0);
	_ustoa(ay, str);
	mh_print(30,10,str,0,0,0);
*/

	if (ABS(ax-128)>ANALOG_THRESHOLD)		// 閾値を超えたら入力アリとする
	{
		*mx += (ax-128)/24;
	}

	if (ABS(ay-128)>ANALOG_THRESHOLD)		// 閾値を超えたら入力アリとする
	{
		*my += (ay-128)/24;
	}

	if (*mx<0)
	{
		*mx = 0;
	}
	else if (*mx>480-32)
	{
		*mx = 480-32;
	}

	if (*my<0)
	{
		*my = 0;
	}
	else if (*my>272-32)
	{
		*my = 272-32;
	}
}

