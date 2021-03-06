/********************************************************************
*																	*
*							Scene.h									*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains information relevant to scene management	*
*																	*
********************************************************************/

#ifndef SCENE_H
#define SCENE_H

#include "Common.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Scene access functions					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Construction							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	InitializeSceneObject - Initialize primary scene object			*
********************************************************************/

BOOL InitializeSceneObject (pScene sceneObj);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Cutscene management						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	KeywordLookup - Lookup a keyword for evaluation					*
********************************************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Cutscene methods						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	BeginScene - Initializes a scene for playback					*
********************************************************************/

void BeginScene (pScene sceneObj, dataType type, long lParam);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Destruction								*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	DeinitializeSceneObject - Deinitialize a scene object			*
********************************************************************/

BOOL DeinitializeSceneObject (pScene sceneObj);

#endif