#ifndef BULL_SCRIPT
#define BULL_SCRIPT

#include <stddef.h>

#define IDENTIFIER_MAX_LENGTH 128
#define EXPR_UNIT_LIST_MAX_LENGTH 256
#define STATEMENT_MAX_LENGTH 512

enum Types{int_t, float_t, void_t};
enum ExpressionLevels{assExpr, orExpr, andExpr, eqlExpr, relaExpr, addExpr, mulExpr, unaExpr, priExpr};
enum Operators{
	add = '+',
	sub = '-',
	mul = '*',
	divi = '/',
	mod = '%',
	lp = '(',
	rp = ')',
	ass = '=',
	eql = ':',
	grtr = '>',
	less = '<',
	and = '&',
	or = '|',
	not = '!',
};

enum Tokens{idt, kwd, cst, stl, opt, se};

extern char keywords[8][32];

typedef struct{
	union{
		int i;
		float f;
	} val;
	enum Types type;
} VarVal;

typedef struct {
	char name[IDENTIFIER_MAX_LENGTH];
	VarVal value;
} IdentifierProperty;

typedef struct TreeNode *tree;

/*funcs in Expression.c: */

VarVal CalcExprTree(tree T);
/*funcs in Runtime.c: */
void CreateSymbolList(void);
IdentifierProperty *GetIdentifierProperty(
const char *name, int nameLength);
	/*if name dosn't exist, this function will return NULL*/
void AppendToSymbolList(const char *name, int nameLength, enum Types type);
void PrintVarVal(VarVal v);

void handler(char *);

#endif