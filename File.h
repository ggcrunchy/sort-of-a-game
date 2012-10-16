/********************************************************************
*																	*
*							File.h									*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains information relevant to file management	*
*																	*
********************************************************************/

#ifndef FILE_H
#define FILE_H

#include "Common.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Defines									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define MAXCOMPRESSIONINDICES	100
// Maximum value for number of compression indices available

#define INITIAL_VALUE			1
// Used to set initial values in various contexts

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Macros									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define SEPARATE(buf,index,format) G_bufCell						= (buf)->buffer + (index),	\
								   G_bufCell->flags					= LOBYTE(LOWORD(format)),	\
								   G_bufCell->data					= HIBYTE(LOWORD(format)),	\
								   G_bufCell->graph.Char.AsciiChar	= LOBYTE(HIWORD(format)),	\
								   G_bufCell->graph.Attributes		= HIBYTE(HIWORD(format))

							/*     *((buf)->flags + (index)) = LOBYTE(LOWORD(format)),					\
								   *((buf)->data + (index)) = HIBYTE(LOWORD(format)),					\
									((buf)->buffer + (index))->Char.AsciiChar = LOBYTE(HIWORD(format)), \
									((buf)->buffer + (index))->Attributes = HIBYTE(HIWORD(format))*/
// Used to separate merged data

#define MERGE(buf,index)		   MAKELONG(MAKEWORD(((buf)->buffer + (index))->flags,					\
													 ((buf)->buffer + (index))->data),					\
										    MAKEWORD(((buf)->buffer + (index))->graph.Char.AsciiChar,	\
													 ((buf)->buffer + (index))->graph.Attributes))

							/*	   MAKELONG(MAKEWORD(*((buf)->flags + (index)),*((buf)->data + (index))),								\
								    		 MAKEWORD(((buf)->buffer + (index))->Char.AsciiChar,((buf)->buffer + (index))->Attributes))*/
// Used to merge data

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							File access functions					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static void MakePrimary (pFile fileObj, fileStar secondary);

static void MakeSecondary (pFile fileObj, fileStar secondary);

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

BOOL InitializeFileObject (pFile fileObj);

/********************************************************************
*	AllocateBuffer - Load memory into an OutputBuffer structure		*
********************************************************************/

BOOL AllocateBuffer (pOutputBuffer outputBuf, int dimensions);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							File management							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void ReopenFile (pFile fileObj, String filename, FileAccess access, FileStyle style);

void ShutFile (pFile fileObj);

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

static void LoadTextSequence (pFile fileObj, String string);

/********************************************************************
*	LoadWindowData - Load common data into a Window structure		*
********************************************************************/

static void LoadWindowData (pFile fileObj, pWindow window);

/********************************************************************
*	LoadSeparators - Load separators into a parent window			*
********************************************************************/

static BOOL LoadSeparators (pFile fileObj, pParentWindow parentWindow);

/********************************************************************
*	LoadVisuals - Load visuals into a basic window					*
********************************************************************/

static BOOL LoadVisuals (pFile fileObj, pWindow window);

/********************************************************************
*	LoadBasicWindow - Load basic window								*
********************************************************************/

static BOOL LoadBasicWindow (pFile fileObj, pWindow window);

/********************************************************************
*	LoadEditBoxWindow - Load edit box window						*
********************************************************************/

static BOOL LoadEditBoxWindow (pFile fileObj, pWindow window);

/********************************************************************
*	LoadMenuWindow - Load menu window								*
********************************************************************/

static BOOL LoadMenuWindow (pFile fileObj, pWindow window);

/********************************************************************
*	LoadMessageBoxWindow - Load message box window					*
********************************************************************/

static BOOL LoadMessageBoxWindow (pFile fileObj, pWindow window);

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

static void Inflate (pFile fileObj, pOutputBuffer outputBuf, int dimensions);

/********************************************************************
*	ReadMerge - Merged data is parsed and placed into output buffer	*
********************************************************************/

static void ReadMerge (pFile fileObj, pOutputBuffer outputBuf, intStar index, int width);

/********************************************************************
*	ReadCompressZeroes - Zero chains are inflated during reading	*
********************************************************************/

static void ReadCompressZeroes (pFile fileObj, pOutputBuffer outputBuf, intStar index, int width);

/********************************************************************
*	ReadNotZero - Non-zero entries are placed into an output buffer	*
********************************************************************/

static void ReadNotZero (pFile fileObj, pOutputBuffer outputBuf, intStar index, int width);

/********************************************************************
*	ReadConstantValue - Constant chains are inflated during reading	*
********************************************************************/

static void ReadConstantValue (pFile fileObj, pOutputBuffer outputBuf, intStar index, int width);

/********************************************************************
*	ReloadImage - Load an image from a file to memory				*
********************************************************************/

BOOL ReloadImage (pFile fileObj, pImage image, String filename);

/********************************************************************
*	ReloadAnimation - Load an animation from a file to memory		*
********************************************************************/

BOOL ReloadAnimation (pFile fileObj, pAnimation animation, String filename);

/********************************************************************
*	ReloadPattern - Load a pattern from a file to memory			*
********************************************************************/

BOOL ReloadPattern (pFile fileObj, pPattern pattern, String filename);

/********************************************************************
*	ReloadPointSet - Load a point set from a file to memory			*
********************************************************************/

BOOL ReloadPointSet (pFile fileObj, pPointSet pointSet, String filename);

/********************************************************************
*	ReloadParentWindow - Load a parent window from a file to memory	*
********************************************************************/

BOOL ReloadParentWindow (pFile fileObj, pParentWindow parentWindow, String filename);

/********************************************************************
*	ReloadMenu - Load a menu from a file into memory				*
********************************************************************/

BOOL ReloadMenu (pFile fileObj, pMenu menu, String filename);

/********************************************************************
*	ReloadMap - Load a map from a file into memory					*
********************************************************************/

BOOL ReloadMap (pFile fileObj, pMap map, String filename);

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

static void Compress (pFile fileObj, OutputBuffer const * outputBuf, int dimensions);

/********************************************************************
*	WriteMerge - Output buffer cells are merged prior to writing	*
********************************************************************/

static void WriteMerge (pFile fileObj, OutputBuffer const * outputBuf, intStar index, int width);

/********************************************************************
*	WriteCompressZeroes - Zero chains are flattened before writing	*
********************************************************************/

static void WriteCompressZeroes (pFile fileObj, OutputBuffer const * outputBuf, intStar index, int width);

/********************************************************************
*	WriteNotZero - Only non-zeroes values are written to the file	*
********************************************************************/

static void WriteNotZero (pFile fileObj, OutputBuffer const * outputBuf, intStar index, int width);

/********************************************************************
*	WriteConstantValue - Constant chains are flattened				*
********************************************************************/

static void WriteConstantValue (pFile fileObj, OutputBuffer const * outputBuf, intStar index, int width);

/********************************************************************
*	WriteImage - Write an image from memory to a file				*
********************************************************************/

BOOL WriteImage (pFile fileObj, Image const * image);

/********************************************************************
*	WritePattern - Write a pattern from memory to a file			*
********************************************************************/

BOOL WritePattern (pFile fileObj, Pattern const * pattern);

/********************************************************************
*	WritePointSet - Write a point set from memory to a file			*
********************************************************************/

BOOL WritePointSet (pFile fileObj, PointSet const * pointSet);

/********************************************************************
*	WriteMap - Write a map from memory to a file					*
********************************************************************/

BOOL WriteMap (pFile fileObj, Map const * map);

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

BOOL DeinitializeFileObject (pFile fileObj);

#endif