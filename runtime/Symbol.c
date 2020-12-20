#include <stdio.h>
#include <string.h>
#include "../header/BullScript.h"
#include "../container/list.h"


const int tblSize = 256;
list_t lists[256];

size_t Hash(const char *name, int nameLength);

void CreateSymbolList(void)
{
	size_t i;
	for(i = 0; i < tblSize; i++){
		lists[i] = List_Create(sizeof(IdentifierProperty));
	}
	return;
}

IdentifierProperty *GetIdentifierProperty(
const char *name, int nameLength)
{
	list_t l = lists[Hash(name, nameLength)];
	list_elementIndex p = l->ptrToFirstNode;
	while(p != NULL &&
		strncmp(
		name, ((IdentifierProperty*)(p->ptrToElement))->name, nameLength) != 0
	) p = p->next;
	if(p != NULL){
		return p->ptrToElement;
	}else
		return NULL;
}

void AppendToSymbolList(const char *name, int nameLength, enum Types type)
{
	list_t l = lists[Hash(name, nameLength)];
	IdentifierProperty i;
	i.value.type = type;
	strncpy(i.name, name, nameLength);
	List_InsertIntoFirst(l, &i);
	return;
}

void PrintVarVal(VarVal v)
{
	if(v.type == int_t)
		printf("%d\n", v.val.i);
	else if(v.type == float_t)
		printf("%f\n", v.val.f);
	return;
}

/*private funcs: */
size_t Hash(const char *name, int nameLength)
{
	size_t hashVal = 0;
	size_t i;
	for(i = 0; i < nameLength; i++)
		hashVal = (hashVal << 5) + name[i];
	return hashVal % tblSize;
}