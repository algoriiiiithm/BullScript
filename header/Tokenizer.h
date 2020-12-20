#ifndef TOKENIZER_
#define TOKENIZER_
#include "../header/BullScript.h"

enum TokenTypes{
	lBrace,
	rBrace,
	lParenthesis,
	rParenthesis,
	comma,
	plus,
	minus,
	multiply,
	divide,
	modulo,
	assign,
	equality,
	greater,
	smaller,
	logicalNOT,
	semicolon,
	identifier,
	whileKWD,
	ifKWD,
	intKWD,
	floatKWD,
	constant,
	floatConstant,
	stringConstant,
	null
};

typedef struct {
	enum TokenTypes type;
	char *identifierString;
} Token;

void newChar(char c);
void initializeTokenizer(void);
#endif