#include <stdio.h>
#include "../header/BullScript.h"
#include "../header/Expression.h"
#include "../header/test.h"

void PrintExprTree(tree T, int depth)
{
	int n = depth;
	while(n--) printf("\t\t");
	if(T->eU.tokType == opt){
		putchar(T->eU.val.op);
		putchar('\n');
		PrintExprTree(T->left, depth + 1);
		if(T->right != NULL)
			PrintExprTree(T->right, depth + 1);
	}else if(T->eU.tokType == cst){
		if(T->eU.val.value.type == int_t)
			printf("%d:int\n", T->eU.val.value.val.i);
		if(T->eU.val.value.type == float_t)
			printf("%f:float\n", T->eU.val.value.val.f);
	}else if(T->eU.tokType == idt){
		if(T->eU.val.ptr->value.type == int_t)
			printf("%d:int\n", T->eU.val.ptr->value.val.i);
		else
			printf("%f:float\n", T->eU.val.ptr->value.val.f);
	}
	return;
}


/*
void PrintExprUnitsList(ExprUnit *L, int length)
{
	int i;
	for(i = 0; i < length; i++){
		if(L[i].tokType == opt){
			putchar(L[i].val.op);
			putchar('\n');
		}else if(L[i].tokType == cst){
			if(L[i].val.value.type == int_t)
				printf("%d:int\n", L[i].val.value.val.i);
			if(L[i].val.value.type == float_t)
				printf("%f:float\n", L[i].val.value.val.f);
		}else if(L[i].tokType == idt){
			printf(L[i].val.ptr->name);
		}
	}
	return;
}
*/