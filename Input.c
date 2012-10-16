/********************************************************************
*																	*
*							Input.c									*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains implementation of input tool				*
*																	*
********************************************************************/

#include "Input.h"

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

void SetKeyCode (Key * keyObj, int keycode)
{
	assert (keyObj);
	// Verify that keyObj points to valid memory

	keyObj->keycode = keycode;	// Assign keycode value to the Key structure
}

/********************************************************************
*	GetKeyCode - Returns the Key structure's keycode value			*
*	Input:	Pointer to the Key structure							*
*	Output:	Keycode value											*
********************************************************************/

int GetKeyCode (Key const * keyObj)
{
	assert (keyObj);
	// Verify that keyObj points to valid memory

	return keyObj->keycode;
	// Retrieve keycode value from the Key structure
}

/********************************************************************
*	ProcessInput - Call function associated with Key structure		*
*	Input:	Key structure, context, type of context, information	*
*	Output:	Key structure function is called						*
********************************************************************/

int ProcessInput (Key const * keyObj, voidStar data, dataType type, long lParam)
{
	assert (keyObj && data);
	// Verify that keyObj and data point to valid memory

	return keyObj->function (data, type, lParam);
	// Call function associated with Key structure and return result
}

/********************************************************************
*	GetMouseXPos - Get the mouse's x coordinate						*
*	Input:	An input structure										*
*	Output:	The mouse's x coordinate								*
********************************************************************/

int GetMouseXPos (Input const * inputObj)
{
	assert (inputObj);
	// Verify that inputObj points to valid memory

	return inputObj->mouse.mouseCoord.X;
	// Retrieve mouse's x coordinate
}

/********************************************************************
*	GetMouseYPos - Get the mouse's y coordinate						*
*	Input:	An input structure										*
*	Output:	The mouse's y coordinate								*
********************************************************************/

int GetMouseYPos (Input const * inputObj)
{
	assert (inputObj);
	// Verify that inputObj points to valid memory

	return inputObj->mouse.mouseCoord.Y;
	// Retrieve mouse's y coordinate
}

/********************************************************************
*	MouseWasMoved - Indicates that the mouse has been moved			*
*	Input:	An input structure										*
*	Output:	Mouse movement status									*
********************************************************************/

BOOL MouseWasMoved (Input const * inputObj)
{
	assert (inputObj);
	// Verify that inputObj points to valid memory

	return inputObj->mouse.mouseMoved;
	// Retrieve mouse movement status
}

/********************************************************************
*	MouseWasWheeled - Indicates that the mouse has been wheeled		*
*	Input:	An input structure										*
*	Output:	Mouse wheel status
********************************************************************/

BOOL MouseWasWheeled (Input const * inputObj)
{
	assert (inputObj);
	// Verify that inputObj points to valid memory

	return inputObj->mouse.wheelUsed;
	// Retrieve mouse wheel status
}

/********************************************************************
*	MouseWasDoubleClicked - Indicates that a double-click occurred	*
*	Input: An input structure										*
*	Output: Mouse double-click status								*
********************************************************************/

BOOL MouseWasDoubleClicked (Input const * inputObj)
{
	assert (inputObj);
	// Verify that inputObj points to valid memory

	return inputObj->mouse.doubleClicked;
	// Retrive mouse double-click status
}

/********************************************************************
*	LeftMouseButtonIsDown - Indicates left mouse button status		*
*	Input:	An input structure										*
*	Output:	The left mouse button's status
********************************************************************/

BOOL LeftMouseButtonIsDown (Input const * inputObj)
{
	assert (inputObj);
	// Verify that inputObj points to valid memory

	return inputObj->mouse.leftClicked;
	// Retrieve status of left mouse button
}

/********************************************************************
*	RightMouseButtonIsDown - Indicates right mouse button status	*
*	Input:	An input structure										*
*	Output:	The right mouse button's status							*
********************************************************************/

BOOL RightMouseButtonIsDown (Input const * inputObj)
{
	assert (inputObj);
	// Verify that inputObj points to valid memory

	return inputObj->mouse.rightClicked;
	// Retrieve status of right mouse button
}

/********************************************************************
*	GetLastChar - Retrieve the last character received as input		*
*	Input: An input structure										*
*	Output: The last character received as input					*
********************************************************************/

char GetLastChar (Input const * inputObj)
{
	assert (inputObj);
	// Verify that inputObj points to valid memory

	return inputObj->lastChar;
	// Retreive lastChar value from the Input structure
}

/********************************************************************
*	GetLastKeyCode - Retrieve the last keycode received as input	*
*	Input:	An input structure										*
*	Output:	The last keycode received as input						*
********************************************************************/

int GetLastKeyCode (Input const * inputObj)
{
	assert (inputObj);
	// Verify that inputObj points to valid memory

	return inputObj->lastKeycode;
	// Retrieve lastKeycode value from the Input structure
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Input setup functions					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	SetInputMethod - Assign a method to the input structure			*
*	Input:	An input structure, and the address of a function		*
*	Output:	Input structure is assigned the given method			*
********************************************************************/

void SetInputMethod (Key * keyObj, int (* function) (voidStar data, dataType type, long lParam))
{
	assert (keyObj && function);
	// Verify that keyObj and function point to valid memory

	keyObj->function = function;
	// Assign the given method to the key structure
}

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

BOOL InitializeInputObject (Input * inputObj)
{
	ZeroMemory (inputObj, sizeof (Input));

	inputObj->information = NULL;

	inputObj->inputH = GetStdHandle (STD_INPUT_HANDLE);

	if (inputObj->inputH == INVALID_HANDLE_VALUE)
	{
		ERROR_MESSAGE("InitializeInputObject failed","1");
		// Return failure
	}

	return TRUE;
}

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

int GetInput (Input * inputObj, InputStyle style)
{
	int keyCode;	// Keycode to return as received input

	assert (inputObj);
	// Verify that inputObj points to valid memory

	switch (style)	// Get input style
	{
	case kSync:		// Synchronous case
		keyCode = GetInputSync (inputObj);	
		// Get synchronous input

		break;	// Break out of switch statement

	case kAsync:	// Asynchronous case
		keyCode = GetInputAsync (inputObj);
		// Get asynchronous input

		break;	// Break out of switch statement

	default:
		NOBREAK_MESSAGE("Unsupported input mode: GetInput failed","1");
		keyCode = 0;	// Invalid case

		break;	// Break out of switch statement
	}

	inputObj->lastChar = keyCode ? inputObj->inputRec.Event.KeyEvent.uChar.AsciiChar : END;
	// Store the last character received as input
	inputObj->lastKeycode = keyCode;
	// Store the last keycode received as input

	UpdateMouse (inputObj);
	// Update mouse information

	return keyCode;
	// Return input received
}

/********************************************************************
*	GetInputSync - Retrieve synchronous input						*
*	Input:	An input structure										*
*	Output:	Return appropriate virtual-keycode to process			*
********************************************************************/

static int GetInputSync (Input * inputObj)
{
	if (!ReadConsoleInput (inputObj->inputH, &inputObj->inputRec, 1, &inputObj->Count))
	{
		ERROR_MESSAGE("GetInputSync failed","1");
		// Return failure
	}
	// Read console input

	if (inputObj->inputRec.EventType == KEY_EVENT && inputObj->inputRec.Event.KeyEvent.bKeyDown)
	{
		return inputObj->inputRec.Event.KeyEvent.wVirtualKeyCode;
		// Return keycode if conditions are met
	}

	return 0;
	// Return null input otherwise
}

/********************************************************************
*	GetInputAsync - Retrieve asynchronous input						*
*	Input:	An input structure										*
*	Output:	Return appropriate virtual-key code to process			*
********************************************************************/

static int GetInputAsync (Input * inputObj)
{
	if (!PeekConsoleInput (inputObj->inputH, &inputObj->inputRec, 1, &inputObj->Count))
	{
		ERROR_MESSAGE("GetInputAsync failed","1");
		// Return failure
	}
	// Peek for input

	if (!inputObj->Count)
	{
		return 0;
		// Return null input if no input is available
	}

	return GetInputSync (inputObj);
	// If input is available, return input retrieved by GetInputSync
}

/********************************************************************
*	UpdateMouse - Updates mouse information							*
*	Input:	An input structure										*
*	Output:	General mouse information is updated					*
********************************************************************/

static void UpdateMouse (Input * inputObj)
{
	if (inputObj->inputRec.EventType == MOUSE_EVENT)
	{
		inputObj->mouse.leftClicked = FLAGSET(inputObj->inputRec.Event.MouseEvent.dwButtonState,FROM_LEFT_1ST_BUTTON_PRESSED);
		inputObj->mouse.rightClicked = FLAGSET(inputObj->inputRec.Event.MouseEvent.dwButtonState,RIGHTMOST_BUTTON_PRESSED);
	
		inputObj->mouse.doubleClicked = inputObj->inputRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK;
		inputObj->mouse.wheelUsed = inputObj->inputRec.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED;
		inputObj->mouse.mouseMoved = inputObj->inputRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED;

		inputObj->mouse.mouseCoord = inputObj->inputRec.Event.MouseEvent.dwMousePosition;
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
*	DeinitializeInputObject - Deinitialize an input object			*
********************************************************************/

BOOL DeinitializeInputObject (Input * inputObj)
{
	if (inputObj->information)	// Ensure that inputObj's information field points to something
	{
		FREE(inputObj->information->infoItems);

		FREE(inputObj->information);
	}

	if (!CloseHandle (inputObj->inputH))
	{
		ERROR_MESSAGE("DeinitializeInputObject failed","1");
		// Return failure
	}

	return TRUE;
	// Return success
}