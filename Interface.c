/********************************************************************
*																	*
*							Interface.c								*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains implementation of interface tool			*
*																	*
********************************************************************/

#include "Interface.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							External includes						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "ADT.h"
#include "Input.h"
#include "Output.h"
#include "Resources.h"

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
static pAnimation G_endAnimation;
// Used to quicken Animation loops
static pPattern G_endPattern;
// Used to quicken Pattern loops
static pPointSet G_endSet;
// Used to quicken PointSet loops
static pTriangle G_endTriangle;
// Used to quicken Triangle loops
static pTriangulation G_endTriangulation;
// Used to quicken Triangulation loops
static PCHAR_INFO G_endCell;
// Used to quicken CHAR_INFO loops

static PCOORD G_location;
// Used to redraw windows

static PCHAR_INFO G_cell;
// Used for output
static pCell G_end, G_bufCell;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Interface access functions				*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	IncXOffset - Increment the x offset								*
********************************************************************/

static void IncXOffset (Window * window)
{
	window->xOffset++;
}

/********************************************************************
*	DecXOffset - Decrement the x offset								*
********************************************************************/

static void DecXOffset (Window * window)
{
	window->xOffset--;
}

/********************************************************************
*	IncYOffset - Increment the y offset								*
********************************************************************/

static void IncYOffset (Window * window)
{
	window->yOffset++;
}

/********************************************************************
*	DecYOffset - Decrement the y offset								*
********************************************************************/

static void DecYOffset (Window * window)
{
	window->yOffset--;
}

/********************************************************************
*	AdvanceCursor - Advance a Window structure's read/write cursor	*
********************************************************************/

static void AdvanceCursor (Window * window, int amount)
{
	if (window->IO->cursor + amount < window->IO->text.limit)
	{
		window->IO->cursor += amount;
	}
}

/********************************************************************
*	RegressCursor - Regress a Window structure's read/write cursor	*
********************************************************************/

static void RegressCursor (Window * window, int amount)
{
	if (window->IO->cursor - amount >= 0)
	{
		window->IO->cursor -= amount;
	}
}

/********************************************************************
*	AssignFocus - Assign focus to a Window structure				*
********************************************************************/

static void AssignFocus (ParentWindow * parentWindow, int index)
{
	assert (parentWindow);
	// Verify that parentWindow points to valid memory

	SETFLAG((parentWindow->windows + index)->state,HASFOCUS);

	parentWindow->focus = index;
}

/********************************************************************
*	ReleaseFocus - Release Window structure's focus					*
********************************************************************/

static void ReleaseFocus (Window * window)
{
	assert (window);
	// Verify that window points to valid memory

	CLEARFLAG(window->state,HASFOCUS);
}

/********************************************************************
*	HasFocus - Indicates if a Window structure has the focus		*
********************************************************************/

static BOOL HasFocus (Window const * window)
{
	assert (window);
	// Verify that window points to valid memory

	return FLAGSET(window->state,HASFOCUS);
}

/********************************************************************
*	ActivateWindows - Set a ParentWindow structure to active		*
********************************************************************/

void ActivateWindows (ParentWindow * parentWindow)
{
	assert (parentWindow);
	// Verify that parentWindow points to valid memory

	SETFLAG(parentWindow->state,WINDOWSACTIVE);

	G_location = &parentWindow->location;
}

/********************************************************************
*	DeactivateWindows - Set a ParentWindow structure to inactive	*
********************************************************************/

void DeactivateWindows (ParentWindow * parentWindow)
{
	assert (parentWindow);
	// Verify that parentWindow points to valid memory

	CLEARFLAG(parentWindow->state,WINDOWSACTIVE);

	G_location = NULL;
}

/********************************************************************
*	WindowsActive - Indicates if a ParentWindow structure is active	*
********************************************************************/

BOOL WindowsActive (ParentWindow const * parentWindow)
{
	assert (parentWindow);
	// Verify that parentWindow points to valid memory

	return FLAGSET(parentWindow->state,WINDOWSACTIVE);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Construction							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	MakeParentWindow - Constructs a ParentWindow object				*
********************************************************************/

void MakeParentWindow (ParentWindow * parentWindow, int x, int y, int focus)
{
	pWindow window;

	assert (parentWindow && parentWindow->windows);
	// Verify that parentWindow and parentWindow's windows field point to valid memory

	InitParentWindow (parentWindow);

	G_endWindow = parentWindow->windows + parentWindow->numWindows;

	for (window = parentWindow->windows; window < G_endWindow; window++)
	{
		if (window->mode == kMenu)
		{
			InitMenu (window);
		}
	}

	parentWindow->location.X = x;
	parentWindow->location.Y = y;

	AssignFocus (parentWindow, focus);
}

/********************************************************************
*	InitMenu - Initialize a menu associated with a Window object	*
********************************************************************/

static void InitMenu (Window * window)
{
	int offset;
	pMenu menu;
	pMenuItem menuItem;

	assert (window->menu);

	menu = window->menu;

	G_endItem = menu->menu + menu->numItems;

	for (menuItem = menu->menu; menuItem < G_endItem; menuItem++)
	{
		offset = menuItem->location.Y * window->width + menuItem->location.X;

		WriteText (&window->display, *GETMENUITEMTEXT(menuItem), offset);
	}

	HighlightMenuItem (window, menu->itemChosen);
}

/********************************************************************
*	InitParentWindow - Initialize a parent window					*
********************************************************************/

static void InitParentWindow (ParentWindow * parentWindow)
{
	int i, offset;					// Loop variable; an offset variable
	int dimensions;
	BYTE current;
	pSeparatorEntry separatorEntry;

	assert (parentWindow);
	// Verify that parentWindow points to valid memory

	dimensions = parentWindow->width * parentWindow->height;

	for (i = 0; i < parentWindow->width; i++)
	{
		SETBIT(*(parentWindow->backData + i),TOPEDGE);
	}

	for (i = 0, offset = (parentWindow->height - 1) * parentWindow->width; i < parentWindow->width; i++)
	{
		SETBIT(*(parentWindow->backData + (offset + i)),BOTTOMEDGE);
	}

	for (i = 0, offset = 0; i < parentWindow->height; i++, offset += parentWindow->width)
	{
		SETBIT(*(parentWindow->backData + offset),LEFTEDGE);
	}

	for (i = 0, offset = parentWindow->width - 1; i < parentWindow->height; i++, offset += parentWindow->width)
	{
		SETBIT(*(parentWindow->backData + offset),RIGHTEDGE);
	}

	if (parentWindow->separators)	// Ensure that parentWindow's separators field points to something
	{
		G_endSepEntry = parentWindow->separators->horzSeparators + parentWindow->separators->numHorzSeparators;

 		for (separatorEntry = parentWindow->separators->horzSeparators; separatorEntry < G_endSepEntry; separatorEntry++)
		{
			offset = (separatorEntry->offsets.Y + 1) * parentWindow->width + separatorEntry->offsets.X;

			for (i = 0; i < separatorEntry->extent; i++, offset++)
			{
				SETBIT(*(parentWindow->backData + offset),HORZSEPARATOR);
			}
		}

		G_endSepEntry = parentWindow->separators->vertSeparators + parentWindow->separators->numVertSeparators;

		for (separatorEntry = parentWindow->separators->vertSeparators; separatorEntry < G_endSepEntry; separatorEntry++)
		{
			offset = separatorEntry->offsets.Y * parentWindow->width + (separatorEntry->offsets.X + 1);

			for (i = 0; i < separatorEntry->extent; i++, offset += parentWindow->width)
			{
				SETBIT(*(parentWindow->backData + offset),VERTSEPARATOR);
			}
		}
	}

	for (i = 0; i < dimensions; i++)
	{
		current = *(parentWindow->backData + i);

		if (BITSET(current,TOPEDGE) && BITSET(current,LEFTEDGE))
		{
			*(parentWindow->backData + i) = TOPLEFTBORDER;
		}

		else if (BITSET(current,TOPEDGE) && BITSET(current,RIGHTEDGE))
		{
			*(parentWindow->backData + i) = TOPRIGHTBORDER;
		}

		else if (BITSET(current,BOTTOMEDGE) && BITSET(current,LEFTEDGE))
		{
			*(parentWindow->backData + i) = BOTTOMLEFTBORDER;
		}

		else if (BITSET(current,BOTTOMEDGE) && BITSET(current,RIGHTEDGE))
		{
			*(parentWindow->backData + i) = BOTTOMRIGHTBORDER;
		}

		else if (BITSET(current,LEFTEDGE) && BITSET(current,HORZSEPARATOR))
		{
			*(parentWindow->backData + i) = LEFTSEPARATOR;
		}

		else if (BITSET(current,RIGHTEDGE) && BITSET(current,HORZSEPARATOR))
		{
			*(parentWindow->backData + i) = RIGHTSEPARATOR;
		}

		else if (BITSET(current,TOPEDGE) && BITSET(current,VERTSEPARATOR))
		{
			*(parentWindow->backData + i) = HIGHSEPARATOR;
		}

		else if (BITSET(current,BOTTOMEDGE) && BITSET(current,VERTSEPARATOR))
		{
			*(parentWindow->backData + i) = LOWSEPARATOR;
		}

		else if (BITSET(current,HORZSEPARATOR) && BITSET(current,VERTSEPARATOR))
		{
			*(parentWindow->backData + i) = JOINEDSEPARATORS;
		}

		else if (BITSET(current,TOPEDGE) || BITSET(current,BOTTOMEDGE) || BITSET(current,HORZSEPARATOR))
		{
			*(parentWindow->backData + i) = LEFTTORIGHTBORDER;
		}

		else if (BITSET(current,LEFTEDGE) || BITSET(current,RIGHTEDGE) || BITSET(current,VERTSEPARATOR))
		{
			*(parentWindow->backData + i) = UPTODOWNBORDER;
		}

		else
		{
			*(parentWindow->backData + i) = NORMALBACKGROUND;
		}
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Interface management					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	GrabWindow - Handle cases where mouse grabs the window			*
********************************************************************/

static void GrabWindow (ParentWindow * parentWindow, Objects * objects)
{
	int mouseX, mouseY;
	int left = parentWindow->location.X;
	int top = parentWindow->location.Y;

	mouseX = GetMouseXPos (&objects->inputObj);
	mouseY = GetMouseYPos (&objects->inputObj);

	if (mouseX >= left && mouseX < left + parentWindow->width && mouseY >= top && mouseY < top + parentWindow->height)
	{
		if (*(parentWindow->backData + ((mouseY - top) * parentWindow->width + (mouseX - left))) && !FLAGSET(parentWindow->state,FIXED))
		{
			if (!FLAGSET(parentWindow->state,GRABBED))
			{
				SETFLAG(parentWindow->state,GRABBED);

				parentWindow->grabPoints.X = mouseX - left;
				parentWindow->grabPoints.Y = mouseY - top;
			}
		}

		else
		{
			LoadHotKey (parentWindow, objects);
		}
	}
}

/********************************************************************
*	Update - Update a ParentWindow structure						*
********************************************************************/

void Update (ParentWindow * parentWindow, Objects * objects, voidStar data)
{
	int keycode;
	pWindow window;

	assert (parentWindow && objects);
	// Verify that parentWindow and objects point to valid memory

	G_endWindow = parentWindow->windows + parentWindow->numWindows;

	if (FLAGSET(parentWindow->state,GRABBED) && MouseWasMoved (&objects->inputObj))
	{
		ClearScreen (&objects->outputObj);
		CopyBuffer (&objects->outputObj.outputBuf, parentWindow->back);
		RepositionWindows (parentWindow, objects);
		RedrawWindows (parentWindow, objects);
	}

 	keycode = GetInput (&objects->inputObj, kAsync);

	for (window = parentWindow->windows; window < G_endWindow; window++)
	{
		if (FLAGSET(window->state,ACTIVEWITHOUTFOCUS) && !HasFocus (window))
		{
			UpdateWindowContents (window, objects, data);
		}
	}

	SendInformation (parentWindow, objects);
	UpdateWindowContents (GETFOCUS(parentWindow), objects, data);

	if (LeftMouseButtonIsDown (&objects->inputObj))
	{
		GrabWindow (parentWindow, objects);
	}

	else
	{
		if (FLAGSET(parentWindow->state,GRABBED))
		{
			CLEARFLAG(parentWindow->state,GRABBED);
		}
	}
	
	if (keycode)
	{
		if (keycode == parentWindow->focusKey)
		{
			SwitchFocus (parentWindow);
		}

		if (keycode == parentWindow->closeKey)
		{
			DeactivateWindows (parentWindow);
		}

		if (keycode == parentWindow->confirmKey)
		{
			for (window = parentWindow->windows; window < G_endWindow; window++)
			{
				if (HasFocus (window) || FLAGSET(window->state,SAVEWITHOUTFOCUS))
				{
					SaveMessage (window);
				}
			}
		}
	}

	UpdateScreen (&objects->outputObj);

	Sleep (parentWindow->delay);
}

/********************************************************************
*	SendInformation - Primary wrapper for updating the focus		*
********************************************************************/

static void SendInformation (ParentWindow * parentWindow, Objects * objects)
{
	switch (GETFOCUS(parentWindow)->mode)
	{
	case kBasic:
		SendBasicInformation (parentWindow, objects);
		break;	// Break out of switch statement

	case kEditBox:
		SendEditBoxInformation (parentWindow, objects);
		break;	// Break out of switch statement

	case kMenu:
		SendMenuInformation (parentWindow, objects);
		break;	// Break out of switch statement

	case kMessageBox:
		SendMessageBoxInformation (parentWindow, objects);
		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported mode: SendInformation failed","1");
		// Return failure
	}
}

/********************************************************************
*	SendBasicInformation - Send information to a basic window		*
********************************************************************/

static void SendBasicInformation (ParentWindow * parentWindow, Objects * objects)
{
	int lastKey = GetLastKeyCode (&objects->inputObj);
	pWindow window = GETFOCUS(parentWindow);
	HorzScroll horzMode = kHorzFix;
	VertScroll vertMode = kVertFix;

	SCROLL(window,horzMode,vertMode,lastKey);
}

/********************************************************************
*	SendEditBoxInformation - Send information to an edit box		*
********************************************************************/

static void SendEditBoxInformation (ParentWindow * parentWindow, Objects * objects)
{
	char nextChar = GetLastChar (&objects->inputObj);
	int lastKey = GetLastKeyCode (&objects->inputObj);
//	PCHAR_INFO cell;
	pWindow window;
	HorzScroll horzMode = kHorzFix;
	VertScroll vertMode = kVertFix;
	
	window = GETFOCUS(parentWindow);

	SCROLL(window,horzMode,vertMode,lastKey);

	switch (window->IO->readMode)	// Get the read mode
	{
	case kReadAll:					// ReadAll case
		break;	// Break out of switch statement

	case kAlphaOnly:				// AlphaOnly case
		nextChar = isalpha (nextChar) ? nextChar : END;
		break;	// Break out of switch statement

	case kNumeric:					// Numeric case
		nextChar = isdigit (nextChar) ? nextChar : END;
		break;	// Break out of switch statement

	case kPrintOnly:				// PrintOnly case
		nextChar = isprint (nextChar) ? nextChar : END;
		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported read mode: SendEditBoxInformation failed","1");
		// Return failure
	}

	if (lastKey != parentWindow->focusKey && lastKey != parentWindow->closeKey && lastKey != parentWindow->confirmKey)
	{
	/*	cell*/G_bufCell = GETCURSORCELL(window);

		if (lastKey == VK_BACK)
		{
		/*	cell*/G_bufCell->graph.Char.AsciiChar = SPACE;
			RegressCursor (window, 1);

		/*	cell*/G_bufCell->graph.Char.AsciiChar = SPACE;
			*GETCURSORTEXT(window) = END;
		}

		else
		{
			if (nextChar)
			{
			/*	cell*/G_bufCell->graph.Char.AsciiChar = nextChar;
				*GETCURSORTEXT(window) = nextChar;

				AdvanceCursor (window, 1);
				*GETCURSORTEXT(window) = END;
			}	
		}
	}
}

/********************************************************************
*	SendMenuInformation	- Send information to a menu				*
********************************************************************/

static void SendMenuInformation (ParentWindow * parentWindow, Objects * objects)
{
	int lastKey = GetLastKeyCode (&objects->inputObj);
	BOOL updateReceivers = FALSE;
	pWindow window = GETFOCUS(parentWindow);
	pReceivers receivers = window->receivers;
	pReceiverEntry receiverEntry;
	pMenu menu = window->menu;
	pKey keys = menu->keys;
	
	G_endKey = menu->keys + menu->numKeys;

	for (keys = menu->keys; keys < G_endKey && !updateReceivers; keys++)
	{
		if (lastKey == keys->keycode)
		{
			ProcessInput (keys, menu, kMenu, (long) window);

			updateReceivers = TRUE;
		}
	}

	G_endRecEntry = receivers->receivers + receivers->numReceivers;

	if (updateReceivers)
	{
		for (receiverEntry = receivers->receivers; receiverEntry < G_endRecEntry; receiverEntry++)
		{
			BroadcastMessage (parentWindow->windows + receiverEntry->windowID, receiverEntry->instruction, menu->itemChosen);
		}
	}
}

/********************************************************************
*	SendMessageBoxInformation - Send information to a message box	*
********************************************************************/

static void SendMessageBoxInformation (ParentWindow * parentWindow, Objects * objects)
{
	int lastKey = GetLastKeyCode (&objects->inputObj);
	pWindow window = GETFOCUS(parentWindow);
	HorzScroll horzMode = kHorzFix;
	VertScroll vertMode = kVertFix;

	SCROLL(window,horzMode,vertMode,lastKey);

	if (lastKey)
	{
		window->IO->writeMode = kWriteAll;
	}
}

/********************************************************************
*	BroadcastMessage - Primary wrapper for updating receivers		*
********************************************************************/

static void BroadcastMessage (Window * window, int instruction, int index)
{
	switch (window->mode)	// Get the window mode
	{
	case kBasic:			// Basic case
		BroadcastBasicMessage (window, instruction, index);

		break;	// Break out of switch statement

	case kEditBox:			// Edit box case
		BroadcastEditBoxMessage (window, instruction, index);

		break;	// Break out of switch statement

	case kMenu:				// Menu case
		BroadcastMenuMessage (window, instruction, index);

		break;	// Break out of switch statement

	case kMessageBox:		// Message box case
		BroadcastMessageBoxMessage (window, instruction, index);

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported mode: BroadcastMessage failed","1");
		// Return failure
	}
}

/********************************************************************
*	BroadcastBasicMessage - Update a basic receiver					*
********************************************************************/

static void BroadcastBasicMessage (Window * window, BasicBroadcast instruction, int index)
{
	pVisuals visuals = window->visuals;
	pAnimation animation;
	pPattern pattern;

	G_endAnimation = visuals->animations + visuals->numAnimations;
	G_endPattern = visuals->patterns + visuals->numPatterns;

	switch (instruction)
	{
	case kDecState:
		for (animation = visuals->animations; animation < G_endAnimation; animation++)
		{
			SetCurrentState (animation, animation->curState - 1);
		}

		break;	// Break out of switch statement

	case kIncState:
		for (animation = visuals->animations; animation < G_endAnimation; animation++)
		{
			SetCurrentState (animation, animation->curState + 1);
		}

		break;	// Break out of switch statement

	case kIndexState:
		for (animation = visuals->animations; animation < G_endAnimation; animation++)
		{
			SetCurrentState (animation, index);
		}

		break;	// Break out of switch statement

	case kDecFrame:
		for (animation = visuals->animations; animation < G_endAnimation; animation++)
		{
	//		SetCurrentFrame (animation, (float) (*(animation->curFrame + animation->curState) - 1.0));
		}

		break;	// Break out of switch statement

	case kIncFrame:
		for (animation = visuals->animations; animation < G_endAnimation; animation++)
		{
	//		SetCurrentFrame (animation, (float) (*(animation->curFrame + animation->curState) + 1.0));
		}

		break;	// Break out of switch statement

	case kIndexFrame:
		for (animation = visuals->animations; animation < G_endAnimation; animation++)
		{
	//		SetCurrentFrame (animation, (float) index);
		}

		break;	// Break out of switch statement

	case kResetState:
		for (animation = visuals->animations; animation < G_endAnimation; animation++)
		{
			SetCurrentFrame (animation, 0.0);
		}

		break;	// Break out of switch statement

	case kSwitchPatternLock:
		for (pattern = visuals->patterns; pattern < G_endPattern; pattern++)
		{
			pattern->lock = !pattern->lock;
		}

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported instruction: BroadcastBasicMessage failed","1");
		// Return failure
	}
}

/********************************************************************
*	BroadcastEditBoxMessage - Update an edit box receiver			*
********************************************************************/

static void BroadcastEditBoxMessage (Window * window, EditBoxBroadcast instruction, int index)
{
	intStar readMode = &window->IO->readMode;

	switch (instruction)
	{
	case kAddCharacter:
		break;	// Break out of switch statement

	case kRemoveCharacter:
		break;	// Break out of switch statement

	case kSaveMessage:
		SaveMessage (window);

		break;	// Break out of switch statement

	case kDecReadMode:
		*readMode = (*readMode ? *readMode : kReadModesCount) - 1;

		break;	// Break out of switch statement

	case kIncReadMode:
		*readMode = (*readMode + 1) % kReadModesCount;

		break;	// Break out of switch statement

	case kIndexReadMode:
		*readMode = index % kReadModesCount;

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported instruction: BroadcastEditBoxMessage failed","1");
		// Return failure
	}
}

/********************************************************************
*	BroadcastMenuMessage - Update a menu receiver					*
********************************************************************/

static void BroadcastMenuMessage (Window * window, MenuBroadcast instruction, int index)
{
	switch (instruction)
	{
	case kPreviousItem:
		MoveToPreviousItem (window->menu, kMenu, (long) window);

		break;	// Break out of switch statement

	case kNextItem:
		MoveToNextItem (window->menu, kMenu, (long) window);

		break;	// Break out of switch statement

	case kToggleLeft:
		ToggleMenuItemLeft (window->menu, kMenu, (long) window);

		break;	// Break out of switch statement

	case kToggleRight:
		ToggleMenuItemRight (window->menu, kMenu, (long) window);

		break;	// Break out of switch statement

	case kToggleLeftAll:
		break;	// Break out of switch statement

	case kToggleRightAll:
		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported instruction: BroadcastMenuMessage failed","1");
		// Return failure
	}
}

/********************************************************************
*	BroadcastMessageBoxMessage - Update a message box receiver		*
********************************************************************/

static void BroadcastMessageBoxMessage (Window * window, MessageBoxBroadcast instruction, int index)
{
	intStar writeMode = &window->IO->writeMode;
 
	switch (instruction)
	{
	case kPreviousMessage:
		break;	// Break out of switch statement

	case kNextMessage:
		break;	// Break out of switch statement

	case kIndexMessage:
		break;	// Break out of switch statement

	case kDecWriteMode:
		*writeMode = (*writeMode ? *writeMode : kWriteModesCount) - 1;

		break;	// Break out of switch statement

	case kIncWriteMode:
		*writeMode = (*writeMode + 1) % kWriteModesCount;

		break;	// Break out of switch statement

	case kIndexWriteMode:
		*writeMode = index % kWriteModesCount;

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported instruction: BroadcastMessageBoxMessage failed","1");
		// Return failure
	}
}

/********************************************************************
*	UpdateWindowContents - Wrapper for updating a Window structure	*
********************************************************************/

static void UpdateWindowContents (Window * window, Objects * objects, voidStar data)
{
	switch (window->mode)	// Get the window mode
	{
	case kBasic:			// Basic case
		UpdateBasicWindow (window, objects, data);

		break;	// Break out of switch statement

	case kEditBox:			// Edit box case
		UpdateEditBoxWindow (window, objects, data);

		break;	// Break out of switch statement

	case kMenu:				// Menu case
		UpdateMenuWindow (window, objects, data);

		break;	// Break out of switch statement

	case kMessageBox:		// Message box case
		UpdateMessageBox (window, objects, data);

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported mode: UpdateWindowContents failed","1");
		// Return failure
	}

 	DrawWindow (window, objects);
}

/********************************************************************
*	UpdateBasicWindow - Update a basic Window structure				*
********************************************************************/

static void UpdateBasicWindow (Window * window, Objects * objects, voidStar data)
{
	int i;
	int pitch;

	pitch = window->width - window->windowWidth;

/*	G_cell*/G_bufCell = window->display.buffer + (window->yOffset * window->width) + window->xOffset;

	if (window->visuals)	// Ensure that window's visuals field points to something
	{
		pAnimation animation;
		pImage image;
		pPattern pattern;

		for (i = 0; i < window->windowHeight; i++)
		{
/*			for (G_endCell = G_cell + window->windowWidth; G_cell < G_endCell; G_cell++)
			{
				G_cell->Char.AsciiChar = NORMALBACKGROUND;
				G_cell->Attributes = window->background;
			}

			G_cell += pitch;*/
			for (G_end = G_bufCell + window->windowWidth; G_bufCell < G_end; G_bufCell++)
			{
				G_bufCell->graph.Char.AsciiChar = NORMALBACKGROUND;
				G_bufCell->graph.Attributes = window->background;
			}

			G_bufCell += pitch;
		}

		G_endPattern = window->visuals->patterns + window->visuals->numPatterns;

		for (pattern = window->visuals->patterns; pattern < G_endPattern; pattern++)
		{
			DisplayPatternToWindow (pattern, window);
		}

		G_endImage = window->visuals->images + window->visuals->numImages;

		for (image = window->visuals->images; image < G_endImage; image++)
		{
			DisplayImageToWindow (image, window);
		}

		G_endAnimation = window->visuals->animations + window->visuals->numAnimations;

		for (animation = window->visuals->animations; animation < G_endAnimation; animation++)
		{
			DisplayAnimationToWindow (animation, window);
		}
	}
}

/********************************************************************
*	UpdateEditBoxWindow - Update an edit box Window structure		*
********************************************************************/

static void UpdateEditBoxWindow (Window * window, Objects * objects, voidStar data)
{
/*	PCHAR_INFO cell = GETCURSORCELL(window);
	String CursorChar = &cell->Char.AsciiChar;

	*CursorChar = (*CursorChar == UNDERSCORE) ? SPACE : UNDERSCORE;*/
	String CursorChar;

	G_bufCell = GETCURSORCELL(window);

	CursorChar = &G_bufCell->graph.Char.AsciiChar;

	*CursorChar = (*CursorChar == UNDERSCORE) ? SPACE : UNDERSCORE;
}

/********************************************************************
*	UpdateMenuWindow - Update a menu Window structure				*
********************************************************************/

static void UpdateMenuWindow (Window * window, Objects * objects, voidStar data)
{
}

/********************************************************************
*	UpdateMessageBox - Update a message box Window structure		*
********************************************************************/

static void UpdateMessageBox (Window * window, Objects * objects, voidStar data)
{
	pIO_Descriptor IO = window->IO;

	if (IO->text.position == IO->text.numChars)
	{
		return;
	}

	switch (IO->writeMode)
	{
	case kWriteAll:
		while (IO->text.position + 1 != IO->text.numChars)
		{
			WriteMessageWord (window, IO);
		}

		break;	// Break out of switch statement

	case kWriteWord:
		WriteMessageWord (window, IO);

		break;	// Break out of switch statement

	case kWriteCharacter:
		WriteMessageCharacter (window, IO);

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported mode: UpdateMessageBox failed","1");
		// Return failure
	}
}

/********************************************************************
*	RepositionWindows - Reposition a ParentWindow structure			*
********************************************************************/

static void RepositionWindows (ParentWindow * parentWindow, Objects * objects)
{
	int xDelta, yDelta;
	int left = parentWindow->location.X;
	int right = left + parentWindow->width;
	int top = parentWindow->location.Y;
	int bottom = top + parentWindow->height;

	xDelta = GetMouseXPos (&objects->inputObj) - (left + parentWindow->grabPoints.X);
	yDelta = GetMouseYPos (&objects->inputObj) - (top + parentWindow->grabPoints.Y);

	if (xDelta < 0)
	{
		parentWindow->location.X = left + xDelta >= 0 ? left + xDelta : 0;
	}

	else
	{
		parentWindow->location.X = right + xDelta < SCREEN_WIDTH ? left + xDelta : SCREEN_WIDTH - parentWindow->width;
	}
	
	if (yDelta < 0)
	{
		parentWindow->location.Y = top + yDelta >= 0 ? top + yDelta : 0;
	}

	else
	{
		parentWindow->location.Y = bottom + yDelta < SCREEN_HEIGHT ? top + yDelta : SCREEN_HEIGHT - parentWindow->height;
	}	
}

/********************************************************************
*	LoadHotKey - Load a hotkey value from a window cell				*
********************************************************************/

static void LoadHotKey (ParentWindow * parentWindow, Objects * objects)
{
	int offset;
	int left;
	int top;
	int right;
	int bottom;
	int mouseX, mouseY;
	int windowX, windowY;
	pWindow window;

	mouseX = GetMouseXPos (&objects->inputObj);
	mouseY = GetMouseYPos (&objects->inputObj);

	windowX = parentWindow->location.X;
	windowY = parentWindow->location.Y;

	G_endWindow = parentWindow->windows + parentWindow->numWindows;

	for (window = parentWindow->windows; window < G_endWindow; window++)
	{
		left = windowX + window->windowCoord.X;
		right = left + window->windowWidth;
		top = windowY + window->windowCoord.Y;
		bottom = top + window->windowHeight;

		if (mouseX >= left && mouseX < right && mouseY >= top && mouseY < bottom)
		{
			offset = (mouseY - top) * window->width + (mouseX - left);

//			if (FLAGSET(*(window->display.flags + offset),HOTKEY))
			if (FLAGSET((window->display.buffer + offset)->flags,HOTKEY))
			{
				*(objects->inputObj.information->infoItems + window->data) = (window->display.buffer + offset)->data;//*(window->display.data + offset);
			}

			break;
		}
	}
}

/********************************************************************
*	RedrawWindows - Draw a ParentWindow and its components			*
********************************************************************/

void RedrawWindows (ParentWindow * parentWindow, Objects * objects)
{
	int i;								// Loop variables
	int left = parentWindow->location.X;
	int top = parentWindow->location.Y;
	int pitch;
	PBYTE backData;
	BYTE check;
	pWindow window;

	G_cell = *(objects->outputObj.outputBuf.buffer + top) + left;
	backData = parentWindow->backData;

	pitch = (SCREEN_WIDTH - parentWindow->width);

	for (i = 0; i < parentWindow->height; i++)
	{
		for (G_endCell = G_cell + parentWindow->width; G_cell < G_endCell; G_cell++)
		{
			check = G_cell->Char.AsciiChar = *backData++;

			if (check)
			{
				G_cell->Attributes = parentWindow->border;
			}

			else
			{
				G_cell->Attributes = parentWindow->background;
			}
		}

		G_cell += pitch;
	}

	G_endWindow = parentWindow->windows + parentWindow->numWindows;

	for (window = parentWindow->windows; window < G_endWindow; window++)
	{
		DrawWindow (window, objects);
	}
}

/********************************************************************
*	DrawWindow - Draw the contents of a Window structure			*
********************************************************************/

static void DrawWindow (Window * window, Objects * objects)
{
	int i;				// Loop variables
	int windowIndex;	// Index into a given window's buffer	
	int pitch, winPitch;
	
	// Initialization block
	{
		int xCoord, yCoord;	// Screen coordinates of window

		xCoord = G_location->X + window->windowCoord.X;
		yCoord = G_location->Y + window->windowCoord.Y;

		pitch = SCREEN_WIDTH - window->windowWidth;
		winPitch = window->width - window->windowWidth; 

		windowIndex = window->yOffset * window->width + window->xOffset;
		G_cell = *(objects->outputObj.outputBuf.buffer + yCoord) + xCoord;

		G_bufCell = window->display.buffer + windowIndex;
	}

	for (i = 0; i < window->windowHeight; i++)
	{
/*		for (G_endCell = G_cell + window->windowWidth; G_cell < G_endCell; windowIndex++, G_cell++)
		{
			*G_cell = *(window->display.buffer + windowIndex);
		}

		windowIndex += winPitch;
		G_cell += pitch;*/
		for (G_end = G_bufCell + window->windowWidth; G_bufCell < G_end; G_bufCell++)
		{
			*G_cell = G_bufCell->graph;
		}

		G_cell += pitch;
		G_bufCell += winPitch;
	}
}
 
/********************************************************************
*	SwitchFocus - Assign focus to next applicable Window structure	*
********************************************************************/

static void SwitchFocus (ParentWindow * parentWindow)
{
	int i;	// Loop variable

	for (i = parentWindow->focus + 1; i < parentWindow->numWindows; i++)
	{
		if (FLAGSET((parentWindow->windows + i)->state,CANRECEIVEFOCUS))
		{
			ReleaseFocus (GETFOCUS(parentWindow));

			AssignFocus (parentWindow, i);

			return;
		}
	}

	if (parentWindow->focus)
	{
		for (i = 0; i < parentWindow->focus; i++)
		{
			if (FLAGSET((parentWindow->windows + i)->state,CANRECEIVEFOCUS))
			{
				ReleaseFocus (GETFOCUS(parentWindow));

				AssignFocus (parentWindow, i);

				return;
			}
		}
	}
}

/********************************************************************
*	WriteMessageWord - Write a word to a message box				*
********************************************************************/

static void WriteMessageWord (Window * window, IO_Descriptor * IO)
{
	int numSpaces;
	int wrapBound, length;
	String text, word;

	IO->text.length = 0;

/*	G_cell*/G_bufCell = GETCURSORCELL(window);

	text = IO->text.text + IO->text.position;
	word = IO->text.word;
		
	numSpaces = SkipSpaces (text, 0);

//	for (G_endCell = G_cell + numSpaces; G_cell < G_endCell; G_cell++)
	for (G_end = G_bufCell + numSpaces; G_bufCell < G_end; G_bufCell++)
	{
	/*	G_cell*/G_bufCell->graph.Char.AsciiChar = SPACE;

		AdvanceCursor (window, 1);
		IO->text.position++;
		text++;
	}

	while (!isspace (*text) && *text)
	{
		*word++ = *text++;
		IO->text.length++;
		IO->text.position++;
	}

	*word = END;

	wrapBound = IO->cursor % window->width;

	if (wrapBound + IO->text.length >= window->width)
	{
		length = window->width - wrapBound;

		AdvanceCursor (window, length);
	}

	WriteText (&window->display, IO->text.word, IO->cursor);

	AdvanceCursor (window, IO->text.length);
}

/********************************************************************
*	WriteMessageCharacter - Write a character to a message box		*
********************************************************************/

static void WriteMessageCharacter (Window * window, IO_Descriptor * IO)
{
	int wrapBound, length;
	String text, check;

/*	G_cell*/G_bufCell = GETCURSORCELL(window);

	text = IO->text.text + IO->text.position;

	if (isspace (*text))
	{
	/*	G_cell*/G_bufCell->graph.Char.AsciiChar = SPACE;

		AdvanceCursor (window, 1);
		IO->text.position++;
	}

	else
	{
		if (!IO->text.length)
		{
			check = text;

			while (!isspace (*check) && *check)
			{
				IO->text.length++;

				check++;
			}

			wrapBound = IO->cursor % window->width;

			if (wrapBound + IO->text.length >= window->width)
			{
				length = window->width - wrapBound;
				IO->text.limit += length;

				AdvanceCursor (window, length);

			/*	G_cell*/G_bufCell = GETCURSORCELL(window);
			}
		}

	/*	G_cell*/G_bufCell->graph.Char.AsciiChar = *text;

		AdvanceCursor (window, 1);
		IO->text.position++;
		
		IO->text.length--;
	}
}

/********************************************************************
*	HighlightMenuItem - Highlights a menu item						*
********************************************************************/

static void HighlightMenuItem (Window * window, int index)
{
	int i;
	int pitch;
	BYTE value;
	pMenuItem menuItem = window->menu->menu + index;

	value = menuItem->highlight;

	pitch = window->width - menuItem->width;

/*	G_cell*/G_bufCell = window->display.buffer + (menuItem->location.Y * window->width) + menuItem->location.X;

	for (i = 0; i < menuItem->height; i++)
	{
/*		for (G_endCell = G_cell + menuItem->width; G_cell < G_endCell; G_cell++)
		{
			G_cell->Attributes = value;
		}

		G_cell += pitch;*/
		for (G_end = G_bufCell + menuItem->width; G_bufCell < G_end; G_bufCell++)
		{
			G_bufCell->graph.Attributes = value;
		}

		G_bufCell += pitch;
	}
}

/********************************************************************
*	RemoveHighlight - Remove a menu item's highlight				*
********************************************************************/

static void RemoveHighlight (Window * window, int index)
{
	int i;
	int pitch;
	BYTE value;
	pMenuItem menuItem = window->menu->menu + index;

	value = window->background;

	pitch = window->width - menuItem->width;

/*	G_cell*/G_bufCell = window->display.buffer + (menuItem->location.Y * window->width) + menuItem->location.X;

	for (i = 0; i < menuItem->height; i++)
	{
/*		for (G_endCell = G_cell + menuItem->width; G_cell < G_endCell; G_cell++)
		{
			G_cell->Attributes = value;
		}

		G_cell += pitch;*/
		for (G_end = G_bufCell + menuItem->width; G_bufCell < G_end; G_bufCell++)
		{
			G_bufCell->graph.Attributes = value;
		}

		G_bufCell += pitch;
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Interface navigation					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	ScrollWindows - Scroll the contents of the given window			*
********************************************************************/

void ScrollWindows (Window * window, HorzScroll horzMode, VertScroll vertMode)
{
	assert (window);
	// Verify that window points to valid memory

	switch (horzMode)	// Get the horizontal scroll mode
	{
	case kLeft:			// Left case
		if (FLAGSET(window->state,XSCROLLABLE) && window->xOffset)
		{
			DecXOffset (window);
			// Scroll window to left
		}

		break;	// Break out of switch statement

	case kRight:		// Right case
		if (FLAGSET(window->state,XSCROLLABLE) && window->xOffset + window->windowWidth <= window->width)
		{
			IncXOffset (window);
			// Scroll window to right
		}

		break;	// Break out of switch statement

	case kHorzFix:		// Fixed case
		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported horzMode: ScrollWindow failed","1");
		// Return failure
	}

	switch (vertMode)	// Get the vertical scroll mode
	{
	case kUp:			// Up case
		if (FLAGSET(window->state,YSCROLLABLE) && window->yOffset)
		{
			DecYOffset (window);
			// Scroll window up 
		}

		break;	// Break out of switch statement

	case kDown:			// Down case
		if (FLAGSET(window->state,YSCROLLABLE) && window->yOffset + window->windowHeight <= window->height)
		{
			IncYOffset (window);
			// Scroll window down
		}

		break;	// Break out of switch statement

	case kVertFix:		// Fixed case
		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported vertMode: ScrollWindow failed","2");
		// Return failure
	}
}

/********************************************************************
*	MoveToPreviousItem - Regress to the previous menu item			*
********************************************************************/

int MoveToPreviousItem (voidStar data, dataType type, long lParam)
{
	pMenu menu = (pMenu) data;
	pWindow window = (pWindow) lParam;
	pMenuItem menuItem;
	intStar itemChosen = &menu->itemChosen;

	menuItem = GETMENUITEM(menu);

	RemoveHighlight (window, *itemChosen);

	*itemChosen = (*itemChosen ? *itemChosen : menu->numItems) - 1;

	HighlightMenuItem (window, *itemChosen);

	return 0;
}

/********************************************************************
*	MoveToNextItem - Advance to the next menu item					*
********************************************************************/

int MoveToNextItem (voidStar data, dataType type, long lParam)
{
	pMenu menu = (pMenu) data;
	pWindow window = (pWindow) lParam;
	pMenuItem menuItem;
	intStar itemChosen = &menu->itemChosen;

	menuItem = GETMENUITEM(menu);

	RemoveHighlight (window, *itemChosen);

	*itemChosen = (*itemChosen + 1) % menu->numItems;

	HighlightMenuItem (window, *itemChosen);

	return 0;
}

/********************************************************************
*	ToggleMenuItemLeft - Toggle the chosen menu item's entry left	*
********************************************************************/

int ToggleMenuItemLeft (voidStar data, dataType type, long lParam)
{
	int offset;
	pMenuItem menuItem = GETMENUITEM((pMenu) data);
	pWindow window = (pWindow) lParam;
	intStar entry = &menuItem->entry;
	
	// Initialization block
	{
		int length = strlen (*GETMENUITEMTEXT(menuItem));
		
		offset = menuItem->location.Y * window->width + menuItem->location.X;
	
		ClearText (&window->display, offset, length);
	}

	*entry = (*entry + 1) % menuItem->numItems;

	WriteText (&window->display, *GETMENUITEMTEXT(menuItem), offset);

	return 0;
}

/********************************************************************
*	ToggleMenuItemRight - Toggle the chosen menu item's entry right	*
********************************************************************/

int ToggleMenuItemRight (voidStar data, dataType type, long lParam)
{
	int offset;
	pMenuItem menuItem = GETMENUITEM((pMenu) data);
	pWindow window = (pWindow) lParam;
	intStar entry = &menuItem->entry;
	
	// Initialization block
	{
		int length = strlen (*GETMENUITEMTEXT(menuItem));
		
		offset = menuItem->location.Y * window->width + menuItem->location.X;
	
		ClearText (&window->display, offset, length);
	}

	*entry = (*entry ? *entry : menuItem->numItems) - 1;

	WriteText (&window->display, *GETMENUITEMTEXT(menuItem), offset);

	return 0;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Interface IO							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	LoadVisuals - Load visual data into a basic window				*
********************************************************************/

/********************************************************************
*	LoadMessage - Load a message into a message box					*
********************************************************************/

/********************************************************************
*	LoadMenuItems - Load menu items into a menu						*
********************************************************************/

/********************************************************************
*	SaveMessage - Save the message in the edit box					*
********************************************************************/

void SaveMessage (Window * window)
{
	pIO_Descriptor IO;
	voidStar data;
	dataType type;

	assert (window);
	// Verify that window points to valid memory

	if (!window->dataADT.data.data || window->mode != kEditBox)	// Ensure that window's dataADT field's data field's data field points to something and that window's mode is edit box
	{
		return;
	}

	IO = window->IO;

	switch (IO->readMode)
	{
	case kReadAll:
	case kAlphaOnly:
	case kPrintOnly:
		// Save block
		{
			String string;

			CALLOC(string,strlen (IO->text.text),char);

			strcpy (string, IO->text.text);

			data = string;
			type = kChar;
		}

		break;	// Break out of switch statement

	case kNumeric:
		// Save block
		{
			intStar number;

			MALLOC(number,int);

			*number = atoi (IO->text.text);

			data = number;
			type = kChar;
		}

		break;	// Break out of switch statement
	}

	switch (window->dataADT.type)
	{
	case kList:
		AppendListNode (window->dataADT.data.list, data, type);

		break;	// Break out of switch statement

	case kQueue:
		EnqueueQueueNode (window->dataADT.data.queue, data, type);

		break;	// Break out of switch statement

	case kStack:
		PushStackNode (window->dataADT.data.stack, data, type);

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported ADTStyle: SaveMessage failed","1");
		// Return failure
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Destruction								*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	ReleaseWindow - Clean up a Window once it is finished with		*
********************************************************************/
