#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern FILE *yyin, *yyout , *yyout1 ,*yyout2 ;

typedef enum eTOKENS
{
	TOKEN_ID,
	TOKEN_MULTIPLICATION,
	TOKEN_DIVISION,
	TOKEN_ASSIGNMENT,
	TOKEN_INT_NUMBER,
	TOKEN_REAL_NUMBER,
	TOKEN_KW_PROGRAM, 
	TOKEN_KW_RETURN,
	TOKEN_KW_INTEGER,
	TOKEN_KW_REAL,
	TOKEN_KW_VOID,
	TOKEN_KW_END,
	TOKEN_COMMA,
	TOKEN_SEMICOLON,
	TOKEN_ROUND_BRACKET_RIGHT,
	TOKEN_ROUND_BRACKET_LEFT,
	TOKEN_SQUARE_BRACKET_RIGHT,
	TOKEN_SQUARE_BRACKET_LEFT,
	TOKEN_CURLY_BRACKET_RIGHT,
	TOKEN_CURLY_BRACKET_LEFT,
	TOKEN_COMMENT,
	TOKEN_EOF } eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;

}Token;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;

} Node;

Token *next_token();
Token *back_token();
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
void handle_error(eTOKENS *followArr, eTOKENS *firstArr, int size_follow, int size_first, Token *currentNode, int nullable);
void print_array(eTOKENS *arr, int size);
Token* reset_head();
void reset_parsing();

#endif