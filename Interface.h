/********************************************************************
*																	*
*							Interface.h								*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains information relevant to interface			*
*																	*
********************************************************************/

#ifndef INTERFACE_H
#define INTERFACE_H

#include "Common.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Macros									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define SCROLL(window,horzMode,vertMode,input)	\
	switch (input)								\
	{											\
	case VK_LEFT:								\
		horzMode = kLeft; break;				\
	case VK_RIGHT:								\
		horzMode = kRight; break;				\
	case VK_UP:									\
		vertMode = kUp; break;					\
	case VK_DOWN:								\
		vertMode = kDown; break;				\
	}											\
												\
	ScrollWindows (window, horzMode, vertMode);
// Used to send scroll messages to windows

#define GETFOCUS(parentWindow)					\
	((parentWindow)->windows + (parentWindow)->focus)
// Used to retrieve the window with focus

#define GETCURSORCELL(window)					\
	((window)->display.buffer + (window)->IO->cursor)
#define GETCURSORTEXT(window)					\
	((window)->IO->text.text + (window)->IO->cursor)
// Used to retrieve information denoted by the cursor

#define GETMENUITEM(menus)						\
	((menus)->menu + (menus)->itemChosen)
// Used to retrieve the chosen menu item

#define GETMENUITEMTEXT(menuItem)				\
	((menuItem)->text + (menuItem)->entry)
#define GETMENUITEMLPARAM(menuItem)				\
	((menuItem)->lParams + (menuItem)->entry)
// Used to retrieve the current information in a menu item

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Defines									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define HASFOCUS			0x1
// Denotes that a Window structure has the focus
#define ACTIVEWITHOUTFOCUS	0x2
// Denotes that a Window structure should update even without focus
#define SAVEWITHOUTFOCUS	0x4
// Denotes that a Window structure should save information even without focus
#define CANRECEIVEFOCUS		0x8
// Denotes that a Window structure can receive the focus
#define XSCROLLABLE			0x10
// Denotes that a Window's width exceeds its display width
#define YSCROLLABLE			0x20
// Denotes that a Window's height exceeds its display height

#define WINDOWSACTIVE		0x1
// Denotes that a ParentWindow structure is active
#define GRABBED				0x2
// Denotes that a ParentWindow structure has been grabbed
#define FIXED				0x4
// Denotes that a Window structure is immovable

#define TOPEDGE				0x0
// A portion of a window that is occupied by the top border
#define BOTTOMEDGE			0x1
// A portion of a window that is occupied by the bottom border
#define LEFTEDGE			0x2
// A portion of a window that is occupied by the left border
#define RIGHTEDGE			0x3
// A portion of a window that is occupied by the right border
#define HORZSEPARATOR		0x4
// A portion of a window that is occupied by a horizontal separator
#define VERTSEPARATOR		0x5
// A portion of a window that is occupied by a vertical separator

#define NORMALBACKGROUND  '\x00'
// Character used as default background

#define TOPLEFTBORDER	  '\xC9'
#define TOPRIGHTBORDER	  '\xBB'
#define BOTTOMLEFTBORDER  '\xC8'
#define BOTTOMRIGHTBORDER '\xBC'
// Characters used to form the corners of output buffers used to
// compose windows

#define LEFTTORIGHTBORDER '\xCD'
#define UPTODOWNBORDER	  '\xBA'
// Characters used to form the horizontal and vertical borders of
// output buffers used to compose windows

#define LEFTSEPARATOR	  '\xCC'
#define RIGHTSEPARATOR	  '\xB9'
#define LOWSEPARATOR	  '\xCA'
#define HIGHSEPARATOR	  '\xCB'
// Characters used to smoothly join separators with the borders of
// output buffers used to compose windows

#define JOINEDSEPARATORS  '\xCE'
// Character used to smoothly join intersecting separators

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Interface access functions				*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	AdvanceCursor - Advance a Window structure's read/write cursor	*
********************************************************************/

static void AdvanceCursor (pWindow window, int amount);

/********************************************************************
*	RegressCursor - Regress a Window structure's read/write cursor	*
********************************************************************/

static void RegressCursor (pWindow window, int amount);

/********************************************************************
*	AssignFocus - Assign focus to a Window structure				*
********************************************************************/

static void AssignFocus (pParentWindow parentWindow, int index);

/********************************************************************
*	ReleaseFocus - Release Window structure's focus					*
********************************************************************/

static void ReleaseFocus (pWindow window);

/********************************************************************
*	HasFocus - Indicates if a Window structure has the focus		*
********************************************************************/

static BOOL HasFocus (Window const * window);

/********************************************************************
*	ActivateWindows - Set a ParentWindow structure to active		*
********************************************************************/

void ActivateWindows (pParentWindow parentWindow);

/********************************************************************
*	DeactivateWindows - Set a ParentWindow structure to inactive	*
********************************************************************/

void DeactivateWindows (pParentWindow parentWindow);

/********************************************************************
*	WindowsActive - Indicates if a ParentWindow structure is active	*
********************************************************************/

BOOL WindowsActive (ParentWindow const * parentWindow);

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

void MakeParentWindow (pParentWindow parentWindow, int x, int y, int focus);

/********************************************************************
*	InitParentWindow - Initialize a parent window					*
********************************************************************/

static void InitParentWindow (pParentWindow parentWindow);

/********************************************************************
*	InitMenu - Initialize a menu associated with a Window object	*
********************************************************************/

static void InitMenu (pWindow window);

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

static void GrabWindow (pParentWindow parentWindow, pObjects objects);

/********************************************************************
*	Update - Update a ParentWindow structure						*
********************************************************************/

void Update (pParentWindow parentWindow, pObjects objects, voidStar data);

/********************************************************************
*	UpdateWindowContents - Wrapper for updating a Window structure	*
********************************************************************/

static void UpdateWindowContents (pWindow window, pObjects objects, voidStar data);

/********************************************************************
*	RepositionWindows - Reposition a ParentWindow structure			*
********************************************************************/

static void RepositionWindows (pParentWindow parentWindow, pObjects objects);

/********************************************************************
*	SendInformation - Primary wrapper for updating the focus		*
********************************************************************/

static void SendInformation (pParentWindow parentWindow, pObjects objects);

/********************************************************************
*	SendBasicInformation - Send information to a basic window		*
********************************************************************/

static void SendBasicInformation (pParentWindow parentWindow, pObjects objects);

/********************************************************************
*	SendEditBoxInformation - Send information to an edit box		*
********************************************************************/

static void SendEditBoxInformation (pParentWindow parentWindow, pObjects objects);

/********************************************************************
*	SendMenuInformation	- Send information to a menu				*
********************************************************************/

static void SendMenuInformation (pParentWindow parentWindow, pObjects objects);

/********************************************************************
*	SendMessageBoxInformation - Send information to a message box	*
********************************************************************/

static void SendMessageBoxInformation (pParentWindow parentWindow, pObjects objects);

/********************************************************************
*	BroadcastMessage - Primary wrapper for updating receivers		*
********************************************************************/

static void BroadcastMessage (pWindow window, int instruction, int index);

/********************************************************************
*	BroadcastBasicMessage - Update a basic receiver					*
********************************************************************/

static void BroadcastBasicMessage (pWindow window, BasicBroadcast instruction, int index);

/********************************************************************
*	BroadcastEditBoxMessage - Update an edit box receiver			*
********************************************************************/

static void BroadcastEditBoxMessage (pWindow window, EditBoxBroadcast instruction, int index);

/********************************************************************
*	BroadcastMenuMessage - Update a menu receiver					*
********************************************************************/

static void BroadcastMenuMessage (pWindow window, MenuBroadcast instruction, int index);

/********************************************************************
*	BroadcastMessageBoxMessage - Update a message box receiver		*
********************************************************************/

static void BroadcastMessageBoxMessage (pWindow window, MessageBoxBroadcast instruction, int index);

/********************************************************************
*	LoadHotKey - Load a hotspot value from a window cell			*
********************************************************************/

static void LoadHotKey (pParentWindow parentWindow, pObjects objects);

/********************************************************************
*	RedrawWindows - Draw a ParentWindow and its components			*
********************************************************************/

void RedrawWindows (pParentWindow parentWindow, pObjects objects);

/********************************************************************
*	DrawWindow - Draw the contents of a Window structure			*
********************************************************************/

static void DrawWindow (pWindow window, pObjects objects);

/********************************************************************
*	UpdateBasicWindow - Update a basic Window structure				*
********************************************************************/

static void UpdateBasicWindow (pWindow window, pObjects objects, voidStar data);

/********************************************************************
*	UpdateEditBoxWindow - Update an edit box Window structure		*
********************************************************************/

static void UpdateEditBoxWindow (pWindow window, pObjects objects, voidStar data);

/********************************************************************
*	UpdateMenuWindow - Update a menu Window structure				*
********************************************************************/

static void UpdateMenuWindow (pWindow window, pObjects objects, voidStar data);

/********************************************************************
*	UpdateMessageBox - Update a message box Window structure		*
********************************************************************/

static void UpdateMessageBox (pWindow window, pObjects objects, voidStar data);

/********************************************************************
*	SwitchFocus - Assign focus to next applicable Window structure	*
********************************************************************/

static void SwitchFocus (pParentWindow parentWindow);

/********************************************************************
*	HighlightMenuItem - Highlights a menu item						*
********************************************************************/

static void HighlightMenuItem (pWindow window, int index);

/********************************************************************
*	RemoveHighlight - Remove a menu item's highlight				*
********************************************************************/

static void RemoveHighlight (pWindow window, int index);

/********************************************************************
*	WriteMessageWord - Write a word to a message box				*
********************************************************************/

static void WriteMessageWord (pWindow window, pIO_Descriptor IO);

/********************************************************************
*	WriteMessageCharacter - Write a character to a message box		*
********************************************************************/

static void WriteMessageCharacter (pWindow window, pIO_Descriptor IO);

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

void ScrollWindows (pWindow window, HorzScroll horzMode, VertScroll vertMode);

/********************************************************************
*	MoveToPreviousItem - Regress to the previous menu item			*
********************************************************************/

int MoveToPreviousItem (voidStar data, dataType type, long lParam);

/********************************************************************
*	MoveToNextItem - Advance to the next menu item					*
********************************************************************/

int MoveToNextItem (voidStar data, dataType type, long lParam);

/********************************************************************
*	ToggleMenuItemLeft - Toggle the chosen menu item's entry left	*
********************************************************************/

int ToggleMenuItemLeft (voidStar data, dataType type, long lParam);

/********************************************************************
*	ToggleMenuItemRight - Toggle the chosen menu item's entry right	*
********************************************************************/

int ToggleMenuItemRight (voidStar data, dataType type, long lParam);

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

void SaveMessage (pWindow window);

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

#endif