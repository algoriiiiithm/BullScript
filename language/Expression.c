#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../header/BullScript.h"
#include "../header/Expression.h"

#define isnum(c) (isdigit(c) || (c) == '.' || (c) == 'e' || (c) == 'E')

typedef struct TreeNode{
	ExprUnit eU;
	struct TreeNode *left;
	struct TreeNode *right;
} *tree;

typedef int (*ExprUnitcalctchFunc)(const ExprUnit*, size_t, size_t);


void FreeExprTreeWithoutCalc(tree T);
tree BuildNode(const ExprUnit *eu);
ptrdiff_t FindFirstOccurence(
const ExprUnit *L, size_t length, ExprUnitcalctchFunc func, int direction);
/*direction: 0 left to right, 1 right to left*/
int Equalitycalctch(const ExprUnit* list, size_t length, size_t index);
int Relationalcalctch(const ExprUnit *list, size_t length, size_t index);
int Additivecalctch(const ExprUnit* list, size_t length, size_t index);
int Multiplicativecalctch(const ExprUnit* list, size_t length, size_t index);
int Unarycalctch(const ExprUnit* list, size_t length, size_t index);
int Primarycalctch(const ExprUnit* list, size_t length, size_t index);
int Assignmentcalctch(const ExprUnit* list, size_t length, size_t index);
VarVal BinaryCalc(VarVal l, VarVal r, enum Operators op);

VarVal CalcExprTree(tree T)
{
	VarVal retVal, l, r;
	enum Operators op = T->eU.val.op;
	if(T->eU.tokType == opt){
		switch(op){
		case mod:
			l = CalcExprTree(T->left);
			r = CalcExprTree(T->right);
			retVal.type = int_t;
			retVal.val.i = l.val.i % r.val.i;
			break;
		case mul: case divi:  case eql: case grtr: case less:
			l = CalcExprTree(T->left);
			r = CalcExprTree(T->right);
			retVal = BinaryCalc(l, r, op);
			break;
		case add: case sub:
			if(T->right != NULL){
				l = CalcExprTree(T->left);
				r = CalcExprTree(T->right);
				retVal = BinaryCalc(l, r, op);
			}else{
				retVal = CalcExprTree(T->left);
				if(op == sub)
					(retVal.type == float_t)?
					(retVal.val.f = - retVal.val.f):
					(retVal.val.i = - retVal.val.i);
			}
			break;
		case not:
			retVal.type = int_t;
			retVal.val.i = !getBoolVal(CalcExprTree(T->left));
			break;
		case ass:
			{
			VarVal *idData = &(T->left->eU.val.ptr->value);
			r = CalcExprTree(T->right);
			if(idData->type == float_t){
				idData->val.f = (r.type == float_t) ?
				(r.val.f) : (r.val.i);
			}else{
				idData->val.i = (r.type == float_t) ?
				(r.val.f) : (r.val.i);
			}
			retVal = *idData;
			retVal.type = void_t;
			}
			break;
		default:
			break;
		}
	}else if(T->eU.tokType == cst){
		retVal = T->eU.val.value;
	}else if(T->eU.tokType == idt){
		retVal = T->eU.val.ptr->value;
	}
	return retVal;
}

/*private funcs:*/
tree GetExprTreeFromUnitList(
const ExprUnit *L, size_t length, enum ExpressionLevels level, int *err)
{
	tree root = NULL;
	ptrdiff_t i;
	switch(level){
	case assExpr:
		i = FindFirstOccurence(L, length, Assignmentcalctch, 0);
		if(i != -1){
			root = BuildNode(L + i);
			root->left = GetExprTreeFromUnitList(L, i, priExpr, err);
			root->right = GetExprTreeFromUnitList(
			L + i + 1, length - i - 1, assExpr, err);
		}else{
			root = GetExprTreeFromUnitList(L, length, eqlExpr, err);
		}
		break;
	case eqlExpr:
		i = FindFirstOccurence(L, length, Equalitycalctch, 1);
		if(i != -1){
			root = BuildNode(L + i);
			root->left = GetExprTreeFromUnitList(L, i, eqlExpr, err);
			root->right = GetExprTreeFromUnitList(
			L + i + 1, length - i - 1, relaExpr, err);
		}else{
			root = GetExprTreeFromUnitList(L, length, relaExpr, err);
		}
		break;
	case relaExpr:
		i = FindFirstOccurence(L, length, Relationalcalctch, 1);
		if(i != -1){
			root = BuildNode(L + i);
			root->left = GetExprTreeFromUnitList(L, i, relaExpr, err);
			root->right = GetExprTreeFromUnitList(
			L + i + 1, length - i - 1, addExpr, err);
		}else{
			root = GetExprTreeFromUnitList(L, length, addExpr, err);
		}
		break;
	case addExpr:
		i = FindFirstOccurence(L, length, Additivecalctch, 1);
		if(i != -1){
			root = BuildNode(L + i);
			root->left = GetExprTreeFromUnitList(L, i, addExpr, err);
			root->right = GetExprTreeFromUnitList(
			L + i + 1, length - i - 1, mulExpr, err);
		}else{
			root = GetExprTreeFromUnitList(L, length, mulExpr, err);
		}
		break;
	case mulExpr:
		i = FindFirstOccurence(L, length, Multiplicativecalctch, 1);
		if(i != -1){
			root = BuildNode(L + i);
			root->left = GetExprTreeFromUnitList(L, i, mulExpr, err);
			root->right = GetExprTreeFromUnitList(
			L + i + 1, length - i - 1, unaExpr, err);
		}else{
			root = GetExprTreeFromUnitList(L, length, unaExpr, err);
		}
		break;
	case unaExpr:
		i = FindFirstOccurence(L, length, Unarycalctch, 1);
		if(i != -1){
			root = BuildNode(L + i);
			root->left = GetExprTreeFromUnitList(
			L + i + 1, length - i - 1, unaExpr, err);
			root->right = NULL;
		}else{
			root = GetExprTreeFromUnitList(L, length, priExpr, err);
		}
		break;
	case priExpr:
		i = FindFirstOccurence(L, length, Primarycalctch, 1);
		if(i == -1){
			if(length == 1 && (L[0].tokType == cst || L[0].tokType == idt)){
				root = BuildNode(L + 0);
			}else{
				goto except; 
			}
		}else{
			if(L[length - 1].tokType == opt && L[length - 1].val.op == rp)
				root = GetExprTreeFromUnitList(L + 1, length - 2, assExpr, err);
			else
				goto except;
		}
		break;
	default:
		except:
		*err = 1;
		break;
	}
	*err = 0;
	return root;
}

void FreeExprTreeWithoutCalc(tree T)
{
	if(T == NULL)
		return;
	else{
		FreeExprTreeWithoutCalc(T->left);
		FreeExprTreeWithoutCalc(T->right);
		free(T);
	}
	return;
}

tree BuildNode(const ExprUnit *eu)
{
	tree T;
	T = malloc(sizeof(*T));
	T->eU = *eu;
	T->left = NULL;
	T->right = NULL;
	return T;
}

VarVal BinaryCalc(VarVal l, VarVal r, enum Operators op)
{
	#define calc(op)  \
	do{  \
		if(l.type == float_t || r.type == float_t){  \
			retVal.type = float_t;  \
			retVal.val.f =   \
			(  \
				(l.type == float_t) ? (l.val.f) : (l.val.i)  \
			) op  \
			(  \
				(r.type == float_t) ? (r.val.f) : (r.val.i)  \
			);  \
		}else{  \
			retVal.type = int_t;  \
			retVal.val.i =  \
			(  \
				(l.type == float_t) ? (l.val.f) : (l.val.i)  \
			) op   \
			(  \
				(r.type == float_t) ? (r.val.f) : (r.val.i)  \
			);  \
		}  \
	}while(0)

	VarVal retVal;
	switch(op)
	{
	case mul:
		calc(*);
		break;
	case divi:
		calc(/);
		break;
	case add:
		calc(+);
		break;
	case sub:
		calc(-);
		break;
	case eql:
		calc(==);
		break;
	case grtr:
		calc(>);
		break;
	case less:
		calc(<);
		break;
	default:
		break;
	}
	return retVal;
	#undef calc
}

ptrdiff_t FindFirstOccurence(
const ExprUnit *L, size_t length, ExprUnitcalctchFunc func, int direction)
{
	ptrdiff_t i;
	int pc = 0;
	if(direction == 0){
		for(i = 0; i < length; i++){
			if(L[i].tokType == opt){
				if(L[i].val.op == lp) pc++;
				if(L[i].val.op == rp) pc--;
			}
			if(pc) continue;
			if(func(L, length, i)) return i;
		}
	}else{
		for(i = length - 1; i >= 0; i--){
			if(L[i].tokType == opt){
				if(L[i].val.op == lp) pc++;
				if(L[i].val.op == rp) pc--;
			}
			if(pc) continue;
			if(func(L, length, i)) return i;
		}
	}
	return -1;
}

int Equalitycalctch(const ExprUnit* list, size_t length, size_t index)
{
	return list[index].tokType == opt && 
	(list[index].val.op == eql);
}

int Relationalcalctch(const ExprUnit *list, size_t length, size_t index)
{
	return list[index].tokType == opt && 
	(list[index].val.op == grtr || list[index].val.op == less);
}

int Additivecalctch(const ExprUnit* list, size_t length, size_t index)
{
	return list[index].tokType == opt &&
	(list[index].val.op == sub  || list[index].val.op == add) &&
	index >= 1 &&
	(list[index - 1].tokType != opt || 
		(list[index - 1].tokType == opt && list[index - 1].val.op == rp)
	);
}

int Multiplicativecalctch(const ExprUnit* list, size_t length, size_t index)
{
	return list[index].tokType == opt && 
	(list[index].val.op == mul || list[index].val.op == divi || list[index].val.op == mod);
}

int Assignmentcalctch(const ExprUnit* list, size_t length, size_t index)
{
	return list[index].tokType == opt &&
	list[index].val.op == ass;
}

int Unarycalctch(const ExprUnit* list, size_t length, size_t index)
{
	return index == 0 && list[index].tokType == opt &&
	(list[index].val.op == add || list[index].val.op == sub || list[index].val.op == not);
}

int Primarycalctch(const ExprUnit* list, size_t length, size_t index)
{
	return index == 0 &&
	(list[index].tokType == opt && list[index].val.op == lp);
}

int getBoolVal(VarVal x)
{
	if(x.type == int_t)
		return x.val.i != 0;
	else if(x.type == float_t)
		return x.val.f != 0;
}