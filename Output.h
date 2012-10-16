/********************************************************************
*																	*
*							Output.h								*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains information relevant to output management	*
*																	*
********************************************************************/

#ifndef OUTPUT_H
#define OUTPUT_H

#include "Common.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Defines									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define SPECIALOUTPUT	0x1
#define NONVISIBLE		0x1
// Denotes a need to process output in a specialized manner; or a 
// transparent portion of a map or output buffer

#define REDRAWREADY		0x2
#define	SOLID			0x2
#define HOTKEY			0x2
// Denotes that specialized output may be processed; or an impassible
// section on a map; or a portion of an output buffer that yields
// information when accessed by the user

#define SOUNDON			0x4
#define OBSCURE			0x4
#define HIGH			0x4
// Denotes that sounds are active; or a portion of the map that obscures
// the view of an image; or a portion of an output buffer that cannot be
// obscured in this manner

#define MUSICON			0x8
#define OCCUPIED		0x8
#define SHOWSECONDARY	0x8
// Denotes that music is active; or a portion of the map that is presently
// in occupation and is thus impassible, but may lead to more data; or an
// indication that an output buffer should display using a secondary color

#define TRIGGER			0x10
#define FLASH			0x10
// Denotes a portion of the map that triggers some sort of event; or an
// indication that an output buffer should reverse the status of its 
// SHOWSECONDARY flag

#define DANGER			0x20
// Denotes a portion of the map that is harmful in some way

#define SHIMMER			0x40
// Denotes a portion of the map that blends with overlaid imagery

#define EXIT			0x80
// Denotes a portion of the map that lead elsewhere

#define DATA_AND_FLAGS '\xB2'
// Character used to display data and flags

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

Clipper GetClipper (void);

/********************************************************************
*	SetCurrentState - Sets the current state of an animation		*
********************************************************************/

void SetCurrentState (pAnimation animation, int curState);

/********************************************************************
*	SetCurrentFrame - Sets the current frame of animation state		*
********************************************************************/

void SetCurrentFrame (pAnimation animation, float curFrame);

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

BOOL InitializeOutputObject (pOutput outputObj);

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

int SkipSpaces (String text, int index);

/********************************************************************
*	ClearText - Clear text written to output buffer					*
********************************************************************/

void ClearText (pOutputBuffer outputBuf, int index, int extent);

/********************************************************************
*	WriteText - Write contents of text buffer to output buffer		*
********************************************************************/

void WriteText (pOutputBuffer outputBuf, String text, int index);

/********************************************************************
*																	*
*							Screen Buffer Management				*
*																	*
********************************************************************/

/********************************************************************
*	ClearScreen - Clear the contents of the primary screen buffer	*
********************************************************************/

void ClearScreen (pOutput outputObj);

/********************************************************************
*	CopyBuffer - Copy contents of one screen buffer into another	*
********************************************************************/

void CopyBuffer (pScreenBuffer dest, ScreenBuffer const * source);

/********************************************************************
*	UpdateScreen - Update the contents of the screen buffer			*
********************************************************************/

void UpdateScreen (pOutput output);

/********************************************************************
*																	*
*							Map Display Routines					*
*																	*
********************************************************************/

/********************************************************************
*	CopyMapToBuffer - Copy contents from offset of map to a buffer	*
********************************************************************/

void CopyMapToBuffer (pScreenBuffer buffer, Map const * map);

/********************************************************************
*	CopyMapFlagsToBuffer - Copy the flags from map offset to buffer	*
********************************************************************/

void CopyMapFlagsToBuffer (pScreenBuffer buffer, Map const * map);

/********************************************************************
*	CopyMapDataToBuffer - Copy the data from map offset to buffer	*
********************************************************************/

void CopyMapDataToBuffer (pScreenBuffer buffer, Map const * map);

/********************************************************************
*	ScrollMap - Scroll the contents of the given map				*
********************************************************************/

void ScrollMap (pMap map, HorzScroll horzMode, VertScroll vertMode);

/********************************************************************
*																	*
*							Image Display Routines					*
*																	*
********************************************************************/

/********************************************************************
*	ClipImageToScreen - Clip an image to be displayed to the screen *
********************************************************************/

static BOOL ClipImageToScreen (pImage image);

/********************************************************************
*	ClipImageToWindow - Clip an image to be displayed to a window	*
********************************************************************/

static BOOL ClipImageToWindow (pImage image, Window const * window);

/********************************************************************
*	DisplayImageToScreen - Primary wrapper for image display		*
********************************************************************/

void DisplayImageToScreen (pImage image, pScreenBuffer buffer, Map const * map);

/********************************************************************
*	DisplayImageToWindow - Display an image to a window				*
********************************************************************/

void DisplayImageToWindow (pImage image, pWindow window);

/********************************************************************
*	DisplayImageFlags - Display an image composed of its flags		*
********************************************************************/

void DisplayImageFlags (pImage image, pScreenBuffer buffer);

/********************************************************************
*	DisplayImageData - Display an image composed of its data		*
********************************************************************/

void DisplayImageData (pImage image, pScreenBuffer buffer);
 
/********************************************************************
*	DisplayImage - Display an image normally						*
********************************************************************/

void DisplayImage (Image const * image, pScreenBuffer buffer);

/********************************************************************
*	DisplayImageToMap - Display an image against a map				*
********************************************************************/

void DisplayImageToMap (Image const * image, pScreenBuffer buffer, Map const * map);

/********************************************************************
*																	*
*							Animation Display Routines				*
*																	*
********************************************************************/

/********************************************************************
*	UpdateAnimation - Update an animation for display				*
********************************************************************/

void UpdateAnimation (pAnimation animation);

/********************************************************************
*	DisplayAnimationToScreen - Display an animation to the screen	*
********************************************************************/

void DisplayAnimationToScreen (pAnimation animation, pScreenBuffer buffer, Map const * map);

/********************************************************************
*	DisplayAnimationToWindow - Display an animation to a window		*
********************************************************************/

void DisplayAnimationToWindow (pAnimation animation, pWindow window);

/********************************************************************
*																	*
*							Pattern Display Routines				*
*																	*
********************************************************************/

/********************************************************************
*	ClipPatternToScreen - Clip a pattern to display to the screen	*
********************************************************************/

static BOOL ClipPatternToScreen (pPattern pattern);

/********************************************************************
*	ClipPatternToWindow - Clip a pattern to display to a window		*
********************************************************************/

static BOOL ClipPatternToWindow (pPattern pattern, Window const * window);

/********************************************************************
*	WrapPattern - Wrap a pattern to be displayed					*
********************************************************************/

static void WrapPattern (pPattern pattern);

/********************************************************************
*	DisplayPatternToScreen - Wrapper for pattern display to screen	*
********************************************************************/

void DisplayPatternToScreen (pPattern pattern, pScreenBuffer buffer);

/********************************************************************
*	DisplayPatternToWindow - Wrapper for pattern display to window	*
********************************************************************/

void DisplayPatternToWindow (pPattern pattern, pWindow window);

/********************************************************************
*	DisplayPatternFlags - Display a pattern composed of its flags	*
********************************************************************/

void DisplayPatternFlags (pPattern pattern, pScreenBuffer buffer);

/********************************************************************
*	DisplayPatternData - Display a pattern composed of its data		*
********************************************************************/

void DisplayPatternData (pPattern pattern, pScreenBuffer buffer);

/********************************************************************
*	GetQuadrantInfo - Used to gather information about a quadrant	*
********************************************************************/

static void GetQuadrantInfo (Pattern const * pattern, Quadrant quadrant, intStar index, intStar width, intStar height, intStar patPitch);

/********************************************************************
*	DisplayQuadrantToScreen - Display a quadrant to the screen		*
********************************************************************/

void DisplayQuadrantToScreen (pPattern pattern, pScreenBuffer buffer, Quadrant quadrant);

/********************************************************************
*	DisplayQuadrantToWindow - Display a quadrant to a window		*
********************************************************************/

void DisplayQuadrantToWindow (pPattern pattern, pWindow window, Quadrant quadrant);

/********************************************************************
*	DisplayLockedPatternToScreen - Display locked pattern to screen	*
********************************************************************/

void DisplayLockedPatternToScreen (pPattern pattern, pScreenBuffer buffer);

/********************************************************************
*	DisplayLockedPatternToWindow - Display locked pattern to window	*
********************************************************************/

void DisplayLockedPatternToWindow (pPattern pattern, pWindow window);

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

BOOL DeinitializeOutputObject (pOutput outputObj);

#endif