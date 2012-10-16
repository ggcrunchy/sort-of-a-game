/********************************************************************
*																	*
*							File.c									*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains implementation of file tool				*
*																	*
********************************************************************/

#include "File.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							External includes						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "ADT.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Globals									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static pWindow G_endWindow;
// Used to quicken Window looping
static pSeparatorEntry G_endSepEntry;
// Used to quicken SeparatorEntry looping
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

static PCHAR_INFO G_cell;
// Used for output

static pCell G_bufCell, G_end;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							File access functions					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static void MakePrimary (File * fileObj, fileStar secondary)
{
	ShutFile (fileObj);

	fileObj->fp = secondary;
}

static void MakeSecondary (File * fileObj, fileStar secondary)
{
	secondary = fileObj->fp;

	fileObj->fp = NULL;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Construction							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	InitializeFileObject - Initialize primary file object			*
********************************************************************/

BOOL InitializeFileObject (File * fileObj)
{
	ZeroMemory (fileObj, sizeof (File));
	// Zero memory out

	fileObj->numIndices = INITIAL_VALUE;	// Set the default index count

	strcpy (fileObj->filename, "No file opened");	// Assign a basic message to the filename

	CALLOC(fileObj->compression,MAXCOMPRESSIONINDICES,CompressionIndex);
	// Assign memory to fileObj's compression field

	if (!fileObj->compression)
	{
		ERROR_MESSAGE("InitializeFileObject failed","1");
		// Return failure
	}
	// Verify that fileObj's compression field points to valid memory

	return TRUE;
	// Return success
}

/********************************************************************
*	AllocateBuffer - Load memory into an OutputBuffer structure		*
********************************************************************/

BOOL AllocateBuffer (OutputBuffer * outputBuf, int dimensions)
{
	switch (outputBuf->bufSharing)
	{
	case kSingleOwner:
/*		CALLOC(outputBuf->buffer,dimensions,CHAR_INFO);
		CALLOC(outputBuf->flags,dimensions,BYTE);
		CALLOC(outputBuf->data,dimensions,BYTE);
		// Assign memory to outputBuf's buffer, flags, and data fields

		if (!outputBuf->buffer || !outputBuf->flags || !outputBuf->data)
		{
			ERROR_MESSAGE("AllocateBuffer failed","1");
			// Return failure
		}
		// Verify that outputBuf's buffer, flags, and data fields point to valid memory
*/
		CALLOC(outputBuf->buffer,dimensions,Cell);

		if (!outputBuf->buffer)
		{
			ERROR_MESSAGE("AllocateBuffer failed","1");
			// Return failure
		}
		// Verify that outputBuf's buffer points to valid memory

		break;	// Break out of switch statement

	case kShared:
		break;	// Break out of switch statement

	default:
		ERROR_MESSAGE("Unsupported exclusive mode: AllocateBuffer failed","2");
		// Return failure
	}

	return TRUE;
	// Return success
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							File management							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void ReopenFile (File * fileObj, String filename, FileAccess access, FileStyle style)
{
	char format [3];			// A format used to open a file
	char store [WORD_LENGTH];	// Filename storage

	assert (fileObj && filename);
	// Verify that fileObj and filename point to valid memory

	strcpy (store, filename);	// Copy filename into store

	if (fileObj->fp)	// Ensure that fileObj's fp field points to something
	{
		ShutFile (fileObj);
		// Close active file
	}

	strcpy (fileObj->filename, store);	// Copy store into fileObj's filename field

	switch (access)	// Get the access mode
	{
	case kRead:		// Read access case
		strcpy (format, "r");
		// Mark format for reading

		break;	// Break out of switch statement

	case kWrite:	// Write access case
		strcpy (format, "w");
		// Mark format for writing

		break;	// Break out of switch statement

	case kAppend:	// Append access case
		strcpy (format, "a");
		// Mark format for appending

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported mode: ReopenFile failed","1");
		// Return failure
	}

	switch (style)	// Get the file style
	{
	case kText:		// Text case
		strcat (format, "t");
		// Mark format for text IO

		break;	// Break out of switch statement

	case kBinary:	// Binary case
		strcat (format, "b");
		// Mark format for binary IO

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported style: ReopenFile failed","2");
		// Return failure
	}

	fileObj->fp = fopen (fileObj->filename, format);
	// Open the file in the appropriate manner

	if (!fileObj->fp)
	{
		NORET_MESSAGE("ReopenFile failed","3");
		// Return failure
	}
	// Verify that fileObj's fp field points to valid memory
}

void ShutFile (File * fileObj)
{
	assert (fileObj);
	// Verify that fileObj points to valid memory

	if (fileObj->fp)
	{
		fclose (fileObj->fp);
		// Close the active file
	}

	strcpy (fileObj->filename, "No file opened");	// Assign a basic message to the filename
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Chunk reading							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	LoadTextSequence - Load a text sequence into a string			*
********************************************************************/

static void LoadTextSequence (File * fileObj, String string)
{
	static int adjustment = 10 - 'A';
	int entry;
	int highNybble, lowNybble;
	char formatBuf [WORD_LENGTH << 2];
	String bufWalker = formatBuf;

	while (TRUE)
	{
		entry = fgetc (fileObj->fp);

		if (entry == '#')
		{
			highNybble = fgetc (fileObj->fp);
			lowNybble = fgetc (fileObj->fp);

			if (isalpha (highNybble))
			{
				highNybble = toupper (highNybble) + adjustment;
			}

			if (isalpha (lowNybble))
			{
				lowNybble = toupper (lowNybble) + adjustment;
			}

			*(bufWalker++) = (char) ((highNybble << 4) + lowNybble);

			if (!highNybble && !lowNybble)
			{
				break;
			}
		}

		else
		{
			*(bufWalker++) = (char) entry;
		}
	}

	CALLOC(string,strlen (formatBuf) + 1,char);

	if (!string)
	{
		NORET_MESSAGE("LoadTextSequence failed","1");
		// Return failure
	}

	strcpy (string, formatBuf);
}

/********************************************************************
*	LoadWindowData - Load common data into a Window structure		*
********************************************************************/

static void LoadWindowData (File * fileObj, Window * window)
{
	fscanf (fileObj->fp, "%d\n", &window->data);
	fscanf (fileObj->fp, "%d\n", &window->background);
	fscanf (fileObj->fp, "%d %d\n", &window->xOffset, &window->yOffset);
	fscanf (fileObj->fp, "%d %d\n", &window->width, &window->height);
	fscanf (fileObj->fp, "%d %d\n", &window->windowWidth, &window->windowHeight);
	fscanf (fileObj->fp, "%d %d\n", &window->windowCoord.X, &window->windowCoord.Y);
	fscanf (fileObj->fp, "%d\n", &window->state);
	fscanf (fileObj->fp, "%d\n", &window->mode);
}

/********************************************************************
*	LoadSeparators - Load separators into a parent window			*
********************************************************************/

static BOOL LoadSeparators (File * fileObj, ParentWindow * parentWindow)
{
	pSeparator separators = parentWindow->separators;
	pSeparatorEntry separatorEntry;

	MALLOC(separators,Separator);

	if (!separators)
	{
		ERROR_MESSAGE("LoadSeparators failed","1");
		// Return failure
	}

	fscanf (fileObj->fp, "%d %d\n", &separators->numHorzSeparators, &separators->numVertSeparators);

	CALLOC(separators->horzSeparators,separators->numHorzSeparators,SeparatorEntry);

	if (!separators->horzSeparators)
	{
		ERROR_MESSAGE("LoadSeparators failed","2");
		// Return failure
	}

	G_endSepEntry = separators->horzSeparators + separators->numHorzSeparators;

	for (separatorEntry = separators->horzSeparators; separatorEntry < G_endSepEntry; separatorEntry++)
	{
		fscanf (fileObj->fp, "%d %d\n", &separatorEntry->offsets.X, &separatorEntry->offsets.Y);
		fscanf (fileObj->fp, "%d\n", &separatorEntry->extent);
	}

	CALLOC(separators->vertSeparators,separators->numVertSeparators,SeparatorEntry);

	if (!separators->vertSeparators)
	{
		ERROR_MESSAGE("LoadSeparators failed","3");
		// Return failure
	}

	G_endSepEntry = separators->vertSeparators + separators->numVertSeparators;

	for (separatorEntry = separators->vertSeparators; separatorEntry < G_endSepEntry; separatorEntry++)
	{
		fscanf (fileObj->fp, "%d %d\n", &separatorEntry->offsets.X, &separatorEntry->offsets.Y);
		fscanf (fileObj->fp, "%d\n", &separatorEntry->extent);
	}

	return TRUE;
	// Return success
}

/********************************************************************
*	LoadVisuals - Load visuals into a basic window					*
********************************************************************/

static BOOL LoadVisuals (File * fileObj, Window * window)
{
	char filename [WORD_LENGTH];
	fileStar secondary = NULL;
	pVisuals visuals = window->visuals;
	pAnimation animation;
	pImage image;
	pPattern pattern;

	MALLOC(visuals,Visuals);

	if (!visuals)
	{
		ERROR_MESSAGE("LoadVisuals failed","1");
		// Return failure
	}

	fscanf (fileObj->fp, "%d\n", visuals->numAnimations); 

	G_endAnimation = visuals->animations + visuals->numAnimations;

	for (animation = visuals->animations; animation < G_endAnimation; animation++)
	{
		fscanf (fileObj->fp, "%s\n", filename);

		MakeSecondary (fileObj, secondary);

		if (!ReloadAnimation (fileObj, animation, filename))
		{
			ERROR_MESSAGE("LoadVisuals failed","2");
			// Return failure
		}

		MakePrimary (fileObj, secondary);
	}

	fscanf (fileObj->fp, "%d\n", visuals->numImages); 

	G_endImage = visuals->images + visuals->numImages;

	for (image = visuals->images; image < G_endImage; image++)
	{
		fscanf (fileObj->fp, "%s\n", filename);

		MakeSecondary (fileObj, secondary);

		if (!ReloadImage (fileObj, image, filename))
		{
			ERROR_MESSAGE("LoadVisuals failed","3");
			// Return failure
		}

		MakePrimary (fileObj, secondary);
	}

	fscanf (fileObj->fp, "%d\n", visuals->numPatterns); 

	G_endPattern = visuals->patterns + visuals->numPatterns;

	for (pattern = visuals->patterns; pattern < G_endPattern; pattern++)
	{
		fscanf (fileObj->fp, "%s\n", filename);

		MakeSecondary (fileObj, secondary);

		if (!ReloadPattern (fileObj, pattern, filename))
		{
			ERROR_MESSAGE("LoadVisuals failed","4");
			// Return failure
		}

		MakePrimary (fileObj, secondary);
	}

	return TRUE;
	// Return success
}

/********************************************************************
*	LoadBasicWindow - Load basic window								*
********************************************************************/

static BOOL LoadBasicWindow (File * fileObj, Window * window)
{
	BOOL usingVisuals;

	fscanf (fileObj->fp, "%d\n", &usingVisuals);

	if (usingVisuals)
	{
		if (!LoadVisuals (fileObj, window))
		{
			ERROR_MESSAGE("LoadBasicWindow failed","1");
			// Return failure
		}
	}

	return TRUE;
	// Return success
}

/********************************************************************
*	LoadEditBoxWindow - Load edit box window						*
********************************************************************/

static BOOL LoadEditBoxWindow (File * fileObj, Window * window)
{
	pIO_Descriptor IO = window->IO;

	MALLOC(IO,IO_Descriptor);

	if (!IO)
	{
		ERROR_MESSAGE("LoadEditBoxWindow failed","1");
		// Return failure
	}
	// Verify that window's IO field points to valid memory

	fscanf (fileObj->fp, "%d\n", &IO->readMode);

	fscanf (fileObj->fp, "%d %d\n", &IO->text.limit, &IO->text.numChars);

	CALLOC(IO->text.text,IO->text.numChars,char);

	if (!IO->text.text)
	{
		ERROR_MESSAGE("LoadEditBoxWindow failed","2");
		// Return failure
	}

	return TRUE;
	// Return success
}

/********************************************************************
*	LoadMenuWindow - Load menu window								*
********************************************************************/

static BOOL LoadMenuWindow (File * fileObj, Window * window)
{
	char filename [WORD_LENGTH];
	fileStar secondary = NULL;

	MALLOC(window->menu,Menu);

	if (!window->menu)
	{
		ERROR_MESSAGE("LoadMenuWindow failed","1");
		// Return failure
	}

	fscanf (fileObj->fp, "%s\n", filename);

	MakeSecondary (fileObj, secondary);

	if (!ReloadMenu (fileObj, window->menu, filename))
	{
		ERROR_MESSAGE("LoadMenuWindow failed","2");
		// Return failure
	}

	MakePrimary (fileObj, secondary);

	return TRUE;
	// Return success
}

/********************************************************************
*	LoadMessageBoxWindow - Load message box window					*
********************************************************************/

static BOOL LoadMessageBoxWindow (File * fileObj, Window * window)
{
	pIO_Descriptor IO = window->IO;

	MALLOC(IO,IO_Descriptor);

	if (!IO)
	{
		ERROR_MESSAGE("LoadMessageBoxWindow failed","1");
		// Return failure
	}
	// Verify that window's IO field points to valid memory

	fscanf (fileObj->fp, "%d\n", &IO->writeMode);

	fscanf (fileObj->fp, "%d %d\n", &IO->text.limit, &IO->text.numChars);

	CALLOC(IO->text.text,IO->text.numChars,char);

	if (!IO->text.text)
	{
		ERROR_MESSAGE("LoadMessageBoxWindow failed","2");
		// Return failure
	}
	// Verify that window's IO field points to valid memory

	return TRUE;
	// Return success
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							File Reading							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	ReadConfigFile - Primary wrapper for config file reading		*
********************************************************************/

/********************************************************************
*	Inflate - Used to inflate data stored within a file				*
********************************************************************/

static void Inflate (File * fileObj, OutputBuffer * outputBuf, int dimensions)
{
	int i = 0;				// Loop variable
	int index = 0;			// Index variable
	int inflateMode, width;	// Mode used to inflate data; width of region to read

	fscanf (fileObj->fp, "%d\n", &fileObj->numIndices);
	// Read the count of compression indices

	while (i < fileObj->numIndices)
	{
		fscanf (fileObj->fp, "\n%d %d\n", &inflateMode, &width);
		// Read inflateMode and width at given index

		(fileObj->compression + i)->indexValue = index;	// Assign index value to compression table
		(fileObj->compression + i)->mode = inflateMode;	// Assign inflate mode to compression table

		switch (inflateMode)	// Get the inflation mode
		{
		case kMerge:			// Merge case
			ReadMerge (fileObj, outputBuf, &index, width);
			// Read merged data from given index

			break;	// Break out of switch statement

		case kCompressZeroes:	// CompressZeroes case
			ReadCompressZeroes (fileObj, outputBuf, &index, width);
			// Read compressed-zero data from given index

			break;	// Break out of switch statement

		case kNotZero:			// NotZero case
			ReadNotZero (fileObj, outputBuf, &index, width);
			// Read non-zero data from given index

			break;	// Break out of switch statement

		case kConstantValue:	// ConstantValue case
			ReadConstantValue (fileObj, outputBuf, &index, width);
			// Read constant data from given index

			break;	// Break out of switch statement

		default:
			NORET_MESSAGE("Unsupported inflateMode: Inflate failed","1");
			// Return failure
		}

		i++;	// Increment compression table index
	}
}

/********************************************************************
*	ReadMerge - Merged data is parsed and placed into output buffer	*
********************************************************************/

static void ReadMerge (File * fileObj, OutputBuffer * outputBuf, intStar index, int width)
{
	int extent;	// Extent to read to
	int format;	// Format for data read

	extent = (*index) + width;	// Compute the extent to read

	for (*index; *index < extent; (*index)++)
	{
		fscanf (fileObj->fp, "%d ", &format);
		// Read formatted data

		SEPARATE(outputBuf,*index,format);
		// Parse the data read
	}
}

/********************************************************************
*	ReadCompressZeroes - Zero chains are inflated during reading	*
********************************************************************/

static void ReadCompressZeroes (File * fileObj, OutputBuffer * outputBuf, intStar index, int width)
{
	int extent;				// Extent to read to
	int numZeroes;			// Count of zeroes
	int format;				// Format for data read
	BOOL breakSet = TRUE;	// Loop control variable

	extent = (*index) + width;	// Compute the extent to read

	for (*index; *index < extent; (*index)++)
	{
		fscanf (fileObj->fp, "%d ", &format);
		// Read formatted data

		if (!format)	// Check for zero values
		{
			fscanf (fileObj->fp, "%d ", &numZeroes);
			// Read the count of zeroes

			while (breakSet)
			{
				SEPARATE(outputBuf,*index,0);
				// Parse the data read

				if (--numZeroes)
				{	
					(*index)++;
					// Increment the index while in zero loop
				}

				else
				{
					breakSet = FALSE;	// Set condition to break out of zero loop
				}
			}

			breakSet = TRUE;	// Reset loop control
		}

		else
		{
			SEPARATE(outputBuf,*index,format);
			// Parse the data read
		}
	}
}

/********************************************************************
*	ReadNotZero - Non-zero entries are placed into an output buffer	*
********************************************************************/

static void ReadNotZero (File * fileObj, OutputBuffer * outputBuf, intStar index, int width)
{
	int i;					// Loop variable
	int extent;				// Extent to read to
	int indValue, notZero;	// An index value; count of non-zero values
	int format;				// Format for data read

	fscanf (fileObj->fp, "%d\n", &notZero);
	// Read the count of non-zero values

	extent = (*index) + width;	// Compute the extent to read

	for (*index; *index < extent; (*index)++)
	{
		SEPARATE(outputBuf,*index,0);
		// Parse the data read
	}

	for (i = 0; i < notZero; i++)
	{
		fscanf (fileObj->fp, "%d %d ", &indValue, &format);
		// Read the index value and formatted data

		SEPARATE(outputBuf,indValue,format);
		// Parse the data read
	}
}

/********************************************************************
*	ReadConstantValue - Constant chains are inflated during reading	*
********************************************************************/

static void ReadConstantValue (File * fileObj, OutputBuffer * outputBuf, intStar index, int width)
{
	int extent;				// Extent to read to
	int constant;			// Count of constant values
	int format;				// Format for data read
	BOOL breakSet = TRUE;	// Loop control variable

	extent = (*index) + width;	// Compute the extent to read

	for (*index; *index < extent; (*index)++)
	{
		fscanf (fileObj->fp, "%d %d ", &constant, &format);
		// Read the count of constant values and formatted data

		while (breakSet)
		{
			SEPARATE(outputBuf,*index,format);
			// Parse the data read

			if (--constant)
			{	
				(*index)++;
				// Increment the index while in constant loop
			}

			else
			{
				breakSet = FALSE;	// Set condition to break out of constant loop
			}
		}

		breakSet = TRUE;	// Reset loop control
	}
}

/********************************************************************
*	ReloadImage - Load an image from a file to memory				*
********************************************************************/

BOOL ReloadImage (File * fileObj, Image * image, String filename)
{
	int dimensions;	// Area of image

	assert (fileObj && image && filename);
	// Verify that fileObj, image, and filename point to valid memory

	ReopenFile (fileObj, filename, kRead, kBinary);
	// Open the desired image

	fscanf (fileObj->fp, "%d %d\n", &image->width, &image->height);
	// Read image's width and height

	if (!image->width || !image->height)
	{
		ERROR_MESSAGE("ReloadImage failed","1");
		// Return failure
	}
	// Verify that image's width and height fields are non-zero

	dimensions = image->width * image->height;	// Calculate image's area

	if (!AllocateBuffer (&image->image, dimensions))
	{
		ERROR_MESSAGE("ReloadImage failed","2");
		// Return failure
	}

	switch (image->image.bufSharing)
	{
	case kSingleOwner:
		Inflate (fileObj, &image->image, dimensions);
		// Inflate image's visual data

		break;	// Break out of switch statement

	case kShared:
		break;	// Break out of switch statement
	}

	return TRUE;
	// Return success
}

/********************************************************************
*	ReloadAnimation - Load an animation from a file to memory		*
********************************************************************/

BOOL ReloadAnimation (File * fileObj, Animation * window, String filename)
{
	return TRUE;
	// Return success
}

/********************************************************************
*	ReloadPattern - Load a pattern from a file to memory			*
********************************************************************/

BOOL ReloadPattern (File * fileObj, Pattern * pattern, String filename)
{
	return TRUE;
	// Return success
}

/********************************************************************
*	ReloadParentWindow - Load a parent window from a file to memory	*
********************************************************************/

BOOL ReloadParentWindow (File * fileObj, ParentWindow * parentWindow, String filename)
{
	int dimensions;
	BOOL usingSeparators, usingADT;
	pWindow window;

	assert (fileObj && parentWindow && filename);
	// Verify that fileObj, parentWindow, and filename point to valid memory

	ReopenFile (fileObj, filename, kRead, kBinary);

	fscanf (fileObj->fp, "%d\n", &parentWindow->bufferSharing);

	switch (parentWindow->bufferSharing)
	{
	case kSingleOwner:
		MALLOC(parentWindow->back,ScreenBuffer);

		if (!parentWindow->back)
		{
			ERROR_MESSAGE("ReloadParentWindow failed","1");
			// Return failure
		}
		// Verify that parentWindow's back field points to valid memory

		break;	// Break out of switch statement

	case kShared:
		break;	// Break out of switch statement
	}

	fscanf (fileObj->fp, "%d\n", &parentWindow->delay);

	fscanf (fileObj->fp, "%d %d\n", &parentWindow->width, &parentWindow->height);

	CALLOC(parentWindow->backData,parentWindow->width * parentWindow->height,BYTE);

	if (!parentWindow->backData)
	{
		ERROR_MESSAGE("ReloadParentWindow failed","2");
		// Return failure
	}
	// Verify that parentWindow's backData field points to valid memory

	fscanf (fileObj->fp, "%c %c\n", &parentWindow->background, &parentWindow->border);

	fscanf (fileObj->fp, "%d\n", &usingSeparators);

	if (usingSeparators)
	{
		if (!LoadSeparators (fileObj, parentWindow))
		{
			ERROR_MESSAGE("ReloadParentWindow failed","3");
			// Return failure
		}
	}

	fscanf (fileObj->fp, "%d\n", &parentWindow->focusKey);
	fscanf (fileObj->fp, "%d\n", &parentWindow->closeKey);
	fscanf (fileObj->fp, "%d\n", &parentWindow->confirmKey);

	fscanf (fileObj->fp, "%d\n", &parentWindow->numWindows);

	CALLOC(parentWindow->windows,parentWindow->numWindows,Window);

	if (!parentWindow->windows)
	{
		ERROR_MESSAGE("ReloadParentWindow failed","4");
		// Return failure
	}

	G_endWindow = parentWindow->windows + parentWindow->numWindows;

	for (window = parentWindow->windows; window < G_endWindow; window++)
	{
		fscanf (fileObj->fp, "%d\n", &usingADT);

		if (usingADT)
		{
			fscanf (fileObj->fp, "%d %d", &window->dataADT.ADTSharing, &window->dataADT.type);

			switch (window->dataADT.ADTSharing)
			{
			case kSingleOwner:
				switch (window->dataADT.type)
				{
				case kList:
					window->dataADT.data.list = CreateList (kRing);

					break;	// Break out of switch statement

				case kQueue:
					window->dataADT.data.queue = CreateQueue ();

					break;	// Break out of switch statement

				case kStack:
					window->dataADT.data.stack = CreateStack ();

					break;	// Break out of switch statement

				default:
					ERROR_MESSAGE("Unsupported ADT style: ReloadParentWindow failed","5");
					// Return failure
				}

				break;	// Break out of switch statement

			case kShared:
				break;	// Break out of switch statement
			}
		}

		LoadWindowData (fileObj, window);

		dimensions = window->width * window->height;

		if (!AllocateBuffer (&window->display, dimensions))
		{
			ERROR_MESSAGE("ReloadParentWindow failed","6");
			// Return failure
		}

		switch (window->display.bufSharing)
		{
		case kSingleOwner:
/*			G_endCell = window->display.buffer + dimensions;

			for (G_cell = window->display.buffer; G_cell < G_endCell; G_cell++)
			{
				G_cell->Attributes = window->background;
			}*/
			G_end = window->display.buffer + dimensions;

			for (G_bufCell = window->display.buffer; G_bufCell < G_end; G_bufCell++)
			{
				G_bufCell->graph.Attributes = window->background;
			}

			break;	// Break out of switch statement

		case kShared:
			break;	// Break out of switch statement
		}

		switch (window->mode)
		{
		case kBasic:
			if (!LoadBasicWindow (fileObj, window))
			{
				ERROR_MESSAGE("ReloadParentWindow failed","7");
				// Return failure
			}

			break;	// Break out of switch statement

		case kEditBox:
			if (!LoadEditBoxWindow (fileObj, window))
			{
				ERROR_MESSAGE("ReloadParentWindow failed","8");
				// Return failure
			}

			break;	// Break out of switch statement

		case kMenu:
			if (!LoadMenuWindow (fileObj, window))
			{
				ERROR_MESSAGE("ReloadParentWindow failed","9");
				// Return failure
			}

			break;	// Break out of switch statement

		case kMessageBox:
			if (!LoadMessageBoxWindow (fileObj, window))
			{
				ERROR_MESSAGE("ReloadParentWindow failed","10");
				// Return failure
			}

			break;	// Break out of switch statement

		default:
			ERROR_MESSAGE("Unsupported mode: ReloadParentWindow failed","11");
			// Return failure
		}
	}

	fscanf (fileObj->fp, "%d\n", &parentWindow->state);

	return TRUE;
	// Return success
}

/********************************************************************
*	ReloadMenu - Load a menu from a file to memory					*
********************************************************************/

BOOL ReloadMenu (File * fileObj, Menu * menu, String filename)
{
	return TRUE;
	// Return success
}

/********************************************************************
*	ReloadMap - Load a map from a file to memory					*
********************************************************************/

BOOL ReloadMap (File * fileObj, Map * map, String filename)
{
	int dimensions;	// Area of map

	assert (fileObj && map && filename);
	// Verify that fileObj, map, and filename point to valid memory

	ReopenFile (fileObj, filename, kRead, kBinary);
	// Open the desired map

	fscanf (fileObj->fp, "%d %d\n", &map->width, &map->height);
	// Read map's width and height

	if (!map->width || !map->height)
	{
		ERROR_MESSAGE("ReloadMap failed","1");
		// Return failure
	}
	// Verify that map's width and height are non-zero

	fscanf (fileObj->fp, "%d %d\n", &map->xScroll, &map->yScroll);
	// Read map's horizontal and vertical scroll values

	dimensions = map->width * map->height;	// Calculate map's area

	if (!AllocateBuffer (&map->world, dimensions))
	{
		ERROR_MESSAGE("ReloadMap failed","2");
		// Return failure
	}

	switch (map->world.bufSharing)
	{
	case kSingleOwner:
		Inflate (fileObj, &map->world, dimensions);
		// Inflate map's visual data

		break;	// Break out of switch statement

	case kShared:
		break;	// Break out of switch statement
	}

	return TRUE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							File Writing							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	WriteConfigFile	- Primary wrapper for config file writing		*
********************************************************************/

/********************************************************************
*	Compress - Used to compress data stored within an output buffer	*
********************************************************************/

static void Compress (File * fileObj, OutputBuffer const * outputBuf, int dimensions)
{
	int i = 0;				// Loop variable
	int index = 0;			// Index variable
	int value;				// Index value
	int compressMode, width;// Mode used to compress data; width of region

	fprintf (fileObj->fp, "%d\n", fileObj->numIndices);
	// Write the count of compression indices

	while (i < fileObj->numIndices)
	{
		value = (fileObj->compression + i)->indexValue;	// Retrieve index value from compression table
		compressMode = (fileObj->compression + i)->mode;// Retrieve compress mode from compression table

		if (++i < fileObj->numIndices)
		{
			width = (fileObj->compression + i)->indexValue - value;
			// Calculate width of normal region in compression table
		}

		else
		{
			width = dimensions - value;
			// Calculate width of final entry in compression table
		}

		fprintf (fileObj->fp, "\n\n%d %d\n", compressMode, width);
		// Write compressMode and width at given index

		switch (compressMode)	// Get the compression mode
		{
		case kMerge:			// Merge case
			WriteMerge (fileObj, outputBuf, &index, width);
			// Write merged data at given index

			break;	// Break out of switch statement

		case kCompressZeroes:	// CompressZeroes case
			WriteCompressZeroes (fileObj, outputBuf, &index, width);
			// Write compressed-zero data at given index

			break;	// Break out of switch statement

		case kNotZero:			// NotZero case
			WriteNotZero (fileObj, outputBuf, &index, width);
			// Write non-zero data at given index

			break;	// Break out of switch statement

		case kConstantValue:	// ConstantValue case
			WriteConstantValue (fileObj, outputBuf, &index, width);
			// Write constant data at given index

			break;	// Break out of switch statement

		default:
			NORET_MESSAGE("Unsupported compressMode: Compress failed","1");
			// Return failure
		}
	}
}

/********************************************************************
*	WriteMerge - Output buffer cells are merged prior to writing	*
********************************************************************/

static void WriteMerge (File * fileObj, OutputBuffer const * outputBuf, intStar index, int width)
{
	int extent;	// Extent to write to
	int format;	// Format for data written

	extent = (*index) + width;	// Compute the extent to write

	for (*index; *index < extent; (*index)++)
	{
		format = MERGE(outputBuf,*index);
		// Merge the data to be written

		fprintf (fileObj->fp, "%d ", format);
		// Write formatted data
	}
}

/********************************************************************
*	WriteCompressZeroes - Zero chains are flattened before writing	*
********************************************************************/

static void WriteCompressZeroes (File * fileObj, OutputBuffer const * outputBuf, intStar index, int width)
{
	int extent;			// Extent to write to
	int numZeroes = 0;	// Count of zeroes
	int format;			// Format for data written

	extent = (*index) + width;	// Compute the extent to write

	for (*index; *index < extent; (*index)++)
	{
		format = MERGE(outputBuf,*index);
		// Merge the data to be written

		if (!format)	// Check for zero values
		{
			numZeroes++;
			// Increment the count of zero values
		}

		else
		{
			if (numZeroes)	// Check for zero chains
			{
				fprintf (fileObj->fp, "0 %d ", numZeroes);
				// Write the count of zeroes

				numZeroes = 0;	// Reset the count of zeroes
			}

			fprintf (fileObj->fp, "%d ", format);
			// Write formatted data
		}
	}

	if (numZeroes)	// Check for a final zero chain
	{
		fprintf (fileObj->fp, "0 %d ", numZeroes);
		// Write the count of zeroes
	}
}

/********************************************************************
*	WriteNotZero - Only non-zeroes values are written to the file	*
********************************************************************/

static void WriteNotZero (File * fileObj, OutputBuffer const * outputBuf, intStar index, int width)
{
	int i;				// Loop variable
	int extent;			// Extent to write to
	int notZero = 0;	// Count of non-zero values
	int format;			// Format for data written

	extent = (*index) + width;	// Compute the extent to write

	for (i = *index; i < extent; i++)
	{
		if (MERGE(outputBuf,i))	// Check for non-zero values
		{
			notZero++;
			// Increment the count of non-zero values
		}
	}

	fprintf (fileObj->fp, "%d%c", notZero, notZero ? '\n' : SPACE);
	// Write the count of non-zero values

	for (*index; *index < extent; (*index)++)
	{
		format = MERGE(outputBuf,*index);
		// Merge the data to be written

		if (format)	// Check for non-zero values
		{
			fprintf (fileObj->fp, "%d %d ", *index, format);
			// Write the index value and formatted data
		}
	}
}

/********************************************************************
*	WriteConstantValue - Constant chains are flattened				*
********************************************************************/

static void WriteConstantValue (File * fileObj, OutputBuffer const * outputBuf, intStar index, int width)
{
	int extent;				// Extent to write to
	int constant = 0, prev;	// Count of constant values; previous value
	int format;				// Format for data written

	extent = (*index) + width;	// Compute the extent to write

	for (*index; *index < extent; (*index)++)
	{
		format = MERGE(outputBuf,*index);
		// Merge the data to be written

		if (!constant)	// Check for first index read
		{
			constant = INITIAL_VALUE;	// Set constant to an initial value
		}

		else if (format == prev)	// Compare format to prev
		{
			constant++;
			// Increment the count of constant values
		}

		else
		{
			fprintf (fileObj->fp, "%d %d ", constant, prev);
			// Write the count of constant values and the previous value
			
			constant = INITIAL_VALUE;	// Set constant to an initial value
		}

		prev = format;	// Set prev to the value of format
	}

	fprintf (fileObj->fp, "%d %d ", constant, prev);
	// Write the final count of constant values and the previous value
}

/********************************************************************
*	WriteImage - Write an image from memory to a file				*
********************************************************************/

BOOL WriteImage (File * fileObj, Image const * image)
{
	int dimensions;	// Area of image

	assert (fileObj && image && fileObj->fp);
	// Verify that fileObj, image, and fileObj's fp field point to valid memory
	assert (image->width && image->height);
	// Verify that image's width and height values are non-zero
	assert (image->image.buffer);
//	assert (image->image.buffer && image->image.flags && image->image.data);
	// Verify that image's image field's buffer, flags, and data fields point to valid memory

	fprintf (fileObj->fp, "%d %d\n", image->width, image->height);
	// Write image's width and height

	dimensions = image->width * image->height;	// Calculate image's area

	Compress (fileObj, &image->image, dimensions);
	// Compress image's visual data

	return TRUE;
	// Return success
}

/********************************************************************
*	WriteMap - Write a map from memory to a file					*
********************************************************************/

BOOL WriteMap (File * fileObj, Map const * map)
{
	int dimensions;	// Area of map

	assert (fileObj && map && fileObj->fp);
	// Verify that fileObj, map, and fileObj's fp field point to valid memory
	assert (map->width && map->height);
	// Verify that map's width and height values are non-zero
	assert (map->world.buffer);
//	assert (map->world.buffer && map->world.flags && map->world.data);
	// Verify that map's world field's buffer, flags, and data fields point to valid memory

	fprintf (fileObj->fp, "%d %d\n", map->width, map->height);
	// Write map's width and height

	fprintf (fileObj->fp, "%d %d\n", map->xScroll, map->yScroll);
	// Write map's horizontal and vertical scroll values

	dimensions = map->width * map->height;	// Calculate map's area

	Compress (fileObj, &map->world, dimensions);
	// Compress map's visual data

	return TRUE;
	// Return success
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Destruction								*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	DeinitializeFileObject - Deinitialize a file object				*
********************************************************************/

BOOL DeinitializeFileObject (File * fileObj)
{
	ShutFile (fileObj);
	// Close active file

	FREE(fileObj->compression);
	// Free memory pointed to by fileObj's compression field

	return TRUE;
	// Return success
}