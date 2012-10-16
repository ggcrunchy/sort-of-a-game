/********************************************************************
*																	*
*							Output.c								*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains implementation of output tool				*
*																	*
********************************************************************/

#include "Output.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							External includes						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "Mathematics.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Globals									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*
static PCHAR_INFO G_endCell;
// Used to quicken CHAR_INFO loops

static PBYTE G_flags, G_flags2, G_flags2;
static PBYTE G_data, G_data2, G_data3;
static PCHAR_INFO G_cell, G_cell2, G_cell3;
// Used for output*/

static PCHAR_INFO G_dest;

static pCell G_end;
static pCell G_bufCell, G_imgCell, G_mapCell, G_patCell, G_winCell;

static BYTE G_surface [SCREEN_WIDTH] [SCREEN_HEIGHT];
static Clipper G_clipper;
static Wrapper G_wrapper;
// Used to format visual output

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Output access functions					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	GetClipper - Export the global clipper object					*
********************************************************************/

Clipper GetClipper (void)
{
	return G_clipper;
}

/********************************************************************
*	IncXOffset - Increment the x offset by xScroll					*
********************************************************************/

static void IncXOffset (Map * map)
{
	map->xOffset += map->xScroll;	// Increment map's xOffset field by map's xScroll field value
}

/********************************************************************
*	DecXOffset - Decrement the x offset by xScroll					*
********************************************************************/

static void DecXOffset (Map * map)
{
	map->xOffset -= map->xScroll;	// Decrement map's xOffset field by map's xScroll field value
}

/********************************************************************
*	IncYOffset - Increment the y offset by yScroll					*
********************************************************************/

static void IncYOffset (Map * map)
{
	map->yOffset += map->yScroll;	// Increment map's yOffset field by map's yScroll field value
}

/********************************************************************
*	DecYOffset - Decrement the y offset by yScroll					*
********************************************************************/

static void DecYOffset (Map * map)
{
	map->yOffset -= map->yScroll;	// Decrement map's yOffset field by map's yScroll field value
}

/********************************************************************
*	SetCurrentState - Sets the current state of an animation		*
********************************************************************/

void SetCurrentState (Animation * animation, int curState)
{
	assert (animation);
	// Verify that animation points to valid memory

	animation->curState = (curState < animation->numStates && curState > 0) ? curState : 0;
	// Set the current animation state
}

/********************************************************************
*	SetCurrentFrame - Sets the current frame of animation state		*
********************************************************************/

void SetCurrentFrame (Animation * animation, float curFrame)
{
	pAnimationState animState;

	assert (animation);
	// Verify that animation points to valid memory

	animState = animation->states + animation->curState;

	animState->curFrame = (curFrame < animState->numFrames && curFrame > 0.0) ? curFrame : (float) 0.0;
	// Set the current frame of animation in the current animation state

	animation->activeImage = animState->frames + (int) animState->curFrame;
	// Set the animation's active image
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Construction							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	InitializeOutputObject - Initialize primary output object		*
********************************************************************/

BOOL InitializeOutputObject (Output * outputObj)
{
	ZeroMemory (outputObj, sizeof (Output));
	// Zero memory out

	outputObj->outputBuf.outputRect.Left	= 0;				// Set left edge of outputRect to left edge of screen
	outputObj->outputBuf.outputRect.Top		= 0;				// Set top edge of outputRect to top edge of screen
	outputObj->outputBuf.outputRect.Right	= SCREEN_WIDTH - 1;	// Set right edge of outputRect to right edge of screen
	outputObj->outputBuf.outputRect.Bottom	= SCREEN_HEIGHT - 1;// Set bottom edge of outputRect to bottom edge of screen

	outputObj->outputH = GetStdHandle (STD_OUTPUT_HANDLE);		// Retrieve an output handle

	if (outputObj->outputH == INVALID_HANDLE_VALUE)
	{
		ERROR_MESSAGE("InitializeOutputObject failed","1");
		// Return failure
	}
	// Verify that outputObj's outputH field contains a valid handle value

	return TRUE;
	// Return success
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Display									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*																	*
*							Text output								*
*																	*
********************************************************************/
							
/********************************************************************
*	SkipSpaces - Increment past any space characters				*
********************************************************************/

int SkipSpaces (String text, int index)
{
	int numSpaces = 0;
	String string = text + index;

	while (isspace (*string++))
	{
		numSpaces++;
	}

	return numSpaces;
}

/********************************************************************
*	ClearText - Clear text written to output buffer					*
********************************************************************/

void ClearText (OutputBuffer * outputBuf, int index, int extent)
{
/*	G_cell = outputBuf->buffer + index;

	for (G_endCell = G_cell + extent; G_cell < G_endCell; G_cell++)
	{
		G_cell->Char.AsciiChar = SPACE;
	}*/
	G_bufCell = outputBuf->buffer + index;

	for (G_end = G_bufCell + extent; G_bufCell < G_end; G_bufCell++)
	{
		G_bufCell->graph.Char.AsciiChar = SPACE;
	}
}

/********************************************************************
*	WriteText - Write contents of text buffer to output buffer		*
********************************************************************/

void WriteText (OutputBuffer * outputBuf, String text, int index)
{
/*	G_cell = outputBuf->buffer + index;

	while (*text)
	{
		(G_cell++)->Char.AsciiChar = *text++;
	}*/
	G_bufCell = outputBuf->buffer + index;

	while (*text)
	{
		G_bufCell->graph.Char.AsciiChar = *text++;
	}
}

/********************************************************************
*																	*
*							Screen Buffer Management				*
*																	*
********************************************************************/

/********************************************************************
*	ClearScreen - Clear the contents of the primary screen buffer	*
********************************************************************/

void ClearScreen (Output * outputObj)
{
	static size = SCREEN_WIDTH * SCREEN_HEIGHT * sizeof (CHAR_INFO);	// Constant value used to clear screen

	assert (outputObj);
	// Verify that outputObj points to valid memory

	ZeroMemory (&outputObj->outputBuf.buffer, size);	
	// Zero memory out
}

/********************************************************************
*	CopyBuffer - Copy contents of one screen buffer into another	*
********************************************************************/

void CopyBuffer (ScreenBuffer * dest, ScreenBuffer const * source)
{
	static size = sizeof (ScreenBuffer);	// Constant value used to copy screen buffers

	assert (dest && source);
	// Verify that dest and source point to valid memory

	memcpy (dest, source, size);
	// Copy data from source to dest
}

/********************************************************************
*	UpdateScreen - Update the contents of the screen buffer			*
********************************************************************/

void UpdateScreen (Output * outputObj)
{
	static COORD dimensions = {SCREEN_WIDTH, SCREEN_HEIGHT};// Constant value used as dimensions to update screen
	static COORD bufCoord = {0, 0};							// Constant value used as offset to update screen

	assert (outputObj);
	// Verify that outputObj points to valid memory

	if (!WriteConsoleOutput (outputObj->outputH, *outputObj->outputBuf.buffer, dimensions, bufCoord, &outputObj->outputBuf.outputRect))
	{
		NORET_MESSAGE("UpdateScreen failed","1");
		// Return failure
	}
	// Update the screen buffer
}

/********************************************************************
*																	*
*							Map Display Routines					*
*																	*
********************************************************************/

/********************************************************************
*	CopyMapToBuffer - Copy contents from offset of map to a buffer	*
********************************************************************/

void CopyMapToBuffer (ScreenBuffer * buffer, Map const * map)
{
	int i;			// Loop variable
	int pitch;		// Pitch used to increment index

	assert (buffer && map);
	// Verify that buffer and map point to valid memory

	// Initialization block
	{
		int index = map->yOffset * map->width + map->xOffset;

		pitch = map->width - SCREEN_WIDTH;
	
/*		G_flags = map->world.flags + index;

		G_cell = *buffer->buffer;
		G_cell2 = map->world.buffer + index;*/
		G_dest = *buffer->buffer;

		G_mapCell = map->world.buffer + index;
	}

	for (i = 0; i < SCREEN_HEIGHT; i++)
	{
/*		for (G_endCell = G_cell + SCREEN_WIDTH; G_cell < G_endCell; G_flags++, G_cell++, G_cell2++)
		{
			if (!FLAGSET(*G_flags,NONVISIBLE))
			{
				*G_cell = *G_cell2;
				// Memberwise copy from map to screen buffer
			}
			// Check whether map is visible at index
		}

		G_flags += pitch;
		G_cell2 += pitch;*/
		for (G_end = G_mapCell + SCREEN_WIDTH; G_mapCell < G_end; G_dest++, G_mapCell++)
		{
			if (!FLAGSET(G_mapCell->flags,NONVISIBLE))
			{
				*G_dest = G_mapCell->graph;
			}
		}

		G_mapCell += pitch;
	}
}

/********************************************************************
*	CopyMapFlagsToBuffer - Copy the flags from map offset to buffer	*
********************************************************************/

void CopyMapFlagsToBuffer (ScreenBuffer * buffer, Map const * map)
{
	int i;			// Loop variable
	int pitch;		// Pitch used to increment index
	
	assert (buffer && map);
	// Verify that buffer and map point to valid memory

	// Initialization block
	{
		int index = map->yOffset * map->width + map->xOffset;

		pitch = map->width - SCREEN_WIDTH;

/*		G_flags = map->world.flags + index;

		G_cell = *buffer->buffer;*/
		G_dest = *buffer->buffer;

		G_mapCell = map->world.buffer + index;
	}

	for (i = 0; i < SCREEN_HEIGHT; i++)
	{
/*		for (G_endCell = G_cell + SCREEN_WIDTH; G_cell < G_endCell; G_flags++, G_cell++)
		{
			G_cell->Char.AsciiChar = DATA_AND_FLAGS;// Set given screen buffer cell to "data and flags" character
			G_cell->Attributes = *G_flags;			// Set given screen buffer cell to attribute with value equivalent to given flag
		}

		G_flags += pitch;*/
		for (G_end = G_mapCell + SCREEN_WIDTH; G_mapCell < G_end; G_dest++, G_mapCell++)
		{
			G_dest->Char.AsciiChar = DATA_AND_FLAGS;
			G_dest->Attributes = G_mapCell->flags;
		}

		G_mapCell += pitch;
	}
}

/********************************************************************
*	CopyMapDataToBuffer - Copy the data from map offset to buffer	*
********************************************************************/

void CopyMapDataToBuffer (ScreenBuffer * buffer, Map const * map)
{
	int i;			// Loop variable
	int pitch;		// Pitch used to increment index
	
	assert (buffer && map);
	// Verify that buffer and map point to valid memory

	// Initialization block
	{
		int index = map->yOffset * map->width + map->xOffset;
		// Set offsets to proper index

		pitch = map->width - SCREEN_WIDTH;

/*		G_data = map->world.data + index;

		G_cell = *buffer->buffer;*/
		G_dest = *buffer->buffer;

		G_mapCell = map->world.buffer + index;
	}

	for (i = 0; i < SCREEN_HEIGHT; i++)
	{
/*		for (G_endCell = G_cell + SCREEN_WIDTH; G_cell < G_endCell; G_data++, G_cell++)
		{
			G_cell->Char.AsciiChar = DATA_AND_FLAGS;// Set given screen buffer cell to "data and flags" character
			G_cell->Attributes = *G_data;			// Set given screen buffer cell to attribute with value equivalent to given datum
		}

		G_data += pitch;*/
		for (G_end = G_mapCell + SCREEN_WIDTH; G_mapCell < G_end; G_dest++, G_mapCell++)
		{
			G_dest->Char.AsciiChar = DATA_AND_FLAGS;
			G_dest->Attributes = G_mapCell->data;
		}

		G_mapCell += pitch;
	}
}

/********************************************************************
*	ScrollMap - Scroll the contents of the given map				*
********************************************************************/

void ScrollMap (Map * map, HorzScroll horzMode, VertScroll vertMode)
{
	assert (map);
	// Verify that map points to valid memory

	switch (horzMode)	// Get the horizontal scroll mode
	{
	case kLeft:			// Left case
		if (map->xOffset - map->xScroll >= 0)
		{
			DecXOffset (map);
			// Scroll map to left by horizontal scroll value
		}

		else if (map->xOffset > 0)
		{
			map->xOffset--;
			// Scroll map to left by one
		}

		break;	// Break out of switch statement

	case kRight:		// Right case
		if (map->xOffset + map->xScroll + SCREEN_WIDTH <= map->width)
		{
			IncXOffset (map);
			// Scroll map to right by horizontal scroll value
		}

		else if (map->xOffset + SCREEN_WIDTH < map->width)
		{
			map->xOffset++;
			// Scroll map to right by one
		}

		break;	// Break out of switch statement

	case kHorzFix:		// Fixed case
		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported horzMode: ScrollScreen failed","1");
		// Return failure
	}

	switch (vertMode)	// Get the vertical scroll mode
	{
	case kUp:			// Up case
		if (map->yOffset - map->yScroll >= 0)
		{
			DecYOffset (map);
			// Scroll map up by vertical scroll value 
		}

		else if (map->yOffset > 0)
		{
			map->yOffset--;
			// Scroll map up by one
		}

		break;	// Break out of switch statement

	case kDown:			// Down case
		if (map->yOffset + map->yScroll + SCREEN_HEIGHT <= map->height)
		{
			IncYOffset (map);
			// Scroll map down by vertical scroll value
		}

		else if (map->yOffset + SCREEN_HEIGHT < map->height)
		{
			map->yOffset++;
			// Scroll map down by one
		}

		break;	// Break out of switch statement

	case kVertFix:		// Fixed case
		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported vertMode: ScrollScreen failed","2");
		// Return failure
	}
}

/********************************************************************
*																	*
*							Image Display Routines					*
*																	*
********************************************************************/

/********************************************************************
*	ClipImageToScreen - Clip an image to be displayed to the screen	*
********************************************************************/

static BOOL ClipImageToScreen (Image * image)
{
	int originX, originY;
	int extentX, extentY;

	originX = image->location.X;
	originY = image->location.Y;

	extentX = originX + image->width;
	extentY = originY + image->height;

	G_clipper.visible = (extentX > 0 && originX < SCREEN_WIDTH)
					 && (extentY > 0 && originY < SCREEN_HEIGHT);

	if (G_clipper.visible)
	{
		G_clipper.clipRegion.left   = originX > 0 ? originX : 0;
		G_clipper.clipRegion.top	= originY > 0 ? originY : 0;
		G_clipper.clipRegion.right  = extentX < SCREEN_WIDTH ? extentX : SCREEN_WIDTH;
		G_clipper.clipRegion.bottom = extentY < SCREEN_HEIGHT ? extentY : SCREEN_HEIGHT;

		return TRUE;
	}

	return FALSE;
}

/********************************************************************
*	ClipImageToWindow - Clip an image to be displayed to a window	*
********************************************************************/

static BOOL ClipImageToWindow (Image * image, Window const * window)
{
	int originX, originY;
	int extentX, extentY;
	int winExtentX, winExtentY;

	originX = image->location.X;
	originY = image->location.Y;

	extentX = originX + image->width;
	extentY = originY + image->height;

	winExtentX = window->xOffset + window->windowWidth;
	winExtentY = window->yOffset + window->windowHeight;

	G_clipper.visible = (extentX > window->xOffset && originX < winExtentX)
			  	     && (extentY > window->yOffset && originY < winExtentY);

	if (G_clipper.visible)
	{
		G_clipper.clipRegion.left   = originX > window->xOffset ? originX : window->xOffset;
		G_clipper.clipRegion.top	= originY > window->yOffset ? originY : window->yOffset;
		G_clipper.clipRegion.right  = extentX < winExtentX ? extentX : winExtentX;
		G_clipper.clipRegion.bottom = extentY < winExtentY ? extentY : winExtentY;

		return TRUE;
	}

	return FALSE;
}

/********************************************************************
*	DisplayImageToScreen - Primary wrapper for image display		*
********************************************************************/

void DisplayImageToScreen (Image * image, ScreenBuffer * buffer, Map const * map)
{
	assert (image && buffer);
	// Verify that image and buffer point to valid memory

	if (!ClipImageToScreen (image))
	{
		return;
	}

	if (map)	// Ensure that map points to something
	{
		DisplayImageToMap (image, buffer, map);
	}

	else
	{
		DisplayImage (image, buffer);
	}
}

/********************************************************************
*	DisplayImageToWindow - Display an image to a window				*
********************************************************************/

void DisplayImageToWindow (Image * image, Window * window)
{
	int i;				// Loop variable
	int width, height;
	int pitch, winPitch;

	assert (image && window);
	// Verify that image and window point to valid memory

	if (!ClipImageToWindow (image, window))
	{
		return;
	}

	// Initialization block
	{
		int index;
		int originX, originY;
		int xOffset, yOffset;
		int winXOffset, winYOffset;

		originX = G_clipper.clipRegion.left;
		originY = G_clipper.clipRegion.top;

		xOffset = originX - image->location.X;
		yOffset = originY - image->location.Y;

		winXOffset = originX - window->xOffset;
		winYOffset = originY - window->yOffset;

		width = G_clipper.clipRegion.right - originX;
		height = G_clipper.clipRegion.bottom - originY;

		index = yOffset * image->width + xOffset;

/*		G_flags = image->image.flags + index;
		G_data = image->image.data + index;

		G_cell = window->display.buffer + ((window->yOffset + winYOffset) * window->width + (window->xOffset + winXOffset));
		G_cell2 = image->image.buffer + index;*/
		G_winCell = window->display.buffer + ((window->yOffset + winYOffset) * window->width + (window->xOffset + winXOffset));

		G_imgCell = image->image.buffer + index;

		pitch = image->width - width;
		winPitch = window->width - width;
	}

	for (i = 0; i < height; i++)
	{
/*		for (G_endCell = G_cell + width; G_cell < G_endCell; G_flags++, G_data++, G_cell++, G_cell2++)
		{
			if (FLAGSET(*G_flags,FLASH))
			{
				FLIPFLAG(*G_flags,SHOWSECONDARY);
			}

			if (!FLAGSET(*G_flags,NONVISIBLE))
			{
				if (FLAGSET(*G_flags,SHOWSECONDARY))
				{
					G_cell->Attributes = *G_data;
				}

				else
				{
					G_cell->Attributes = G_cell2->Attributes;
				}

				G_cell->Char.AsciiChar = G_cell2->Char.AsciiChar;
			}
		}

		G_flags += pitch, G_data += pitch, G_cell2 += pitch;
		G_cell += winPitch;*/
		for (G_end = G_winCell + width; G_winCell < G_end; G_imgCell++, G_winCell++)
		{
			if (FLAGSET(G_imgCell->flags,FLASH))
			{
				FLIPFLAG(G_imgCell->flags,SHOWSECONDARY);
			}

			if (!FLAGSET(G_imgCell->flags,NONVISIBLE))
			{
				if (FLAGSET(G_imgCell->flags,SHOWSECONDARY))
				{
					G_winCell->graph.Attributes = G_imgCell->data;
				}

				else
				{
					G_winCell->graph.Attributes = G_imgCell->graph.Attributes;
				}

				G_winCell->graph.Char.AsciiChar = G_imgCell->graph.Char.AsciiChar;
			}
		}

		G_imgCell += pitch;
		G_winCell += winPitch;
	}
}


/********************************************************************
*	DisplayImageFlags - Display an image composed of its flags		*
********************************************************************/

void DisplayImageFlags (Image * image, ScreenBuffer * buffer)
{
	int i;
	int width, height;
	int pitch, imgPitch;

	assert (image && buffer);
	// Verify that image and buffer point to valid memory

	if (!ClipImageToScreen (image))
	{
		return;
	}

	// Initialization block
	{
		int index;
		int originX, originY;
		int xOffset, yOffset;

		originX = G_clipper.clipRegion.left;
		originY = G_clipper.clipRegion.top;

		xOffset = originX - image->location.X;
		yOffset = originY - image->location.Y;

		width  = G_clipper.clipRegion.right - originX;
		height = G_clipper.clipRegion.bottom - originY;

		index = yOffset * image->width + xOffset;

/*		G_flags = image->image.flags + index;

		G_cell = *(buffer->buffer + originY) + originX;*/
		G_dest = *(buffer->buffer + originY) + originX;

		G_imgCell = image->image.buffer + index;

		pitch = SCREEN_WIDTH - width;
		imgPitch = image->width - width;
	}

	for (i = 0; i < height; i++)
	{
/*		for (G_endCell = G_cell + width; G_cell < G_endCell; G_flags++, G_cell++)
		{
			G_cell->Char.AsciiChar = DATA_AND_FLAGS;
			G_cell->Attributes = *G_flags;
		}

		G_cell += pitch;
		G_flags += imgPitch;*/
		for (G_end = G_imgCell + width; G_imgCell < G_end; G_dest++, G_imgCell++)
		{
			G_dest->Char.AsciiChar = DATA_AND_FLAGS;
			G_dest->Attributes = G_imgCell->flags;
		}

		G_dest += pitch;
		G_imgCell += imgPitch;
	}
}

/********************************************************************
*	DisplayImageData - Display an image composed of its data		*
********************************************************************/

void DisplayImageData (Image * image, ScreenBuffer * buffer)
{
	int i;
	int width, height;
	int pitch, imgPitch;

	assert (image && buffer);
	// Verify that image and buffer point to valid memory

	if (!ClipImageToScreen (image))
	{
		return;
	}

	// Initialization block
	{
		int index;
		int originX, originY;
		int xOffset, yOffset;

		originX = G_clipper.clipRegion.left;
		originY = G_clipper.clipRegion.top;

		xOffset = originX - image->location.X;
		yOffset = originY - image->location.Y;

		width  = G_clipper.clipRegion.right - originX;
		height = G_clipper.clipRegion.bottom - originY;

		index = yOffset * image->width + xOffset;

/*		G_data = image->image.data + index;

		G_cell = *(buffer->buffer + originY) + originX;*/
		G_dest = *(buffer->buffer + originY) + originX;

		G_imgCell = image->image.buffer + index;

		pitch = SCREEN_WIDTH - width;
		imgPitch = image->width - width;
	}	

	for (i = 0; i < height; i++)
	{
/*		for (G_endCell = G_cell + width; G_cell < G_endCell; G_data++, G_cell++)
		{
			G_cell->Char.AsciiChar = DATA_AND_FLAGS;
			G_cell->Attributes = *G_data;
		}

		G_cell += pitch;
		G_data += imgPitch;*/
		for (G_end = G_imgCell + width; G_imgCell < G_end; G_dest++, G_imgCell++)
		{
			G_dest->Char.AsciiChar = DATA_AND_FLAGS;
			G_dest->Attributes = G_imgCell->data;
		}

		G_dest += pitch;
		G_imgCell += imgPitch;
	}
}

/********************************************************************
*	DisplayImage - Display an image normally						*
********************************************************************/

void DisplayImage (Image const * image, ScreenBuffer * buffer)
{
	int i;
	int width, height;
	int pitch, imgPitch;

	// Initialization block
	{
		int index;
		int originX, originY;
		int xOffset, yOffset;

		originX = G_clipper.clipRegion.left;
		originY = G_clipper.clipRegion.top;

		xOffset = originX - image->location.X;
		yOffset = originY - image->location.Y;

		width  = G_clipper.clipRegion.right - originX;
		height = G_clipper.clipRegion.bottom - originY;
	
		index = yOffset * image->width + xOffset;

/*		G_flags = image->image.flags + index;
		G_data = image->image.data + index;

		G_cell = *(buffer->buffer + originY) + originX;
		G_cell2 = image->image.buffer + index;*/
		G_dest = *(buffer->buffer + originY) + originX;

		G_imgCell = image->image.buffer + index;

		pitch = SCREEN_WIDTH - width;
		imgPitch = image->width - width;
	}

	for (i = 0; i < height; i++)
	{
/*		for (G_endCell = G_cell + width; G_cell < G_endCell; G_flags++, G_data++, G_cell++, G_cell2++)
		{
			if (FLAGSET(*G_flags,FLASH))
			{
				FLIPFLAG(*G_flags,SHOWSECONDARY);
			}

			if (!FLAGSET(*G_flags,NONVISIBLE))
			{
				if (FLAGSET(*G_flags,SHOWSECONDARY))
				{
					G_cell->Attributes = *G_data;
				}

				else
				{
					G_cell->Attributes = G_cell2->Attributes;
				}

				G_cell->Char.AsciiChar = G_cell2->Char.AsciiChar;
			}
		}

		G_cell += pitch;
		G_flags += imgPitch, G_data += imgPitch, G_cell2 += imgPitch;*/
		for (G_end = G_imgCell + width; G_imgCell < G_end; G_dest++, G_imgCell++)
		{
			if (FLAGSET(G_imgCell->flags,FLASH))
			{
				FLIPFLAG(G_imgCell->flags,SHOWSECONDARY);
			}

			if (!FLAGSET(G_imgCell->flags,NONVISIBLE))
			{
				if (FLAGSET(G_imgCell->flags,SHOWSECONDARY))
				{
					G_dest->Attributes = G_imgCell->data;
				}

				else
				{
					G_dest->Attributes = G_imgCell->graph.Attributes;
				}

				G_dest->Char.AsciiChar = G_imgCell->graph.Char.AsciiChar;
			}
		}

		G_dest += pitch;
		G_imgCell += imgPitch;
	}
}

/********************************************************************
*	DisplayImageToMap - Display an image against a map				*
********************************************************************/

void DisplayImageToMap (Image const * image, ScreenBuffer * buffer, Map const * map)
{
	int i;
	int width, height;
	int pitch, imgPitch, mapPitch;

	// Initialization block
	{
		int index, mapIndex;
		int originX, originY;
		int xOffset, yOffset;

		originX = G_clipper.clipRegion.left;
		originY = G_clipper.clipRegion.top;

		xOffset = originX - image->location.X;
		yOffset = originY - image->location.Y;

		width  = G_clipper.clipRegion.right - originX;
		height = G_clipper.clipRegion.bottom - originY;
	
		index = yOffset * image->width + xOffset;
		mapIndex = (map->yOffset + originY) * map->width + (map->xOffset + originX);

/*		G_flags = image->image.flags + index;
		G_flags2 = map->world.flags + mapIndex;
		G_data = image->image.data + index;

		G_cell = *(buffer->buffer + originY) + originX;
		G_cell2 = image->image.buffer + index;
		G_cell3 = map->world.buffer + mapIndex;*/
		G_dest = *(buffer->buffer + originY) + originX;

		G_imgCell = image->image.buffer + index;
		G_mapCell = map->world.buffer + mapIndex;

		pitch = SCREEN_WIDTH - width;
		imgPitch = image->width - width;
		mapPitch = map->width - width;
	}

	for (i = 0; i < height; i++)
	{
/*		for (G_endCell = G_cell + width; G_cell < G_endCell; G_flags++, G_flags2++, G_data++, G_cell++, G_cell2++, G_cell3++)
		{
			if (FLAGSET(*G_flags,FLASH))
			{
				FLIPFLAG(*G_flags,SHOWSECONDARY);
			}

			if (!FLAGSET(*G_flags,NONVISIBLE) && (FLAGSET(*G_flags,HIGH) || !FLAGSET(*G_flags2,OBSCURE)))
			{
				if (FLAGSET(*G_flags,SHOWSECONDARY))
				{
					if (FLAGSET(*G_flags2,SHIMMER))
					{
						G_cell->Attributes = MAKEBYTE(*G_data,HINYBBLE(G_cell3->Attributes));
					}

					else
					{
						G_cell->Attributes = *G_data;
					}
				}

				else
				{
					if (FLAGSET(*G_flags2,SHIMMER))
					{
						G_cell->Attributes = MAKEBYTE(G_cell2->Attributes,HINYBBLE(G_cell3->Attributes));
					}

					else
					{
						G_cell->Attributes = G_cell2->Attributes;
					}
				}

				G_cell->Char.AsciiChar = G_cell2->Char.AsciiChar;
			}
		}

		G_cell += pitch;
		G_cell2 += imgPitch, G_flags += imgPitch, G_data += imgPitch;
		G_cell3 += mapPitch, G_flags2 += mapPitch;*/
		for (G_end = G_imgCell + width; G_imgCell < G_end; G_dest++, G_mapCell++, G_imgCell++)
		{
			if (FLAGSET(G_imgCell->flags,FLASH))
			{
				FLIPFLAG(G_imgCell->flags,SHOWSECONDARY);
			}

			if (!FLAGSET(G_imgCell->flags,NONVISIBLE) && (FLAGSET(G_imgCell->flags,HIGH) || !FLAGSET(G_mapCell->flags,OBSCURE)))
			{
				if (FLAGSET(G_imgCell->flags,SHOWSECONDARY))
				{
					if (FLAGSET(G_mapCell->flags,SHIMMER))
					{
						G_dest->Attributes = MAKEBYTE(G_imgCell->data,HINYBBLE(G_mapCell->graph.Attributes));
					}

					else
					{
						G_dest->Attributes = G_imgCell->data;
					}
				}

				else
				{
					if (FLAGSET(G_mapCell->flags,SHIMMER))
					{
						G_dest->Attributes = MAKEBYTE(G_imgCell->graph.Attributes,HINYBBLE(G_mapCell->graph.Attributes));
					}

					else
					{
						G_dest->Attributes = G_imgCell->graph.Attributes;
					}
				}

				G_dest->Char.AsciiChar = G_imgCell->graph.Char.AsciiChar;
			}

			G_dest += pitch;
			G_mapCell += mapPitch;
			G_imgCell += imgPitch;
		}
	}
}

/********************************************************************
*																	*
*							Animation Display Routines				*
*																	*
********************************************************************/

/********************************************************************
*	UpdateAnimation - Update an animation for display				*
********************************************************************/

void UpdateAnimation (Animation * animation)
{
	animation->t += animation->stepSize;
	// Increment animation's t field by animation's stepSize field's value

	if (animation->t > (float) 1.0)
	{
		animation->t = 0.0;	// Reset parameter
	}
	// Check whether parameter has reached end of its path

	SetCurrentFrame (animation, (animation->states + animation->curState)->curFrame + animation->frameRate);
	// Update the current frame and active image

	switch (animation->type)	// Get the animation's path type
	{
	case kStationary:			// Stationary case
		break;	// Break out of switch statement

	case kLinear:				// Linear case
		animation->globalLoc = PointPlusVector2 (animation->path.vector.tail, ScaleVector2 (animation->path.vector, animation->t));
		// Calculate animation's position along a line

		break;	// Break out of switch statement

	case kBezier:				// Bézier case
		animation->globalLoc = Bezier2 (animation->path, animation->t);
		// Calculate animation's position along a Bézier curve

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported type: UpdateAnimation failed","1");
		// Return failure
	}
}

/********************************************************************
*	DisplayAnimationToScreen - Display an animation to the screen	*
********************************************************************/

void DisplayAnimationToScreen (Animation * animation, ScreenBuffer * buffer, Map const * map)
{
	assert (animation && buffer);
	// Verify that animation and buffer point to valid memory

	UpdateAnimation (animation);
	
	if (map)	// Ensure that map points to something
	{
		animation->activeImage->location.X = (int) (animation->globalLoc.x - map->xOffset);
		animation->activeImage->location.Y = (int) (animation->globalLoc.y - map->yOffset);
	}

	else
	{
		animation->activeImage->location.X = (int) animation->globalLoc.x;
		animation->activeImage->location.Y = (int) animation->globalLoc.y;
	}

	DisplayImageToScreen (animation->activeImage, buffer, map);
}

/********************************************************************
*	DisplayAnimationToWindow - Display an animation to a window		*
********************************************************************/

void DisplayAnimationToWindow (Animation * animation, Window * window)
{
	assert (animation && window);
	// Verify that animation and window point to valid memory

	UpdateAnimation (animation);

	animation->activeImage->location.X = (int) animation->globalLoc.x;
	animation->activeImage->location.Y = (int) animation->globalLoc.y;

	DisplayImageToWindow (animation->activeImage, window);
}

/********************************************************************
*																	*
*							Pattern Display Routines				*
*																	*
********************************************************************/

/********************************************************************
*	ClipPatternToScreen - Clip a pattern to display to the screen	*
********************************************************************/

static BOOL ClipPatternToScreen (Pattern * pattern)
{
	int originX, originY;
	int extentX, extentY;

	originX = pattern->location.X;
	originY = pattern->location.Y;

	extentX = originX + pattern->width;
	extentY = originY + pattern->height;

	G_clipper.visible = (extentX > 0 && originX < SCREEN_WIDTH)
					 && (extentY > 0 && originY < SCREEN_HEIGHT);

	if (G_clipper.visible)
	{
		G_clipper.clipRegion.left   = originX > 0 ? originX : 0;
		G_clipper.clipRegion.top	= originY > 0 ? originY : 0;
		G_clipper.clipRegion.right  = extentX < SCREEN_WIDTH ? extentX : SCREEN_WIDTH;
		G_clipper.clipRegion.bottom = extentY < SCREEN_HEIGHT ? extentY : SCREEN_HEIGHT;

		return TRUE;
	}

	return FALSE;
}

/********************************************************************
*	ClipPatternToWindow - Clip a pattern to display to a window		*
********************************************************************/

static BOOL ClipPatternToWindow (Pattern * pattern, Window const * window)
{
	int originX, originY;
	int extentX, extentY;
	int winExtentX, winExtentY;

	originX = pattern->location.X;
	originY = pattern->location.Y;

	extentX = originX + pattern->width;
	extentY = originY + pattern->height;

	winExtentX = window->xOffset + window->windowWidth;
	winExtentY = window->yOffset + window->windowHeight;

	G_clipper.visible = (extentX > window->xOffset && originX < winExtentX)
			  	     && (extentY > window->yOffset && originY < winExtentY);

	if (G_clipper.visible)
	{
		G_clipper.clipRegion.left   = originX > window->xOffset ? originX : window->xOffset;
		G_clipper.clipRegion.top	= originY > window->yOffset ? originY : window->yOffset;
		G_clipper.clipRegion.right  = extentX < winExtentX ? extentX : winExtentX;
		G_clipper.clipRegion.bottom = extentY < winExtentY ? extentY : winExtentY;

		return TRUE;
	}

	return FALSE;
}

/********************************************************************
*	WrapPattern - Wrap a pattern to be displayed					*
********************************************************************/

static void WrapPattern (Pattern * pattern)
{
	pattern->xOffset = (pattern->xOffset + pattern->xScroll) % pattern->width;
	pattern->yOffset = (pattern->yOffset + pattern->yScroll) % pattern->height;

	if (pattern->xOffset >= G_clipper.clipRegion.left && pattern->xOffset <= G_clipper.clipRegion.right)
	{
		G_wrapper.firstX = pattern->xOffset;
		G_wrapper.secondX = G_clipper.clipRegion.left;

		G_wrapper.firstWidth = G_clipper.clipRegion.right - G_wrapper.firstX;
		G_wrapper.secondWidth = G_wrapper.firstX - G_wrapper.secondX;
	}

	else
	{
		G_wrapper.firstX = G_clipper.clipRegion.left;

		G_wrapper.firstWidth = G_clipper.clipRegion.right - G_wrapper.firstX;
		G_wrapper.secondWidth = 0;
	}

	if (pattern->yOffset >= G_clipper.clipRegion.top && pattern->yOffset <= G_clipper.clipRegion.bottom)
	{
		G_wrapper.firstY = pattern->yOffset;
		G_wrapper.secondY = G_clipper.clipRegion.top;

		G_wrapper.firstHeight = pattern->yOffset - G_wrapper.firstY;
		G_wrapper.secondHeight = G_wrapper.firstY - G_wrapper.secondY;
	}

	else
	{
		G_wrapper.firstY = G_clipper.clipRegion.top;

		G_wrapper.firstHeight = G_clipper.clipRegion.bottom - G_wrapper.firstY;
		G_wrapper.secondHeight = 0;
	}
}

/********************************************************************
*	DisplayPatternToScreen - Wrapper for pattern display to screen	*
********************************************************************/

void DisplayPatternToScreen (Pattern * pattern, ScreenBuffer * buffer)
{
	if (!ClipPatternToScreen (pattern))
	{
		return;
	}

	if (!pattern->lock)
	{
		WrapPattern (pattern);
	
		DisplayQuadrantToScreen (pattern, buffer, kQuadOne);

		if (G_wrapper.secondWidth)
		{
			DisplayQuadrantToScreen (pattern, buffer, kQuadTwo);
		}

		if (G_wrapper.secondHeight)
		{
			DisplayQuadrantToScreen (pattern, buffer, kQuadThree);

			if (G_wrapper.secondWidth)
			{
				DisplayQuadrantToScreen (pattern, buffer, kQuadFour);
			}
		}
	} 

	else
	{
		DisplayLockedPatternToScreen (pattern, buffer);
	}
}

/********************************************************************
*	DisplayPatternToWindow - Wrapper for pattern display to window	*
********************************************************************/

void DisplayPatternToWindow (Pattern * pattern, Window * window)
{
}

/********************************************************************
*	DisplayPatternFlags - Display a pattern composed of its flags	*
********************************************************************/

void DisplayPatternFlags (Pattern * pattern, ScreenBuffer * buffer)
{
}

/********************************************************************
*	DisplayPatternData - Display a pattern composed of its data		*
********************************************************************/

void DisplayPatternData (Pattern * pattern, ScreenBuffer * buffer)
{
}

/********************************************************************
*	GetQuadrantInfo - Used to gather information about a quadrant	*
********************************************************************/

static void GetQuadrantInfo (Pattern const * pattern, Quadrant quadrant, intStar index, intStar width, intStar height, intStar patPitch)
{
	switch (quadrant)
	{
	case kQuadOne:
		*width = G_wrapper.firstWidth;
		*height = G_wrapper.firstHeight;

		*index = pattern->yOffset * pattern->width + pattern->xOffset;

		*patPitch = G_wrapper.firstPitch;

	/*	G_cell*/G_dest += G_wrapper.firstY * SCREEN_WIDTH + G_wrapper.firstX;

		break;	// Break out of switch statement

	case kQuadTwo:
		*width = G_wrapper.secondWidth;
		*height = G_wrapper.firstHeight;

		*index = pattern->yOffset * pattern->width + (pattern->xOffset - *width);

		*patPitch = G_wrapper.secondPitch;

	/*	G_cell*/G_dest += G_wrapper.firstY * SCREEN_WIDTH + G_wrapper.secondX;

		break;	// Break out of switch statement

	case kQuadThree:
		*width = G_wrapper.firstWidth;
		*height = G_wrapper.secondHeight;

		*index = (pattern->yOffset - *height) * pattern->width + pattern->xOffset;

		*patPitch = G_wrapper.firstPitch;

	/*	G_cell*/G_dest += G_wrapper.secondY * SCREEN_WIDTH + G_wrapper.firstX;

		break;	// Break out of switch statement

	case kQuadFour:
		*width = G_wrapper.secondWidth;
		*height = G_wrapper.secondHeight;

		*index = (pattern->yOffset - *height) * pattern->width + (pattern->xOffset - *width);

		*patPitch = G_wrapper.secondPitch;

	/*	G_cell*/G_dest += G_wrapper.secondY * SCREEN_WIDTH + G_wrapper.secondX;

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported quadrant: GetQuadrantInfo failed","1");
		// Return failure
	}
}

/********************************************************************
*	DisplayQuadrantToScreen - Displays a quadrant to the screen		*
********************************************************************/

void DisplayQuadrantToScreen (Pattern * pattern, ScreenBuffer * buffer, Quadrant quadrant)
{
	int i;
	int width, height;
	int pitch, patPitch;

/*	G_cell*/G_dest = *buffer->buffer;

	// Initialization block
	{
		int index;

		GetQuadrantInfo (pattern, quadrant, &index, &width, &height, &patPitch);

/*		G_flags = pattern->pattern.flags + index;

		G_cell2 = pattern->pattern.buffer + index;*/
		G_patCell = pattern->pattern.buffer + index;

		pitch = SCREEN_WIDTH - width;
	}

	for (i = 0; i < height; i++)
	{
/*		for (G_endCell = G_cell + width; G_cell < G_endCell; G_flags++, G_cell++, G_cell2++)
		{
			if (!FLAGSET(*G_flags,NONVISIBLE))
			{
				G_cell->Char.AsciiChar = G_cell2->Char.AsciiChar;
				G_cell->Attributes = G_cell2->Attributes;
			}
		}

		G_cell += pitch;
		G_cell2 += patPitch, G_flags += patPitch;*/
		for (G_end = G_patCell + width; G_patCell < G_end; G_dest++, G_patCell++)
		{
			if (!FLAGSET(G_patCell->flags,NONVISIBLE))
			{
				*G_dest = G_patCell->graph;
			}
		}

		G_dest += pitch;
		G_patCell += patPitch;
	}
}

/********************************************************************
*	DisplayQuadrantToWindow - Display a quadrant to a window		*
********************************************************************/

void DisplayQuadrantToWindow (Pattern * pattern, Window * window, Quadrant quadrant)
{
}

/********************************************************************
*	DisplayLockedPatternToScreen - Display locked pattern to screen	*
********************************************************************/

void DisplayLockedPatternToScreen (Pattern * pattern, ScreenBuffer * buffer)
{
	int i;
	int width, height;
	int pitch, patPitch;

	// Initialization block
	{
		int index;
		int originX, originY;
		int xOffset, yOffset;

		originX = G_clipper.clipRegion.left;
		originY = G_clipper.clipRegion.top;

		xOffset = originX - pattern->location.X;
		yOffset = originY - pattern->location.Y;

		width  = G_clipper.clipRegion.right - originX;
		height = G_clipper.clipRegion.bottom - originY;
	
		index = yOffset * pattern->width + xOffset;

/*		G_flags = pattern->pattern.flags + index;

		G_cell = *(buffer->buffer + originY) + originX;
		G_cell2 = pattern->pattern.buffer + index;*/
		G_dest = *(buffer->buffer + originY) + originX;

		G_patCell = pattern->pattern.buffer + index;

		pitch = SCREEN_WIDTH - width;
		patPitch = pattern->width - width;
	}

	for (i = 0; i < height; i++)
	{
/*		for (G_endCell = G_cell + width; G_cell < G_endCell; G_flags++, G_data++, G_cell++, G_cell2++)
		{
			if (!FLAGSET(*G_flags,NONVISIBLE))
			{
				*G_cell = *G_cell2;
			}
		}

		G_cell += pitch;
		G_flags += patPitch, G_cell2 += patPitch;*/
		for (G_end = G_patCell + width; G_patCell < G_end; G_dest++, G_patCell++)
		{
			if (!FLAGSET(G_patCell->flags,NONVISIBLE))
			{
				*G_dest = G_patCell->graph;
			}
		}

		G_dest += pitch;
		G_patCell += patPitch;
	}
}

/********************************************************************
*	DisplayLockedPatternToWindow - Display locked pattern to window	*
********************************************************************/

void DisplayLockedPatternToWindow (Pattern * pattern, Window * buffer);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Sound									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Destruction								*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	DeinitializeOutputObject - Deinitialize an output object		*
********************************************************************/

BOOL DeinitializeOutputObject (Output * outputObj)
{
	if (!CloseHandle (outputObj->outputH))
	{
		ERROR_MESSAGE("DeinitializeOutputObject failed","1");
		// Return failure
	}

	return TRUE;
	// Return success
}