//enemy.h

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "utility.h"

//----------------------------------------------------------
#define	ENEMYMAX	(100)

//----------------------------------------------------------
// ìGÇÃç\ë¢ëÃ
typedef struct ENEMY_tag
{
	u8		isUse;		// unuse/use
	FIX16	fx;			// position
	FIX16	fy;
	FIX16	speed;		// speed
	FIX16	dir;		// direction
	RECT	hitRect;
	
	s16		endMoveStep;	//
	void	(*move)(struct ENEMY_tag *_this);	// function pointer
} ENEMY;

//----------------------------------------------------------
extern void EnemyMove(ENEMY *_this);
extern void UpdateEnemy();
extern void CreateEnemy();
extern void EnemyMove(ENEMY *_this);
extern void ReleaseEnemy(ENEMY *pEnemy);
extern ENEMY *GetEmptyEnemy();
extern void EnemyInit();
extern void SetAllEnemyToPlayer(FIX16 x,FIX16 y);

extern ENEMY enemy[];

#endif
