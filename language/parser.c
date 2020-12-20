#include <stdio.h>
#include "../header/parser.h"
#include "../header/BullScript.h"
#include "../header/statement.h"
#include "../header/Tokenizer.h"
#include "../header/Expression.h"
#include "../header/test.h"

extern StatementObject activeExternalStatement;
extern int externalStatementSignal;

int declarationParser(list_t tokenList, list_elementIndex begin, list_elementIndex end);
int statementParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, StatementObject *stmtPort);


int parser(list_t tokenList, list_elementIndex begin, list_elementIndex end)
{
	int r;
	
	r = declarationParser(tokenList, begin, end);
	if(r == 0){
		List_Free(tokenList);
		initializeTokenizer();
		return 0;
	}
	r = statementParser(tokenList, begin, end, &activeExternalStatement);
	if(r == 0){
		externalStatementSignal = 1;
		List_Free(tokenList);
		initializeTokenizer();
		return 0;
	}

	return -1;
}

int declarationParser(list_t tokenList, list_elementIndex begin, list_elementIndex end)
{
	Token thisToken;
	list_elementIndex iter = begin;
	enum TokenTypes T;
	enum Types typeArg;
	char *name;

	thisToken = *(Token*)(iter->ptrToElement);
	if(thisToken.type != intKWD && thisToken.type != floatKWD)
		return -1;
	T = thisToken.type;
	iter = iter->next;

	thisToken = *(Token*)(iter->ptrToElement);
	if(thisToken.type != identifier)
		return -1;
	name = thisToken.identifierString;
	iter = iter->next;

	thisToken = *(Token*)(iter->ptrToElement);
	if(thisToken.type != semicolon)
		return -1;
	iter = iter->next;

	if(iter != end) return -1;

	if(GetIdentifierProperty(name, strlen(name)) != NULL){
		char errStr[256];
		sprintf(errStr, "redeclaration of identifier %s", name);
		handler(errStr);
	}
	if(T == intKWD)
		typeArg = int_t;
	else if(T == floatKWD)
		typeArg = float_t;
	AppendToSymbolList(name, strlen(name), typeArg);
	extern StatementObject *activeStmt;
	return 0;
}

int exprStatementParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, ExprStmtObject *exprStmtPort);
int compoundStatementParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, CompoundStmtObject *compoundStmtPort);
int whileStatementParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, WhileStmtObject *whileStmtPort);
int ifStatementParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, ifStmtObject *ifStmtPort);

int statementParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, StatementObject *stmtPort)
{
	int r;
	ExprStmtObject eso;
	CompoundStmtObject cso;
	WhileStmtObject wso;
	ifStmtObject iso;

	r = exprStatementParser(tokenList, begin, end, &eso);
	if(r == 0){
		stmtPort->type = exprStmt;
		stmtPort->stmtObjPtr.exprStmt = eso;
		return 0;
	}

	r = compoundStatementParser(tokenList, begin, end, &cso);
	if(r == 0){
		stmtPort->type = compoundStmt;
		stmtPort->stmtObjPtr.compoundStmt = cso;
		return 0;
	}

	r = whileStatementParser(tokenList, begin, end, &wso);
	if(r == 0){
		stmtPort->type = whileStmt;
		stmtPort->stmtObjPtr.whileStmt = wso;
		return 0;
	}

	r = ifStatementParser(tokenList, begin, end, &iso);
	if(r == 0){
		stmtPort->type = ifStmt;
		stmtPort->stmtObjPtr.ifStmt = iso;
		return 0;
	}
	return -1;
}

int expressionParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, tree *whileStmtPort);

int exprStatementParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, ExprStmtObject *exprStmtPort)
{
	tree expr;
	if( ((Token*)(end->prev->ptrToElement))->type != semicolon 	) return -1;
	if(expressionParser(tokenList, begin, end->prev, &expr) == -1)
		return -1;
	exprStmtPort->expr = expr;
	return 0;
}

int compoundStatementParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, CompoundStmtObject *compoundStmtPort)
{
	int remainToken = tokenList->elementsNumber - 2;
	list_elementIndex lPtr;
	Token first, last;
	list_t stmtList;
	int flag = 0;

	
	first = *(Token*)(begin->ptrToElement);
	last = *(Token*)(end->prev->ptrToElement);
	if(!(first.type == lBrace && last.type == rBrace))
		return -1;

	stmtList = List_Create(sizeof(StatementObject));
	lPtr = begin->next;
	while(lPtr != end->prev){
		list_elementIndex rPtr;
		StatementObject stmtO;
		for(rPtr = lPtr->next; rPtr != end->next; rPtr = rPtr->next){
			int r;
			r = statementParser(tokenList, lPtr, rPtr, &stmtO);
			if(r == 0){
				break;
			} else if(r == -1)
				continue;
		}
		if(rPtr == end->next){
			List_Free(stmtList);
			return -1;
		}else{
			List_InsertIntoLast(stmtList, &stmtO);
			lPtr = rPtr;
		}
	}
	compoundStmtPort->stmtList = stmtList;
	return 0;
}

int whileStatementParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, WhileStmtObject *whileStmtPort)
{
	int r, pm = 0;
	StatementObject output;
	list_elementIndex iter;
	tree cond;

	if( ((Token*)(begin->ptrToElement))->type != whileKWD)
		return -1;

	iter = begin->next;
	do{
		if( ((Token*)(iter->ptrToElement))->type == lParenthesis)
			pm++;
		else if( ((Token*)(iter->ptrToElement))->type == rParenthesis)
			pm--;
		iter = iter->next;
		if(iter == end) return -1;
	}while(pm != 0);
	if(expressionParser(tokenList, begin->next, iter, &cond) == -1)
		return -1;
	if(statementParser(tokenList, iter, end, &output) == -1)
		return -1;
	whileStmtPort->condition = cond;
	whileStmtPort->body = malloc(sizeof(output));
	*(whileStmtPort->body) = output;
	return 0;
}

int ifStatementParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, ifStmtObject *ifStmtPort)
{
	int r, pm = 0;
	StatementObject output;
	list_elementIndex iter;
	tree cond;

	if( ((Token*)(begin->ptrToElement))->type != ifKWD)
		return -1;

	iter = begin->next;
	do{
		if( ((Token*)(iter->ptrToElement))->type == lParenthesis)
			pm++;
		else if( ((Token*)(iter->ptrToElement))->type == rParenthesis)
			pm--;
		iter = iter->next;
		if(iter == end) return -1;
	}while(pm != 0);
	if(expressionParser(tokenList, begin->next, iter, &cond) == -1)
		return -1;
	if(statementParser(tokenList, iter, end, &output) == -1)
		return -1;
	ifStmtPort->condition = cond;
	ifStmtPort->body = malloc(sizeof(output));
	*(ifStmtPort->body) = output;
	return 0;
}

int expressionParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, tree *exprPort)
{
	ExprUnit L[EXPR_UNIT_LIST_MAX_LENGTH];
	list_elementIndex iter;
	tree out;
	int err;
	int i = 0;

	for(iter = begin; iter != end; iter = iter->next){
		Token thisToken = *(Token*)(iter->ptrToElement);
		switch(thisToken.type){
		case lParenthesis:
			L[i].tokType = opt;
			L[i].val.op = lp;
			break;
		case rParenthesis:
			L[i].tokType = opt;
			L[i].val.op = rp;
			break;
		case plus:
			L[i].tokType = opt;
			L[i].val.op = add;
			break;
		case minus:
			L[i].tokType = opt;
			L[i].val.op = sub;
			break;
		case multiply:
			L[i].tokType = opt;
			L[i].val.op = mul;
			break;
		case divide:
			L[i].tokType = opt;
			L[i].val.op = divi;
			break;
		case assign:
			L[i].tokType = opt;
			L[i].val.op = ass;
			break;
		case equality:
			L[i].tokType = opt;
			L[i].val.op = eql;
			break;
		case greater:
			L[i].tokType = opt;
			L[i].val.op = grtr;
			break;
		case smaller:
			L[i].tokType = opt;
			L[i].val.op = less;
			break;
		case modulo:
			L[i].tokType = opt;
			L[i].val.op = mod;
			break;
		case logicalNOT:
			L[i].tokType = opt;
			L[i].val.op = not;
			break;
		case identifier:{
			L[i].tokType = idt;
			L[i].val.ptr = GetIdentifierProperty(thisToken.identifierString, strlen(thisToken.identifierString));
			if(L[i].val.ptr == NULL){
				char errstring [256];
				sprintf(errstring, "undeclared identifier '%s'", thisToken.identifierString);
				handler(errstring);
			}
			}break;
		case constant:
			L[i].tokType = cst;
			L[i].val.value.type = int_t;
			L[i].val.value.val.i = atoi(thisToken.identifierString);
			break;
		case floatConstant:
			L[i].tokType = cst;
			L[i].val.value.type = float_t;
			L[i].val.value.val.f = (float)atof(thisToken.identifierString);
			break;
		default:
			return -1;
			break;
		}
		i++;
	}
	out = GetExprTreeFromUnitList(L, i, assExpr, &err);
	if(!err){
		*exprPort = out;
		return 0;
	}else{
		handler("invalid syntax");
		return -1;
	}
}
