/********************************************************************
*																	*
*							Game.c									*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Driver for game										*
*																	*
********************************************************************/

/********************************************************************
*																	*
*							Includes								*
*																	*
********************************************************************/

#include "User.h"

#define HERO		(char) 1//5
#define HERO2		(char) 2//6

#define jumpAmount	(float) 5.0
#define jumpInc		(float) 0.25
#define fallDec		(float) 0.33
#define moveInc		(float) 1.0
#define jumpBonus	(float) 0.15

typedef struct _Hero {
	char displayChar;
	char displayChar2;
	float globalX, globalY;
	BOOL normal;
	BOOL jumping;
	BOOL falling;
	float jumpHeight;
	float jumpMax;
} Hero, * pHero;

typedef enum _direction {
	kMoveLeft,
	kMoveRight
} direction;

void MoveHero (pHero hero, pMap map, pMap back, direction dir)
{
	int width = map->width;

	switch (dir)
	{
	case kMoveLeft:
		if ((int) hero->globalX && !FLAGSET (/**(map->world.flags*/(map->world.buffer + ((int) hero->globalY * width + (int) hero->globalX - 1))->flags,SOLID))
		{
			hero->globalX -= moveInc;

			if ((int) (hero->globalX - map->xOffset) < 60)
			{
				ScrollMap (back, kLeft, kVertFix);
				ScrollMap (map, kLeft, kVertFix);
			}
		}

		break;

	case kMoveRight:
		if ((int) hero->globalX < (width - 1) && !FLAGSET (/**(map->world.flags*/(map->world.buffer + ((int) hero->globalY * width + (int) hero->globalX + 1))->flags,SOLID))
		{
			hero->globalX += moveInc;

			if ((int) (hero->globalX - map->xOffset) > 20)
			{
				ScrollMap (back, kRight, kVertFix);
				ScrollMap (map, kRight, kVertFix);
			}
		}

		break;
	}
}

void Jump (pHero hero, pMap map, pMap back)
{
	int width = map->width;

	if (FLAGSET(/**(map->world.flags*/(map->world.buffer + ((int) (hero->globalY - jumpInc) * width + (int) hero->globalX))->flags,SOLID))
	{
		hero->jumping = FALSE;
		hero->jumpHeight = 0.0;
	}

	else
	{
		hero->jumpHeight += jumpInc;

		if (hero->jumpHeight >= hero->jumpMax)
		{
			hero->jumping = FALSE;
			hero->jumpHeight = 0.0;
		}

		hero->globalY -= jumpInc;

		if (map->yOffset > 20 && (int) (hero->globalY - map->yOffset) < 15)
		{
			ScrollMap (back, kHorzFix, kUp);
			ScrollMap (map, kHorzFix, kUp);
		}

		Sleep (10);
	}	
}

void Fall (pHero hero, pMap map, pMap back)
{
	int width = map->width;
	int height = map->height;

	if (FLAGSET(/**(map->world.flags*/(map->world.buffer + ((int) (hero->globalY + fallDec) * width + (int) hero->globalX))->flags,SOLID) || hero->jumping)
	{
		hero->falling = FALSE;
		return;
	}

	else
	{
		if (!hero->falling)
		{
			hero->falling = TRUE;
			hero->jumpMax = jumpAmount;
		}

		hero->globalY += fallDec;

		if (map->yOffset < (height - 20) && ((int) hero->globalY - map->yOffset) > 35)
		{
			ScrollMap (back, kHorzFix, kDown);
			ScrollMap (map, kHorzFix, kDown);
		}

		Sleep (10);
	}
}

/********************************************************************
*																	*
*							Game Wrapper							*
*																	*
********************************************************************/

void Game (void)
{
	Hero hero;
	pMap map, back;
	Objects objects;
	int xIndex, yIndex, mapIndex;
	int loop = TRUE;

	ZeroMemory (&hero, sizeof (Hero));
	ZeroMemory (&objects, sizeof (Objects));

	hero.displayChar = HERO;
	hero.displayChar2 = HERO2;
	hero.jumpMax = jumpAmount;

	ConsoleInit (&objects, ENABLE_PROCESSED_INPUT, FALSE);

	MALLOC(map,Map);
	MALLOC(back,Map);

	ReloadMap (&objects.fileObj, map, "Map.map");
	ReloadMap (&objects.fileObj, back, "Back.map");

	while (loop)
	{
		if (hero.jumping)
		{
			Jump (&hero, map, back);
		}

		Fall (&hero, map, back);

		switch (GetInput (&objects.inputObj, kAsync))
		{
		case VK_LEFT:
			MoveHero (&hero, map, back, kMoveLeft);

			break;

		case VK_RIGHT:
			MoveHero (&hero, map, back, kMoveRight);

			break;

		case VK_SPACE:
			if (!hero.falling)
			{
				if (!hero.jumping)
				{
					hero.jumping = TRUE;
				}

				else
				{
					hero.jumpMax += jumpBonus;
				}
			}

			break;


		case VK_TAB:
			hero.globalX = hero.globalY  = hero.jumpHeight = 0.0;
			hero.falling = hero.jumping = FALSE;

			map->xOffset = map->yOffset = 0;

			back->xOffset = back->yOffset = 0;

			break;

		case VK_ESCAPE:
			loop = FALSE;
			break;
		
		default:
			break;
		}

		CopyMapToBuffer (&objects.outputObj.outputBuf, back);
		CopyMapToBuffer (&objects.outputObj.outputBuf, map);

		yIndex = (int) (hero.globalY - map->yOffset);
		xIndex = (int) (hero.globalX - map->xOffset);

		mapIndex = (int) hero.globalY * map->width + (int) hero.globalX;

		hero.normal = !hero.normal;

		if (!FLAGSET(/**(map->world.flags*/(map->world.buffer + mapIndex)->flags,OBSCURE))
		{
			(*(objects.outputObj.outputBuf.buffer + yIndex) + xIndex)->Char.AsciiChar = hero.normal ? hero.displayChar : hero.displayChar2;
			(*(objects.outputObj.outputBuf.buffer + yIndex) + xIndex)->Attributes = (WORD) MAKEBYTE(0x1,0x0);
		}

		UpdateScreen (&objects.outputObj);
	}

	DeleteMap (map);
	FREE(map);

	DeleteMap (back);
	FREE(back);

	DeinitializeObjects (&objects);
}

/********************************************************************
*																	*
*							Loop									*
*																	*
********************************************************************/

/********************************************************************
*																	*
*							Termination								*
*																	*
********************************************************************/

/********************************************************************
*																	*
*							Wrappers								*
*																	*
********************************************************************/