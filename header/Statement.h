#ifndef STATEMENT_
#define STATEMENT_
#include "../container/list.h"
#include "../header/BullScript.h"

enum stmtTypes{exprStmt, whileStmt, compoundStmt, ifStmt};

typedef struct{
	list_t stmtList;
} CompoundStmtObject;

typedef struct{
	tree expr;
} ExprStmtObject;

struct StatementObject_;

typedef struct{
	tree condition;
	struct StatementObject_ *body;
} WhileStmtObject;

typedef struct{
	tree condition;
	struct StatementObject_ *body;
} ifStmtObject;

typedef struct StatementObject_{
	enum stmtTypes type;
	union {
		CompoundStmtObject compoundStmt;
		ExprStmtObject exprStmt;
		WhileStmtObject whileStmt;
		ifStmtObject ifStmt;
	} stmtObjPtr;
} StatementObject;
#endif