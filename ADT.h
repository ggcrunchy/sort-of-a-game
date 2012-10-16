/********************************************************************
*																	*
*							ADT.h									*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains information relevant to ADT management		*
*																	*
********************************************************************/

#ifndef ADT_H
#define ADT_H

#include "Common.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Macros									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define EXTRACT(x,node,type)  (x = *((type *) (node)->value))
// Used to extract data of a given type from an ADT node into a variable
#define RETRIEVE(x,node,type) (x = ((type *) (node)->value))
// Used to retrieve address of data of a given type from an ADT node into a variable
 
#define WALKAHEAD(walker) ((walker) = (walker)->next)
// Used to walk to a subsequent node
#define WALKBACK(walker)  ((walker) = (walker)->prev)
// Used to walk to a preceding node

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							ADT access functions					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	IncListNodeCount - Increment the list node count by one			*
*	Input:	A list head												*
*	Output:	Node count is incremented by one						*
********************************************************************/

static void IncListNodeCount (pListHead listHead);

/********************************************************************
*	IncStackNodeCount - Increment the stack node count by one		*
*	Input:	A stack													*
*	Output:	Node count is incremented by one						*
********************************************************************/

static void IncStackNodeCount (pStack stack);

/********************************************************************
*	IncQueueNodeCount - Increment the queue node count by one		*
*	Input:	A queue													*
*	Output:	Node count is incremented by one						*
********************************************************************/

static void IncQueueNodeCount (pQueue queue);

/********************************************************************
*	DecListNodeCount - Decrement the list node count by one			*
*	Input:	A list head												*
*	Output:	Node count is decremented by one						*
********************************************************************/

static void DecListNodeCount (pListHead listHead);

/********************************************************************
*	DecStackNodeCount - Decrement the stack node count by one		*
*	Input:	A stack													*
*	Output:	Node count is decremented by one						*
********************************************************************/

static void DecStackNodeCount (pStack stack);

/********************************************************************
*	DecQueueNodeCount - Decrement the queue node count by one		*
*	Input:	A queue													*
*	Output:	Node count is decremented by one						*
********************************************************************/

static void DecQueueNodeCount (pQueue queue);

/********************************************************************
*	RemoveListData - Call function associated with given ListNode	*
********************************************************************/

int RemoveListData (pListNode listNode);

/********************************************************************
*	RemoveStackData - Call function associated with given StackNode	*
********************************************************************/

int RemoveStackData (pStackNode stackNode);

/********************************************************************
*	RemoveQueueData - Call function associated with given QueueNode	*
********************************************************************/

int RemoveQueueData (pQueueNode queueNode);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							ADT setup functions						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	SetListRemoveMethod - Assign a method to a given ListNode		*
********************************************************************/

void SetListRemoveMethod (pListNode listNode, int (* remove) (voidStar data));

/********************************************************************
*	SetStackRemoveMethod - Assign a method to a given StackNode		*
********************************************************************/

void SetStackRemoveMethod (pStackNode stackNode, int (* remove) (voidStar data));

/********************************************************************
*	SetQueueRemoveMethod - Assign a method to a given QueueNode		*
********************************************************************/

void SetQueueRemoveMethod (pQueueNode queueNode, int (* remove) (voidStar data));

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Construction							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	InitHydra - Initialize a Hydra structure						*
*	Input:	Pointer to a Hydra structure, number of list heads, and *
*			array of mode descriptions								*
*	Output:	Success													*
********************************************************************/

BOOL InitHydra (Hydra * hydra, int numHeads, listMode const * modes);

/********************************************************************
*	CreateList - Create a linked list								*
*	Input:	Style of linked list to create							*
*	Output:	Returns the address of a ListHead structure				*
********************************************************************/

pListHead CreateList (listMode mode);

/********************************************************************
*	CreateStack - Create a stack									*
*	Input:	No input												*
*	Output:	Returns the address of a Stack structure				*
********************************************************************/

pStack CreateStack (void);

/********************************************************************
*	CreateQueue - Create a queue									*
*	Input:	No input												*
*	Output:	Returns the address of a Queue structure				*
********************************************************************/

pQueue CreateQueue (void);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Data entry								*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	InsertListNode - Insert a node into the linked list				*
*	Input:	Head of list to insert node into; data to insert; type	*
*			of data to insert; index to insert node at				*
*	Output:	Success													*
********************************************************************/

BOOL InsertListNode (pListHead listHead, voidStar data, dataType type, int index);

/********************************************************************
*	AppendListNode - Append a node to the end of the linked list	*
*	Input:	Head of list to append node onto; data to append; type	*
*			of data to append										*
*	Output:	Success													*
********************************************************************/

BOOL AppendListNode (pListHead listHead, voidStar data, dataType type);

/********************************************************************
*	PushStackNode - Push a node onto the stack						*
*	Input:	Stack to push node onto; data to push; type of data	to	*
*			push													*
*	Output:	Success													*
********************************************************************/

BOOL PushStackNode (pStack stack, voidStar data, dataType type);

/********************************************************************
*	EnqueueQueueNode - Enqueue a node onto the queue				*
*	Input:	Queue to enqueue node onto; data to enqueue; type of	*
*			data to enqueue											*
*	Output:	Success													*
********************************************************************/

BOOL EnqueueQueueNode (pQueue queue, voidStar data, dataType type);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Data retrieval							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	GetListNode - Retrieve a ListNode at a given index from a list	*
*	Input:	Head of list to retrieve node from; index of node		*
*	Output:	Node at given index in list								*
********************************************************************/

pListNode GetListNode (pListHead listHead, int index);

/********************************************************************
*	StackTop - Get the "last-in" StackNode from the given Stack		*
*	Input:	Stack to retrieve StackNode from						*
*	Output:	"Last-in" StackNode										*
********************************************************************/

pStackNode StackTop (pStack stack);

/********************************************************************
*	QueueHead - Get the "first-in" QueueNode from the given Queue	*
*	Input:	Queue to retrieve QueueNode from						*
*	Output:	"First-in" QueueNode									*
********************************************************************/

pQueueNode QueueHead (pQueue queue);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Data removal							*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	RemoveNodeValue - Remove arbitrary data in a node's value field	*
*	Input:	Node with data to be removed; node type					*
*	Output: Data is removed											*
********************************************************************/

static void RemoveNodeValue (voidStar node, ADTStyle ADT);

/********************************************************************
*	RemoveListNode - Remove a node from the linked list				*
*	Input:	Head of list to remove node from; index into list		*
*	Output:	Success													*
********************************************************************/

BOOL RemoveListNode (pListHead listHead, int index);

/********************************************************************
*	PopStackNode - Pop the node at the top of the stack				*
*	Input:	Stack to pop node from									*
*	Output:	Success													*
********************************************************************/

BOOL PopStackNode (pStack stack);

/********************************************************************
*	DequeueQueueNode - Dequeue the node at the head of the queue	*
*	Input:	Queue to dequeue node from								*
*	Output:	Success													*
********************************************************************/

BOOL DequeueQueueNode (pQueue queue);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Destruction								*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	EmptyList - Removes all nodes from a linked list				*
*	Input:	Head of list to empty									*
*	Output:	Success													*
********************************************************************/

BOOL EmptyList (pListHead listHead);

/********************************************************************
*	EmptyQueue - Removes all nodes from	a queue						*
*	Input:	Queue to empty											*
*	Output:	Success													*
********************************************************************/

BOOL EmptyQueue (pQueue queue);

/********************************************************************
*	EmptyStack - Removes all nodes from a stack						*
*	Input:	Stack to empty											*
*	Output:	Success													*
********************************************************************/

BOOL EmptyStack (pStack stack);

/********************************************************************
*	DestroyList - Destroys a linked list							*
*	Input:	Head of list to destroy									*
*	Output:	Success													*
********************************************************************/

BOOL DestroyList (pListHead listHead);

/********************************************************************
*	DestroyQueue - Destroys a queue									*
*	Input:	Queue to destroy										*
*	Output:	Success													*
********************************************************************/

BOOL DestroyQueue (pQueue queue);

/********************************************************************
*	DestroyStack - Destroys a stack									*
*	Input:	Stack to destroy										*
*	Output:	Success													*
********************************************************************/

BOOL DestroyStack (pStack stack);

/********************************************************************
*	DestroyHydra - Destroys a Hydra structure						*
*	Input:	Address of a Hydra structure to destroy					*
*	Output:	Success													*
********************************************************************/

BOOL DestroyHydra (pHydra hydra);

#endif