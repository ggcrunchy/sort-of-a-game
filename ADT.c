/********************************************************************
*																	*
*							ADT.c									*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains implementation of ADT tool					*
*																	*
********************************************************************/

#include "ADT.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							External includes						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "Resources.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							ADT access functions					*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	IncListNodeCount - Increment the node count by one				*
*	Input:	A list head												*
*	Output:	Node count is incremented by one						*
********************************************************************/

static void IncListNodeCount (ListHead * listHead)
{
	listHead->numNodes++;	// Increment node count
}

/********************************************************************
*	IncStackNodeCount - Increment the stack node count by one		*
*	Input:	A stack													*
*	Output:	Node count is incremented by one						*
********************************************************************/

static void IncStackNodeCount (Stack * stack)
{
	stack->numNodes++;	// Increment node count
}

/********************************************************************
*	IncQueueNodeCount - Increment the queue node count by one		*
*	Input:	A queue													*
*	Output:	Node count is incremented by one						*
********************************************************************/

static void IncQueueNodeCount (Queue * queue)
{
	queue->numNodes++;	// Increment node count
}

/********************************************************************
*	DecListNodeCount - Decrement the node count by one				*
*	Input:	A list head												*
*	Output:	Node count is decremented by one						*
********************************************************************/

static void DecListNodeCount (ListHead * listHead)
{
	listHead->numNodes--;	// Decrement node count
}

/********************************************************************
*	DecStackNodeCount - Decrement the stack node count by one		*
*	Input:	A stack													*
*	Output:	Node count is decremented by one						*
********************************************************************/

static void DecStackNodeCount (Stack * stack)
{
	stack->numNodes--;	// Decrement node count
}

/********************************************************************
*	DecQueueNodeCount - Decrement the queue node count by one		*
*	Input:	A queue													*
*	Output:	Node count is decremented by one						*
********************************************************************/

static void DecQueueNodeCount (Queue * queue)
{
	queue->numNodes--;	// Decrement node count
}

/********************************************************************
*	RemoveListData - Call function associated with given ListNode	*
********************************************************************/

int RemoveListData (ListNode * listNode)
{
	return listNode->remove (listNode->value);
}

/********************************************************************
*	RemoveStackData - Call function associated with given StackNode	*
********************************************************************/

int RemoveStackData (StackNode * stackNode)
{
	return stackNode->remove (stackNode->value);
}

/********************************************************************
*	RemoveQueueData - Call function associated with given QueueNode	*
********************************************************************/

int RemoveQueueData (QueueNode * queueNode)
{
	return queueNode->remove (queueNode->value);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							ADT setup functions						*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*	SetListRemoveMethod - Assign a method to a given ListNode 		*
********************************************************************/

void SetListRemoveMethod (ListNode * listNode, int (* remove) (voidStar data))
{
	listNode->remove = remove;
}

/********************************************************************
*	SetStackRemoveMethod - Assign a method to a given StackNode		*
********************************************************************/

void SetStackRemoveMethod (StackNode * stackNode, int (* remove) (voidStar data))
{
	stackNode->remove = remove;
}

/********************************************************************
*	SetQueueRemoveMethod - Assign a method to a given QueueNode		*
********************************************************************/

void SetQueueRemoveMethod (QueueNode * queueNode, int (* remove) (voidStar data))
{
	queueNode->remove = remove;
}

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

BOOL InitHydra (Hydra * hydra, int numHeads, listMode const * modes)
{
	int i;	// Loop variable

	assert (hydra && modes);
	// Verify that hydra and modes point to valid memory

	hydra->numHeads = numHeads;	// Assign number of heads to hydra

	CALLOC(hydra->listHeads,hydra->numHeads,pListHead);
	// Assign memory to hydra's listHeads field

	if (!hydra->listHeads)
	{
		ERROR_MESSAGE("InitHydra failed","1");
		// Return failure
	}
	// Verify that hydra's listHeads field points to valid memory

	for (i = 0; i < hydra->numHeads; i++)	// Cycle through the ListHead's
	{
		*(hydra->listHeads + i) = CreateList (*(modes + i));
		// Create the ListHead

		if (!*(hydra->listHeads + i))
		{
			ERROR_MESSAGE("InitHydra failed","2");
			// Return failure
		}
		// Verify that hydra's listHeads field points to valid memory at the given index
	}

	return TRUE;
	// Return success
}

/********************************************************************
*	CreateList - Create a linked list								*
*	Input:	Style of linked list to create							*
*	Output:	Returns the address of a ListHead structure				*
********************************************************************/

ListHead * CreateList (listMode mode)
{
	pListHead listHead;		// List head to be created

	MALLOC(listHead,ListHead);
	// Assign memory to listHead

	if (!listHead)
	{
		NULL_MESSAGE("CreateList failed","1");
		// Return null pListHead
	}
	// Verify that listHead points to valid memory

	listHead->head = NULL;	// Nullify the head itself
	listHead->mode = mode;	// Set the appropriate mode

	return listHead;
	// Return the list head
}

/********************************************************************
*	CreateStack - Create a stack									*
*	Input:	No input												*
*	Output:	Returns the address of a Stack structure				*
********************************************************************/

Stack * CreateStack (void)
{
	pStack stack;	// Stack to be created

	MALLOC(stack,Stack);
	// Assign memory to stack

	if (!stack)
	{
		NULL_MESSAGE("CreateStack failed","1");
		// Return null pStack
	}
	// Verify that stack points to valid memory

	stack->top = NULL;	// Nullify the top itself

	return stack;
	// Return the stack
}

/********************************************************************
*	CreateQueue - Create a queue									*
*	Input:	No input												*
*	Output:	Returns the address of a Queue structure				*
********************************************************************/

Queue * CreateQueue (void)
{
	pQueue queue;	// Queue to be created

	MALLOC(queue,Queue);
	// Assign memory to queue

	if (!queue)
	{
		NULL_MESSAGE("CreateQueue failed","1");
		// Return null pQueue
	}
	// Verify that queue points to valid memory

	queue->tail = NULL;	// Nullify the tail itself

	return queue;
	// Return the queue
}

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

BOOL InsertListNode (ListHead * listHead, voidStar data, dataType type, int index)
{
	pListNode newNode, walker;	// A new node to insert; a list walker
	int counter = 0;			// Index pointed at by the walker

	assert (listHead && data);
	// Verify that listHead and data point to valid memory

	if (index > listHead->numNodes)
	{
		ERROR_MESSAGE("InsertListNode failed","1");
		// Return failure
	}
	// Verify that the index is valid

	if (index == listHead->numNodes)
	{
		if (AppendListNode (listHead, data, type))
		{
			return TRUE;	// Return success
		}

		else
		{
			ERROR_MESSAGE("InsertListNode failed","2");
			// Return failure
		}
	}
	// Call AppendListNode to handle this situation

	MALLOC(newNode,ListNode);
	// Assign memory to newNode

	if (!newNode)
	{
		ERROR_MESSAGE("InsertListNode failed","3");
		// Return failure
	}
	// Verify that newNode points to valid memory

	newNode->value = data;	// Assign data to newNode's value field
	newNode->type = type;	// Assign corresponding type

	for (walker = listHead->head; counter < index; counter++)
	{
		WALKAHEAD(walker);
		// Walk the list to the appropriate index
	}

	if (listHead->mode == kRing || index)	// Inserting at given list index
	{
		newNode->prev = walker->prev;	// newNode points back to walker->prev
		newNode->prev->next = newNode;	// Previous node points ahead to newNode

		if (!index)	// Check whether index is head of ringed list
		{
			listHead->head = newNode;	// Reassign newNode to be the list head
		}
	}

	else									// Inserting at head of the list
	{
		newNode->prev = NULL;	// Nullify the back pointer

		listHead->head = newNode;	// Reassign newNode to be the list head
	}

	newNode->next = walker;			// newNode points ahead to walker
	newNode->next->prev = newNode;	// Next node points back to newNode

	IncListNodeCount (listHead);
	// Increase the node count

	return TRUE;
	// Return success
}

/********************************************************************
*	AppendListNode - Append a node to the end of the linked list	*
*	Input:	Head of list to append node onto; data to append; type	*
*			of data to append										*
*	Output:	Success													*
********************************************************************/

BOOL AppendListNode (ListHead * listHead, voidStar data, dataType type)
{
	pListNode newNode, walker, prev;// A new node to insert; a list walker; a previous node
	int counter = 0;				// Index pointed at by the walker

	assert (listHead && data);
	// Verify that listHead and data point to valid memory

	MALLOC(newNode,ListNode);
	// Assign memory to newNode

	if (!newNode)
	{
		ERROR_MESSAGE("AppendListNode failed","1");
		// Return failure
	}
	// Verify that newNode points to valid memory

	newNode->value = data;	// Assign data to newNode's value field
	newNode->type = type;	// Assign corresponding type

	switch (listHead->mode)	// Get list mode
	{
	case kNormal:			// Normal list
		if (!listHead->numNodes)	// Empty list case
		{
			newNode->prev = newNode->next = NULL;	// Nullify the back and next pointers

			listHead->head = newNode;				// Append the node
		}

		else						// Normal case
		{
			for (walker = listHead->head; counter < listHead->numNodes; counter++)
			{
				prev = walker;
				// Store current node in prev 

				WALKAHEAD(walker);
				// Walk to the next node in the list
			}

			newNode->prev = prev;					// newNode points back to prev
			newNode->next = NULL;					// Nullify newNode's next field

			newNode->prev->next = newNode;			// Append the node
		}

		break;	// Break out of switch statement

	case kRing:				// Circular list
		if (!listHead->numNodes)	// Empty list case
		{
			newNode->prev = newNode->next = newNode;// newNode points back and ahead to itself

			listHead->head = newNode;				// Append the node				
		}

		else						// Normal case
		{
			for (walker = listHead->head; counter < listHead->numNodes; counter++)
			{
				prev = walker;
				// Store current node in prev

				WALKAHEAD(walker);
				// Walk to the next node in the list
			}

			newNode->prev = prev;					// newNode points back to prev
			newNode->next = listHead->head;			// newNode points ahead to list head

			newNode->prev->next = newNode;			// Previous node points ahead to newNode
			newNode->next->prev = newNode;			// Next node points back to newNode
		}

		break;	// Break out of switch statement
	}

	IncListNodeCount (listHead);
	// Increase the node count

	return TRUE;
	// Return success
}

/********************************************************************
*	PushStackNode - Push a node onto the stack						*
*	Input:	Stack to push node onto; data to push; type of data	to	*
*			push													*
*	Output:	Success													*
********************************************************************/

BOOL PushStackNode (Stack * stack, voidStar data, dataType type)
{
	pStackNode newNode;	// A new node to push

	assert (stack && data);
	// Verify that stack and data point to valid memory

	MALLOC(newNode,StackNode);
	// Assign memory to newNode

	if (!newNode)
	{
		ERROR_MESSAGE("PushStackNode failed","1");
		// Return failure
	}
	// Verify that newNode points to valid memory

	newNode->value = data;	// Assign data to newNode's value field
	newNode->type = type;	// Assign corresponding type

	newNode->next = stack->top;	// newNode points ahead to stack's top field

	stack->top = newNode;		// Reassign stack's top field to point to newNode

	IncStackNodeCount (stack);
	// Increase the node count

	return TRUE;
	// Return success
}

/********************************************************************
*	EnqueueQueueNode - Enqueue a node onto the queue				*
*	Input:	Queue to enqueue node onto; data to enqueue; type of	*
*			data to enqueue											*
*	Output:	Success													*
********************************************************************/

BOOL EnqueueQueueNode (Queue * queue, voidStar data, dataType type)
{
	pQueueNode newNode;	// A new node to enqueue

	assert (queue && data);
	// Verify that queue and data point to valid memory

	MALLOC(newNode,QueueNode);
	// Assign memory to newNode

	if (!newNode)
	{
		ERROR_MESSAGE("EnqueueQueueNode failed","1");
		// Return failure
	}
	// Verify that newNode points to valid memory

	newNode->value = data;	// Assign data to newNode's value field
	newNode->type = type;	// Assign corresponding type

	newNode->next = queue->tail;	// newNode points ahead to queue's tail field

	queue->tail = newNode;			// Reassign queue's tail field to point to newNode

	IncQueueNodeCount (queue);
	// Increase the node count

	return TRUE;
	// Return success
}

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

ListNode * GetListNode (ListHead * listHead, int index)
{
	pListNode walker;	// A list walker
	int counter = 0;	// Index pointed at by the walker

	assert (listHead && listHead->numNodes);
	// Ensure that list is not empty

	for (walker = listHead->head; counter < index; counter++)
	{
		WALKAHEAD(walker);
		// Walk the list to the appropriate index
	}

	return walker;
	// Return ListNode pointed at by walker
}

/********************************************************************
*	StackTop - Get the "last-in" StackNode from the given Stack		*
*	Input:	Stack to retrieve StackNode from						*
*	Output:	"Last-in" StackNode										*
********************************************************************/

StackNode * StackTop (Stack * stack)
{
	assert (stack && stack->numNodes);
	// Ensure that stack has a top

	return stack->top;
	// Return StackNode pointed at by stack's top field
}

/********************************************************************
*	QueueHead - Get the "first-in" QueueNode from the given Queue	*
*	Input:	Queue to retrieve QueueNode from						*
*	Output:	"First-in" QueueNode									*
********************************************************************/

QueueNode * QueueHead (Queue * queue)
{
	pQueueNode walker, prev;	// A queue walker; a previous node
	int counter = 0;			// Index pointed at by the walker

	assert (queue && queue->numNodes);
	// Ensure that queue has a head

	for (walker = queue->tail; counter < queue->numNodes; counter++)
	{
		prev = walker;
		// Store current node in prev

		WALKAHEAD(walker);
		// Walk to the next node in the queue
	}

	return prev;
	// Return QueueNode pointed at by prev
}

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

static void RemoveNodeValue (voidStar node, ADTStyle ADT)
{
	voidStar data;	// Storage for arbitrary data
	dataType type;	// Type of data to remove

	switch (ADT)	// Get the ADT style
	{
	case kADTList:	// List case
		data = ((pListNode) node)->value;
		// Retrieve data from list node

		type = ((pListNode) node)->type;
		// Retrieve type from list node

		break;	// Break out of switch statement

	case kADTQueue:	// Queue case
		data = ((pQueueNode) node)->value;
		// Retrieve data from queue node

		type = ((pQueueNode) node)->type;
		// Retrieve type from queue node

		break;	// Break out of switch statement

	case kADTStack:	// Stack case
		data = ((pStackNode) node)->value;
		// Retrieve data from stack node

		type = ((pStackNode) node)->type;
		// Retrieve type from queue node

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported ADT: RemoveNodeValue failed","1");
		// Return failure
	}

	switch (type)	// Get data type
	{
	case kChar:				// char 
	case kShort:			// short
	case kInt:				// int
	case kLong:				// long
	case kFloat:			// float
	case kDouble:			// double
	case kLongDouble:		// long double

		break;	// Break out of switch statement

	case kImage:			// Image
		DeleteImage ((pImage) data);
		// Free memory formatted as Image aggregate

		break;	// Break out of switch statement

	case kPattern:			// Pattern
		DeletePattern ((pPattern) data);
		// Free memory formatted as Pattern aggregate

		break;	// Break out of switch statement

	case kAnimation:		// Animation
		DeleteAnimation ((pAnimation) data);
		// Free memory formatted as Animation aggregate

		break;	// Break out of switch statement

	case kMap:				// Map
		DeleteMap ((pMap) data);
		// Free memory formatted as Map aggregate

		break;	// Break out of switch statement

	case kMenus:			// Menu
		DeleteMenus ((pMenu) data);
		// Free memory formatted as Menu aggregate
		
		break;	// Break out of switch statement

	case kWindow:			// Window
		DeleteWindow ((pWindow) data);
		// Free memory formatted as Window aggregate

		break;	// Break out of switch statement

	case kParentWindow:		// ParentWindow
		DeleteParentWindow ((pParentWindow) data);
		// Free memory formatted as ParentWindow aggregate
		
		break;	// Break out of switch statement

	case kList:				// Linked list
		DestroyList ((pListHead) data);
		// Free memory formatted as linked list ADT

		break;	// Break out of switch statement

	case kQueue:			// Queue
		DestroyQueue ((pQueue) data);
		// Free memory formatted as queue ADT

		break;	// Break out of switch statement

	case kStack:			// Stack
		DestroyStack ((pStack) data);
		// Free memory formatted as stack ADT
		
		break;	// Break out of switch statement

	case kHydra:			// Hydra
		DestroyHydra ((pHydra) data);
		// Free memory formatted as Hydra aggregate

		break;	// Break out of switch statement

	case kUserDefined:		// User-defined
		switch (ADT)	// Get the ADT style
		{
		case kADTList:	// List case
			RemoveListData ((pListNode) node);
			// Remove data using list node's remove method

			break;	// Break out of switch statement

		case kADTQueue:	// Queue case
			RemoveQueueData ((pQueueNode) node);
			// Remove data using queue node's remove method

			break;	// Break out of switch statement

		case kADTStack:	// Stack case
			RemoveStackData ((pStackNode) node);
			// Remove data using stack node's remove method

			break;	// Break out of switch statement
		}
		// Free memory formatted in user-defined manner

		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported data type: RemoveNodeValue failed","2");
		// Return failure
	}	 
}

/********************************************************************
*	RemoveListNode - Remove a node from the linked list				*
*	Input:	Head of list to remove node from; index into list		*
*	Output:	Success													*
********************************************************************/

BOOL RemoveListNode (ListHead * listHead, int index)
{
	pListNode walker;	// A list walker
	int counter = 0;	// Index pointed at by the walker

	assert (listHead);
	// Verify that listHead points to valid memory

	if (index >= listHead->numNodes)
	{
		ERROR_MESSAGE("RemoveListNode failed", "1");
		// Return failure
	}
	// Verify that the index is valid

	for (walker = listHead->head; counter < index; counter++)
	{
		WALKAHEAD(walker);
		// Walk the list to the appropriate index
	}

	if (!index)	
	{
		listHead->head = walker->next;	// Reassign list head
	}
	// Determine whether list head is being removed

	if (listHead->mode == kRing || index)
	{
		walker->prev->next = walker->next;	// Previous node points ahead to next node
	}
	// Verify that previous node is non-NULL

	if (listHead->mode == kRing || index + 1 < listHead->numNodes)
	{
		walker->next->prev = walker->prev;	// Next node points back to previous node
	}
	// Verify that next node is non-NULL

	RemoveNodeValue (walker, kADTList);
	// Remove data pointed to by walker's value field

	FREE(walker->value);
	// Free memory pointed to by walker's value field

	FREE(walker);
	// Free memory pointed to by walker

	DecListNodeCount (listHead);
	// Decrease the node count

	return TRUE;
	// Return success
}

/********************************************************************
*	PopStackNode - Pop the node at the top of the stack				*
*	Input:	Stack to pop node from									*
*	Output:	Success													*
********************************************************************/

BOOL PopStackNode (Stack * stack)
{
	pStackNode node;	// A node used to switch the stack top

	assert (stack);
	// Verify that stack points to valid memory

	node = stack->top;		// node points to stack's top field

	stack->top = node->next;// Reassign stack's top field to point to node's next field

	node->next = NULL;		// Nullify node's next field

	RemoveNodeValue (node, kADTStack);
	// Remove value pointed to by node's value field

	FREE(node->value);
	// Free memory pointed to by node's value field

	FREE(node);
	// Free memory pointed to by node

	DecStackNodeCount (stack);
	// Decrease the node count

	return TRUE;
	// Return success
}

/********************************************************************
*	DequeueQueueNode - Dequeue the node at the head of the queue	*
*	Input:	Queue to dequeue node from								*
*	Output:	Success													*
********************************************************************/

BOOL DequeueQueueNode (Queue * queue)
{
	pQueueNode walker, prev;	// A queue walker; a previous node
	int counter = 0, numNodes;	// Index pointed at by the walker; a node count

	assert (queue);
	// Verify that queue points to valid memory

	numNodes = queue->numNodes - 1;
	// Store the node count less one

	for (walker = queue->tail; counter < numNodes; counter++)
	{
		prev = walker;
		// Store current node in prev

		WALKAHEAD(walker);
		// Walk to the next node in the queue
	}

	prev->next = NULL;	// Nullify prev's next field

	RemoveNodeValue (walker, kADTQueue);
	// Remove value pointed to by walker's value field

	FREE(walker->value);
	// Free memory pointed to by walker's value field

	FREE(walker);
	// Free memory pointed to by walker

	DecQueueNodeCount (queue);
	// Decrease the node count

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
*	EmptyList - Removes all nodes from a linked list				*
*	Input:	Head of list to empty									*
*	Output:	Success													*
********************************************************************/

BOOL EmptyList (ListHead * listHead)
{
	assert (listHead);
	// Verify that listHead points to valid memory

	while (listHead->numNodes)	// Check that nodes still exist
	{
		if (!RemoveListNode (listHead, 0))
		{
			ERROR_MESSAGE("EmptyList failed","1");
			// Return failure
		}
		// Remove the node at the first index
	}

	return TRUE;
	// Return success
}

/********************************************************************
*	EmptyQueue - Removes all nodes from	a queue						*
*	Input:	Queue to empty											*
*	Output:	Success													*
********************************************************************/

BOOL EmptyQueue (Queue * queue)
{
	assert (queue);
	// Verify that queue points to valid memory

	while (queue->numNodes)	// Check that nodes still exist
	{
		if (!DequeueQueueNode (queue))
		{
			ERROR_MESSAGE("EmptyQueue failed","1");
			// Return failure
		}
		// Dequeue the node at the head of the queue
	}

	return TRUE;
	// Return success
}

/********************************************************************
*	EmptyStack - Removes all nodes from a stack						*
*	Input:	Stack to empty											*
*	Output:	Success													*
********************************************************************/

BOOL EmptyStack (Stack * stack)
{
	assert (stack);
	// Verify that stack points to valid memory

	while (stack->numNodes)	// Check that nodes still exist
	{
		if (!PopStackNode (stack))
		{
			ERROR_MESSAGE("EmptyStack failed","1");
			// Return failure
		}
		// Pop the node at the top of the stack
	}

	return TRUE;
	// Return success
}

/********************************************************************
*	DestroyList - Destroys a linked list							*
*	Input:	Head of list to destroy									*
*	Output:	Success													*
********************************************************************/

BOOL DestroyList (ListHead * listHead)
{
	assert (listHead);
	// Verify that listHead points to valid memory

	if (!EmptyList (listHead))
	{
		ERROR_MESSAGE("DestroyList failed","1");
		// Return failure
	}
	// Empty the list

	FREE(listHead);
	// Free memory pointed to by listHead

	return TRUE;
	// Return success
}

/********************************************************************
*	DestroyQueue - Destroys a queue									*
*	Input:	Queue to destroy										*
*	Output:	Success													*
********************************************************************/

BOOL DestroyQueue (Queue * queue)
{
	assert (queue);
	// Verify that queue points to valid memory

	if (!EmptyQueue (queue))
	{
		ERROR_MESSAGE("DestroyQueue failed","1");
		// Return failure
	}
	// Empty the queue

	FREE(queue);
	// Free memory pointed to by queue

	return TRUE;
	// Return success
}

/********************************************************************
*	DestroyStack - Destroys a stack									*
*	Input:	Stack to destroy										*
*	Output:	Success													*
********************************************************************/

BOOL DestroyStack (Stack * stack)
{
	assert (stack);
	// Verify that stack points to valid memory

	if (!EmptyStack (stack))
	{
		ERROR_MESSAGE("DestroyStack failed","1");
		// Return failure
	}
	// Empty the stack

	FREE(stack);
	// Free memory pointed to by stack

	return TRUE;
	// Return success
}

/********************************************************************
*	DestroyHydra - Destroys a Hydra structure						*
*	Input:	Address of a Hydra structure to destroy					*
*	Output:	Success													*
********************************************************************/

BOOL DestroyHydra (Hydra * hydra)
{
	int i;	// Loop variable

	assert (hydra);
	// Verify that hydra points to valid memory

	for (i = 0; i < hydra->numHeads; i++)
	{
		if (!DestroyList (*(hydra->listHeads + i)))
		{
			ERROR_MESSAGE("DestroyHydra failed","1");
			// Return failure
		}
		// Destroy the list at the given index
	}

	FREE(hydra->listHeads);
	// Free memory pointed at by hydra's listHeads field

	return TRUE;
	// Return success
}
