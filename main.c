#include <stdio.h>
#include "container/list.h"
#include "header/BullScript.h"
#include "header/Tokenizer.h"
#include "header/Parser.h"
#include "header/Statement.h"
#include "header/Manager.h"

extern list_t tokenList;

int statementParser(list_t tokenList, list_elementIndex begin, list_elementIndex end, StatementObject *stmtPort);

int main(void)
{
	char c;
	int i;
	int r;
	list_elementIndex iter;
	StatementObject stmt;
	void initializeTokenizer(void);
	initializeTokenizer();
	CreateSymbolList();
	while((c = getchar()) !=  EOF){
		if(c == '\\'){
			while((c = getchar()) != '\n') ;
		}
		newChar(c);
		if( tokenList->elementsNumber > 1 && ((Token*)(tokenList->ptrToLastNode->ptrToElement))->type == null){
			parser(tokenList, tokenList->ptrToFirstNode, tokenList->ptrToLastNode);
			externalStatementExecute();
		}
	}
	return 0;
}
