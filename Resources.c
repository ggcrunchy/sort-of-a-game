/********************************************************************
*																	*
*							Resources.c								*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains implementation of resources tool			*
*																	*
********************************************************************/

#include "Resources.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							External includes						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "ADT.h"
#include "File.h"
#include "Input.h"
#include "Output.h"
#include "Scene.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Globals									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static pWindow G_endWindow;
// Used to quicken Window loops
static pMenuItem G_endItem;
// Used to quicken MenuItem loops
static pKey G_endKey;
// Used to quicken Key loops
static pSeparatorEntry G_endSepEntry;
// Used to quicken SeparatorEntry loops
static pReceiverEntry G_endRecEntry;
// Used to quicken ReceiverEntry loops
static pImage G_endImage;
// Used to quicken Image loops
static pAnimationState G_endAnimState;
// Used to quicken AnimationState loops
static pAnimation G_endAnimation;
// Used to quicken Animation loops
static pPattern G_endPattern;
// Used to quicken Pattern loops
static pPointSet G_endSet;
// Used to quicken PointSet loops
static pTriangulation G_endTriangulation;
// Used to quicken Triangulation loops

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

void ShareScreenBuffer (ScreenBuffer ** buffer, Objects * objects)
{
	assert (buffer && *buffer && objects);

	*buffer = &objects->resources.screenBuf;
}

/********************************************************************
*	ShareList - Add a client to the list resource					*
********************************************************************/

void ShareList (ListHead ** listHead, Objects * objects)
{
	assert (listHead && *listHead && objects);

	*listHead = &objects->resources.list;
}

/********************************************************************
*	ShareQueue - Add a client to the queue resource					*
********************************************************************/

void ShareQueue (Queue ** queue, Objects * objects)
{
	assert (queue && *queue && objects);

	*queue = &objects->resources.queue;
}

/********************************************************************
*	ShareStack - Add a client to the stack resource					*
********************************************************************/

void ShareStack (Stack ** stack, Objects * objects)
{
	assert (stack && *stack && objects);

	*stack = &objects->resources.stack;
}

/********************************************************************
*	ShareOutputBuffer - Add a client to an output buffer			*
********************************************************************/

void ShareOutputBuffer (OutputBuffer * outputBuf, OutputBuffer const * source)
{
	assert (outputBuf && source);

	outputBuf->bufSharing = kShared;

/*	outputBuf->buffer = source->buffer;
	outputBuf->flags = source->flags;
	outputBuf->data = source->data;*/
	outputBuf->buffer = source->buffer;
}

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

BOOL ConsoleInit (Objects * objects, DWORD flags, BOOL showCursor)
{
	CONSOLE_CURSOR_INFO cci = {CURSOR_HEIGHT, showCursor};	// Console cursor information
	COORD size = {SCREEN_WIDTH, SCREEN_HEIGHT};				// Screen buffer size

	assert (objects);
	// Verify that objects points to valid memory

	srand (time (NULL));
	// Seed the random number generator

	if (!InitializeFileObject (&objects->fileObj))
	{
		ERROR_MESSAGE("ConsoleInit failed","1");
		// Return failure
	}
	// Initialize the primary file object

	if (!InitializeInputObject (&objects->inputObj))
	{
		ERROR_MESSAGE("ConsoleInit failed","2");
		// Return failure
	}
	// Initialize the primary input object

	if (!InitializeOutputObject (&objects->outputObj))
	{
		ERROR_MESSAGE("ConsoleInit failed","3");
		// Return failure
	}
	// Initialize the primary output object

	if (!InitializeSceneObject (&objects->sceneObj))
	{
		ERROR_MESSAGE("ConsoleInit failed","4");
		// Return failure
	}
	// Initialize the primary scene object
	
	if (!SetConsoleMode (objects->inputObj.inputH, flags))
	{
		ERROR_MESSAGE("ConsoleInit failed","5");
		// Return failure
	}
	// Set the console mode

	if (!SetConsoleCursorInfo (objects->outputObj.outputH, &cci))
	{
		ERROR_MESSAGE("ConsoleInit failed","6");
		// Return failure
	}
	// Set the console cursor info

	if (!SetConsoleScreenBufferSize (objects->outputObj.outputH, size))
	{
		ERROR_MESSAGE("ConsoleInit failed","7");
		// Return failure
	}
	// Set the console screen buffer size

	return TRUE;
	// Return success
}

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

void DeleteImage (Image * image)
{
	if (!image)	// Ensure that image points to something
	{
		return;	// Return if image is NULL
	}

	DeleteOutputBuffer (&image->image);
}

/********************************************************************
*	DeletePattern - Unmaps a Pattern structure's memory				*
********************************************************************/

void DeletePattern (Pattern * pattern)
{
	if (!pattern)	// Ensure that pattern points to something
	{
		return;	// Return if pattern is NULL
	}

	DeleteOutputBuffer (&pattern->pattern);
}

/********************************************************************
*	DeleteAnimationState - Unmaps an AnimationState structure's		*
*		memory														*
********************************************************************/

void DeleteAnimationState (AnimationState * animationState)
{
	pImage image;

	if (!animationState)	// Ensure that animationState points to something
	{
		return;	// Return if animationState is NULL
	}

	for (image = animationState->frames, G_endImage = image + animationState->numFrames; image < G_endImage; image++)
	{
		DeleteImage (image);
		// Remove the image structure at the given index
	}

	FREE(animationState->frames);
	// Free memory pointed to by animationState's frames field
}

/********************************************************************
*	DeleteAnimation - Unmaps an Animation structure's memory		*
********************************************************************/

void DeleteAnimation (Animation * animation)
{
	pAnimationState animState;

	if (!animation)	// Ensure that animation points to something
	{
		return;	// Return if animation is NULL
	}

	for (animState = animation->states, G_endAnimState = animState + animation->numStates; animState < G_endAnimState; animState++)
	{
		DeleteAnimationState (animState);
	}

	FREE(animation->states);
	// Free memory pointed to by animation's states field

	FREE(animation->path.control);
	// Free memory pointed to by animation's path field's control field
}

/********************************************************************
*	DeleteVisuals - Unmaps a Visuals structure's memory				*
********************************************************************/

void DeleteVisuals (Visuals * visuals)
{
	pImage image;
	pPattern pattern;
	pAnimation animation;

	if (!visuals)	// Ensure that visuals points to something
	{
		return;	// Return if visuals is NULL
	}

	for (image = visuals->images, G_endImage = image + visuals->numImages; image < G_endImage; image++)
	{
		DeleteImage (image);
		// Remove the Image structure at the given index
	}

	for (pattern = visuals->patterns, G_endPattern = pattern + visuals->numPatterns; pattern < G_endPattern; pattern++)
	{
		DeletePattern (pattern);
		// Remove the Pattern structure at the given index
	}
		
	for (animation = visuals->animations, G_endAnimation = animation + visuals->numAnimations; animation < G_endAnimation; animation++)
	{
		DeleteAnimation (animation);
		// Remove the Animation structure at the given index
	}

	FREE(visuals->images);
	// Free memory pointed to by visuals's images field
	FREE(visuals->patterns);
	// Free memory pointed to by visuals's patterns field
	FREE(visuals->animations);
	// Free memory pointed to by visuals's animation field
}

/********************************************************************
*	DeleteOutputBuffer - Unmaps an OutputBuffer structure's memory	*
********************************************************************/

void DeleteOutputBuffer (OutputBuffer * outputBuf)
{
	switch (outputBuf->bufSharing)
	{
	case kSingleOwner:
/*		FREE(outputBuf->buffer);
		// Free memory pointed to by outputBuf's buffer field
		FREE(outputBuf->data);
		// Free memory pointed to by outputBuf's data field
		FREE(outputBuf->flags);
		// Free memory pointed to by outputBuf's flags field*/
		FREE(outputBuf->buffer);

		break;	// Break out of switch statement

	case kShared:
/*		outputBuf->buffer = NULL;
		outputBuf->data = NULL;
		outputBuf->flags = NULL;*/
		outputBuf->buffer = NULL;

		break;	// Break out of switch statement
	}
}

/********************************************************************
*	DeleteMap - Unmaps a Map structure's memory						*
********************************************************************/

void DeleteMap (Map * map)
{
	if (!map)	// Ensure that map points to something
	{
		return;	// Return if map is NULL
	}

	DeleteOutputBuffer (&map->world);

	if (map->visuals)	// Ensure that map's visuals field points to something
	{
		DeleteVisuals (map->visuals);
		// Remove the Visuals structure

		FREE(map->visuals);
		// Free memory pointed to by map's visuals field
	}
}

/********************************************************************
*	DeleteMenus - Unmaps a Menu structure's memory					*
********************************************************************/

void DeleteMenus (Menu * menu)
{
	pKey keys;
	pMenuItem menuItem;

	if (!menu)	// Ensure that menu points to something
	{
		return;	// Return if menu is NULL
	}

	for (keys = menu->keys, G_endKey = keys + menu->numKeys; keys < G_endKey; keys++)
	{
		keys->function = NULL;
		// Nullify function field of menu's keys field at given index
	}

	FREE(menu->keys);
	// Free memory pointed to by menu's keys field

	for (menuItem = menu->menu, G_endItem = menuItem + menu->numItems; menuItem < G_endItem; menuItem++)
	{
		FREE(menuItem->text);
		// Free memory pointed to by text field of menu's menu field at given index
	}

	FREE(menu->menu);
	// Free memory pointed to by menu's menu field
}

/********************************************************************
*	DeleteWindow - Unmaps a Window structure's memory				*
********************************************************************/

void DeleteWindow (Window * window)
{
	if (!window)	// Ensure that window points to something
	{
		return;	// Return if window is NULL
	}

	DeleteOutputBuffer (&window->display);

	if (window->IO)	// Ensure that window's IO field points to something
	{
		FREE(window->IO->text.text);
		// Free memory pointed to by window's IO field's text field's text field

		FREE(window->IO);
		// Free memory pointed to by window's IO field
	}

	if (window->menu)	// Ensure that window's menu field points to something
	{
		DeleteMenus (window->menu);
		// Remove the Menu structure

		FREE(window->menu);
		// Free memory pointed to by window's menu field
	}

	if (window->receivers)	// Ensure that window's receivers field points to something
	{
		FREE(window->receivers->receivers);
		// Free memory pointed to by window's receivers's field's receivers field

		FREE(window->receivers);
		// Free memory pointed to by window's receivers field
	}

	if (window->visuals)	// Ensure that window's visuals field points to something
	{
		DeleteVisuals (window->visuals);
		// Remove the Visuals structure
		
		FREE(window->visuals);
		// Free memory pointed to by window's visuals field
	}
	// Verify that window's visuals field points to valid memory

	if (window->dataADT.data.data)	// Ensure that window's ADT field points to something
	{
		switch (window->dataADT.ADTSharing)	// Get the exclusive mode
		{
		case kSingleOwner:					// Single owner case
			switch (window->dataADT.type)	// Get the ADT style
			{
			case kADTList:			// List case
				DestroyList (window->dataADT.data.list);
				// Free memory formatted as linked list ADT

				break;	// Break out of switch statement

			case kADTQueue:			// Queue case
				DestroyQueue (window->dataADT.data.queue);
				// Free memory formatted as queue ADT

				break;	// Break out of switch statement

			case kADTStack:			// Stack case
				DestroyStack (window->dataADT.data.stack);
				// Free memory formatted as stack ADT

				break;	// Break out of switch statement

			default:
				NORET_MESSAGE("Unsupported ADT style: DeleteWindow failed","1");
				// Return failure
			}

			break;	// Break out of switch statement

		case kShared:						// Sharing case
			window->dataADT.data.data = NULL;	// Nullify window's dataADT field's data field's data field

			break;	// Break out of switch statement

		default:
			NORET_MESSAGE("Unsupported exclusive mode: DeleteWindow failed","2");
			// Return failure
		}
	}
}

/********************************************************************
*	DeleteParentWindow - Unmaps a ParentWindow's memory				*
********************************************************************/

void DeleteParentWindow (ParentWindow * parentWindow)
{
	pWindow window;

	if (!parentWindow)	// Ensure that parentWindow points to something
	{
		return;	// Return if parentWindow is NULL
	}

	if (parentWindow->back)	// Ensure that parentWindow's back field points to something
	{
		switch (parentWindow->bufferSharing)	// Get exclusive mode
		{
		case kSingleOwner:						// Single owner case
			FREE(parentWindow->back);
			// Free memory pointed to by parentWindow's back field

			break;	// Break out of switch statement

		case kShared:							// Shared case
			parentWindow->back = NULL;	// Nullify parentWindow's back field

			break;	// Break out of switch statement

		default:
			NORET_MESSAGE("DeleteParentWindow failed","1");
			// Return failure
		}
	}

	if (parentWindow->separators)	// Ensure that parentWindow's separators field points to something
	{
		FREE(parentWindow->separators->horzSeparators);
		// Free memory pointed to by parentWindow's separators field's horzSeparators field
		FREE(parentWindow->separators->vertSeparators);
		// Free memory pointed to by parentWindow's separators field's vertSeparators field

		FREE(parentWindow->separators);
		// Free memory pointed to by parentWindow's separators field
	}

	for (window = parentWindow->windows, G_endWindow = window + parentWindow->numWindows; window < G_endWindow; window++)
	{
		DeleteWindow (window);
		// Remove the Window structure at the given index
	}

	FREE(parentWindow->windows);
	// Free memory pointed to by parentWindow's windows field

	FREE(parentWindow->backData);
	// Free memory pointed to by parentWindow's backData field
}

/********************************************************************
*	CleanupResources - Cleans up memory of a Resources structure	*
********************************************************************/

BOOL CleanupResources (Resources * resources)
{
	if (!EmptyList (&resources->list))
	{
		ERROR_MESSAGE("CleanupResources failed","1");
		// Return failure
	}

	if (!EmptyQueue (&resources->queue))
	{
		ERROR_MESSAGE("CleanupResources failed","2");
		// Return failure
	}

	if (!EmptyStack (&resources->stack))
	{
		ERROR_MESSAGE("CleanupResources failed","3");
		// Return failure
	}

	return TRUE;
	// Return success
}

/********************************************************************
*	DeinitializeObjects - Deinitializes an Objects structure		*
********************************************************************/

BOOL DeinitializeObjects (Objects * objects)
{
	assert (objects);
	// Verify that objects points to valid memory

	if (!DeinitializeFileObject (&objects->fileObj))
	{
		ERROR_MESSAGE("DeinitializeObjects failed","1");
		// Return failure
	}
	// Deinitialize the primary file object

	if (!DeinitializeInputObject (&objects->inputObj))
	{
		ERROR_MESSAGE("DeinitializeObjects failed","2");
		// Return failure
	}
	// Deinitialize the primary input object

	if (!DeinitializeOutputObject (&objects->outputObj))
	{
		ERROR_MESSAGE("DeinitializeObjects failed","3");
		// Return failure
	}
	// Deinitialize the primary output object

	if (!DeinitializeSceneObject (&objects->sceneObj))
	{
		ERROR_MESSAGE("DeinitializeObjects failed","4");
		// Return failure
	}
	// Deinitialize the primary scene object

	if (!CleanupResources (&objects->resources))
	{
		ERROR_MESSAGE("DeinitializeObjects failed","5");
		// Return failure
	}
	// Clean up resources

	return TRUE;
	// Return success
}