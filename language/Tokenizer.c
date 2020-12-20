#include <stdio.h>
#include <ctype.h>
#include "../container/list.h"
#include "../header/BullScript.h"
#include "../header/Tokenizer.h"
#include "../header/statement.h"
#include "../header/Manager.h"
#include "../header/parser.h"

list_t tokenList; /*list of `Token`*/

int isalpha_(char c);
int isdigit_(char c);
int isNumber(char *);

void newToken(void);

void initializeTokenizer(void)
{
	Token nullToken;
	nullToken.type = null;
	tokenList = List_Create(sizeof(Token));
	List_InsertIntoLast(tokenList, &nullToken);
	return;
}

void newChar(char c)
{
	Token *lastToken;
	lastToken = tokenList->ptrToLastNode->ptrToElement;

	if(isalpha_(c)){
		if(lastToken->type != null && lastToken->type != identifier){
			Token nullToken;
			nullToken.type = null;
			List_InsertIntoLast(tokenList, &nullToken);
			lastToken = tokenList->ptrToLastNode->ptrToElement;
		}
		if(lastToken->type == null){
			lastToken->identifierString = malloc(sizeof(char) * IDENTIFIER_MAX_LENGTH);
			lastToken->identifierString[0] = c;
			lastToken->identifierString[1] = '\0';
			lastToken->type = identifier;
		}else{
			int i;
			for(i = 0; lastToken->identifierString[i] != '\0'; i++) ;
			lastToken->identifierString[i] = c;
			lastToken->identifierString[i + 1] = '\0';
		}
		return;
	}

	if(isdigit_(c)){
		if(lastToken->type != null && lastToken->type != constant && lastToken->type != floatConstant){
			Token nullToken;
			nullToken.type = null;
			List_InsertIntoLast(tokenList, &nullToken);
			lastToken = tokenList->ptrToLastNode->ptrToElement;
		}
		if(lastToken->type == null){
			lastToken->identifierString = malloc(sizeof(char) * IDENTIFIER_MAX_LENGTH);
			lastToken->identifierString[0] = c;
			lastToken->identifierString[1] = '\0';
			lastToken->type = constant;
			if(c == '.') lastToken->type = floatConstant;
		}else{
			int i;
			for(i = 0; lastToken->identifierString[i] != '\0'; i++) ;
			lastToken->identifierString[i] = c;
			lastToken->identifierString[i + 1] = '\0';
			if(c == '.') lastToken->type = floatConstant;
		}
		return;
	}

	if(lastToken->type != null){
		Token nullToken;
		if(lastToken->type == identifier){
			if(strcmp(lastToken->identifierString, "int") == 0)
				lastToken->type = intKWD; 
			else if(strcmp(lastToken->identifierString, "float") == 0)
				lastToken->type = floatKWD;
			else if(strcmp(lastToken->identifierString, "while") == 0)
				lastToken->type = whileKWD;
			else if(strcmp(lastToken->identifierString, "if") == 0)
				lastToken->type = ifKWD;
		}else if(lastToken->type == constant || lastToken->type == floatConstant){
			if(!isNumber(lastToken->identifierString))
				handler("invalid syntax.");
		}
		nullToken.type = null;
		List_InsertIntoLast(tokenList, &nullToken);
	}

	lastToken = tokenList->ptrToLastNode->ptrToElement;
	switch(c){
	case '{':
		lastToken->type = lBrace;
		break;
	case '}':
		lastToken->type = rBrace;
		break;
	case '(':
		lastToken->type = lParenthesis;
		break;
	case ')':
		lastToken->type = rParenthesis;
		break;
	case ',':
		lastToken->type = comma;
		break;
	case '+':
		lastToken->type = plus;
		break;
	case '-':
		lastToken->type = minus;
		break;
	case '*':
		lastToken->type = multiply;
		break;
	case '/':
		lastToken->type = divide;
		break;
	case '%':
		lastToken->type = modulo;
		break;
	case '=':
		lastToken->type = assign;
		break;
	case ';':
		lastToken->type = semicolon;
		break;
	case ':':
		lastToken->type = equality;
		break;
	case '>':
		lastToken->type = greater;
		break;
	case '<':
		lastToken->type = smaller;
		break;
	case '!':
		lastToken->type = logicalNOT;
		break;
	default:
		if(!isspace(c))
			handler("Unrecogonized character");
	}
	if(lastToken->type != null){
		Token nullToken;
		nullToken.type = null;
		List_InsertIntoLast(tokenList, &nullToken);
	}
	//parser(tokenList, )
	return;
}

int isalpha_(char c)
{
	return isalpha(c) || c == '_';
}

int isdigit_(char c)
{
	return isdigit(c) || c == '.';
}

int TransTable[11][4] = {
	{1,2,3,4},
	{4,2,3,4},
	{4,2,5,6},
	{4,7,4,4},
	{4,4,4,4},
	{4,8,4,6},
	{9,10,4,4},
	{4,7,4,6},
	{4,8,4,6},
	{4,10,4,4},
	{4,10,4,4},
};

int char2num(char);

int isNumber(char *s)
{
	int state = 0;
	int i, p, chn;

	while(*s == ' ')
		s++;
	for(i = 0; s[i] != '\0' && s[i] != ' '; i++)
		;
    p = i;
	for(;s[i] != '\0'; i++){
		if(s[i] != ' '){
			return 0;
		}
	}
    s[p] = '\0';

	while(*s != '\0'){
        chn = char2num(*s);
        if(chn == -1) return 0;
		state = TransTable[state][chn];
		s++;
	}
	switch(state){
	case 2: case 5: case 7: case 8: case 10:
		return 1;
		break;
	default:
		return 0;
		break;
	}
}

int char2num(char c)
{
	switch(c){
	case '0': case '1': case '2': case '3': case '4': 
	case '5': case '6': case '7': case '8': case '9':
		return 1;
		break;
	case '+': case '-':
		return 0;
		break;
	case '.':
		return 2;
		break;
	case 'e': case 'E':
		return 3;
		break;
	default:
        return -1;
        break;
    }
}
