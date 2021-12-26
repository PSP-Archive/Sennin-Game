#include "enemy.h"
#include "sennindata.h"

ENEMY enemy[ENEMYMAX];



// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// prototype
void EnemyMove0(ENEMY *_this);
void EnemyMove1(ENEMY *_this);

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void EnemyInit()
{
	int i;
	for(i=0;i<ENEMYMAX; i++)
	{
		enemy[i].isUse = FALSE;
		enemy[i].fx = enemy[i].fy = enemy[i].speed = enemy[i].dir = 0;
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
ENEMY *GetEmptyEnemy()
{
	int i;
	for(i=0;i<ENEMYMAX; i++)
	{
		if (!enemy[i].isUse)	
		{
			// empty buffer found.
			enemy[i].isUse = TRUE;
			return &enemy[i];
		}
	}
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void CreateEnemy()
{
	ENEMY *pEnemy = GetEmptyEnemy();
	
	pEnemy->fx = INT2F(480);
	pEnemy->fy = INT2F( (brand()*2)%(272-32) );
	pEnemy->speed = FLOAT2F(-4.0f);
	pEnemy->dir = 64;
	pEnemy->move = EnemyMove0;
	pEnemy->endMoveStep = 0;
	
	// collision rect
	pEnemy->hitRect.x = 8;
	pEnemy->hitRect.y = 8;
	pEnemy->hitRect.w = 16;
	pEnemy->hitRect.h = 16;

}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void SetAllEnemyToPlayer(FIX16 fx,FIX16 fy)
{
	int i;
	for(i=0;i<ENEMYMAX; i++)
	{
		if (enemy[i].isUse)	
		{
			enemy[i].dir =
				ATAN2( fy - enemy[i].fy , fx - enemy[i].fx );

			// set 'move function'
			enemy[i].move = EnemyMove1;

		}
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void ReleaseEnemy(ENEMY *pEnemy)
{
	int i;
	
	for(i=0;i<ENEMYMAX; i++)
	{
		if (&enemy[i] == pEnemy)	
		{
			// set 'not use'
			enemy[i].isUse = FALSE;
		}
	}
	
	//ASSERT!
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// normal
void EnemyMove0(ENEMY *_this)
{
	_this->fx += ((FIX32)FixSin(_this->dir) * _this->speed) >>16;
	_this->fy += ((FIX32)FixCos(_this->dir) * _this->speed) >>16;

	if (_this->fx < INT2F(-32))	// ‰æ–ÊŠO
	{
		ReleaseEnemy(_this);
		return;
	}

	// Draw enemy 'sennin'
	pgBitBltTrans0(FIX2I(_this->fx),FIX2I(_this->fy),32,32,sennnindata);
}



// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// move to player 
void EnemyMove1(ENEMY *_this)
{
	_this->fx += ((FIX32)FixSin(_this->dir) * _this->speed) >>16;
	_this->fy += ((FIX32)FixCos(_this->dir) * _this->speed) >>16;

	if (_this->fx < INT2F(-32) || _this->fy < INT2F(-32) || _this->fx > INT2F(480) || _this->fy > INT2F(272))	// out of screen area
	{
		ReleaseEnemy(_this);
		return;
	}

	// Draw enemy 'sennin'
	pgBitBltTrans0(FIX2I(_this->fx),FIX2I(_this->fy),32,32,sennnindata);
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void UpdateEnemy()
{
	int i;
	for(i=0;i<ENEMYMAX; i++)
	{
		if (enemy[i].isUse)	
		{
			(*enemy[i].move)(&enemy[i]);
		}
	}
}

