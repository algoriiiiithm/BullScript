#include "../header/statement.h"
#include "../header/Expression.h"

StatementObject stmt;
StatementObject activeExternalStatement;
int externalStatementSignal = 0;

void statementExecute(StatementObject stmt);
void destroyStatement(StatementObject *stmt);

void externalStatementExecute(void)
{
	if(!externalStatementSignal) return;
	statementExecute(activeExternalStatement);
	externalStatementSignal = 0;
	destroyStatement(&activeExternalStatement);
	return;
}

void statementExecute(StatementObject stmt)
{
	if(stmt.type == exprStmt){
		PrintVarVal(CalcExprTree(stmt.stmtObjPtr.exprStmt.expr));
	}else if(stmt.type == compoundStmt){
		int i;
		list_elementIndex iter = stmt.stmtObjPtr.compoundStmt.stmtList->ptrToFirstNode;
		for(i = 0; i < stmt.stmtObjPtr.compoundStmt.stmtList->elementsNumber; i++){
			statementExecute(*(StatementObject*)iter->ptrToElement);
			iter = iter->next;
		}
	}else if(stmt.type == whileStmt){
		while(getBoolVal(CalcExprTree(stmt.stmtObjPtr.whileStmt.condition))){
			statementExecute(*(stmt.stmtObjPtr.whileStmt.body));
		}
	}else if(stmt.type == ifStmt){
		if(getBoolVal(CalcExprTree(stmt.stmtObjPtr.ifStmt.condition))){
			statementExecute(*(stmt.stmtObjPtr.ifStmt.body));
		}
	}
}

void destroyStatement(StatementObject *stmt)
{
	switch (stmt->type)
	{
	case exprStmt:
		FreeExprTreeWithoutCalc(stmt->stmtObjPtr.exprStmt.expr);
		break;
	case compoundStmt:{
		list_elementIndex iter;
		list_t stmtList = stmt->stmtObjPtr.compoundStmt.stmtList;
		for(iter = stmtList->ptrToFirstNode; iter != NULL; iter = iter->next)
			destroyStatement((StatementObject*)(iter->ptrToElement));
		List_Free(stmtList);
		}break;
	case whileStmt:
		FreeExprTreeWithoutCalc(stmt->stmtObjPtr.whileStmt.condition);
		destroyStatement(stmt->stmtObjPtr.whileStmt.body);
		free(stmt->stmtObjPtr.whileStmt.body);
		break;
	default:
		break;
	}
}