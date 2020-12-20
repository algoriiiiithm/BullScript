#ifndef LIST_H
#define LIST_H
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../header/macro.h"

typedef struct List_Node *list_elementIndex;

typedef struct List_PropertyObj {
	/*user visible:*/
	size_t elementSize;
	size_t elementsNumber;
	/*hidden:*/
	struct List_Node *ptrToFirstNode;
	struct List_Node *ptrToLastNode;
} *list_t;

struct List_Node {
	/*user visible:*/
	void *ptrToElement;
	/*hidden:*/
	struct List_Node *next;
	struct List_Node *prev;
};

/*public declarations:*/
list_t List_Create(size_t elementSize);
void List_Free(list_t L);
void List_InsertIntoFirst(list_t L, void *element);
void List_InsertIntoLast(list_t L, void *element);
void List_RemoveFirstElement(list_t L, void *dest);
void List_RemoveLastElement(list_t L, void *dest);
list_elementIndex List_IndexAdvance(list_elementIndex ptr);
void List_Output(list_t L, void (*func)(void *));

#endif