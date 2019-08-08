#include "Token.h"
#include "SymbolTable.h"
#ifndef HEADER
#define HEADER

 bool match(eTOKENS t);

void Parser();
void Parse_PROGRAM();
element_attr* Parse_VAR_DEFINITIONS();
element_attr* Parse_VAR_DEFINITIONS_TAG();
element_attr* Parse_VAR_DEFINITION();
element_attr* Parse_TYPE();
element_attr* Parse_VARIABLES_LIST(element_attr*);
element_attr* Parse_VARIABLES_LIST_TAG(element_attr*);
element_attr* Parse_VARIABLE(element_attr*);
element_attr* Parse_VARIABLE_TAG();
void Parse_FUNC_DEFINITIONS();
void Parse_FUNC_DEFINITIONS_TAG();
void Parse_FUNC_DEFINITION();
element_attr* Parse_RETURNED_TYPE();
element_attr* Parse_PARAM_DEFINITIONS();
void Parse_STATEMENTS(element_attr*);
void Parse_STATEMENTS_TAG(element_attr*);
void Parse_STATEMENT(element_attr*);
element_attr* Parse_STATEMENT_TAG();
element_attr* Parse_STATEMENT_TAG2();
void Parse_BLOCK(element_attr*);
element_attr* Parse_PARAMETERS_LIST();
element_attr* Parse_EXPRESSION();
element_attr* Parse_EXPRESSION_TAG();

//void error(int errorType, int line, char* lexeme);
//
//
//typedef enum eErrorType {
//	DUPLICATED_DECLARATION_ERROR,
//	DUPLICATED_TYPE_DECLARATION_ERROR,
//	VARIABLE_NOT_DECLARED_ERROR,
//	TYPE_NOT_DECLARED_ERROR,
//	ASSIGNMENT_TO_ARRAY_ERROR,
//	ARRAY_INDEX_ERROR,
//	TYPE_CONSISTENCY_ERROR
//
//}eErrorType;

#endif