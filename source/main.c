// Hello World for PSP
// Created 2005.04.30  by nem 

// SENNIN GAME
// modified Jun28,2005 by MAGiC TOUCH(AND).

#include "utility.h"
#include "syscall.h"
#include "pg.h"
#include "font.h"
#include "sennindata.h"
#include "game.h"
#include "enemy.h"

//----------------------------------------------------------
enum
{
	eMODELOGO = 0,
	eMODETITLE,
	eMODEGAME,
	eMODEGAMEOVER
};

//----------------------------------------------------------
// value
char pg_mypath[MAX_PATH];
char pg_workdir[MAX_PATH];

//----------------------------------------------------------
// prototype
int ModeLogo();
int ModeTitle();
int ModeGame();
int ModeGameOver();

//----------------------------------------------------------
int result = 0;
int gcnt = 0;
int scrMode = 0;
int score=0;
int high_score=0;
int	waitTimer = 0;
u32 fillColor = 0;
PLAYER player;

#define SOUND_ON
// サウンドをOFFにすると PSPEでも動くようになるけど
// PSPEはアナログステック対応じゃないので微妙

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//	main
//
//
//
//
//
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int main(u32 args, void *argp)
{
	int ret;
	int n;

	// 環境変数の取得
	n=args;
	if (n>sizeof(pg_mypath)-1)
	{
		n= sizeof(pg_mypath)-1;
	}
	_memcpy(pg_mypath,argp,n);
	pg_mypath[sizeof(pg_mypath)-1] = '\0';;
	_strcpy(pg_workdir,pg_mypath);

	for (n=_strlen(pg_workdir); n>0 && pg_workdir[n-1]!='/'; --n)
	{
		pg_workdir[n-1]=0;
	}

	//--------------------------------------------------------
	// initalize
	pgInit();
	SetupCallbacks();			// callback( HOME Button )
	pgScreenFrame(2,0);

	// Pad init
	sceCtrlInit(0);
	sceCtrlSetAnalogMode(1);

	// sound init
#ifdef SOUND_ON
	pgaInit();
	wavoutInit();
	BGM_LOAD();
	BGM_PLAY();
#endif


	// 乱数初期化
	BRandInit();

	// 背景設定
	InitBG();


	fillColor = rgb2col(0,0,0);
	score = 0;
	waitTimer=90;

	//------------------------------
	// Main Loop
	int mode = eMODELOGO;
	while(1)
	{
		//------------------------------
		SetOldKey();
		pgScreenFlipV();
		pgFillvram(fillColor);
		SetNowKey();

		switch(mode)
		{
			case eMODELOGO:
				result = ModeLogo();
				if (result)
				{
					mode = eMODETITLE;
					fillColor = rgb2col(0,0,0);
					waitTimer=10*60;
				}
				break;

			case eMODETITLE:
				result = ModeTitle();
				switch(result)
				{
					case 1:
						mode = eMODEGAME;

						EnemyInit();

						player.fx = 64;
						player.fy = (272+32)/2;
						player.hitRect.x = 8;
						player.hitRect.y = 8;
						player.hitRect.w = 16;
						player.hitRect.h = 16;

						fillColor = rgb2col(0,255,255);
						score = 0;
						break;
						
					case 2:
						mode = eMODELOGO;
						waitTimer= 90;
						break;
				}
				break;
				
			case eMODEGAME:
				result = ModeGame();
				if (result)
				{
					mode = eMODEGAMEOVER;
				}
				break;
				
			case eMODEGAMEOVER:
				result = ModeGameOver();
				if (result)
				{
					mode = eMODETITLE;
					fillColor = rgb2col(0,0,0);
					waitTimer= 10*60;
				}
				break;
		}
		gcnt++;		// global counter
	}

	return 0;
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int ModeGame()
{
	char str[10];

	PlayerMove(&player.fx,&player.fy);

	// se test & scrool mode swap
	if (GetPBT_CROSS())
	{
		SE_PLAY();
		scrMode = 1-scrMode;
	}

	//-----------------------------------
	// born enemy
	if (gcnt%8 == 0 )
	{
		CreateEnemy();
	}

	//-----------------------------------
	// scrool
	switch(scrMode)
	{
		case 0:
			DrawBG();
			break;

		case 1:
			SinRasterTest();
			break;
	}

	//-----------------------------------
	// 敵描画
	UpdateEnemy();

	//-----------------------------------
	// 自分描画
	pgBitBltTrans0(player.fx,player.fy,32,32,playerdata+((gcnt/30)&1)*32*32);

	//-----------------------------------
	// Score Draw
	score+= 1+(player.fx/96);

	mh_print(250, 4,"HIGH SCORE",rgb2col(0,0,0),0,0);
	_ustoa(high_score, str);
	mh_print(250+24+ 80-_strlen(str)*10, 4,str,rgb2col(0,0,0),0,0);

	mh_print(400  , 4,"SCORE",rgb2col(0,0,0),0,0);
	_ustoa(score, str);
	mh_print(400+80-_strlen(str)*10, 4,str,rgb2col(0,0,0),0,0);

	//-----------------------------------
	// collision check
	result = Judge(&player);
	if (result)
	{
		// hit me!
		return 1;
	}
	return 0;

}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int ModeLogo()
{
	pgBitBlt(0,0,480,272,1,logodata);

	waitTimer--;
	if (GetPBT_Start()  || waitTimer<=0)
	{
		return 1;
	}
	return 0;

}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int ModeTitle()
{
	pgBitBlt(0,0,480,272,1,titledata);


	mh_print(378, 240,"MAGiC TOUCH Presents",rgb2col(0,0,0),0,0);
	mh_print(338, 250,"http://www10.big.or.jp/~and/",rgb2col(0,0,0),0,0);

	char title[150];
	_strcpy(title,"Free PSP development tools test program version 0.02 ");
	_strcat(title,__DATE__);
	_strcat(title," [");
	_strcat(title,__TIME__);
	_strcat(title,"]");
	mh_print(104, 260,title,rgb2col(0,0,0),0,0);

	char str[256];

	mh_print(250, 4,"HIGH SCORE",rgb2col(0,0,0),0,0);
	_ustoa(high_score, str);
	mh_print(250+24+ 80-_strlen(str)*10, 4,str,rgb2col(0,0,0),0,0);

	mh_print(400  , 4,"SCORE",rgb2col(0,0,0),0,0);
	_ustoa(score, str);
	mh_print(400+80-_strlen(str)*10, 4,str,rgb2col(0,0,0),0,0);

	
	if (GetPBT_Start())
	{
		return 1;
	}
	
	waitTimer--;
	if (waitTimer<=0)
	{
		return 2;
	}
		return 0;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int ModeGameOver()
{
	// ハイスコアー更新？
	if (high_score<score)
	{
		high_score = score;
	}

	pgWaitVn(3*60);

	return 1;
}

