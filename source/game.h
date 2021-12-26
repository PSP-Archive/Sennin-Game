#ifndef _GAME_H_
#define _GAME_H_

//----------------------------------------------------------
typedef struct
{
	FIX16 x,y;
	int height;
	int offsety;
	int speed;
} BGSCREEN;

typedef struct PLAYER_tag
{
	//u8		isUse;		// 未使用/使用
	FIX16	fx;			// 座標
	FIX16	fy;
	FIX16	speed;		// 速度
	FIX16	dir;		// 方向
	RECT	hitRect;
	// 移動用の関数ポインタ
	//void	(*move)(struct ENEMY_tag *_this);
} PLAYER;


//----------------------------------------------------------
enum
{
	BGSCREEN_MAX = 5,
	BG_WIDTHPITCH = 480,
};

//----------------------------------------------------------
// 擬似多重スクロール用
typedef struct
{
	int offsety;
	int height;
	FIX16 speed;
} BGDATA;

void InitBG();
void BitBltBG(u32 x,u32 y,u32 w,u32 h,const u16 *pSrc,int pitch);
void DrawBG();
void SinRasterTest();

int Judge(PLAYER *player);
void PlayerMove(FIX16 *mx,FIX16 *my);

#endif
