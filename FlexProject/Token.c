#include "Token.h"

/* This package describes the storage of tokens identified in the input text.
* The storage is a bi-directional list of nodes.
* Each node is an array of tokens; the size of this array is defined as TOKEN_ARRAY_SIZE.
* Such data structure supports an efficient way to manipulate tokens.

There are three functions providing an external access to the storage:
- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.
- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)
*/

int currentIndex = 0;
Node* currentNode = NULL;
Node* head = NULL;
Node* tail = NULL;
bool flag = 1;

#define TOKEN_ARRAY_SIZE 100

/*
* This function creates a token and stores it in the storage.
*/
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{ 
	int length = strlen(lexeme)+1;
	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if(currentNode == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);
		if(currentNode->tokensArray == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if(currentNode == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);

			if(currentNode->tokensArray == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next = NULL;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	

	currentNode->tokensArray[currentIndex].kind = kind;	
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;

	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*length);

	if (flag == 1) {
		head = currentNode;
		flag = 0;
	}
	#ifdef _WIN32
		strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
	#else
		strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
	#endif	

}

/*
* This function returns the token in the storage that is stored immediately before the current token (if exsits).
*/
Token *back_token() {

	if (!currentNode){
		return NULL;
	}

	if (currentIndex == 0 && currentNode == head){
		return NULL;
	}

	if (currentIndex == 0){
		currentIndex = 99;
		currentNode = currentNode->prev;

		if (!currentNode){
			return NULL;
		}
		else
		{
			return &(currentNode->tokensArray[currentIndex]);
		}
	}

	return &(currentNode->tokensArray[--currentIndex]);
}

/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token): 
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token *next_token() {

	if (!currentNode){
		return NULL;
	}

	if (currentNode == tail && currentIndex == 99){
		return NULL;
	}

	if (currentIndex == 99){
		currentIndex = 0;
		currentNode = currentNode->next;

		if (!currentNode){
			return NULL;
		}
		else
		{
			return &(currentNode->tokensArray[currentIndex]);
		}
	}
	else

		return &(currentNode->tokensArray[++currentIndex]);
	
}

void print_array(eTOKENS *arr, int size)
{
	int i;

	for (i = 0; i < size; i++)
	{
		fprintf(yyout, "%s, ", myenum_str(arr[i]));
	}
}




void handle_error(eTOKENS *followArr, eTOKENS *firstArr, int size_follow, int size_first, Token *currentNode, int nullable)
{
	int i ;
	int flag = 1;
	
	fprintf(yyout, "Expected: one of tokens ");
	print_array(firstArr, size_first);

	if (nullable == 1) {
		print_array(followArr, size_follow);
	}

	fprintf(yyout, "at line current: %d, Actual token: %s, lexeme: ' %s '.\n"
	, currentNode->lineNumber, myenum_str(currentNode->kind) , currentNode->lexeme);
	
	for (i=0 ; i<size_follow ; i++)
	  {
		while (currentNode && flag)
		{
			if (currentNode->kind != followArr[i] && currentNode->kind != TOKEN_EOF)
			{
			currentNode = next_token();
			}
			else
			{
				flag = 0;
			}
		}
	}

	if (currentNode->kind == TOKEN_EOF)
	{
		exit(0);
	}
	else
		currentNode = back_token();
}

Token* reset_head()
{
	currentNode = head;
	currentIndex = 0;
	return &(currentNode->tokensArray[currentIndex]);
}

void reset_parsing() 
{
	currentIndex = 0;
	currentNode = NULL;
	head = NULL;
	tail = NULL;
	flag = 1;
}


