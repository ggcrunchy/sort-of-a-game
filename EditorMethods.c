#include "User.h"

/********************************************************************
*																	*
*							Globals									*
*																	*
********************************************************************/

static PCHAR_INFO G_endCell;
// Used to quicken CHAR_INFO loops

static PCHAR_INFO G_cell;
// Used for output

static Clipper G_clipper;
// Used to format visual output

void DisplayBorders (Image * image, Output * outputObj)
{
	int xOffset = image->width + image->location.X;
	int yOffset = image->height + image->location.Y;
	int i, len;
	BOOL xValid = xOffset < SCREEN_WIDTH ? TRUE : FALSE;
	BOOL yValid = yOffset < SCREEN_HEIGHT ? TRUE : FALSE;

	if (xValid)
	{
		len = yValid ? yOffset : SCREEN_HEIGHT;

		G_cell = *outputObj->outputBuf.buffer + xOffset;

		for (i = 0; i < len; i++, G_cell += SCREEN_WIDTH)
		{
			G_cell->Char.AsciiChar = UPTODOWNBORDER;
			G_cell->Attributes = 0xF;
		}
	}

	if (yValid)
	{
		len = xValid ? xOffset : SCREEN_WIDTH;

		G_cell = *(outputObj->outputBuf.buffer + yOffset);

		for (i = 0, G_endCell = G_cell + len; G_cell < G_endCell; i++, G_cell++)
		{
			G_cell->Char.AsciiChar = LEFTTORIGHTBORDER;
			G_cell->Attributes = 0xF;
		}
	}

	G_cell = *(outputObj->outputBuf.buffer + yOffset) + xOffset;

	if (xValid && yValid)
	{
		G_cell->Char.AsciiChar = BOTTOMRIGHTBORDER;
		G_cell->Attributes = 0xF;
	}
}

void ClipCompression (Image * image)
{
	int originX, originY;
	int extentX, extentY;

	originX = image->location.X;
	originY = image->location.Y;

	extentX = originX + image->width;
	extentY = originY + image->height;

	G_clipper = GetClipper ();
	
	G_clipper.visible = (extentX > 0 && originX < SCREEN_WIDTH)
					 && (extentY > 0 && originY < SCREEN_HEIGHT);

	if (G_clipper.visible)
	{
		G_clipper.clipRegion.left   = originX > 0 ? originX : 0;
		G_clipper.clipRegion.top	= originY > 0 ? originY : 0;
		G_clipper.clipRegion.right  = extentX < SCREEN_WIDTH ? extentX : SCREEN_WIDTH;
		G_clipper.clipRegion.bottom = extentY < SCREEN_HEIGHT ? extentY : SCREEN_HEIGHT;
	}
}

void DisplayCompression (CHAR_INFO * indices, ScreenBuffer * buffer, voidStar data, dataType type)
{
	int i;
	int originX, originY;
	int xOffset, yOffset;
	int width, height;
	int offsets;
	int pitch, pitch2;
	PCHAR_INFO cell;
	pImage image;
	pMap map;
	pPattern pattern;

	switch (type)
	{
	case kImage:
		image = (pImage) data;

		ClipCompression (image);

		originX = G_clipper.clipRegion.left;
		originY = G_clipper.clipRegion.top;

		xOffset = originX - image->location.X;
		yOffset = originY - image->location.Y;

		width  = G_clipper.clipRegion.right - originX;
		height = G_clipper.clipRegion.bottom - originY;
	
		offsets = yOffset * image->width + xOffset;

		pitch = SCREEN_WIDTH - width;
		pitch2 = image->width - width;

		G_cell = *(buffer->buffer + originY) + originX;
		cell = indices + offsets;

		for (i = 0; i < height; i++)
		{
			for (G_endCell = G_cell + width; G_cell < G_endCell; cell++, G_cell++)
			{
				*G_cell = *cell;
			}

			G_cell += pitch;
			cell += pitch2;
		}

		break;

	case kMap:
		map = (pMap) data;

		offsets = map->yOffset * map->width + map->xOffset;

		G_cell = *buffer->buffer;
		cell = indices + offsets;

		pitch = map->width - SCREEN_WIDTH;

		for (i = 0; i < SCREEN_HEIGHT; i++)
		{
			for (G_endCell = G_cell + SCREEN_WIDTH; G_cell < G_endCell; cell++, G_cell++)
			{
				*G_cell = *cell;
			}

			cell += pitch;
		}

		break;

	case kPattern:
		pattern = (pPattern) data;

		break;
	}
}