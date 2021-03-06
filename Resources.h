/********************************************************************
*																	*
*							Resources.h								*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains information relevant to resources			*
*																	*
********************************************************************/

#ifndef RESOURCES_H
#define RESOURCES_H

#include "Common.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Resources access functions				*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Resource sharing						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	ShareScreenBuffer - Add a client to the screen buffer resource	*
********************************************************************/

void ShareScreenBuffer (pScreenBuffer * buffer, pObjects objects);

/********************************************************************
*	ShareList - Add a client to the list resource					*
********************************************************************/

void ShareList (pListHead * listHead, pObjects objects);

/********************************************************************
*	ShareQueue - Add a client to the queue resource					*
********************************************************************/

void ShareQueue (pQueue * queue, pObjects objects);

/********************************************************************
*	ShareStack - Add a client to the stack resource					*
********************************************************************/

void ShareStack (pStack * stack, pObjects objects);

/********************************************************************
*	ShareOutputBuffer - Add a client to an output buffer			*
********************************************************************/

void ShareOutputBuffer (pOutputBuffer outputBuf, OutputBuffer const * source);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Thread management						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Console initialization					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	ConsoleInit - Readies the primary objects for use				*
********************************************************************/

BOOL ConsoleInit (pObjects objects, DWORD flags, BOOL showCursor);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							MemoryManagement						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	DeleteImage - Unmaps an Image structure's memory				*
********************************************************************/

void DeleteImage (pImage image);

/********************************************************************
*	DeletePattern - Unmaps a Pattern structure's memory				*
********************************************************************/

void DeletePattern (pPattern pattern);

/********************************************************************
*	DeleteAnimationState - Unmaps an AnimationState structure's		*
*		memory														*
********************************************************************/

void DeleteAnimationState (pAnimationState animationState);

/********************************************************************
*	DeleteAnimation - Unmaps an Animation structure's memory		*
********************************************************************/

void DeleteAnimation (pAnimation animation);

/********************************************************************
*	DeleteVisuals - Unmaps a Visuals structure's memory				*
********************************************************************/

void DeleteVisuals (pVisuals visuals);

/********************************************************************
*	DeleteOutputBuffer - Unmaps an OutputBuffer structure's memory	*
********************************************************************/

void DeleteOutputBuffer (pOutputBuffer outputBuf);

/********************************************************************
*	DeleteMap - Unmaps a Map structure's memory						*
********************************************************************/

void DeleteMap (pMap map);

/********************************************************************
*	DeleteMenus - Unmaps a Menu structure's memory					*
********************************************************************/

void DeleteMenus (pMenu menu);

/********************************************************************
*	DeleteWindow - Unmaps a Window structure's memory				*
********************************************************************/

void DeleteWindow (pWindow window);

/********************************************************************
*	DeleteParentWindow - Unmaps a ParentWindow's memory				*
********************************************************************/

void DeleteParentWindow (pParentWindow parentWindow);

/********************************************************************
*	CleanupResources - Cleans up memory of a Resources structure	*
********************************************************************/

BOOL CleanupResources (pResources resources);

/********************************************************************
*	DeinitializeObjects - Deinitializes an Objects structure		*
********************************************************************/

BOOL DeinitializeObjects (pObjects objects);

#endif