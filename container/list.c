#include "list.h"

/*public function definations:*/
list_t List_Create(size_t elementSize)
{
	list_t L = malloc(sizeof(struct List_PropertyObj));
	L->elementSize = elementSize;
	L->elementsNumber = 0;
	L->ptrToFirstNode = NULL;
	L->ptrToLastNode = NULL;
	return L;
}

void List_Free(list_t L)
{
	list_elementIndex ptr, next;
	for(ptr = L->ptrToFirstNode; ptr != NULL; ptr = next){
		free(ptr->ptrToElement);
		next = ptr->next;
		free(ptr);
	}
	free(L);
	return;
}

void List_InsertIntoFirst(list_t L, void *element)
{
	list_elementIndex newNode = malloc(sizeof(struct List_Node));
	newNode->ptrToElement = malloc(L->elementSize);
	memcpy(newNode->ptrToElement, element, L->elementSize);
	if(IsEmpty(L)){
		L->ptrToFirstNode = newNode;
		L->ptrToLastNode = newNode;
		newNode->next = NULL;
		newNode->prev = NULL;
	}else{
		newNode->next = L->ptrToFirstNode;
		newNode->prev = NULL;
		L->ptrToFirstNode->prev = newNode;
		L->ptrToFirstNode = newNode;
	}
	(L->elementsNumber)++;
	return;
}

void List_InsertIntoLast(list_t L, void *element)
{
	list_elementIndex newNode = malloc(sizeof(struct List_Node));
	newNode->ptrToElement = malloc(L->elementSize);
	memcpy(newNode->ptrToElement, element, L->elementSize);
	if(IsEmpty(L)){
		L->ptrToFirstNode = newNode;
		L->ptrToLastNode = newNode;
		newNode->next = NULL;
		newNode->prev = NULL;	
	}else{
		newNode->prev = L->ptrToLastNode;
		newNode->next = NULL;
		L->ptrToLastNode->next = newNode;
		L->ptrToLastNode = newNode; 
	}
	(L->elementsNumber)++;
}

void List_RemoveFirstElement(list_t L, void *dest)
{
	list_elementIndex tmp = L->ptrToFirstNode;
	(L->elementsNumber)--;
	memcpy(dest, tmp->ptrToElement, L->elementSize);
	free(tmp->ptrToElement);
	L->ptrToFirstNode = tmp->next;
	free(tmp);
	if(L->elementsNumber == 0)
		L->ptrToLastNode = NULL;
	return;
}

void List_RemoveLastElement(list_t L, void *dest)
{
	list_elementIndex tmp = L->ptrToLastNode;
	(L->elementsNumber)--;
	memcpy(dest, tmp->ptrToElement, L->elementSize);
	free(tmp->ptrToElement);
	L->ptrToLastNode = tmp->prev;
	free(tmp);
	if(L->elementsNumber == 0)
		L->ptrToFirstNode = NULL;
	return;
}

list_elementIndex List_IndexAdvance(list_elementIndex ptr)
{
	return ptr->next;
}

void List_Output(list_t L, void (* func)(void *))
{
	list_elementIndex iter;
	int i;
	iter = L->ptrToFirstNode;
	for(i = 0; i < L->elementsNumber; i++){
		func(iter->ptrToElement);
		iter = iter->next;
	}
	return;
}