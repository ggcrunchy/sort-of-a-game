/********************************************************************
*																	*
*							Common.h								*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains information relevant to entire SDK			*
*																	*
********************************************************************/

#ifndef COMMON_H
#define COMMON_H

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Includes								*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Defines									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define SCREEN_WIDTH	80
#define SCREEN_HEIGHT	50
// Designate the display area dimensions

#define CURSOR_HEIGHT 100
// Designate the height of cursor, if displayed

#define WORD_LENGTH 80
// Designate the maximum length of specialized character buffers

#define SPACE			  '\x20'
// Character used to denote a space
#define UNDERSCORE		  '\x5F'
// Character used to denote an underscore
#define END				  '\x00'
// Character used to seal off text strings

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Macros									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define MAKEBYTE(lonybble,hinybble) ((BYTE) (((hinybble) & 0xF) << 4)) \
									| ((lonybble) & 0xF)
// Used to compose bytes

#define LONYBBLE(byte) (byte) & 0xF
#define HINYBBLE(byte) ((byte) >> 4) & 0xF
// Used to extract pieces of bytes

#define SETFLAG(x,flag)	  (x) |= (flag)
#define FLAGSET(x,flag)	  ((x) & (flag))
#define CLEARFLAG(x,flag) (x) &= ~(flag)
// Used to set variables' flow-of-control bits

#define SETBIT(x,bit)	(x) |= 1 << (bit)
#define BITSET(x,bit)	((x) & (1 << (bit)))
#define CLEARBIT(x,bit) (x) &= ~(1 << (bit))
// Used to set variables' flow-of-control bits

#define FLIPFLAG(x,flag) (x) ^= (flag)
#define FLIPBIT(x,bit)	 (x) ^= 1 << (bit)	   
// Used to toggle variables' flow-of-control bits

#define MALLOC(x,type)	   (x) = (type *) malloc (sizeof (type)),		  \
						   ZeroMemory ((x), sizeof (type))
#define CALLOC(x,num,type) (x) = (type *) malloc ((num) * sizeof (type)), \
						   ZeroMemory ((x), (num) * sizeof (type))
#define FREE(x)			   free (x),									  \
						   (x) = NULL
// Used to malloc and free memory

#define NOBREAK_MESSAGE(message,error) MessageBox (NULL, (message), "Error" ## error, MB_OK);
#define NORET_MESSAGE(message,error)   MessageBox (NULL, (message), "Error" ## error, MB_OK); \
									   return
#define ERROR_MESSAGE(message,error)   MessageBox (NULL, (message), "Error" ## error, MB_OK); \
									   return FALSE
#define NULL_MESSAGE(message,error)	   MessageBox (NULL, (message), "Error" ## error, MB_OK); \
									   return NULL
// Used to produce an error message

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Universal data structures				*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*																	*
*							Enumeration: _dataType					*
*																	*
*	Purpose:	Descriptor for a type of data						*
*																	*
********************************************************************/

typedef enum _dataType {
	kChar, kShort, kInt, kLong, kFloat, kDouble, kLongDouble,	// Primitives
	kImage,														// Image
	kPattern,													// Pattern
	kAnimation,													// Animation
	kMap,														// Map
	kMenus,														// Menu
	kWindow,													// Window
	kParentWindow,												// ParentWindow
	kList,														// Linked list
	kQueue,														// Queue
	kStack,														// Stack
	kHydra,														// Hydra
	kUserDefined,												// User-defined data
	kDefinedTypesCount											// Count of defined types
} dataType;

/********************************************************************
*																	*
*							Enumeration: _exclusivity				*
*																	*
*	Purpose:	Descriptor for a sharing level						*
*																	*
********************************************************************/

typedef enum _exclusivity {
	kSingleOwner,			// Entry is specific to one datum
	kShared,				// Entry is shared among several data
	kExclusiveLevelsCount	// Count of available exclusive levels
} exclusivity;

/********************************************************************
*																	*
*							Alias: voidStar							*
*																	*
*	Purpose:	Used to encapsulate and abstract "void *"			*
*																	*
********************************************************************/

typedef void * voidStar;

/********************************************************************
*																	*
*							Alias: String							*
*																	*
*	Purpose:	Used to encapsulate and abstract "char *"			*
*																	*
********************************************************************/

typedef char * String;

/********************************************************************
*																	*
*							Alias: shortStar						*
*																	*
*	Purpose:	Used to encapsulate and abstract "short *"			*
*																	*
********************************************************************/

typedef short * shortStar;

/********************************************************************
*																	*
*							Alias: intStar							*
*																	*
*	Purpose:	Used to encapsulate and abstract "int *"			*
*																	*
********************************************************************/

typedef int * intStar;

/********************************************************************
*																	*
*							Alias: longStar							*
*																	*
*	Purpose:	Used to encapsulate and abstract "long *"			*
*																	*
********************************************************************/

typedef long * longStar;

/********************************************************************
*																	*
*							Alias: floatStar						*
*																	*
*	Purpose:	Used to encapsulate and abstract "float *"			*
*																	*
********************************************************************/

typedef float * floatStar;

/********************************************************************
*																	*
*							Alias: doubleStar						*
*																	*
*	Purpose:	Used to encapsulate and abstract "double *"			*
*																	*
********************************************************************/

typedef double * doubleStar;

/********************************************************************
*																	*
*							Alias: longdoubleStar					*
*																	*
*	Purpose:	Used to encapsulate and abstract "long double *"	*		*
*																	*
********************************************************************/

typedef long double * longdoubleStar;

/********************************************************************
*																	*
*							Alias: fileStar							*
*																	*
*	Purpose:	Used to encapsulate and abstract "FILE *" data		*
*																	*
********************************************************************/

typedef FILE * fileStar;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							ADT data structures						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*																	*
*							Enumeration: _ADTStyle					*
*																	*
*	Purpose:	Descriptor for a type of ADT						*
*																	*
********************************************************************/

typedef enum _ADTStyle {
	kADTList,		// Linked list
	kADTQueue,		// Queue
	kADTStack,		// Stack
	kADTStylesCount // Count of available ADT styles
} ADTStyle;

/********************************************************************
*																	*
*							Enumeration: _listMode					*
*																	*
*	Purpose:	Descriptor for a means of managing a linked list	*
*																	*
********************************************************************/

typedef enum _listMode {
	kNormal,		// Normal linked list
	kRing,			// Circular linked list
	kListModesCount // Count of available list modes
} listMode;

/********************************************************************
*																	*
*							Aggregate: _ListNode					*
*																	*
*	Purpose:	Used to compose a linked list ADT					*
*	Fields:															*
*		> value		- Storage for generic data						*
*		> prev		- Pointer to previous node in list				*
*		> next		- Pointer to subsequent node in list			*
*		> type		- Descriptor of type of data stored in node		*
*		> remove	- Method for user-defined data removal			*
*																	*
********************************************************************/

typedef struct _ListNode {
	voidStar value;
	struct _ListNode * prev;
	struct _ListNode * next;
	dataType type;
	int (* remove) (voidStar data);
} ListNode, * pListNode;

/********************************************************************
*																	*
*							Aggregate: _ListHead					*
*																	*
*	Purpose:	Used to compose a linked list ADT; primary object	*
*	Fields:															*
*		> numNodes	- Count of list nodes							*
*		> mode		- Descriptor of the means of implementing list	*
*		> head		- Head of linked list							*
*																	*
********************************************************************/

typedef struct _ListHead {
	int numNodes;
	listMode mode;
	pListNode head;
} ListHead, * pListHead;

/********************************************************************
*																	*
*							Aggregate: _Hydra						*
*																	*
*	Purpose:	Used to store an array of linked lists				*
*	Fields:															*
*		> numHeads	- Count of list heads							*
*		> listHeads - Array of list heads							*
*																	*
********************************************************************/

typedef struct _Hydra {
	int numHeads;
	pListHead * listHeads;
} Hydra, * pHydra;

/********************************************************************
*																	*
*							Aggregate: _StackNode					*
*																	*
*	Purpose:	Used to compose a stack ADT							*
*	Fields:															*
*		> value		- Storage for generic data						*
*		> next		- Pointer to subsequent node in stack			*
*		> type		- Descriptor of type of data stored in node		*
*		> remove	- Method for user-defined data removal			*
*																	*
********************************************************************/

typedef struct _StackNode {
	voidStar value;
	struct _StackNode * next;
	dataType type;
	int (* remove) (voidStar data);
} StackNode, * pStackNode;

/********************************************************************
*																	*
*							Aggregate: _Stack						*
*																	*
*	Purpose:	Used to compose a stack ADT; primary object			*
*	Fields:															*
*		> numNodes	- Count of stack nodes							*
*		> top		- Top of stack									*
*																	*
********************************************************************/

typedef struct _Stack {
	int numNodes;
	pStackNode top;
} Stack, * pStack;

/********************************************************************
*																	*
*							Aggregate: _QueueNode					*
*																	*
*	Purpose:	Used to compose a queue ADT							*
*	Fields:															*
*		> value		- Storage for generic data						*
*		> next		- Pointer to subsequent node in queue			*
*		> type		- Descriptor of type of data stored in node		*
*		> remove	- Method for user-defined data removal			*
*																	*
********************************************************************/

typedef struct _QueueNode {
	voidStar value;
	struct _QueueNode * next;
	dataType type;
	int (* remove) (voidStar data);
} QueueNode, * pQueueNode;

/********************************************************************
*																	*
*							Aggregate: _Queue						*
*																	*
*	Purpose:	Used to compose a queue ADT; primary object			*
*	Fields:															*
*		> numNodes	- Count of list nodes							*
*		> tail		- Tail of queue									*
*																	*
********************************************************************/

typedef struct _Queue {
	int numNodes;
	pQueueNode tail;
} Queue, * pQueue;

/********************************************************************
*																	*
*							Aggregate: _ADTUnion					*
*																	*
*	Purpose:	To store a variant style ADT						*
*	Members:														*
*		> list	- A linked list member								*
*		> queue	- A queue member									*
*		> stack - A stack member									*
*		> data	- Used to generically ensure data storage			*
*																	*
********************************************************************/

typedef union _ADTUnion {
	pListHead list;
	pQueue queue;
	pStack stack;
	voidStar data;
} ADTUnion, * pADTUnion;

/********************************************************************
*																	*
*							Aggregate: _ADT							*
*																	*
*	Purpose:	Representation of an abstract data type				*
*	Fields:															*
*		> data			- ADT member								*
*		> type			- Type of ADT stored as member				*
*		> ADTSharing	- Indicates whether ADT is shared			*
*																	*
********************************************************************/

typedef struct _ADT {
	ADTUnion data;
	ADTStyle type;
	exclusivity ADTSharing;
} ADT, * pADT;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Input data structures					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*																	*
*							Enumeration: _InputStyle				*
*																	*
*	Purpose:	Descriptor for a means of input retrieval			*
*																	*
********************************************************************/

typedef enum _InputStyle {
	kSync,				// Synchronous input
	kAsync,				// Asynchronous input
	kInputStylesCount	// Count of available input styles
} InputStyle;

/********************************************************************
*																	*
*							Aggregate: _MouseInfo					*
*																	*
*	Purpose:	Used to manage mouse information					*
*	Fields:															*
*		> leftClicked	- Indicates left mouse button is down		*
*		> rightClicked	- Indicates right mouse button is down		*
*		> doubleClicked - Indicates that a double-click occurred	*
*		> mouseMoved	- Indicates that the mouse was moved		*
*		> wheelUsed		- Indicates that the mouse was wheeled		*
*		> mouseCoord	- The current mouse position				*
*																	*
********************************************************************/

typedef struct _MouseInfo {
	BOOL leftClicked;
	BOOL rightClicked;
	BOOL doubleClicked;
	BOOL mouseMoved;
	BOOL wheelUsed;
	COORD mouseCoord;
} MouseInfo, * pMouseInfo;

/********************************************************************
*																	*
*							Aggregate: _InputInfo					*
*																	*
*	Purpose:	Used to store application-defined information		*
*	Fields:															*
*		> numItems	- Count of infoItems							*
*		> infoItems - Array of infoItems
*																	*
********************************************************************/

typedef struct _InputInfo {
	int numItems;
	PBYTE infoItems;
} InputInfo, * pInputInfo;

/********************************************************************
*																	*
*							Aggregate: _Input						*
*																	*
*	Purpose:	Primary input management object						*
*	Fields:															*
*		> inputH		- An input handle							*
*		> Count			- Count of input events						*
*		> mouse			- Mouse information							*
*		> inputRec		- An input record							*
*		> lastKeycode	- Last keycode retrieved as input			*
*		> information	- Application-defined information			*
*		> lastChar		- Last character retrieved as input			*
*																	*
********************************************************************/

typedef struct _Input {
	HANDLE inputH;
	DWORD Count;
	MouseInfo mouse;
	INPUT_RECORD inputRec;
	int lastKeycode;
	pInputInfo information;
	char lastChar;
} Input, * pInput;

/********************************************************************
*																	*
*							Aggregate: _Key							*
*																	*
*	Purpose:	Used to manage input specific to a given key		*
*	Fields:															*
*		> keycode	- Keycode value									*
*		> function	- Method called by key when triggered			*
*																	*
********************************************************************/

typedef struct _Key {
	int keycode;
	int (* function) (voidStar data, dataType type, long lParam);
} Key, * pKey;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Mathematics data structures				*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*																	*
*							Enumeration: _AngleMode					*
*																	*
*	Purpose:	Descriptor for a means of computing with angles		*
*																	*
********************************************************************/

typedef enum _AngleMode {
	kDegrees,		// Evaluate degrees
	kGradians,		// Evaluate gradians
	kRadians,		// Evaluate radians
	kAngleModesCount// Count of available angle modes
} AngleMode;

/********************************************************************
*																	*
*							Enumeration: _BezierMode				*
*																	*
*	Purpose:	Descriptor for a means of computing a Bézier curve	*
*																	*
********************************************************************/

typedef enum _BezierMode {
	kQuadratic,			// Quadratic curve
	kCubic,				// Cubic curve
	kQuartic,			// Quartic curve
	kQuintic,			// Quintic curve
	kBezierModesCount	// Count of available Bezier modes
} BezierMode;

/********************************************************************
*																	*
*							Enumeration: _Intersection				*
*																	*
*	Purpose:	Descriptor for computing vector intersections		*
*																	*
********************************************************************/

typedef enum _Intersection {
	kVectorsParallel,		// Parallel vectors
	kVectorsSkew,			// Skew vectors
	kVectorsIntersect,		// Vectors do intersect
	kVectorsWillIntersect,	// Vectors will intersect
	kIntersectionsCount		// Count of available intersections
} Intersection;

/********************************************************************
*																	*
*							Enumeration: _StandardVector			*
*																	*
*	Purpose:	Descriptor for standard vectors						*
*																	*
********************************************************************/

typedef enum _StandardVector {
	kX_Axis,				// X-axis vector
	kY_Axis,				// Y-axis vector
	kZ_Axis,				// Z-axis vector
	kStandardVectorsCount	// Count of available standard vectors
} StandardVector;

/********************************************************************
*																	*
*							Aggregate: _Point2						*
*																	*
*	Purpose:	Two-dimensional point used to indicate location		*
*	Fields:															*
*		> x	- x coordinate											*
*		> y	- y coordinate											*
*																	*
********************************************************************/

typedef struct _Point2 {
	float x, y;
} Point2, * pPoint2;

/********************************************************************
*																	*
*							Aggregate: _Vector2						*
*																	*
*	Purpose:	Two-dimensional vector indicating displacement		*
*	Fields:															*
*		> x		- x component										*
*		> y		- y component										*
*		> tail	- Point where vector is rooted in affine space		*
*																	*
********************************************************************/

typedef struct _Vector2 {
	float x, y;
	Point2 tail;
} Vector2, * pVector2;

/********************************************************************
*																	*
*							Aggregate: _Point3						*
*																	*
*	Purpose:	Three-dimensional points used to indicate location	*
*	Fields:															*
*		> x	- x coordinate											*
*		> y - y coordinate											*
*		> z - z coordinate											*
*																	*
********************************************************************/

typedef struct _Point3 {
	float x, y, z;
} Point3, * pPoint3;

/********************************************************************
*																	*
*							Aggregate: _Vector3						*
*																	*
*	Purpose:	Three-dimensional vector indicating displacement	*
*	Fields:															*
*		> x		- x component										*
*		> y		- y component										*
*		> z		- z component										*
*		> tail	- Point where vector is rooted in affine space		*
*																	*
********************************************************************/

typedef struct _Vector3 {
	float x, y, z;
	Point3 tail;
} Vector3, * pVector3;

/********************************************************************
*																	*
*							Aggregate: _Curve2						*
*																	*
*	Purpose:	Two-dimensional representation of a Bézier curve	*
*	Fields:															*
*		> start			- Initial endpoint							*
*		> end			- Terminal endpoint							*
*		> vector		- Vector between endpoints					*
*		> numControls	- Count of control points					*
*		> control		- Array of control points					*
*		> mode			- Bézier mode								*
*																	*
********************************************************************/

typedef struct _Curve2 {
	Point2 start, end;
	Vector2 vector;
	int numControls;
	pPoint2 control;
	BezierMode mode;
} Curve2, * pCurve2;

/********************************************************************
*																	*
*							Aggregate: _Curve3						*
*																	*
*	Purpose:	Three-dimensional representation of a Bézier curve	*
*	Fields:															*
*		> start			- Initial endpoint							*
*		> end			- Terminal endpoint							*
*		> vector		- Vector between endpoints					*
*		> numControls	- Count of control points					*
*		> control		- Array of control points					*
*		> mode			- Bézier mode								*
*																	*
********************************************************************/

typedef struct _Curve3 {
	Point3 start, end;
	Vector3 vector;
	int numControls;
	pPoint3 control;
	BezierMode mode;
} Curve3, * pCurve3;

/********************************************************************
*																	*
*							Aggregate: _PointSet					*
*																	*
*	Purpose:	A set of three-dimensional points					*
*	Fields:															*
*		> numPoints	- Count of points								*
*		> points	- Array of points								*
*		> axis		- Axis to revolve points around					*
*		> mode		- Mode describing units of rotation				*
*		> lock		- Indicates whether point set is locked			*
*																	*
********************************************************************/

typedef struct _PointSet {
	int numPoints;
	pPoint3 points;
	StandardVector axis;
	AngleMode mode;
	BOOL lock;
} PointSet, * pPointSet;

/********************************************************************
*																	*
*							Aggregate: _Triangle					*
*																	*
*	Purpose:	Representation of a triangle						*
*	Fields:															*
*		> points	- Array of two-dimensional integral points		*
*		> edges		- Array of three-dimensional vectors			*
*		> color		- Color used to fill triangle					*
*		> character	- Character used to fill triangle				*
*																	*
********************************************************************/

typedef struct _Triangle {
	POINT points [3];
	Vector3 edges [3];
	BYTE color;
	char character;
} Triangle, * pTriangle;

/********************************************************************
*																	*
*							Aggregate: _Triangulation				*
*																	*
*	Purpose:	A set of three-dimensional triangles				*
*	Fields:															*
*		> numTriangles	- Count of triangles						*
*		> triangles		- Array of triangles						*
*		> lock			- Indicates whether triangulation is locked	*
*																	*
********************************************************************/

typedef struct _Triangulation {
	int numTriangles;
	pTriangle triangles;
	BOOL lock;
} Triangulation, * pTriangulation;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Output data structures					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*																	*
*							Enumeration: _HorzScroll				*
*																	*
*	Purpose:	Descriptor for handling horizontal scrolling		*
*																	*
********************************************************************/

typedef enum _HorzScroll {
	kLeft,				// Left
	kRight,				// Right
	kHorzFix,			// No horizontal scroll
	kHorzScrollsCount	// Count of available horizontal scrolling modes
} HorzScroll;

/********************************************************************
*																	*
*							Enumeration: _VertScroll				*
*																	*
*	Purpose:	Descriptor for handling vertical scrolling			*
*																	*
********************************************************************/

typedef enum _VertScroll {
	kUp,				// Upward
	kDown,				// Downward
	kVertFix,			// No vertical scroll
	kVertScrollsCount	// Count of available vertical scrolling modes
} VertScroll;

/********************************************************************
*																	*
*							Enumeration: _AnimationPath				*
*																	*
*	Purpose:	Descriptor for computing an animation's path		*
*																	*
********************************************************************/

typedef enum _AnimationPath {
	kStationary,		// Animation remains stationary
	kLinear,			// Animation follows a line
	kBezier,			// Animation traces a Bézier curve
	kAnimationPathsCount// Count of available animation paths
} AnimationPath;

/********************************************************************
*																	*
*							Enumeration: _Quadrant					*
*																	*
*	Purpose:	Descriptor for quadrant of some visual data			*
*																	*
********************************************************************/

typedef enum _Quadrant {
	kQuadOne,		// Quadrant one
	kQuadTwo,		// Quadrant two
	kQuadThree,		// Quadrant three
	kQuadFour,		// Quadrant four
	kQuadrantsCount	// Count of available quadrants
} Quadrant;

/********************************************************************
*																	*
*							Aggregate: _ScreenBuffer				*
*																	*
*	Purpose:	Back buffer drawn to before updating to output		*
*	Fields:															*
*		> buffer		- Storage for display information			*
*		> outputRect	- Region to draw to							*
*																	*
********************************************************************/

typedef struct _ScreenBuffer {
	CHAR_INFO buffer [SCREEN_HEIGHT] [SCREEN_WIDTH];
	SMALL_RECT outputRect;
} ScreenBuffer, * pScreenBuffer;

/********************************************************************
*																	*
*							Aggregate: _Output						*
*																	*
*	Purpose:	Primary output management object					*
*	Fields:															*
*		> outputH	- An output handle								*
*		> outputBuf	- Screen buffer									*
*		> state		- Output's state information					*
*																	*
********************************************************************/

typedef struct _Output {
	HANDLE outputH;
	ScreenBuffer outputBuf;
	int state;
} Output, * pOutput;

/********************************************************************
*																	*
*							Aggregate: _Cell						*
*																	*
*	Purpose:	Used to compose visual information					*
*	Fields:															*
*		> graph	- Visual information								*
*		> flags	- Application-defined control flags					*
*		> data	- Application-defined data storage					*
*																	*
********************************************************************/

typedef struct _Cell {
	CHAR_INFO graph;
	BYTE flags;
	BYTE data;
} Cell, * pCell;

/********************************************************************
*																	*
*							Aggregate: _OutputBuffer				*
*																	*
*	Purpose:	Storage for important display information			*
*	Fields:															*
*		> buffer		- Visual data								*
*		> flags			- Application-defined control flags			*
*		> data			- Application-defined data storage			*
*		> bufSharing	- Indicates whether output buffer is shared	*
*																	*
********************************************************************/

typedef struct _OutputBuffer {
//	PCHAR_INFO buffer;
//	PBYTE flags;
//	PBYTE data;
	pCell buffer;
	exclusivity bufSharing;
} OutputBuffer, * pOutputBuffer;

/********************************************************************
*																	*
*							Aggregate: _Clipper						*
*																	*
*	Purpose:	Used to clip images prior to display				*
*	Fields:															*
*		> clipRegion	- Corners of displayable image region		*
*		> visible		- Indicates that image is displayable		*
*																	*
********************************************************************/

typedef struct _Clipper {
	RECT clipRegion;
	BOOL visible;
} Clipper, * pClipper;

/********************************************************************
*																	*
*							Aggregate: _Image						*
*																	*
*	Purpose:	Representation of an image							*
*	Fields:															*
*		> width		- Width of an image								*
*		> height	- Height of an image							*
*		> location	- Location of image relative to screen			*
*		> image		- Output information of image					*
*																	*
********************************************************************/

typedef struct _Image {
	int width;
	int height;
	COORD location;
	OutputBuffer image;
} Image, * pImage;

/********************************************************************
*																	*
*							Aggregate: _Wrapper						*
*																	*
*	Purpose:	Used to wrap patterns prior to display				*
*	Fields:															*
*		> firstX		- Primary horizontal offset					*
*		> secondX		- Secondary horizontal offset				*
*		> firstY		- Primary vertical offset					*
*		> secondY		- Secondary vertical offset					*
*		> firstWidth	- Primary width								*
*		> secondWidth	- Secondary width							*
*		> firstHeight	- Primary height							*
*		> secondHeight	- Secondary height							*
*		> firstPitch	- Primary pitch								*
*		> secondPitch	- Secondary pitch							*	 
*																	*
********************************************************************/

typedef struct _Wrapper {
	int firstX, secondX;
	int firstY, secondY;
	int firstWidth, secondWidth;
	int firstHeight, secondHeight;
	int firstPitch, secondPitch;
} Wrapper, * pWrapper;

/********************************************************************
*																	*
*							Aggregate: _Pattern						*
*																	*
*	Purpose:	Representation of a pattern							*
*	Fields:															*
*		> width		- Width of a pattern							*
*		> height	- Height of a pattern							*
*		> lock		- Indicates whether pattern is locked			*
*		> location	- Location of pattern relative to screen		*
*		> xScroll	- Amount to scroll pattern horizontally			*
*		> yScroll	- Amount to scroll pattern vertically			*
*		> xOffset	- Horizontal offset of pattern					*
*		> yOffset	- Vertical offset of pattern					*
*		> pattern	- Output information of pattern					*
*																	*
********************************************************************/

typedef struct _Pattern {
	int width;
	int height;
	BOOL lock;
	COORD location;
	int xScroll, yScroll;
	int xOffset, yOffset;
	OutputBuffer pattern;
} Pattern, * pPattern;

/********************************************************************
*																	*
*							Aggregate: _AnimationState				*
*																	*
*	Purpose:	Storage for a state in an animation					*
*	Fields:															*
*		> numFrames	- Count of images								*
*		> frames	- Array of images								*
*		> curFrame	- Index of current frame						*
*																	*
********************************************************************/

typedef struct _AnimationState {
	int numFrames;
	pImage frames;
	float curFrame;
} AnimationState, * pAnimationState;

/********************************************************************
*																	*
*							Aggregate: _Animation					*
*																	*
*	Purpose:	Representation of an animation						*
*	Fields:															*
*		> activeImage	- Image at current state and frame			*
*		> numStates		- Count of animation states					*
*		> states		- Array of animation states					*
*		> curState		- Current animation state					*
*		> frameRate		- Amount to increment current frame			*
*		> t				- Parameter of animation along path			*
*		> stepSize		- Amount to increment t						*
*		> globalLoc		- Global location of animation				*
*		> path			- Path animation traces						*
*		> type			- Descriptor of means of tracing path		*
*																	*
********************************************************************/

typedef struct _Animation {
	pImage activeImage;
	int numStates;
	pAnimationState states;
	int curState;
	float frameRate;
	float t;
	float stepSize;
	Point2 globalLoc;
	Curve2 path;
	AnimationPath type;
} Animation, * pAnimation;

/********************************************************************
*																	*
*							Aggregate: _Visuals						*
*																	*
*	Purpose:	Composition of visual data							*
*	Fields:															*
*		> numImages		- Count of images							*
*		> images		- Array of images							*
*		> numPatterns	- Count of patterns							*
*		> patterns		- Array of patterns							*
*		> numAnimations	- Count of animations						*
*		> animations	- Array of animations						*
*																	*
********************************************************************/

typedef struct _Visuals {
	int numImages;
	pImage images;
	int numPatterns;
	pPattern patterns;
	int numAnimations;
	pAnimation animations;
} Visuals, * pVisuals;

/********************************************************************
*																	*
*							Aggregate: _Map							*
*																	*
*	Purpose:	Representation of a map								*
*	Fields:															*
*		> width		- Width of a map								*
*		> height	- Height of a map								*
*		> xScroll	- Amount to scroll map horizontally				*
*		> yScroll	- Amount to scroll map vertically				*
*		> xOffset	- Horizontal offset of map						*
*		> yOffset	- Vertical offset of map						*
*		> world		- Output information of map						*
*		> visuals	- Visuals displayed against map					*
*																	*
********************************************************************/

typedef struct _Map {
	int width;
	int height;
	int xScroll, yScroll;
	int xOffset, yOffset;
	OutputBuffer world;
	pVisuals visuals;
} Map, * pMap;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Interface data structures				*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*																	*
*							Enumeration: _WindowMode				*
*																	*
*	Purpose:	Descriptor for a means of managing a window			*
*																	*
********************************************************************/

typedef enum _WindowMode {
	kBasic,				// Normal window
	kEditBox,			// Window that can be written to
	kMenu,				// Window that provides options
	kMessageBox,		// Window that outputs text
	kWindowModesCount	// Count of available window modes
} WindowMode;

/********************************************************************
*																	*
*							Enumeration: _WriteMode					*
*																	*
*	Purpose:	Descriptor for a means of producing text output		*
*																	*
********************************************************************/

typedef enum _WriteMode {
	kWriteAll,		// Write whole text buffer
	kWriteWord,		// Write one word at a time
	kWriteCharacter,// Write one character at a time
	kWriteModesCount// Count of available write modes
} WriteMode;

/********************************************************************
*																	*
*							Enumeration: _ReadMode					*
*																	*
*	Purpose:	Descriptor for a means of retrieving user input		*
*																	*
********************************************************************/

typedef enum _ReadMode {
	kReadAll,		// Read all data entered
	kAlphaOnly,		// Read only alphabetic entries
	kNumeric,		// Read only numeric entries
	kPrintOnly,		// Read only printable entries
	kReadModesCount	// Count of available read modes
} ReadMode;

/********************************************************************
*																	*
*							Enumeration: _BasicBroadcast			*
*																	*
*	Purpose:	Descriptor for interpreting basic broadcasts		*
*																	*
********************************************************************/

typedef enum _BasicBroadcast {
	kDecState,				// Decrement animation states
	kIncState,				// Increment animation states
	kIndexState,			// Index animation states
	kDecFrame,				// Decrement frame of current states
	kIncFrame,				// Increment frame of current states
	kIndexFrame,			// Index frame of current states
	kResetState,			// Reset frame of current states
	kSwitchPatternLock,		// Toggle lock state
	kBasicBroadcastsCount	// Count of basic broadcasts available
} BasicBroadcast;

/********************************************************************
*																	*
*							Enumeration: _EditBoxBroadcast			*
*																	*
*	Purpose:	Descriptor for interpreting edit box broadcasts		*
*																	*
********************************************************************/

typedef enum _EditBoxBroadcast {
	kAddCharacter,			// Insert a character
	kRemoveCharacter,		// Remove a character
	kSaveMessage,			// Save edit box's message
	kDecReadMode,			// Decrement edit box's read mode
	kIncReadMode,			// Increment edit box's read mode
	kIndexReadMode,			// Index edit box's read mode
	kEditBoxBroadcastsCount	// Count of edit box broadcast available
} EditBoxBroadcast;

/********************************************************************
*																	*
*							Enumeration: _MenuBroadcast				*
*																	*
*	Purpose:	Descriptor for interpreting menu broadcasts			*
*																	*
********************************************************************/

typedef enum _MenuBroadcast {
	kPreviousItem,		// Move to previous item
	kNextItem,			// Move to next item
	kToggleLeft,		// Toggle left among item's entries
	kToggleRight,		// Toggle right among item's entries
	kToggleLeftAll,		// Toggle left among all items' entries
	kToggleRightAll,	// Toggle right among all items' entries
	kMenuBroadcastsCount// Count of menu broadcasts available
} MenuBroadcast;

/********************************************************************
*																	*
*							Enumeration: _MessageBoxBroadcast		*
*																	*
*	Purpose:	Descriptor for interpreting message box broadcasts	*
*																	*
********************************************************************/

typedef enum _MessageBoxBroadcast {
	kPreviousMessage,			// Write the previous message
	kNextMessage,				// Write the next message
	kIndexMessage,				// Index the message
	kDecWriteMode,				// Decrement message box's write mode
	kIncWriteMode,				// Increment message box's write mode
	kIndexWriteMode,			// Index message box's write mode
	kMessageBoxBroadcastsCount	// Count of message box broadcasts available
} MessageBoxBroadcast;

/********************************************************************
*																	*
*							Aggregate: _MenuItem					*
*																	*
*	Purpose:	Used to compose a menu								*
*	Fields:															*
*		> location	- Location of menu item relative to window		*
*		> width		- Width of a menu item							*
*		> height	- Height of a menu item							*
*		> entry		- Index of selected menu item entry				*
*		> numItems	- Count of text / lParam entries				*
*		> text		- Array of text entries							*
*		> lParams	- Array of lParam entries						*
*		> highlight	- Value used to highlight selected menu item	*
*																	*
********************************************************************/

typedef struct _MenuItem {
	COORD location;
	int width;
	int height;
	int entry;
	int numItems;
	String * text;
	longStar lParams;
	BYTE highlight;
} MenuItem, * pMenuItem;

/********************************************************************
*																	*
*							Aggregate: _Menu						*
*																	*
*	Purpose:	Representation of a menu							*
*	Fields:															*
*		> itemChosen	- Index of selected menu item				*
*		> numKeys		- Count of keys for navigating menu			*
*		> keys			- Array of keys for navigating menu			*
*		> numItems		- Count of menu items						*
*		> menu			- Array of menu items						*
*																	*
********************************************************************/

typedef struct _Menu {
	int itemChosen;
	int numKeys;
	pKey keys;
	int numItems;
	pMenuItem menu;
} Menu, * pMenu;

/********************************************************************
*																	*
*							Aggregate: _ReceiverEntry				*
*																	*
*	Purpose:	Used to broadcast information across windows		*
*	Fields:															*
*		> windowID	  - Window to broadcast information to			*
*		> instruction - Information to send to window				*
*																	*
********************************************************************/

typedef struct _ReceiverEntry {
	int windowID;
	int instruction;
} ReceiverEntry, * pReceiverEntry;

/********************************************************************
*																	*
*							Aggregate: _Receivers					*
*																	*
*	Purpose:	Composed of entries used to broadcast information	*
*	Fields:															*
*		> numReceivers	- Count of receiver entries					*
*		> receivers		- Array of receiver entries					*
*																	*
********************************************************************/

typedef struct _Receivers {
	int numReceivers;
	pReceiverEntry receivers;
} Receivers, * pReceivers;

/********************************************************************
*																	*
*							Aggregate: _TextBuffer					*
*																	*
*	Purpose:	Composition of text input and output information	*
*	Fields:															*
*		> length	- Length of current word						*
*		> limit		- Limiting number of characters					*
*		> position	- Index into text								*
*		> numChars	- Count of text characters						*
*		> text		- Array of text characters						*
*		> word		- Temporary storage of a word parsed from text	*
*																	*
********************************************************************/

typedef struct _TextBuffer {
	int length;
	int limit;
	int position;
	int numChars;
	String text;
	char word [WORD_LENGTH];
} TextBuffer, * pTextBuffer;

/********************************************************************
*																	*
*							Aggregate: _IO_Descriptor				*
*																	*
*	Purpose:	Composition of window text information handlers		*
*	Fields:															*
*		> readMode	- Descriptor of the means text is retrieved		*
*		> writeMode	- Descriptor of the means text is output		*
*		> text		- Text information								*
*		> cursor	- Index of read / write cursor					*
*																	*
********************************************************************/

typedef struct _IO_Descriptor {
	ReadMode readMode;
	WriteMode writeMode;
	TextBuffer text;
	int cursor;
} IO_Descriptor, * pIO_Descriptor;

/********************************************************************
*																	*
*							Aggregate: _SeparatorEntry				*
*																	*
*	Purpose:	Used to separate a portion of a window				*
*	Fields:															*
*		> offsets	- Offsets into the window						*
*		> extent	- Lateral extent of separator entry				*
*																	*
********************************************************************/

typedef struct _SeparatorEntry {
	COORD offsets;
	int extent;
} SeparatorEntry, * pSeparatorEntry;

/********************************************************************
*																	*
*							Aggregate: _Separators					*
*																	*
*	Purpose:	Mechanism for separating individual windows			*
*	Fields:															*
*		> numHorzSeparators	- Count of horizontal separator entries	*
*		> horzSeparators	- Array of horizontal separator entries	*
*		> numVertSeparators	- Count of vertical separator entries	*
*		> vertSeparators	- Array of vertical separator entries	*
*																	*
********************************************************************/

typedef struct _Separator {
	int numHorzSeparators;
	pSeparatorEntry horzSeparators;
	int numVertSeparators;
	pSeparatorEntry vertSeparators;
} Separator, * pSeparator;

/********************************************************************
*																	*
*							Aggregate: _Window						*
*																	*
*	Purpose:	Representation of a window							*
*	Fields:															*
*		> windowCoord	- Location of window relative to parent		*
*		> windowWidth	- Width of a window's display region		*
*		> windowHeight	- Height of a window's display region		*
*		> xOffset		- Horizontal offset of window				*
*		> yOffset		- Vertical offset of window					*
*		> width			- Width of a window							*
*		> height		- Height of a window						*
*		> display		- Output information of window				*
*		> IO			- Text information handler					*
*		> menu			- Menu information							*
*		> receivers		- Other windows to broadcast information to	*
*		> visuals		- Graphical data to display to window		*
*		> dataADT		- ADT for data transfer						*
*		> state			- Window's state information				*
*		> mode			- Descriptor of window management			*
*		> data			- Application-defined data					*
*		> background	- Background color of window				*
*																	*
********************************************************************/

typedef struct _Window {
	COORD windowCoord;
	int windowWidth;
	int windowHeight;
	int xOffset, yOffset;
	int width;
	int height;
	OutputBuffer display;
	pIO_Descriptor IO;
	pMenu menu;
	pReceivers receivers;
	pVisuals visuals;
	ADT dataADT;
	int state;
	WindowMode mode;
	WORD data;
	BYTE background;
} Window, * pWindow;

/********************************************************************
*																	*
*							Aggregate: _ParentWindow				*
*																	*
*	Purpose:	Representation of a parent window					*
*	Fields:															*
*		> location		- Location of parent relative to screen		*
*		> width			- Width of a parent window					*
*		> height		- Height of a parent window					*
*		> grabPoints	- Portion of window grabbed by mouse		*
*		> delay			- Delay between updates of the parent		*
*		> back			- Surface parent window is drawn against	*
*		> bufferSharing	- Indicates whether surface is shared		*
*		> numWindows	- Count of windows							*
*		> windows		- Array of windows							*
*		> separators	- Separators between individual windows		*
*		> backData		- Visual information of parent window		*
*		> focus			- Index of window with focus				*
*		> state			- Parent window's state information			*
*		> focusKey		- Keycode used to switch focus				*
*		> closeKey		- Keycode used to deactive parent window	*
*		> confirmKey	- Keycode used to confirm information		*
*		> background	- Background color of parent window			*
*		> border		- Border / separator color					*
*																	*
********************************************************************/

typedef struct _ParentWindow {
	COORD location;
	int width;
	int height;
	COORD grabPoints;
	DWORD delay;
	pScreenBuffer back;
	exclusivity bufferSharing;
	int numWindows;
	pWindow windows;
	pSeparator separators;
	PBYTE backData;
	int focus;
	int state;
	int focusKey;
	int closeKey;
	int confirmKey;
	BYTE background;
	BYTE border;
} ParentWindow, * pParentWindow;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							File data structures					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*																	*
*							Enumeration: _FileAccess				*
*																	*
*	Purpose:	Descriptor for a mode of file access				*
*																	*
********************************************************************/

typedef enum _FileAccess {
	kRead,				// Read access
	kWrite,				// Write access
	kAppend,			// Append access
	kFileAccessesCount	// Count of file access modes
} FileAccess;

/********************************************************************
*																	*
*							Enumeration: _FileStyle					*
*																	*
*	Purpose:	Descriptor for a means of reading file data			*
*																	*
********************************************************************/

typedef enum _FileStyle {
	kText,			// Text style
	kBinary,		// Binary style
	kFileStylesCount// Count of file reading modes
} FileStyle;

/********************************************************************
*																	*
*							Enumeration: _StorageMode				*
*																	*
*	Purpose:	Descriptor for a mode of data storage				*
*																	*
********************************************************************/

typedef enum _StorageMode {
	kMerge,				// Merged storage
	kCompressZeroes,	// Flatten lines of zeroes into a zero followed by an integer
	kNotZero,			// Store non-zero values only
	kConstantValue,		// Flatten lines of constant values into a count followed by a value
	kStorageModesCount	// Count of storage modes
} StorageMode;

/********************************************************************
*																	*
*							Aggregate: _CompressionIndex			*
*																	*
*	Purpose:	Compression indices to minimize file storage		*
*	Fields:															*
*		> indexValue	- Index into a buffer to perform file IO on	*
*		> mode			- Manner in which data is stored at index	*
*																	*
********************************************************************/

typedef struct _CompressionIndex {
	int indexValue;
	StorageMode mode;
} CompressionIndex, * pCompressionIndex;

/********************************************************************
*																	*
*							Aggregate: _File						*
*																	*
*	Purpose:	Primary file management object						*
*	Fields:															*
*		> fp			- Pointer to active file					* 
*		> mode			- Mode used to store data to active file	*
*		> filename		- Name of active file						*
*		> numIndices	- Count of compression indices				*
*		> compression	- Array of compression indices				*
*																	*
********************************************************************/

typedef struct _File {
	fileStar fp;
	char filename [WORD_LENGTH];
	int numIndices;
	pCompressionIndex compression;
} File, * pFile;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Scene data structures					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*																	*
*							Aggregate: _Scene						*
*																	*
*	Purpose:	Primary scene management object						*
*	Fields:															*
*		> visuals	- Visual data used throughout scene				*
*		> state		- Scene's state information						*
*		> dwField	- Double word-sized datum						*
*		> dwField2	- Double word-sized datum						*
*		> dwField3	- Double word-sized datum						*
*		> wField	- Word-sized datum								*
*		> wField2	- Word-sized datum								*
*		> wField3	- Word-sized datum								*
*		> bField	- Byte-sized datum								*
*		> bField2	- Byte-sized datum								*
*		> bField3	- Byte-sized datum								*
*																	*
********************************************************************/

typedef struct _Scene {
	Visuals visuals;
	int state;
	DWORD dwField, dwField2, dwField3;
	WORD wField, wField2, wField3;
	BYTE bField, bField2, bField3;
} Scene, * pScene;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Resources data structures				*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*																	*
*							Aggregate: _RefCounts					*
*																	*
*	Purpose:	Tracks number of clients sharing resources			*
*	Fields:															*
*		> bufCount		- Count of clients sharing screen buffer	*
*		> listCount		- Count of clients sharing list				*
*		> queueCount	- Count of clients sharing queue			*
*		> stackCount	- Count of clients sharing stack			*
*																	*
********************************************************************/

typedef struct _RefCounts {
	int bufCount;
	int listCount;
	int queueCount;
	int stackCount;
} RefCounts, * pRefCounts;

/********************************************************************
*																	*
*							Aggregate: _Resources					*
*																	*
*	Purpose:	Resources for shared use throughout application		*
*	Fields:															*
*		> screenBuf	- Shareable screen buffer for saving surfaces	*
*		> list		- Shareable linked list for data storage		*
*		> queue		- Shareable queue for data storage				*
*		> stack		- Shareable stack for data storage				*
*		> refCounts	- Count of clients sharing resources			*
*																	*
********************************************************************/

typedef struct _Resources {
	ScreenBuffer screenBuf;
	ListHead list;
	Queue queue;
	Stack stack;
	RefCounts refCounts;
} Resources, * pResources;

/********************************************************************
*																	*
*							Aggregate: _Objects						*
*																	*
*	Purpose:	Composition of primary objects						*
*	Fields:															*
*		> inputObj	- Primary input object							*
*		> outputObj	- Primary output object							*
*		> fileObj	- Primary file object							*
*		> sceneObj	- Primary scene object							*
*		> resources	- Shared resources								*
*																	*
********************************************************************/

typedef struct _Objects {
	Input inputObj;
	Output outputObj;
	File fileObj;
	Scene sceneObj;
	Resources resources;
} Objects, * pObjects;

#endif