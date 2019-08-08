%option noyywrap

%{
#include "Token.h"
FILE *yyout1 = NULL;
FILE *yyout2 = NULL;

 void TokenStoreAndPrint(eTOKENS kind, char* lexeme, int numOfLine); 
 const char *myenum_str(eTOKENS e);
%}

DIGIT		[0-9]
NONZERO		[1-9]

COMMENT               "--".*
MULTIPLICATION        "*"
DIVISION              "/"
ASSIGNMENT            "="
INT_NUM               ("+"|"-")?(0|[1-9][0-9]*)
REAL_NUM              ({NONZERO}{DIGIT}*|0)"."{DIGIT}+
COMMA                 ","
SEMICOLON             ";"
ROUND_BRACKET_RIGHT   ")"
ROUND_BRACKET_LEFT    "("
SQUARE_BRACKET_RIGHT  "]"
SQUARE_BRACKET_LEFT   "["
CURLY_BRACKET_RIGHT   "}"
CURLY_BRACKET_LEFT    "{"
ID                    [A-Za-z][0-9A-Za-z]*(_{0,1}[0-9A-Za-z]+)*




%%  
[\n]                    { yylineno++; }
[ \t\n] 	            {} 

"program" 		{  TokenStoreAndPrint(TOKEN_KW_PROGRAM , yytext , yylineno);}
"end" 			{  TokenStoreAndPrint(TOKEN_KW_END , yytext , yylineno);}
"real" 			{  TokenStoreAndPrint(TOKEN_KW_REAL , yytext , yylineno);}
"integer"		{  TokenStoreAndPrint(TOKEN_KW_INTEGER , yytext , yylineno);}
"void"			{  TokenStoreAndPrint(TOKEN_KW_VOID , yytext , yylineno);}
"return"		{  TokenStoreAndPrint(TOKEN_KW_RETURN , yytext , yylineno);}
{COMMENT}               {fprintf(yyout ,"Token of kind COMMENT was found at line: %d, lexeme: '%s'.\n" , yylineno, yytext); }
{MULTIPLICATION}        {TokenStoreAndPrint(TOKEN_MULTIPLICATION, yytext, yylineno);}
{DIVISION}              {TokenStoreAndPrint(TOKEN_DIVISION, yytext, yylineno);}
{ASSIGNMENT}            {TokenStoreAndPrint(TOKEN_ASSIGNMENT, yytext, yylineno);}
{INT_NUM}               {TokenStoreAndPrint(TOKEN_INT_NUMBER, yytext, yylineno);}
{REAL_NUM}              {TokenStoreAndPrint(TOKEN_REAL_NUMBER, yytext, yylineno);}
{COMMA}                 {TokenStoreAndPrint(TOKEN_COMMA, yytext, yylineno);}
{SEMICOLON}             {TokenStoreAndPrint(TOKEN_SEMICOLON, yytext, yylineno);}
{ROUND_BRACKET_RIGHT}   {TokenStoreAndPrint(TOKEN_ROUND_BRACKET_RIGHT, yytext, yylineno);}
{ROUND_BRACKET_LEFT}    {TokenStoreAndPrint(TOKEN_ROUND_BRACKET_LEFT, yytext, yylineno);}
{SQUARE_BRACKET_RIGHT}  {TokenStoreAndPrint(TOKEN_SQUARE_BRACKET_RIGHT, yytext, yylineno);}
{SQUARE_BRACKET_LEFT}   {TokenStoreAndPrint(TOKEN_SQUARE_BRACKET_LEFT, yytext, yylineno);}
{CURLY_BRACKET_RIGHT}   {TokenStoreAndPrint(TOKEN_CURLY_BRACKET_RIGHT, yytext, yylineno);}
{CURLY_BRACKET_LEFT}    {TokenStoreAndPrint(TOKEN_CURLY_BRACKET_LEFT, yytext, yylineno);}
{ID}                    {TokenStoreAndPrint(TOKEN_ID, yytext, yylineno);}
<<EOF>>                 {TokenStoreAndPrint(TOKEN_EOF, yytext, yylineno);  return 1;} 
	.       	        {fprintf(yyout ,"The character '%s' at line: %d does not begin any legal token in the language.\n", yytext, yylineno );}

%%

 void TokenStoreAndPrint(eTOKENS kind, char* lexeme, int numOfLine){
	
	create_and_store_token(kind, lexeme, numOfLine);
	fprintf(yyout ,"Token of kind %s was found at line: %d, lexeme: '%s'.\n",myenum_str(kind) , numOfLine, lexeme); 
	
}  


const char *myenum_str(eTOKENS e)
{
    switch(e) {
	case TOKEN_ID: return "TOKEN_ID";
	case TOKEN_KW_PROGRAM : return "TOKEN_KW_PROGRAM";
	case TOKEN_KW_REAL: return "TOKEN_KW_REAL";
	case TOKEN_KW_END: return "TOKEN_KW_END";
	case TOKEN_KW_INTEGER: return "TOKEN_KW_INTEGER";
	case TOKEN_KW_VOID: return "TOKEN_KW_VOID";
	case TOKEN_KW_RETURN: return "TOKEN_KW_RETURN";	
    case TOKEN_MULTIPLICATION : return "TOKEN_MULTIPLICATION";
	case TOKEN_DIVISION : return "TOKEN_DIVISION";
	case TOKEN_ASSIGNMENT : return "TOKEN_ASSIGNMENT";
    case TOKEN_INT_NUMBER: return "TOKEN_INT_NUMBER";
	case TOKEN_REAL_NUMBER: return "TOKEN_REAL_NUMBER";
	case TOKEN_COMMA: return "TOKEN_COMMA";
	case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
	case TOKEN_ROUND_BRACKET_RIGHT: return "TOKEN_ROUND_BRACKET_RIGHT";
	case TOKEN_ROUND_BRACKET_LEFT: return "TOKEN_ROUND_BRACKET_LEFT";
	case TOKEN_SQUARE_BRACKET_RIGHT: return "TOKEN_SQUARE_BRACKET_RIGHT";
	case TOKEN_SQUARE_BRACKET_LEFT: return "TOKEN_SQUARE_BRACKET_LEFT";
	case TOKEN_CURLY_BRACKET_RIGHT : return "TOKEN_CURLY_BRACKET_RIGHT";
	case TOKEN_CURLY_BRACKET_LEFT : return "TOKEN_CURLY_BRACKET_LEFT";
	case TOKEN_EOF: return "TOKEN_EOF";
	
    }
    return "Invalid";
}

int main(){

	const char* test1_Resource = "C:\\temp\\test1.txt";
	const char* test2_Resource = "C:\\temp\\test2.txt";
	
	const char* lexOutput1  = "C:\\temp\\test1_021953237_203217898_lex.txt";
	const char* lexOutput2  = "C:\\temp\\test2_021953237_203217898_lex.txt";
	
	const char* syntacticOutput1 = "C:\\temp\\test1_021953237_203217898_syntactic.txt";
    const char* syntacticOutput2 = "C:\\temp\\test2_021953237_203217898_syntactic.txt";

	const char* semanticOutput1 = "C:\\temp\\test1_021953237_203217898_semantic.txt";
	const char* semanticOutput2 = "C:\\temp\\test2_021953237_203217898_semantic.txt";
	
	
	yyin = fopen(test1_Resource, "r"); // test 1 - LEX
	yyout = fopen(lexOutput1, "w");

	if (yyin) {
		yylex();
		fclose(yyin);
		fclose(yyout);
	}
	else {
		printf("The file 'test1' is not found.\n");
	}

	yyin = fopen(test1_Resource, "r"); //  test 1 - Syntactic and Semantic
	yyout1 = fopen(syntacticOutput1, "w");
	yyout2 = fopen(semanticOutput1, "w");


	if (yyin)
	{
		yylineno = 1;
		Parser();
		yyrestart(yyin);

		fclose(yyin);
		fclose(yyout1);
		fclose(yyout2);
	}

	else {
		printf("The file 'test1' is not found.\n");
	}

	yyout2 = fopen(syntacticOutput1, "w");


	reset_parsing();

	yyin = fopen(test2_Resource, "r"); // test 2 - LEX
	yyout = fopen(lexOutput2, "w");

	if (yyin) {
		yylineno = 1;
		yylex();
		fclose(yyin);
		fclose(yyout);
	}
	else {
		printf("The file 'test2' is not found.\n");
	}


	yyin = fopen(test2_Resource, "r"); //  test 2 - Syntactic and Semantic
	yyout1 = fopen(syntacticOutput2, "w");
	yyout2 = fopen(semanticOutput2, "w");


	if (yyin)
	{
		yylineno = 1;
		Parser();
		yyrestart(yyin);
		fclose(yyin);
		fclose(yyout1);
		fclose(yyout2);
	}

	else {
		printf("The file 'test2' is not found.\n");
	}
	
}




