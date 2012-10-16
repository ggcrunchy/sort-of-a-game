/********************************************************************
*																	*
*							Main.c									*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Main function of user-defined application			*
*																	*
********************************************************************/

/********************************************************************
*																	*
*							Includes								*
*																	*
********************************************************************/

#include "User.h"

/********************************************************************
*																	*
*							Main									*
*																	*
********************************************************************/

void main (int argc, char ** argv)
{
	/*UserMode*/int mode;

	argc--;	// Decrement the argument count
	argv++;	// Increment beyond the program name
	
	mode = atoi (*argv);

	switch (mode)	// Get the user mode
	{
	case /*kGame*/0:		// Game case
		Game ();	// Call Game function

		break;	// Break out of switch statement

	case /*kEditor*/1:	// Editor case
		Editor ();	// Call Editor function

		break;	// Break out of switch statement

	case 2:
		Rotate ();
		break;

	case 3:
		ThreeD ();
		break;

	case 4:
		WindowTest ();
		break;
	}
}

void Rotate ()
{
	static void (* function [3]) (pPoint3, pVector3, float, float, int) = 
	{xRotate3, yRotate3, zRotate3};

	int X = SCREEN_WIDTH >> 1, Y = SCREEN_HEIGHT >> 1;
	int yMin1 = SCREEN_HEIGHT - 1;
	char degData [20], Char;
	int i, j, len;
	int index = 0;
	float degrees = 15.0;
	float cosine = (float) cos (DEGTORAD(degrees));
	float sine = (float) sin (DEGTORAD(degrees));
	float step = (float) 0.2;
	float max = 360.0;
	Objects objects;
	Point3 points [32] = {
		{0,0,0},
		{0,0,12},
		{0,12,0},
		{0,12,12},
		{12,0,0},
		{12,0,12},
		{12,12,0},
		{12,12,12},
		{3,0,0},{6,0,0},{9,0,0},
		{3,0,12},{6,0,12},{9,0,12},
		{3,12,0},{6,12,0},{9,12,0},
		{3,12,12},{6,12,12},{9,12,12},
		{0,3,0},{0,6,0},{0,9,0},
		{0,3,12},{0,6,12},{0,9,12},
		{12,3,0},{12,6,0},{12,9,0},
		{12,3,12},{12,6,12},{12,9,12}
	};
	pPoint3 point;
	POINT proj [32];
	PPOINT projs;
	PCHAR_INFO Cell;
	int loop = TRUE;

	ZeroMemory (&objects, sizeof (Objects));

	ConsoleInit (&objects, ENABLE_PROCESSED_INPUT, FALSE);

	while (loop)
	{
		switch (rand () & 3)
		{
		case 0:
			break;

		case 1:
			index = 0;
			Char = 'X';

			break;

		case 2:

			index = 1;
			Char = 'Y';

			break;

		case 3:
			index = 2;
			Char = 'Z';

			break;
		}

		degrees += step;

		if (fabs (degrees) > max)
		{
			step = -step;
		}

		cosine = (float) cos (DEGTORAD(degrees));
		sine   = (float) sin (DEGTORAD(degrees));

		ClearScreen (&objects.outputObj);

		switch (GetInput (&objects.inputObj, kAsync))
		{
		case VK_ESCAPE:
			loop = !loop;

			break;
		}

		point = points, function [index] (point, NULL, cosine, sine, 32);
		for (i = 0, projs = proj, point = points; i < 32; i++, projs++, point++)
		{
			projs->x = X + (int) point->x;
			projs->y = Y + (int) point->y;

			Cell = *(objects.outputObj.outputBuf.buffer + projs->y) + projs->x;

			Cell->Char.AsciiChar = '*';
			Cell->Attributes = i & 0xF;
		}

		sprintf (degData, "Degrees: %.2f%c", degrees, '\xF8');

		for (j = 0, len = strlen (degData), Cell = *(objects.outputObj.outputBuf.buffer + yMin1) + 5; j < len; j++, Cell++)
		{
			Cell->Char.AsciiChar = *(degData + j);
			Cell->Attributes = 0xF;
		}

		sprintf (degData, "Axis of rotation: %c", Char);

		for (j = 0, len = strlen (degData), Cell = *(objects.outputObj.outputBuf.buffer + yMin1) + 25; j < len; j++, Cell++)
		{
			Cell->Char.AsciiChar = *(degData + j);
			Cell->Attributes = 0xF;
		}

		UpdateScreen (&objects.outputObj);

		Sleep (35);
	}
}

void compareLow (double test, double * value)
{
	* value = test;
}

#define LESSER(a,b)	(a < b ? a : b)
#define GREATER(a,b) (a > b ? a : b)

#define MINIMUM(a,b,c)	(LESSER (LESSER (a, b), c))
#define MAXIMUM(a,b,c)	(GREATER (GREATER (a, b), c))

void ThreeD (void)
{
	static Vector3 xAxis = {1.0,0.0,0.0}, 
				   yAxis = {0.0,1.0,0.0}, 
				   zAxis = {0.0,0.0,1.0};
	static void (* function [3]) (pPoint3, pVector3, float, float, int) = {xRotate3, yRotate3, zRotate3};
	static int amount = SCREEN_WIDTH * sizeof (CHAR_INFO);
	char buf [80];
	POINT test;
	Objects objects;
	Triangles triArray [3];
	pTriangles tri, index = triArray;
	Vector2 vector, vector2, vector3;
	Vector2 testVector;
	Point2 centroid;
	PCHAR_INFO cell = *objects.outputObj.outputBuf.buffer, drawCell;
	int i, j, len, x, y, funcIndex, ind = 0;
	int width, height;
	int pitch;
	static int xOffset = SCREEN_WIDTH >> 1, yOffset = SCREEN_HEIGHT >> 1;
	int loop = TRUE, redraw = TRUE;
	double tolerance = 0.001;

	ZeroMemory (&objects, sizeof (Objects));

	ConsoleInit (&objects, ENABLE_PROCESSED_INPUT, FALSE);

	ZeroMemory (triArray, 3 * sizeof (Triangles));

	for (j = 0, tri = triArray; j < 3; j++, tri++)
	{
		tri->axis = xAxis;
		tri->axisChar = 'X';
		tri->degrees = 20.0;

		tri->cosine = (float) cos (DEGTORAD(tri->degrees));
		tri->sine   = (float) sin (DEGTORAD(tri->degrees));
	}

	triArray->b.x = 15.0;

	triArray->c.x = 3.0;
	triArray->c.y = 6.0;

	triArray->Char = '0';
	triArray->color = 0x3;

	(triArray + 1)->a.x = (triArray + 1)->a.z = -6.0;
	(triArray + 1)->a.y = 1.0;
	
	(triArray + 1)->b.x = (triArray + 1)->b.y = 3.0;
	(triArray + 1)->b.z = -6.0;
	
	(triArray + 1)->c.y = 11.0;
	(triArray + 1)->c.z = -6.0;

	(triArray + 1)->Char = '8';
	(triArray + 1)->color = 0xF;

	(triArray + 2)->a.x = -8.0;
	(triArray + 2)->a.y = 13.0;
	(triArray + 2)->a.z = -8.0;

	(triArray + 2)->b.x = 16.0;
	(triArray + 2)->b.y = 5.5;
	(triArray + 2)->b.z = -4.0;

	(triArray + 2)->c.x = (triArray + 2)->c.y = (triArray + 2)->c.z = -3.5;

	(triArray + 2)->Char = (char) 2;
	(triArray + 2)->color = 0xE;

	while (loop)
	{
		switch (GetInput (&objects.inputObj, kSync))
		{
		case VK_ESCAPE:
			loop = !loop;

			break;

		case '0':
			index = triArray;
			ind = 0;

			break;

		case '1':
			index = triArray + 1;
			ind = 1;

			break;

		case '2':
			index = triArray + 2;
			ind = 2;

			break;

		case VK_UP:
			index->degrees++;

			if (index->degrees > 360.0)
			{
				index->degrees = 0.0;
			}

			index->cosine = (float) cos (DEGTORAD(index->degrees));
			index->sine   = (float) sin (DEGTORAD(index->degrees));

			ZeroMemory (cell + ind, amount);
			redraw = TRUE;

			break;

		case VK_DOWN:
			index->degrees--;

			if (index->degrees < -360.0)
			{
				index->degrees = 0.0;
			}

			index->cosine = (float) cos (DEGTORAD(index->degrees));
			index->sine   = (float) sin (DEGTORAD(index->degrees));

			ZeroMemory (cell + ind, amount);
			redraw = TRUE;

			break;

		case VK_LEFT:
		case VK_RIGHT:
			index->degrees = -index->degrees;

			index->cosine = (float) cos (DEGTORAD(index->degrees));
			index->sine   = (float) sin (DEGTORAD(index->degrees));

			ZeroMemory (cell + ind, amount);
			redraw = TRUE;

			break;

		case 'X':
			index->axis = xAxis;
			index->axisChar = 'X';
			index->index = 0;

			ZeroMemory (cell + ind, amount);
			redraw = TRUE;
	
			break;

		case 'Y':
			index->axis = yAxis;
			index->axisChar = 'Y';
			index->index = 1;

			ZeroMemory (cell + ind, amount);
			redraw = TRUE;

			break;

		case 'Z':
			index->axis = zAxis;
			index->axisChar = 'Z';
			index->index = 2;

			ZeroMemory (cell + ind, amount);
			redraw = TRUE;

			break;

		case VK_SPACE:
			for (i = 0, tri = triArray; i < 3; i++, tri++)
			{
				funcIndex = tri->index;

				function [funcIndex] (&tri->a, NULL, tri->cosine, tri->sine, 1);

				tri->A.x = (int) tri->a.x;
				tri->A.y = (int) tri->a.y;

				function [funcIndex] (&tri->b, NULL, tri->cosine, tri->sine, 1);

				tri->B.x = (int) tri->b.x;
				tri->B.y = (int) tri->b.y;

				function [funcIndex] (&tri->c, NULL, tri->cosine, tri->sine, 1);

				tri->C.x = (int) tri->c.x;
				tri->C.y = (int) tri->c.y;

				tri->outputRect.left = (int) MINIMUM(tri->A.x,tri->B.x,tri->C.x);
				tri->outputRect.right = (int) MAXIMUM(tri->A.x,tri->B.x,tri->C.x);
				tri->outputRect.top = (int) MINIMUM(tri->A.y,tri->B.y,tri->C.y);
				tri->outputRect.bottom = (int) MAXIMUM(tri->A.y,tri->B.y,tri->C.y);

				compareLow (MINIMUM (tri->a.z, tri->b.z, tri->c.z), &tri->low);
			}

			qsort ((voidStar) triArray, (size_t) 3, sizeof (Triangles), comp);

			PrepareForRedraw (&objects.outputObj, &redraw);

			for (i = 0, tri = triArray; i < 3; i++, tri++)
			{
				width = tri->outputRect.right - tri->outputRect.left;
				height = tri->outputRect.bottom - tri->outputRect.top;

				vector.tail.x = (float) tri->A.x;
				vector.tail.y = (float) tri->A.y;

				vector.x = (float) (tri->B.x - tri->A.x);
				vector.y = (float) (tri->B.y - tri->A.y);

				vector2.tail.x = (float) tri->B.x;
				vector2.tail.y = (float) tri->B.y;

				vector2.x = (float) (tri->C.x - tri->B.x);
				vector2.y = (float) (tri->C.y - tri->B.y);

				vector3.tail.x = (float) tri->C.x;
				vector3.tail.y = (float) tri->C.y;

				vector3.x = (float) (tri->A.x - tri->C.x);
				vector3.y = (float) (tri->A.y - tri->C.y);

				centroid.x = (float) ((tri->A.x + tri->B.x + tri->C.x) / 3.0);//(0.3333 * tri->A.x + 0.3333 * tri->B.x + 0.3333 * tri->C.x);
				centroid.y = (float) ((tri->A.y + tri->B.y + tri->C.y) / 3.0) ;//(0.3333 * tri->A.y + 0.3333 * tri->B.y + 0.3333 * tri->C.y);

				testVector.tail = centroid;

				pitch = SCREEN_WIDTH - width;
			
				for (y = 0, drawCell = *(objects.outputObj.outputBuf.buffer + yOffset) + xOffset; y < height; y++)
				{
					for (x = 0; x < width; x++, drawCell++)
					{
						test.x = tri->outputRect.left + x;
						test.y = tri->outputRect.top + y;

						testVector.x = test.x - centroid.x;
						testVector.y = test.y - centroid.y;

						if (!liesOutside (testVector, vector, tolerance) && !liesOutside (testVector, vector2, tolerance) && !liesOutside (testVector, vector3, tolerance))
						{
							drawCell->Char.AsciiChar = tri->Char;
							drawCell->Attributes = tri->color;
						}
					}

					drawCell += pitch;
				}
			}

			break;
		}

		if (redraw)
		{
			redraw = !redraw;

			for (i = 0, tri = triArray; i < 3; i++, tri++)
			{
				sprintf (buf, "Degrees of rotation: %.2f, Axis of rotation: %c", tri->degrees, tri->axisChar);
				len = strlen (buf);

				pitch = (SCREEN_WIDTH + 2) - len;

				for (j = 0, drawCell = *(objects.outputObj.outputBuf.buffer + i) + 2; j < len; j++, drawCell++)
				{
					drawCell->Char.AsciiChar = *(buf + j);
					drawCell->Attributes = tri->color;
				}

				*buf = END;
			}

			UpdateScreen (&objects.outputObj);
		}
	}
}

void PrepareForRedraw (Output * outputObj, int * redraw)
{
	* redraw = TRUE;

	ClearScreen (outputObj);
}

int comp (const void * dataA, const void * dataB)
{
	pTriangles triA = (pTriangles) dataA, triB = (pTriangles) dataB;

	if (triA->low < triB->low)
	{
		return -1;
	}

	if (triA->low > triB->low)
	{
		return 1;
	}
	
	return 0;
}

int liesOutside (Vector2 test, Vector2 check, double tolerance)
{
	double s, t, det;
	double a, b, c, d;
	Vector2 store, result;

	if (Dot2 (test, check))
	{
		det = (test.x * check.y) - (test.y * check.x);

		a = check.y / det;
		b = -check.x / det;
		c = -test.y / det;
		d = test.x / det;

		store.x = check.tail.x - test.tail.x;
		store.y = check.tail.y - test.tail.y;

		result.x = (float) (a * store.x + b * store.y);
		result.y = (float) (c * store.x + d * store.y);

		t = result.x;
		s = -result.y;

		if (s >= 0/*-tolerance*/ && t >= 0/*-tolerance*/ && s <= 1.0/* + tolerance*/ && t <= 1.0/* + tolerance*/)
		{
			return 1;
		}
	}
	 
	return 0;
}

void WindowTest (void)
{
	/*
	pAnimation animation;
	Map map;
	Objects objects;

	ZeroMemory (&map, sizeof (Map));

	MALLOC(animation,Animation);

	memset (&objects, 0, sizeof (Objects));

	ConsoleInit (&objects, ENABLE_PROCESSED_INPUT, FALSE);

	ReloadMap (&objects.fileObj, &map, "Trans.map");

	animation->numStates = 1;

	CALLOC(animation->numFrames,animation->numStates,int);
	CALLOC(animation->curFrame,animation->numStates,float);

	animation->numFrames [0] = 1;

	CALLOC(animation->frames,animation->numStates,pImage);

	CALLOC(animation->frames [0],animation->numFrames [0],Image);

	ReloadImage (&objects.fileObj, &animation->frames [0] [0], "Trans.img");

	animation->stepSize = (float) 0.01;
	animation->frameRate = (float) 0.33;

	animation->path.numControls = 1;

	CALLOC(animation->path.control,animation->path.numControls,Point2);

	animation->path.start.x = 100;
	animation->path.start.y = 15;

	animation->path.end.x = -25;
	animation->path.end.y = 20;

	animation->path.control [0].x = 85;
	animation->path.control [0].y = 105;

	animation->path.vector = DiffPoints2 (animation->path.start, animation->path.end);

	animation->type = kLinear;

	while (GetInput (&objects.inputObj, kAsync) != VK_ESCAPE)
	{
		CopyMapToBuffer (&objects.outputObj.outputBuf, &map);

		DisplayAnimationToScreen (animation, &objects.outputObj.outputBuf, &map);

		UpdateScreen (&objects.outputObj);

		Sleep (650);
	}

	DeinitializeObjects (&objects);

	DeleteMap (&map);
	DeleteAnimation (animation);
	FREE(animation);
	*/
	String buf = "There is only one way to escape Jochochyo";
	pParentWindow parentWindow;
	pWindow window;
	Objects objects;
	pCell cell, end;
	Image image;
	int i, loop = TRUE;
	int dimensions;

	ZeroMemory (&objects, sizeof (Objects));
	ZeroMemory (&image, sizeof (Image));

	ConsoleInit (&objects, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT, TRUE);

	ReloadImage (&objects.fileObj, &image, "Dragon5.img");

	MALLOC(parentWindow,ParentWindow);

	MALLOC(parentWindow->back,ScreenBuffer);

	DisplayImageToScreen (&image, &objects.outputObj.outputBuf, NULL);

	CopyBuffer (parentWindow->back, &objects.outputObj.outputBuf);

	DeleteImage (&image);

	parentWindow->delay = 50;

	parentWindow->location.X = 5;
	parentWindow->location.Y = 10;

	parentWindow->width = 55;
	parentWindow->height = 25;

	CALLOC(parentWindow->backData,parentWindow->width * parentWindow->height,BYTE);

	parentWindow->background = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	parentWindow->border = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

	MALLOC(parentWindow->separators,Separator);

	parentWindow->separators->numHorzSeparators = parentWindow->separators->numVertSeparators = 2;

	CALLOC(parentWindow->separators->horzSeparators,parentWindow->separators->numHorzSeparators,SeparatorEntry);
	CALLOC(parentWindow->separators->vertSeparators,parentWindow->separators->numVertSeparators,SeparatorEntry);

	parentWindow->separators->horzSeparators->extent = 55;
	(parentWindow->separators->horzSeparators + 1)->extent = 55;

	parentWindow->separators->horzSeparators->offsets.Y = 4;
	(parentWindow->separators->horzSeparators + 1)->offsets.Y = 19;

	parentWindow->separators->vertSeparators->extent = 25;
	(parentWindow->separators->vertSeparators + 1)->extent = 25;

	parentWindow->separators->vertSeparators->offsets.X = 9;
	(parentWindow->separators->vertSeparators + 1)->offsets.X = 22;

	parentWindow->focusKey = VK_TAB;
	parentWindow->closeKey = VK_ESCAPE;

	parentWindow->numWindows = 5;

	CALLOC(parentWindow->windows,parentWindow->numWindows,Window);

	window = parentWindow->windows;

	window->background = BACKGROUND_RED | BACKGROUND_INTENSITY;
	window->width = window->windowWidth = 30;
	window->height = window->windowHeight = 14;
	window->windowCoord.X = 24;
	window->windowCoord.Y = 6;
	window->mode = kEditBox;

	MALLOC(window->IO,IO_Descriptor);

	window->IO->text.limit = window->IO->text.numChars = WORD_LENGTH;
	window->IO->readMode = kPrintOnly;

	CALLOC(window->IO->text.text,window->IO->text.numChars,char);

	dimensions = window->width * window->height;
	AllocateBuffer (&window->display, dimensions);

	for (cell = window->display.buffer, end = cell + dimensions; cell < end; cell++)
	{
		cell->graph.Attributes = window->background;
	}

	SETFLAG(window->state,CANRECEIVEFOCUS);

	window++;

	window->background = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	window->width = window->windowWidth = 12;
	window->height = window->windowHeight = 14;

	window->windowCoord.X = 11;
	window->windowCoord.Y = 6;
	window->mode = kBasic;

	dimensions = window->width * window->height;
	AllocateBuffer (&window->display, dimensions);

	MALLOC(window->visuals,Visuals);

	window->visuals->numAnimations = 1;

	CALLOC(window->visuals->animations,window->visuals->numAnimations,Animation);

	window->visuals->animations->numStates = 1;

	CALLOC(window->visuals->animations->states,window->visuals->animations->numStates,AnimationState);

	window->visuals->animations->states->numFrames = 2;

	CALLOC(window->visuals->animations->states->frames,window->visuals->animations->states->numFrames,Image);

	ReloadImage (&objects.fileObj, window->visuals->animations->states->frames, "Trans.img");
	ReloadImage (&objects.fileObj, window->visuals->animations->states->frames + 1, "Dragon5.img");

	window->visuals->animations->stepSize = (float) 0.005;
	window->visuals->animations->frameRate = (float) 0.05;

	window->visuals->animations->path.start.x = 17;
	window->visuals->animations->path.start.y = 17  ;

	window->visuals->animations->path.end.x = -5;
	window->visuals->animations->path.end.y = -5;

	window->visuals->animations->path.vector = DiffPoints2 (window->visuals->animations->path.start, window->visuals->animations->path.end);

	window->visuals->animations->type = kLinear;

	for (cell = window->display.buffer, end = cell + dimensions; cell < end; cell++)
	{
		cell->graph.Attributes = window->background;
	}

	SETFLAG(window->state,ACTIVEWITHOUTFOCUS);

	window++;

	window->background = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	window->width = window->windowWidth = 30;
	window->height = window->windowHeight = 4;
	window->windowCoord.X = 24;
	window->windowCoord.Y = 1;
	window->mode = kEditBox;

	MALLOC(window->IO,IO_Descriptor);

	window->IO->text.limit = window->IO->text.numChars = WORD_LENGTH;

	CALLOC(window->IO->text.text,window->IO->text.numChars,char);

	dimensions = window->width * window->height;
	AllocateBuffer (&window->display, dimensions);

	for (cell = window->display.buffer, end = cell + dimensions; cell < end; cell++)
	{
		cell->graph.Attributes = window->background;
	}

	SETFLAG(window->state,CANRECEIVEFOCUS);

	window++;

	window->background = BACKGROUND_BLUE | BACKGROUND_GREEN;
	window->width = window->windowWidth = 30;
	window->height = window->windowHeight = 3;
	window->windowCoord.X = 24;
	window->windowCoord.Y = 21;
	window->mode = kMessageBox;

	MALLOC(window->IO,IO_Descriptor);

	window->IO->writeMode = kWriteCharacter;

	window->IO->text.limit = window->IO->text.numChars = strlen (buf) + 1;

	CALLOC(window->IO->text.text,window->IO->text.numChars,char);

	for (i = 0; i < window->IO->text.numChars; i++)
	{
		*(window->IO->text.text + i) = *(buf + i);
	}

	dimensions = window->width * window->height;
	AllocateBuffer (&window->display, dimensions);

	for (cell = window->display.buffer, end = cell + dimensions; cell < end; cell++)
	{
		cell->graph.Attributes = window->background;
	}

	SETFLAG(window->state,ACTIVEWITHOUTFOCUS);

	window++;

	window->background = BACKGROUND_RED | BACKGROUND_INTENSITY;
	window->width = window->windowWidth = 9;
	window->height = window->windowHeight = 14;
	window->windowCoord.X = 1;
	window->windowCoord.Y = 6;
	window->mode = kMenu;

	MALLOC(window->receivers,Receivers);

	window->receivers->numReceivers = 1;

	CALLOC(window->receivers->receivers,window->receivers->numReceivers,ReceiverEntry);

	window->receivers->receivers->windowID = 1;
	window->receivers->receivers->instruction = kIndexFrame;

	MALLOC(window->menu,Menu);

	window->menu->numKeys = 4;

	CALLOC(window->menu->keys,window->menu->numKeys,Key);

	window->menu->keys->keycode = VK_UP;
	SetInputMethod (window->menu->keys, MoveToPreviousItem);
	(window->menu->keys + 1)->keycode = VK_DOWN;
	SetInputMethod (window->menu->keys + 1, MoveToNextItem);
	(window->menu->keys + 2)->keycode = VK_LEFT;
	SetInputMethod (window->menu->keys + 2, ToggleMenuItemLeft);
	(window->menu->keys + 3)->keycode = VK_RIGHT;
	SetInputMethod (window->menu->keys + 3, ToggleMenuItemRight);

	window->menu->numItems = 2;

	CALLOC(window->menu->menu,window->menu->numItems,MenuItem);

	window->menu->menu->numItems = 2;
	CALLOC(window->menu->menu->text,window->menu->menu->numItems,String);
	CALLOC(window->menu->menu->lParams,window->menu->menu->numItems,long);
	CALLOC(*window->menu->menu->text,strlen ("M1") + 1,char);
	strcpy (*window->menu->menu->text, "M1");
	CALLOC(*(window->menu->menu->text + 1),strlen ("M1+") + 1,char);
	strcpy (*(window->menu->menu->text + 1), "M1+");
	window->menu->menu->location.X = 3;
	window->menu->menu->location.Y = 3;
	window->menu->menu->width = 5;
	window->menu->menu->height = 1;
	window->menu->menu->highlight = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

	(window->menu->menu + 1)->numItems = 1;
	CALLOC((window->menu->menu + 1)->text,(window->menu->menu + 1)->numItems,String);
	CALLOC((window->menu->menu + 1)->lParams,(window->menu->menu + 1)->numItems,long);
	CALLOC(*(window->menu->menu + 1)->text,strlen ("M2") + 1,char);
	strcpy (*(window->menu->menu + 1)->text, "M2");
	(window->menu->menu + 1)->location.X = 4;
	(window->menu->menu + 1)->location.Y = 7;
	(window->menu->menu + 1)->width = 3;
	(window->menu->menu + 1)->height = 2;
	(window->menu->menu + 1)->highlight = BACKGROUND_GREEN | BACKGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

	dimensions = window->width * window->height;
	AllocateBuffer (&window->display, dimensions);

	for (cell = window->display.buffer, end = cell + dimensions; cell < end; cell++)
	{
		cell->graph.Attributes = window->background;
	}

	SETFLAG(window->state,CANRECEIVEFOCUS);

	MakeParentWindow (parentWindow, 0, 0, 2);

	while (loop)
	{
		ActivateWindows (parentWindow);

		RedrawWindows (parentWindow, &objects);

		while (WindowsActive (parentWindow))
		{
			Update (parentWindow, &objects, NULL);
		}

		CopyBuffer (&objects.outputObj.outputBuf, parentWindow->back);
		UpdateScreen (&objects.outputObj);
stuck:
		switch (GetInput (&objects.inputObj, kSync))
		{
		case VK_SPACE:
			break;

		case VK_ESCAPE:
			loop = FALSE;

			break;

		default:
			goto stuck;
		}
	}

	DeleteParentWindow (parentWindow);
	FREE(parentWindow);
	DeinitializeObjects (&objects);
	
} 