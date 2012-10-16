/********************************************************************
*																	*
*							User.h									*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains information separate from SDK				*
*																	*
********************************************************************/

#ifndef USER_H
#define USER_H

/********************************************************************
*																	*
*							Includes								*
*																	*
********************************************************************/

#include "Common.h"
#include "ADT.h"
#include "File.h"
#include "Input.h"
#include "Interface.h"
#include "Mathematics.h"
#include "Output.h"
#include "Resources.h"
#include "Scene.h"

/********************************************************************
*																	*
*							Defines									*
*																	*
********************************************************************/

#define NUM_MENUFUNCTIONS 25
// Number of functions in the menu jump table
#define NUM_ITEMFUNCTIONS 25
// Number of functions in the item jump table
#define NUM_CHARFUNCTIONS 25
// Number of functions in the character jump table
#define NUM_MOVEFUNCTIONS 25
// Number of functions in the move jump table

/********************************************************************
*																	*
*							User-specific data structures			*
*																	*
********************************************************************/

typedef enum _UserMode {
	kGame,	// Play game
	kEditor	// Utilize editor
} UserMode;
// Type used to determine which user mode to run in

typedef struct _Triangles {
	Point3 a;
	Point3 b;
	Point3 c;
	POINT A;
	POINT B;
	POINT C;
	RECT outputRect;
	CHAR Char;
	WORD color;
	double low;
	Vector3 axis;
	CHAR axisChar;
	float degrees;
	int index;
	float cosine, sine;
} Triangles, * pTriangles;

/********************************************************************
*																	*
*							Game Wrapper							*
*																	*
********************************************************************/

void Game (void);

/********************************************************************
*																	*
*							Editor Wrapper							*
*																	*
********************************************************************/

void Editor (void);

/********************************************************************
*																	*
*							Initialization							*
*																	*
********************************************************************/

void MapEdit (pMap map, pObjects objects);
void ImageEdit (pImage image, pObjects objects);

void InitCharactersAndColorsWindow (pParentWindow parentWindow, pObjects objects);
void InitDataWindow (pParentWindow parentWindow, pObjects objects);
void InitCompressionWindow (pParentWindow parentWindow, pObjects objects);

void InitImageFlagsWindow (pParentWindow parentWindow, pObjects objects);
void InitMapFlagsWindow (pParentWindow parentWindow, pObjects objects);
void InitPatternFlagsWindow (pParentWindow parentWindow, pObjects objects);

void ClipCompression (pImage image);
void DisplayCompression (PCHAR_INFO outputBuf, pScreenBuffer buffer, voidStar data, dataType type);

void DisplayBorders (pImage image, pOutput outputObj);

void Rotate (void);
void ThreeD (void);
void WindowTest (void);

void compareLow (double test, double * value);
void PrepareForRedraw (pOutput outputObj, intStar redraw);

int liesOutside (Vector2 test, Vector2 check, double tolerance);
int comp (void const * dataA, void const * dataB);

#endif