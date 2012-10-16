#include "User.h"

/********************************************************************
*																	*
*							Globals									*
*																	*
********************************************************************/
/*
static PCHAR_INFO G_endCell;
// Used to quicken looping

static PCHAR_INFO G_cell;
// Used for output
*/
static pCell G_end, G_bufCell;

void InitCharactersAndColorsWindow (ParentWindow * parentWindow, Objects * objects)
{
	int i;
	int area;
	pWindow Chars, fore, back;

 	assert (parentWindow && objects);

	ZeroMemory (parentWindow, sizeof (ParentWindow));

	parentWindow->bufferSharing = kShared;
	ShareScreenBuffer (&parentWindow->back, objects);

	parentWindow->numWindows = 3;

	CALLOC(parentWindow->windows,parentWindow->numWindows,Window);

	if (!parentWindow->windows)
	{
		NORET_MESSAGE("InitCharactersAndColorsWindow failed","1");
		// Return failure
	}

	Chars = parentWindow->windows;
	fore = Chars + 1;
	back = fore + 1;

	parentWindow->width = 18;
	parentWindow->height = 22;

	MALLOC(parentWindow->separators,Separator);

	if (!parentWindow->separators)
	{
		NORET_MESSAGE("InitCharactersAndColorsWindow failed","2");
		// Return failure
	}

	parentWindow->separators->numHorzSeparators = 2;

	CALLOC(parentWindow->separators->horzSeparators,parentWindow->separators->numHorzSeparators,SeparatorEntry);

	if (!parentWindow->separators->horzSeparators)
	{
		NORET_MESSAGE("InitCharactersAndColorsWindow failed","3");
		// Return failure
	}

	parentWindow->separators->horzSeparators->extent = 18;
	(parentWindow->separators->horzSeparators + 1)->extent = 18;

	parentWindow->separators->horzSeparators->offsets.Y = 16;
	(parentWindow->separators->horzSeparators + 1)->offsets.Y = 18;

	parentWindow->background = 0x0;
	parentWindow->border = MAKEBYTE(0x7,0x0);

	CALLOC(parentWindow->backData,parentWindow->width * parentWindow->height,BYTE);

	if (!parentWindow->backData)
	{
		NORET_MESSAGE("InitCharactersAndColorsWindow failed","4");
		// Return failure
	}

	MakeParentWindow (parentWindow, 0, 0, 0);

	Chars->windowCoord.X = Chars->windowCoord.Y = 1;

	Chars->windowWidth = Chars->width = 16;
	Chars->windowHeight = Chars->height = 16;

	area = Chars->width * Chars->height;

	if (!AllocateBuffer (&Chars->display, area))
	{
		NORET_MESSAGE("InitCharactersAndColorsWindow failed","5");
		// Return failure
	}

/*	G_endCell = Chars->display.buffer + area;

	for (i = 0, G_cell = Chars->display.buffer; G_cell < G_endCell; i++, G_cell++)
	{
		G_cell->Char.AsciiChar = (char) i;
		G_cell->Attributes = MAKEBYTE(0xF,0x0);
		SETFLAG(*(Chars->display.flags + i),HOTKEY);
		*(Chars->display.data + i) = i;
	}
*/
	G_end = Chars->display.buffer + area;

	for (i = 0, G_bufCell = Chars->display.buffer; G_bufCell < G_end; i++, G_bufCell++)
	{
		G_bufCell->graph.Char.AsciiChar = (char) i;
		G_bufCell->graph.Attributes = MAKEBYTE(0xF,0x0);
		SETFLAG(G_bufCell->flags,HOTKEY);
		G_bufCell->data = i;
	}

	Chars->data = 0;

	fore->windowCoord.X = 1;
	fore->windowCoord.Y = 18;

	fore->windowWidth = fore->width = 16;
	fore->windowHeight = fore->height = 1;

	area = fore->width * fore->height;

	if (!AllocateBuffer (&fore->display, area))
	{
		NORET_MESSAGE("InitCharactersAndColorsWindow failed","5");
		// Return failure
	}

/*	G_endCell = fore->display.buffer + area;

	for (i = 0, G_cell = fore->display.buffer; G_cell < G_endCell; i++, G_cell++)
	{
		G_cell->Char.AsciiChar = DATA_AND_FLAGS;
		G_cell->Attributes = MAKEBYTE(i,0x0);
		SETFLAG(*(fore->display.flags + i),HOTKEY);
		*(fore->display.data + i) = i;
	}*/
	G_end = fore->display.buffer + area;

	for (i = 0, G_bufCell = fore->display.buffer; G_bufCell < G_end; i++, G_bufCell++)
	{
		G_bufCell->graph.Char.AsciiChar = DATA_AND_FLAGS;
		G_bufCell->graph.Attributes = MAKEBYTE(i,0x0);
		SETFLAG(G_bufCell->flags,HOTKEY);
		G_bufCell->data = i;
	}

	fore->data = 1;

	back->windowCoord.X = 1;
	back->windowCoord.Y = 20;

	back->windowWidth = back->width = 16;
	back->windowHeight = back->height = 1;

	area = back->width * back->height;

	if (!AllocateBuffer (&back->display, area))
	{
		NORET_MESSAGE("InitCharactersAndColorsWindow failed","6");
		// Return failure
	}

/*	G_endCell = back->display.buffer + area;

	for (i = 0, G_cell = back->display.buffer; G_cell < G_endCell; i++, G_cell++)
	{
		G_cell->Char.AsciiChar = DATA_AND_FLAGS;
		G_cell->Attributes = MAKEBYTE(0x0,i);
		SETFLAG(*(back->display.flags + i),HOTKEY);
		*(back->display.data + i) = i;
	}*/
	G_end = back->display.buffer + area;

	for (i = 0, G_bufCell = back->display.buffer; G_bufCell < G_end; i++, G_bufCell++)
	{
		G_bufCell->graph.Char.AsciiChar = DATA_AND_FLAGS;
		G_bufCell->graph.Attributes = MAKEBYTE(0x0,i);
		SETFLAG(G_bufCell->flags,HOTKEY);
		G_bufCell->data = i;
	}

	back->data = 2;

	parentWindow->closeKey = VK_SPACE;
}

void InitMapFlagsWindow (ParentWindow * parentWindow, Objects * objects)
{
	int i;
	int area;
	int index = 0;
	pWindow flags;

 	assert (parentWindow && objects);

	ZeroMemory (parentWindow, sizeof (ParentWindow));

	parentWindow->bufferSharing = kShared;
	ShareScreenBuffer (&parentWindow->back, objects);

	parentWindow->numWindows = 1;

	CALLOC(parentWindow->windows,parentWindow->numWindows,Window);

	if (!parentWindow->windows)
	{
		NORET_MESSAGE("InitMapFlagsWindow failed","1");
		// Return failure
	}

	flags = parentWindow->windows;

	parentWindow->width = 25;
	parentWindow->height = 10;

	parentWindow->background = 0;
	parentWindow->border = MAKEBYTE(0x7,0x0);

	CALLOC(parentWindow->backData,parentWindow->width * parentWindow->height,BYTE);

	if (!parentWindow->backData)
	{
		NORET_MESSAGE("InitMapFlagsWindow failed","2");
		// Return failure
	}

	MakeParentWindow (parentWindow, 0, 0, 0);

	flags->windowCoord.X = flags->windowCoord.Y = 1;

	flags->windowWidth = flags->width = 23;
	flags->windowHeight = flags->height = 8;

	area = flags->width * flags->height;

	if (!AllocateBuffer (&flags->display, area))
	{
		NORET_MESSAGE("InitMapFlagsWindow failed","3");
		// Return failure
	}

	for (i = 0; i < flags->height; i++)
	{
/*		G_cell = flags->display.buffer + index;

		for (G_endCell = G_cell + flags->width; G_cell < G_endCell; index++, G_cell++)
		{
			G_cell->Attributes = MAKEBYTE(0x7,0x0);
			SETFLAG(*(flags->display.flags + index),HOTKEY);
			*(flags->display.data + index) = 1 << i;
		}*/
		G_bufCell = flags->display.buffer + index;

		for (G_end = G_bufCell + flags->width; G_bufCell < G_end; index++, G_bufCell++)
		{
			G_bufCell->graph.Attributes = MAKEBYTE(0x7,0x0);
			SETFLAG(G_bufCell->flags,HOTKEY);
			G_bufCell->data = 1 << i;
		}
	}

	WriteText (&flags->display, " 0 - NONVISIBLE", 0);
	WriteText (&flags->display, " 1 - SOLID", flags->width);
	WriteText (&flags->display, " 2 - OBSCURE", flags->width * 2);
	WriteText (&flags->display, " 3 - OCCUPIED", flags->width * 3);
	WriteText (&flags->display, " 4 - TRIGGER", flags->width * 4);
	WriteText (&flags->display, " 5 - DANGER", flags->width * 5);
	WriteText (&flags->display, " 6 - SHIMMER", flags->width * 6);
	WriteText (&flags->display, " 7 - EXIT", flags->width * 7);

	flags->data = 3;

	parentWindow->closeKey = VK_SPACE;
}

void InitImageFlagsWindow (ParentWindow * parentWindow, Objects * objects)
{
	int i;
	int area;
	int index = 0;
	pWindow flags;

 	assert (parentWindow && objects);

	ZeroMemory (parentWindow, sizeof (ParentWindow));

	parentWindow->bufferSharing = kShared;
	ShareScreenBuffer (&parentWindow->back, objects);

	parentWindow->numWindows = 1;

	CALLOC(parentWindow->windows,parentWindow->numWindows,Window);

	if (!parentWindow->windows)
	{
		NORET_MESSAGE("InitImageFlagsWindow failed","1");
		// Return failure
	}

	flags = parentWindow->windows;

	parentWindow->width = 25;
	parentWindow->height = 10;

	parentWindow->background = 0;
	parentWindow->border = MAKEBYTE(0x7,0x0);

	CALLOC(parentWindow->backData,parentWindow->width * parentWindow->height,BYTE);

	if (!parentWindow->backData)
	{
		NORET_MESSAGE("InitImageFlagsWindow failed","2");
		// Return failure
	}

	MakeParentWindow (parentWindow, 0, 0, 0);

	flags->windowCoord.X = flags->windowCoord.Y = 1;

	flags->windowWidth = flags->width = 23;
	flags->windowHeight = flags->height = 8;

	area = flags->width * flags->height;

	if (!AllocateBuffer (&flags->display, area))
	{
		NORET_MESSAGE("InitImageFlagsWindow failed","3");
		// Return failure
	}

	for (i = 0; i < flags->height; i++)
	{
/*		G_cell = flags->display.buffer + index;

		for (G_endCell = G_cell + flags->width; G_cell < G_endCell; index++, G_cell++)
		{
			G_cell->Attributes = MAKEBYTE(0x7,0x0);
			SETFLAG(*(flags->display.flags + index),HOTKEY);
			*(flags->display.data + index) = 1 << i;
		}*/
		G_bufCell = flags->display.buffer + index;

		for (G_end = G_bufCell + flags->width; G_bufCell < G_end; index++, G_bufCell++)
		{
			G_bufCell->graph.Attributes = MAKEBYTE(0x7,0x0);
			SETFLAG(G_bufCell->flags,HOTKEY);
			G_bufCell->data = 1 << i;
		}
	}

	WriteText (&flags->display, " 0 - NONVISIBLE", 0);
	WriteText (&flags->display, " 1 - ", flags->width);
	WriteText (&flags->display, " 2 - HIGH", flags->width * 2);
	WriteText (&flags->display, " 3 - SHOWSECONDARY", flags->width * 3);
	WriteText (&flags->display, " 4 - FLASH", flags->width * 4);
	WriteText (&flags->display, " 5 - ", flags->width * 5);
	WriteText (&flags->display, " 6 - ", flags->width * 6);
	WriteText (&flags->display, " 7 - ", flags->width * 7);

	flags->data = 3;

	parentWindow->closeKey = VK_SPACE;
}

void InitDataWindow (ParentWindow * parentWindow, Objects * objects)
{
	int i;
	int area;
	pWindow data;

 	assert (parentWindow && objects);

	ZeroMemory (parentWindow, sizeof (ParentWindow));

	parentWindow->bufferSharing = kShared;
	ShareScreenBuffer (&parentWindow->back, objects);

	parentWindow->numWindows = 1;

	CALLOC(parentWindow->windows,parentWindow->numWindows,Window);

	if (!parentWindow->windows)
	{
		NORET_MESSAGE("InitDataWindow failed","1");
		// Return failure
	}

	data = parentWindow->windows;

	parentWindow->width = 18;
	parentWindow->height = 18;

	parentWindow->background = 0x0;
	parentWindow->border = MAKEBYTE(0x7,0x0);

	CALLOC(parentWindow->backData,parentWindow->width * parentWindow->height,BYTE);

	if (!parentWindow->backData)
	{
		NORET_MESSAGE("InitDataWindow failed","2");
		// Return failure
	}

	MakeParentWindow (parentWindow, 0, 0, 0);

	data->windowCoord.X = data->windowCoord.Y = 1;

	data->windowWidth = data->width = 16;
	data->windowHeight = data->height = 16;

	area = data->width * data->height;

	if (!AllocateBuffer (&data->display, area))
	{
		NORET_MESSAGE("InitDataWindow failed","3");
		// Return failure
	}

/*	G_endCell = data->display.buffer + area;

	for (i = 0, G_cell = data->display.buffer; G_cell < G_endCell; i++, G_cell++)
	{
		G_cell->Char.AsciiChar = DATA_AND_FLAGS;
		G_cell->Attributes = i;
		SETFLAG(*(data->display.flags + i),HOTKEY);
		*(data->display.data + i) = i;
	}*/
	G_end = data->display.buffer + area;

	for (i = 0, G_bufCell = data->display.buffer; G_bufCell < G_end; i++, G_bufCell++)
	{
		G_bufCell->graph.Char.AsciiChar = DATA_AND_FLAGS;
		G_bufCell->graph.Attributes = i;
		SETFLAG(G_bufCell->flags,HOTKEY);
		G_bufCell->data = i;
	}

	data->data = 4;

	parentWindow->closeKey = VK_SPACE;
}

void InitCompressionWindow (ParentWindow * parentWindow, Objects * objects)
{
	int i;
	int area;
	int index = 0;
	pWindow comp;

 	assert (parentWindow && objects);

	ZeroMemory (parentWindow, sizeof (ParentWindow));

	parentWindow->bufferSharing = kShared;
	ShareScreenBuffer (&parentWindow->back, objects);

	parentWindow->numWindows = 1;

	CALLOC(parentWindow->windows,parentWindow->numWindows,Window);

	if (!parentWindow->windows)
	{
		NORET_MESSAGE("InitCompressionWindow failed","1");
		// Return failure
	}

	comp = parentWindow->windows;

	parentWindow->width = 21;
	parentWindow->height = 6;

	parentWindow->background = 0x0;
	parentWindow->border = MAKEBYTE(0x7,0x0);

	CALLOC(parentWindow->backData,parentWindow->width * parentWindow->height,BYTE);

	if (!parentWindow->backData)
	{
		NORET_MESSAGE("InitCompressionWindow failed","2");
		// Return failure
	}

	MakeParentWindow (parentWindow, 0, 0, 0);

	comp->windowCoord.X = comp->windowCoord.Y = 1;

	comp->windowWidth = comp->width = 19;
	comp->windowHeight = comp->height = 4;

	area = comp->width * comp->height;

	if (!AllocateBuffer (&comp->display, area))
	{
		NORET_MESSAGE("InitCompressionWindow failed","3");
		// Return failure
	}

	for (i = 0; i < comp->height; i++)
	{
/*		G_cell = comp->display.buffer + index;

		for (G_endCell = G_cell + comp->width; G_cell < G_endCell; index++, G_cell++)
		{
			G_cell->Attributes = MAKEBYTE(0x7,0x0);
			SETFLAG(*(comp->display.flags + index),HOTKEY);
			*(comp->display.data + index) = i;
		}*/
		G_bufCell = comp->display.buffer + index;

		for (G_end = G_bufCell + comp->width; G_bufCell < G_end; index++, G_bufCell++)
		{
			G_bufCell->graph.Attributes = MAKEBYTE(0x7,0x0);
			SETFLAG(G_bufCell->flags,HOTKEY);
			G_bufCell->data = i;
		}
	}

	WriteText (&comp->display, " 1 - Merge", 0);
	WriteText (&comp->display, " 2 - CompressZeroes", comp->width);
	WriteText (&comp->display, " 3 - NotZero", comp->width * 2);
	WriteText (&comp->display, " 4 - ConstantValue", comp->width * 3);

	comp->data = 5;

	parentWindow->closeKey = VK_SPACE;
}