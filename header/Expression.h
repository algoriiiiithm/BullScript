#ifndef EXPRESSION_
#define EXPRESSION_

typedef struct{
	enum Tokens tokType;
	union {
		enum Operators op;
		VarVal value;
		IdentifierProperty *ptr;
	} val;
} ExprUnit;

tree GetExprTreeFromUnitList(
const ExprUnit *L, size_t length, enum ExpressionLevels level, int *err);
int getBoolVal(VarVal);
void FreeExprTreeWithoutCalc(tree T);
#endif