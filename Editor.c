/********************************************************************
*																	*
*							Editor.c								*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Driver for game editor								*
*																	*
********************************************************************/

/********************************************************************
*																	*
*							Includes								*
*																	*
********************************************************************/

#include "User.h"

/********************************************************************
*																	*
*							Globals									*
*																	*
********************************************************************/

static pCell G_mapCell, G_imgCell, G_patCell;

static Clipper G_clipper;
static Wrapper G_wrapper;
// Used to format visual output

/********************************************************************
*																	*
*							Editor Wrapper							*
*																	*
********************************************************************/

void Editor (void)
{
	int read;
	int mode;
	int numIndices;
	pCompressionIndex temp;
	Objects objects;
	pMap map = NULL;
	pImage image = NULL;
	FILE * fp;
	char filename [WORD_LENGTH];

	ZeroMemory (&objects, sizeof (Objects));

	InitializeFileObject (&objects.fileObj);

choice:

	printf ("Choose editor mode:\n\n\t1 - Map\n\t2 - Image\n\n");

	scanf ("%d", &mode);

	switch (mode)
	{
	case 1:
		MALLOC(map,Map);

map:

		printf ("\nDo you wish to load a map?\n\n\t1 - Yes\n\t2 - No\n\n");

		scanf ("%d", &read);

		if (read != 1 && read != 2)
		{
			printf ("Invalid choice\n\n");
			goto map;
		}

		printf ("\nName the map:\t");

		scanf ("%s", objects.fileObj.filename);

		if (read == 1)
		{
			if (!ReloadMap (&objects.fileObj, map, objects.fileObj.filename))
			{
				printf ("\n\nError, ReloadMap failed\n");
				return;
			}
		}
		
		if (read == 2)
		{
			printf ("\n\nEnter map's dimensions:");

			printf ("\n\nWidth:\t");
			scanf ("%d", &map->width);

			if (!map->width)
			{
				printf ("\n\nError, map->width == 0\n");
				return;
			}

			printf ("\n\nHeight:\t");
			scanf ("%d", &map->height);

			if (!map->height)
			{
				printf ("\n\nError, map->height == 0\n");
				return;
			}

			printf ("\n\nEnter scroll values:\n\n");

			printf ("Horizontal:\t");
			scanf ("%d", &map->xScroll);

			if (!map->xScroll && map->width > SCREEN_WIDTH)
			{
				printf ("\n\nError, map->xScroll == 0 && map->width > SCREEN_WIDTH\n\n");
				return;
			}

			printf ("\n\nVertical:\t");
			scanf ("%d", &map->yScroll);

			if (!map->yScroll && map->height > SCREEN_HEIGHT)
			{
				printf ("\n\nError, map->yScroll == 0 && map->height > SCREEN_HEIGHT\n\n");
				return;
			}

			AllocateBuffer (&map->world, map->width * map->height);
		}

		break;

	case 2:
		MALLOC(image,Image);

image:

		printf ("\nDo you wish to load an image?\n\n\t1 - Yes\n\t2 - No\n\n");

		scanf ("%d", &read);

		if (read != 1 && read != 2)
		{
			printf ("Invalid choice\n\n");
			goto image;
		}

		printf ("\nName the image:\t");

		scanf ("%s", objects.fileObj.filename);

		if (read == 1)
		{
			if (!ReloadImage (&objects.fileObj, image, objects.fileObj.filename))
			{
				printf ("\n\nError, ReloadImage failed\n");
				return;
			}
		}

		if (read == 2)
		{
			printf ("\n\nEnter image's dimensions:");

			printf ("\n\nWidth:\t");
			scanf ("%d", &image->width);

			if (!image->width)
			{
				printf ("\n\nError, image->width == 0\n\n");
				return;
			}

			printf ("\n\nHeight:\t");
			scanf ("%d", &image->height);

			if (!image->height)
			{
				printf ("\n\nError, image->height == 0\n\n");
				return;
			}

			AllocateBuffer (&image->image, image->width * image->height);
		}

		break;

	default:
		printf ("\n\nInvalid choice\n\n");
		goto choice;
	}

	numIndices = objects.fileObj.numIndices;

	CALLOC(temp,numIndices,CompressionIndex);

	memcpy (temp, objects.fileObj.compression, numIndices * sizeof (CompressionIndex));

	fp = objects.fileObj.fp;
	strcpy (filename, objects.fileObj.filename);

	ConsoleInit (&objects, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT, TRUE);
	
	objects.fileObj.fp = fp;
	strcpy (objects.fileObj.filename, filename);

	memcpy (objects.fileObj.compression, temp, numIndices * sizeof (CompressionIndex));

	FREE(temp);

	objects.fileObj.numIndices = numIndices;

	MALLOC(objects.inputObj.information,InputInfo);

	if (!objects.inputObj.information)
	{
		MessageBox (NULL, "Editor failed", "Error", MB_OK);
		return;	// Return failure
	}

	objects.inputObj.information->numItems = 6;

	CALLOC(objects.inputObj.information->infoItems,objects.inputObj.information->numItems,BYTE);
	
	if (!objects.inputObj.information->infoItems)
	{
		MessageBox (NULL, "Editor failed", "Error", MB_OK);
		return;	// Return failure
	}

	UpdateScreen (&objects.outputObj);

	if (map)
	{
		MapEdit (map, &objects);

		DeleteMap (map);
		FREE(map);
	}

	if (image)
	{
		ImageEdit (image, &objects);

		DeleteImage (image);
		FREE(image);
	}

	DeinitializeObjects (&objects);
}

/********************************************************************
*																	*
*							Wrappers								*
*																	*
********************************************************************/

void MapEdit (Map * map, Objects * objects)
{
	int i, j;
	int dimensions = map->width * map->height;
	int mode = 0;
	int index;
	int mouseX, mouseY;
	int loop = TRUE;
	BYTE backColor = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	BYTE foreColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	BYTE curFlag = NONVISIBLE;
	BYTE curDatum = 0;
	StorageMode curMode = kMerge;
	WORD curColor = (WORD) MAKEBYTE(foreColor, backColor);
	char curChar = END;
	PCHAR_INFO compression;

	ParentWindow CharactersAndColors, MapFlags, MapData, Compression;

	InitCharactersAndColorsWindow (&CharactersAndColors, objects);
	InitMapFlagsWindow (&MapFlags, objects);
	InitDataWindow (&MapData, objects);
	InitCompressionWindow (&Compression, objects);

	CALLOC(compression,dimensions,CHAR_INFO);

	if (!compression)
	{
		NORET_MESSAGE("MapEdit failed","1");
		// Return failure
	}

	for (i = 0; i < objects->fileObj.numIndices; i++)
	{
		index = (objects->fileObj.compression + i)->indexValue;

		(compression + index)->Char.AsciiChar = DATA_AND_FLAGS;
		(compression + index)->Attributes = (objects->fileObj.compression + i)->mode + 1;
	}

	while (loop)
	{
		switch (GetInput (&objects->inputObj, kSync))
		{
		case VK_LEFT:
			ScrollMap (map, kLeft, kVertFix);
	
			break;

		case VK_RIGHT:
			ScrollMap (map, kRight, kVertFix);

			break;

		case VK_UP:
			ScrollMap (map, kHorzFix, kUp);

			break;

		case VK_DOWN:
			ScrollMap (map, kHorzFix, kDown);
			
			break;

		case VK_TAB:
			mode = (mode + 1) & 3;

			break;

		case 'S':
			for (i = 0, j = 0; i < dimensions; i++)
			{
				if ((compression + i)->Char.AsciiChar == DATA_AND_FLAGS)
				{
					(objects->fileObj.compression + j)->indexValue = i;
					(objects->fileObj.compression + j)->mode = (compression + i)->Attributes - 1;

					j++;
				}
			}

			ReopenFile (&objects->fileObj, objects->fileObj.filename, kWrite, kBinary);
			if (!WriteMap (&objects->fileObj, map))
			{
				NORET_MESSAGE("MapEdit failed","2");
				// Return failure
			}

			break;

		case VK_SPACE:
			switch (mode)
			{
			case 0:
				CopyBuffer (CharactersAndColors.back, &objects->outputObj.outputBuf);

				ActivateWindows (&CharactersAndColors);
				RedrawWindows (&CharactersAndColors, objects);

				while (WindowsActive (&CharactersAndColors))
				{
					Update (&CharactersAndColors, objects, NULL);
				}

				curChar = *objects->inputObj.information->infoItems;

				foreColor = *(objects->inputObj.information->infoItems + 1);
				backColor = *(objects->inputObj.information->infoItems + 2);

				curColor = MAKEBYTE(foreColor,backColor);

				break;
				
			case 1:
				CopyBuffer (MapFlags.back, &objects->outputObj.outputBuf);

				ActivateWindows (&MapFlags);
				RedrawWindows (&MapFlags, objects);

				while (WindowsActive (&MapFlags))
				{
					Update (&MapFlags, objects, NULL);
				}

				curFlag = *(objects->inputObj.information->infoItems + 3);

				break;

			case 2:
				CopyBuffer (MapData.back, &objects->outputObj.outputBuf);

				ActivateWindows (&MapData);
				RedrawWindows (&MapData, objects);

				while (WindowsActive (&MapData))
				{
					Update (&MapData, objects, NULL);
				}

				curDatum = *(objects->inputObj.information->infoItems + 4);

				break;

			case 3:
				CopyBuffer (Compression.back, &objects->outputObj.outputBuf);

				ActivateWindows (&Compression);
				RedrawWindows (&Compression, objects);

				while (WindowsActive (&Compression))
				{
					Update (&Compression, objects, NULL);
				}

				curMode = *(objects->inputObj.information->infoItems + 5);

				break;
			}

			break;

		case VK_ESCAPE:
			loop = FALSE;

			break;

		case 0:
			mouseX = GetMouseXPos (&objects->inputObj);
			mouseY = GetMouseYPos (&objects->inputObj);

			index = (map->yOffset + mouseY) * map->width + (map->xOffset + mouseX);
			G_mapCell = map->world.buffer + index;

			if (LeftMouseButtonIsDown (&objects->inputObj))
			{
				switch (mode)
				{
				case 0:
				/*	(map->world.buffer + index)*/G_mapCell->graph.Char.AsciiChar = curChar;
				/*	(map->world.buffer + index)*/G_mapCell->graph.Attributes = curColor;

					break;

				case 1:
				//	SETFLAG(*(map->world.flags + index),curFlag);
					SETFLAG(G_mapCell->flags,curFlag);

					break;

				case 2:
				//	*(map->world.data + index) = curDatum;
					G_mapCell->data = curDatum;

					break;

				case 3:
					if (objects->fileObj.numIndices >= MAXCOMPRESSIONINDICES && (compression + index)->Char.AsciiChar == END)
					{
						break;
					}

					else
					{
						if ((compression + index)->Char.AsciiChar == END)
						{
							objects->fileObj.numIndices++;
						}

						(compression + index)->Char.AsciiChar = DATA_AND_FLAGS;
						(compression + index)->Attributes = curMode + 1;
					}

					break;
				}
			}

			if (RightMouseButtonIsDown (&objects->inputObj))
			{
				switch (mode)
				{
				case 0:
				/*	(map->world.buffer + index)*/G_mapCell->graph.Char.AsciiChar = END;
				/*	(map->world.buffer + index)*/G_mapCell->graph.Attributes = 0;

					break;

				case 1:
				//	CLEARFLAG(*(map->world.flags + index),curFlag);
					CLEARFLAG(G_mapCell->flags,curFlag);

					break;

				case 2:
				//	*(map->world.data + index) = 0;
					G_mapCell->data = 0;

					break;

				case 3:
					if (!index || index + 1 == dimensions)
					{
						break;
					}

					else
					{
						if ((compression + index)->Char.AsciiChar == DATA_AND_FLAGS)
						{
							objects->fileObj.numIndices--;
						}

						(compression + index)->Char.AsciiChar = END;
						(compression + index)->Attributes = 0;
					}

					break;
				}
			}
		}

		ClearScreen (&objects->outputObj);

		switch (mode)
		{
			case 0:
				CopyMapToBuffer (&objects->outputObj.outputBuf, map);

				break;

			case 1:
				CopyMapFlagsToBuffer (&objects->outputObj.outputBuf, map);

				break;

			case 2:
				CopyMapDataToBuffer (&objects->outputObj.outputBuf, map);

				break;
			
			case 3:
				DisplayCompression (compression, &objects->outputObj.outputBuf, map, kMap);

				break;
		}

		UpdateScreen (&objects->outputObj);
	}

	DeleteParentWindow (&CharactersAndColors);
	DeleteParentWindow (&MapFlags);
	DeleteParentWindow (&MapData);
	DeleteParentWindow (&Compression);

	FREE(compression);
}

void ImageEdit (Image * image, Objects * objects)
{
	int i, j;
	int dimensions = image->width * image->height;
	int mode = 0;
	int index;
	int mouseX, mouseY;
	int imageX, imageY;
	int loop = TRUE;
	BYTE backColor = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	BYTE foreColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	BYTE curFlag = NONVISIBLE;
	BYTE curDatum = 0;
	StorageMode curMode = kMerge;
	WORD curColor = MAKEBYTE(foreColor, backColor);
	char curChar = END;
	PCHAR_INFO compression;

	ParentWindow CharactersAndColors, ImageFlags, ImageData, Compression;

	InitCharactersAndColorsWindow (&CharactersAndColors, objects);
	InitImageFlagsWindow (&ImageFlags, objects);
	InitDataWindow (&ImageData, objects);
	InitCompressionWindow (&Compression, objects);

	CALLOC(compression,dimensions,CHAR_INFO);

	if (!compression)
	{
		NORET_MESSAGE("ImageEdit failed","1");
		// Return failure
	}

	for (i = 0; i < objects->fileObj.numIndices; i++)
	{
		index = (objects->fileObj.compression + i)->indexValue;

		(compression + index)->Char.AsciiChar = DATA_AND_FLAGS;
		(compression + index)->Attributes = (objects->fileObj.compression + i)->mode + 1;
	}

	while (loop)
	{
		switch (GetInput (&objects->inputObj, kSync))
		{
		case VK_LEFT:
			if (image->location.X)
			{
				image->location.X++;
			}

			break;

		case VK_RIGHT:
			if (image->location.X + SCREEN_WIDTH < image->width)
			{
				image->location.X--;
			}

			break;

		case VK_UP:
			if (image->location.Y)
			{
				image->location.Y++;
			}

			break;

		case VK_DOWN:
			if (image->location.Y + SCREEN_HEIGHT < image->height)
			{
				image->location.Y--;
			}

			break;

		case VK_TAB:
			mode = (mode + 1) & 3;

			break;

		case 'S':
			for (i = 0, j = 0; i < dimensions; i++)
			{
				if ((compression + i)->Char.AsciiChar == DATA_AND_FLAGS)
				{
					(objects->fileObj.compression + j)->indexValue = i;
					(objects->fileObj.compression + j)->mode = (compression + i)->Attributes - 1;

					j++;
				}
			}

			ReopenFile (&objects->fileObj, objects->fileObj.filename, kWrite, kBinary);
			if (!WriteImage (&objects->fileObj, image))
			{
				NORET_MESSAGE("ImageEdit failed","2");
				// Return failure
			}

			break;

		case VK_SPACE:
			switch (mode)
			{
			case 0:
				CopyBuffer (CharactersAndColors.back, &objects->outputObj.outputBuf);

				ActivateWindows (&CharactersAndColors);
				RedrawWindows (&CharactersAndColors, objects);

				while (WindowsActive (&CharactersAndColors))
				{
					Update (&CharactersAndColors, objects, NULL);
				}

				curChar = *objects->inputObj.information->infoItems;

				foreColor = *(objects->inputObj.information->infoItems + 1);
				backColor = *(objects->inputObj.information->infoItems + 2);

				curColor = MAKEBYTE(foreColor,backColor);

				break;
				
			case 1:
				CopyBuffer (ImageFlags.back, &objects->outputObj.outputBuf);

				ActivateWindows (&ImageFlags);
				RedrawWindows (&ImageFlags, objects);

				while (WindowsActive (&ImageFlags))
				{
					Update (&ImageFlags, objects, NULL);
				}

				curFlag = *(objects->inputObj.information->infoItems + 3);

				break;

			case 2:
				CopyBuffer (ImageData.back, &objects->outputObj.outputBuf);

				ActivateWindows (&ImageData);
				RedrawWindows (&ImageData, objects);

				while (WindowsActive (&ImageData))
				{
					Update (&ImageData, objects, NULL);
				}

				curDatum = *(objects->inputObj.information->infoItems + 4);

				break;

			case 3:
				CopyBuffer (Compression.back, &objects->outputObj.outputBuf);

				ActivateWindows (&Compression);
				RedrawWindows (&Compression, objects);

				while (WindowsActive (&Compression))
				{
					Update (&Compression, objects, NULL);
				}

				curMode = *(objects->inputObj.information->infoItems + 5);

				break;
			}

			break;

		case VK_ESCAPE:
			loop = FALSE;

			break;

		case 0:
			mouseX = GetMouseXPos (&objects->inputObj);
			mouseY = GetMouseYPos (&objects->inputObj);

			G_clipper = GetClipper ();

			if (G_clipper.visible)
			{
				imageX = G_clipper.clipRegion.left - image->location.X;
				imageY = G_clipper.clipRegion.top - image->location.Y;

				if (!imageX)
				{
					imageX = -image->location.X;
				}

				if (!imageY)
				{
					imageY = -image->location.Y;
				}

				if (mouseX >= G_clipper.clipRegion.left && mouseX <= (G_clipper.clipRegion.right - 1) && mouseY >= G_clipper.clipRegion.top && mouseY <= (G_clipper.clipRegion.bottom - 1))
				{
					index = (imageY + mouseY) * image->width + (imageX + mouseX);
					G_imgCell = image->image.buffer + index;

					if (LeftMouseButtonIsDown (&objects->inputObj))
					{
						switch (mode)
						{
						case 0:
						/*	(image->image.buffer + index)*/G_imgCell->graph.Char.AsciiChar = curChar;
						/*	(image->image.buffer + index)*/G_imgCell->graph.Attributes = curColor;

							break;

						case 1:
						//	SETFLAG(*(image->image.flags + index), curFlag);
							SETFLAG(G_imgCell->flags,curFlag);

							break;

						case 2:
						//	*(image->image.data + index) = curDatum;
							G_imgCell->data = curDatum;

							break;

						case 3:
							if (objects->fileObj.numIndices >= MAXCOMPRESSIONINDICES && (compression + index)->Char.AsciiChar == END)
							{
								break;
							}

							else
							{
								if ((compression + index)->Char.AsciiChar == END)
								{
									objects->fileObj.numIndices++;
								}

								(compression + index)->Char.AsciiChar = DATA_AND_FLAGS;
								(compression + index)->Attributes = curMode + 1;
							}

							break;
						}
					}

					if (RightMouseButtonIsDown (&objects->inputObj))
					{
						switch (mode)
						{
						case 0:
						/*	(image->image.buffer + index)*/G_imgCell->graph.Char.AsciiChar = END;
						/*	(image->image.buffer + index)*/G_imgCell->graph.Attributes = 0;

							break;

						case 1:
						//	CLEARFLAG(*(image->image.flags + index),curFlag);
							CLEARFLAG(G_imgCell->flags,curFlag);

							break;

						case 2:
						//	*(image->image.data + index) = 0;
							G_imgCell->data = 0;

							break;

						case 3:
							if (!index || index + 1 == dimensions)
							{
								break;
							}

							else
							{
								if ((compression + index)->Char.AsciiChar == DATA_AND_FLAGS)
								{
									objects->fileObj.numIndices--;
								}

								(compression + index)->Char.AsciiChar = END;
								(compression + index)->Attributes = 0;
							}
						}
					}
				}
			}

			break;
		}

		ClearScreen (&objects->outputObj);

		switch (mode)
		{
		case 0:
			DisplayImageToScreen (image, &objects->outputObj.outputBuf, NULL);

			break;

		case 1:
			DisplayImageFlags (image, &objects->outputObj.outputBuf);

			break;

		case 2:
			DisplayImageData (image, &objects->outputObj.outputBuf);

			break;

		case 3:
			DisplayCompression (compression, &objects->outputObj.outputBuf, image, kImage);

			break;
		}

		DisplayBorders (image, &objects->outputObj);

		UpdateScreen (&objects->outputObj);
	}

	DeleteParentWindow (&CharactersAndColors);
	DeleteParentWindow (&ImageFlags);
	DeleteParentWindow (&ImageData);
	DeleteParentWindow (&Compression);

	FREE(compression);
}
