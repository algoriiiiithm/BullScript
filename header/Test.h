#ifndef TEST_
#define TEST_
/*extern funcs:*/

/*local funcs:*/
void PrintExprTree(tree T, int depth);
/*void PrintExprUnitsList(ExprUnit *L, int length);*/


typedef struct TreeNode{
	ExprUnit eU;
	struct TreeNode *left;
	struct TreeNode *right;
} *tree;

typedef int (*ExprUnitcalctchFunc)(const ExprUnit*, size_t, size_t);

#endif