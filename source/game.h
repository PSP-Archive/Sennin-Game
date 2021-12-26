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
	//u8		isUse;		// ���g�p/�g�p
	FIX16	fx;			// ���W
	FIX16	fy;
	FIX16	speed;		// ���x
	FIX16	dir;		// ����
	RECT	hitRect;
	// �ړ��p�̊֐��|�C���^
	//void	(*move)(struct ENEMY_tag *_this);
} PLAYER;


//----------------------------------------------------------
enum
{
	BGSCREEN_MAX = 5,
	BG_WIDTHPITCH = 480,
};

//----------------------------------------------------------
// �[�����d�X�N���[���p
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
