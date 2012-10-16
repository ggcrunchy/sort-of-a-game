/********************************************************************
*																	*
*							Input.h									*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains information relevant to input management	*
*																	*
********************************************************************/

#ifndef INPUT_H
#define INPUT_H

#include "Common.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Input access functions					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	SetKeyCode - Assign a keycode value to the Key structure		*
*	Input:	Pointer to the Key structure, and a keycode value		*
*	Output:	Key structure's keycode field is set					*
********************************************************************/

void SetKeyCode (pKey keyObj, int keycode);

/********************************************************************
*	GetKeyCode - Returns the Key structure's keycode value			*
*	Input:	Pointer to the Key structure							*
*	Output:	Keycode value											*
********************************************************************/

int GetKeyCode (Key const * keyObj);

/********************************************************************
*	ProcessInput - Call function associated with Key structure		*
*	Input:	Key structure, data, type of data, information			*
*	Output:	Key structure function is called						*
********************************************************************/

int ProcessInput (Key const * keyObj, voidStar data, dataType type, long lParam);

/********************************************************************
*	GetMouseXPos - Get the mouse's x coordinate						*
*	Input:	An input structure										*
*	Output:	The mouse's x coordinate								*
********************************************************************/

int GetMouseXPos (Input const * inputObj);

/********************************************************************
*	GetMouseYPos - Get the mouse's y coordinate						*
*	Input:	An input structure										*
*	Output:	The mouse's y coordinate								*
********************************************************************/

int GetMouseYPos (Input const * inputObj);

/********************************************************************
*	MouseWasMoved - Indicates that the mouse has been moved			*
*	Input:	An input structure										*
*	Output:	Mouse movement status									*
********************************************************************/

BOOL MouseWasMoved (Input const * inputObj);

/********************************************************************
*	MouseWasWheeled - Indicates that the mouse has been wheeled		*
*	Input:	An input structure										*
*	Output:	Mouse wheel status
********************************************************************/

BOOL MouseWasWheeled (Input const * inputObj);

/********************************************************************
*	MouseWasDoubleClicked - Indicates that a double-click occurred	*
*	Input: An input structure										*
*	Output: Mouse double-click status								*
********************************************************************/

BOOL MouseWasDoubleClicked (Input const * inputObj);

/********************************************************************
*	LeftMouseButtonIsDown - Indicates left mouse button status		*
*	Input:	An input structure										*
*	Output:	The left mouse button's status
********************************************************************/

BOOL LeftMouseButtonIsDown (Input const * inputObj);

/********************************************************************
*	RightMouseButtonIsDown - Indicates right mouse button status	*
*	Input:	An input structure										*
*	Output:	The right mouse button's status							*
********************************************************************/

BOOL RightMouseButtonIsDown (Input const * inputObj);

/********************************************************************
*	GetLastChar - Retrieve the last character received as input		*
*	Input: An input structure										*
*	Output: The last character received as input					*
********************************************************************/

char GetLastChar (Input const * inputObj);

/********************************************************************
*	GetLastKeyCode - Retrieve the last keycode received as input	*
*	Input:	An input structure										*
*	Output:	The last keycode received as input						*
********************************************************************/

int GetLastKeyCode (Input const * inputObj);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Input setup functions					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	SetInputMethod - Assign a method to the Key structure			*
*	Input:	A Key structure, and the address of a function			*
*	Output:	Key structure is assigned the given method				*
********************************************************************/

void SetInputMethod (pKey keyObj, int (* function) (voidStar data, dataType type, long lParam));

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Construction							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	InitializeInputObject - Initialize primary input object			*
********************************************************************/

BOOL InitializeInputObject (pInput inputObj);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Input retrieval							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	GetInput - Primary wrapper for input							*
*	Input:	An input structure, and the style of input to receive	*
*	Output:	Return appropriate virtual-keycode to process			*
********************************************************************/

int GetInput (pInput inputObj, InputStyle style);

/********************************************************************
*	GetInputSync - Retrieve synchronous input						*
*	Input:	An input structure										*
*	Output:	Return appropriate virtual-keycode to process			*
********************************************************************/

static int GetInputSync (pInput inputObj);

/********************************************************************
*	GetInputAsync - Retrieve asynchronous input						*
*	Input:	An input structure										*
*	Output:	Return appropriate virtual-key code to process			*
********************************************************************/

static int GetInputAsync (pInput inputObj);

/********************************************************************
*	UpdateMouse - Updates mouse information							*
*	Input:	An input structure										*
*	Output:	General mouse information is updated					*
********************************************************************/

static void UpdateMouse (pInput inputObj);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Destruction								*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	DeinitializeInputObject - Deinitialize an input object			*
********************************************************************/

BOOL DeinitializeInputObject (pInput inputObj);

#endif