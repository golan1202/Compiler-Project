#include "Parser.h"
#include "Token.h"
#include "SymbolTable.h"


SymbolTable *curr_table = NULL;

//typedef struct Var {
//	char* lexeme;
//	int datatype;
//	int size;
//}Var;
//
//Var VAR_DEFINITION;
//Var VAR_DEFINITION_TAG;
//Var TYPE;
//Var EXPRESSION;
//Var EXPRESSION_TAG;
//
//typedef struct Func {
//	char* lexeme;
//	int datatype;
//	int numOfParameters;
//	Var varsArray[];
//}Func;
//
//Func FUNC_DEFINITION;
//Func FUNC_DEFINITION_TAG;
//
//TableEntry* id_entry;

bool match(eTOKENS t)
{
	Token *currentNode;
	currentNode = next_token();

	if (currentNode->kind != t)
	{
	
		fprintf(yyout1, "Expected token : %s at line: %d, Actual token of type %s, lexeme: ' %s '. \n"
			, myenum_str(t), currentNode->lineNumber, myenum_str(currentNode->kind), currentNode->lexeme);

		return false;
	}
	return true;
}

//void error(int errorType, int line, char* lexeme) {
//	fprintf(yyout2, "(Line %d) ", line);
//	switch (errorType) {
//	case DUPLICATED_DECLARATION_ERROR:
//		fprintf(yyout2, "duplicated declaration of variable '%s'\n", lexeme);
//		break;
//	case DUPLICATED_TYPE_DECLARATION_ERROR:
//		fprintf(yyout2, "duplicated declaration of type '%s'\n", lexeme);
//		break;
//	case VARIABLE_NOT_DECLARED_ERROR:
//		fprintf(yyout2, "variable '%s' is not declared\n", lexeme);
//		break;
//	case TYPE_NOT_DECLARED_ERROR:
//		fprintf(yyout2, "type '%s' is not declared\n", lexeme);
//		break;
//	case ASSIGNMENT_TO_ARRAY_ERROR:
//		fprintf(yyout2, "assignment to array is forbidden\n");
//		break;
//	case TYPE_CONSISTENCY_ERROR:
//		fprintf(yyout2, "mismatch between types of the left and the right sides of the assignment\n");
//		break;
//	case ARRAY_INDEX_ERROR:
//		fprintf(yyout2, "index of array must be integer\n");
//	default:
//		break;
//	}
//}




Token *lookNext() {

	Token *currentNode;
	Token *temp;	

	currentNode = next_token();
	temp = currentNode;
	currentNode = back_token();

	return temp;
}


void Parser()
{
	Parse_PROGRAM();
}

void Parse_PROGRAM()
{
	element_attr* program = init_element_attr();
	curr_table = make_table(curr_table);

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_KW_PROGRAM;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_EOF;

	Token *currentNode = reset_head();

	if (!currentNode)
	{
		return;
	}
	switch (currentNode->kind)
	{
	case TOKEN_KW_PROGRAM:
		fprintf(yyout1, "Rule(PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS )\n");
		Parse_VAR_DEFINITIONS();
		match(TOKEN_SEMICOLON);
		Parse_STATEMENTS(program);
		match(TOKEN_KW_END);
		Parse_FUNC_DEFINITIONS();
		curr_table = pop_table(curr_table);
		break;

	default:
		handle_error(followArr, firstArr, 1, 1, currentNode, 0);
		break;
	}
}

element_attr* Parse_VAR_DEFINITIONS() {

	element_attr *varDefS, *varDef, *varDefS2;
	varDefS = init_element_attr();
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	firstArr[0] = TOKEN_KW_REAL;
	firstArr[1] = TOKEN_KW_INTEGER;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	followArr[0] = TOKEN_SEMICOLON;
	followArr[1] = TOKEN_ROUND_BRACKET_RIGHT;

	Token *currentNode = next_token();

	if (!currentNode)
	{
		return;
	}
	fprintf(yyout1, "Rule(VAR_DEFINITIONS ->  VAR_DEFINITION VAR_DEFINITIONS_TAG )\n");

	varDef = Parse_VAR_DEFINITION();
	varDefS2 = Parse_VAR_DEFINITIONS_TAG();
	varDefS->param_number = varDef->param_number + varDefS2->param_number;
	varDefS->param_list = slist_create();
	slist_add_slist(varDefS->param_list, varDef->param_list);
	slist_add_slist(varDefS->param_list, varDefS2->param_list);
	

	switch (currentNode->kind)
	{
	case TOKEN_KW_REAL:
	case TOKEN_KW_INTEGER:

		currentNode = back_token();
		Parse_VAR_DEFINITION();
		Parse_VAR_DEFINITIONS_TAG();
		break;

	default:
		handle_error(followArr, firstArr, 2, 2, currentNode, 0);
		break;
	}
	return varDefS;

}

element_attr* Parse_VAR_DEFINITIONS_TAG(){

	element_attr *varDefS, *varDefS2;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_SEMICOLON;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	followArr[0] = TOKEN_SEMICOLON;
	followArr[1] = TOKEN_ROUND_BRACKET_RIGHT;

	Token *currentNode = next_token();
	varDefS2 = init_element_attr();

	Token *second_tocken;

	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{
	case TOKEN_SEMICOLON:
	{
		second_tocken = lookNext();
		if (second_tocken->kind == TOKEN_KW_INTEGER || second_tocken->kind == TOKEN_KW_REAL) {
			fprintf(yyout1, "Rule(VAR_DEFINITIONS_TAG -> ; VAR_DEFINITIONS )\n");
			varDefS = Parse_VAR_DEFINITIONS();
			varDefS2->param_number = varDefS->param_number;
			varDefS2->param_list = slist_create();
			slist_add_slist(varDefS2->param_list, varDefS->param_list);
		}
		else
		{
			back_token();
			fprintf(yyout1, "Rule(VAR_DEFINITIONS_TAG ->  EPSILON )\n");
			varDefS2->param_number = 0;
			varDefS2->param_list = NULL;
		}

		break;
	}

	case TOKEN_ROUND_BRACKET_RIGHT :
	{
		fprintf(yyout1, "Rule(VAR_DEFINITIONS_TAG ->  EPSILON ')' )\n");
		currentNode = back_token();
		varDefS2->param_number = 0;
		varDefS2->param_list = NULL;
		break;

	}

	default:
		handle_error(followArr, firstArr, 2, 1, currentNode, 1);
		break;
	}
	return varDefS2;

}

element_attr* Parse_VAR_DEFINITION(){

	element_attr *varDef, *varList, *type;
	varDef = init_element_attr();
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	firstArr[0] = TOKEN_KW_INTEGER;
	firstArr[1] = TOKEN_KW_REAL;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	followArr[0] = TOKEN_SEMICOLON;
	followArr[1] = TOKEN_ROUND_BRACKET_RIGHT;

	Token *currentNode = next_token();

	if (!currentNode)
	{
		return;
	}
	fprintf(yyout1, "Rule(VAR_DEFINITION ->  TYPE VARIABLES_LIST )\n");

	switch (currentNode->kind)
	{
	case TOKEN_KW_INTEGER:
	case TOKEN_KW_REAL:
		currentNode = back_token();
		type = Parse_TYPE();
		varList = Parse_VARIABLES_LIST(type);
		varDef->param_number = varList->param_number;
		varDef->param_list = slist_create();
		slist_add_slist(varDef->param_list, varList->param_list);
		break;

	default:
		handle_error(followArr, firstArr, 2, 2, currentNode, 0);
		break;
	}
	return varDef;

}

element_attr* Parse_TYPE(){

	element_attr *type;
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	firstArr[0] = TOKEN_KW_REAL;
	firstArr[1] = TOKEN_KW_INTEGER;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_ID;


	Token *currentNode = next_token();
	type = init_element_attr();


	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_KW_REAL:
	{
		fprintf(yyout1, "Rule(TYPE ->  real)\n");
		type->type = REAL_type;

		break;
	}

	case TOKEN_KW_INTEGER:
	{
		fprintf(yyout1, "Rule(TYPE ->  integer )\n");
		type->type = INTEGER_type;

		break;
	}
	
	default:
		handle_error(followArr, firstArr, 1, 2, currentNode, 0);
		break;
	}
	return type;

}

element_attr* Parse_VARIABLES_LIST(element_attr* type){

	element_attr * varList, *variable, *varList2;
	varList = init_element_attr();
	varList->type = type->type;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_ID;
	
	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	followArr[0] = TOKEN_SEMICOLON;
	followArr[1] = TOKEN_ROUND_BRACKET_RIGHT;

	Token *currentNode = next_token();

	if (!currentNode)
	{
		return;
	}

	fprintf(yyout1, "Rule(VARIABLES_LIST -> VARIABLE VARIABLES_LIST_TAG )\n");

	switch (currentNode->kind)
	{

	case TOKEN_ID:
		currentNode = back_token();
		variable = Parse_VARIABLE(varList);
		varList2 = Parse_VARIABLES_LIST_TAG(varList);
		varList->param_number = variable->param_number + varList2->param_number;
		varList->param_list = slist_create();
		slist_add_slist(varList->param_list, variable->param_list);
		slist_add_slist(varList->param_list, varList2->param_list);
		break;

	
	default:
		handle_error(followArr, firstArr, 2, 1, currentNode, 0);
		break;
	}
	return varList;
}

element_attr* Parse_VARIABLES_LIST_TAG(element_attr* e) {

	element_attr * varList2_r, *variable, *varList2_l;
	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_COMMA;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	followArr[0] = TOKEN_SEMICOLON;
	followArr[1] = TOKEN_ROUND_BRACKET_RIGHT;

	Token *currentNode = next_token();
	varList2_l = init_element_attr();
	varList2_l->type = e->type;

	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_COMMA:
	{
		fprintf(yyout1, "Rule(VARIABLES_LIST_TAG ->  , VARIABLE VARIABLES_LIST_TAG  )\n");

		variable = Parse_VARIABLE(varList2_l);
		varList2_r = Parse_VARIABLES_LIST_TAG(varList2_l);
		varList2_l->param_number = variable->param_number + varList2_r->param_number;
		varList2_l->param_list = slist_create();
		slist_add_slist(varList2_l->param_list, variable->param_list);
		slist_add_slist(varList2_l->param_list, varList2_r->param_list);
		break;

	}

	case TOKEN_SEMICOLON:
	{
		fprintf(yyout1, "Rule(VARIABLES_LIST_TAG ->  EPSILON  ';')\n");
		currentNode = back_token();
		varList2_l->param_number = 0;
		varList2_l->param_list = NULL;
		break;
	}

	case TOKEN_ROUND_BRACKET_RIGHT:
	{
		fprintf(yyout1, "Rule(VARIABLES_LIST_TAG ->  EPSILON ')' )\n");
		currentNode = back_token();
		varList2_l->param_number = 0;
		varList2_l->param_list = NULL;
		break;
	}
	
	default:
		handle_error(followArr, firstArr, 2, 1, currentNode, 1);
		break;
	}
	return varList2_l;
}

element_attr* Parse_VARIABLE(element_attr* e) {

	element_attr* variable, *id_table_entry, *variable2;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_ID;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	followArr[0] = TOKEN_SEMICOLON;
	followArr[1] = TOKEN_ROUND_BRACKET_RIGHT;
	followArr[2] = TOKEN_COMMA;

	Token *currentNode = next_token();
	variable = init_element_attr();
	variable->type = e->type;

	if (!currentNode)
	{
		return;
	}

	fprintf(yyout1, "Rule(VARIABLE ->  id VARIABLE_TAG)\n");

	switch (currentNode->kind)
	{

	case TOKEN_ID:
		if (variable->type != NULL_type) {
			id_table_entry = insert(curr_table, currentNode->lexeme);
			variable2 = Parse_VARIABLE_TAG();
			if (!id_table_entry) {
				fprintf(yyout2, "Error: duplicated definition of identifier: %s at line: %d\n", currentNode->lexeme, currentNode->lineNumber);
			}
			else if (variable2->size >= 0) {
				if (variable->type == INTEGER_type) {
					set_id_type(id_table_entry, INTEGER_ARRAY_type);
					set_id_size(id_table_entry, variable2->size);
				}
				else if (variable->type == REAL_type) {
					set_id_type(id_table_entry, REAL_ARRAY_type);
					set_id_size(id_table_entry, variable2->size);
				}
			}
			else {
				set_id_type(id_table_entry, variable->type);
			}
		}
		else {
			id_table_entry = find(curr_table, currentNode->lexeme, currentNode->lineNumber);
		}
		if (id_table_entry) {
			variable->param_number = 1;
			variable->param_list = slist_create();
			slist_add_tail(variable->param_list, ((var_attr *)id_table_entry));
		}
		else {
			variable->param_number = 0;
			variable->param_list = NULL;
			break;

	default:
		handle_error(followArr, firstArr, 3, 1, currentNode, 0);
		break;
		}
		return variable;
	}
}

element_attr* Parse_VARIABLE_TAG(){

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_SQUARE_BRACKET_LEFT;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 4);
	followArr[0] = TOKEN_SEMICOLON;
	followArr[1] = TOKEN_ROUND_BRACKET_RIGHT;
	followArr[2] = TOKEN_COMMA;
	followArr[3] = TOKEN_ASSIGNMENT;

	Token *currentNode = next_token();
	element_attr* variable2 = init_element_attr();


	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_SQUARE_BRACKET_LEFT:
	{
		fprintf(yyout1, "Rule(VARIABLE_TAG -> [ int_number ])\n");
		match(TOKEN_INT_NUMBER);
		variable2->size = get_int_value(currentNode->lexeme);
		match(TOKEN_SQUARE_BRACKET_RIGHT);
		break;
	}
	case TOKEN_ASSIGNMENT:
	{
		fprintf(yyout1, "Rule(VARIABLE_TAG -> EPSILON '=' )\n");
		currentNode = back_token();
		break;

	}

	case TOKEN_COMMA:
	{
		fprintf(yyout1, "Rule(VARIABLE_TAG -> EPSILON ',' )\n");
		currentNode = back_token();
		break;

	}

	case TOKEN_SEMICOLON:
	{
		fprintf(yyout1, "Rule(VARIABLE_TAG -> EPSILON ';' )\n");
		currentNode = back_token();
		break;

	}

	case TOKEN_ROUND_BRACKET_RIGHT:
	{
		fprintf(yyout1, "Rule(VARIABLE_TAG -> EPSILON  ')' )\n");
		currentNode = back_token();
		break;

	}

	default:
		handle_error(followArr, firstArr, 4, 1, currentNode, 1);
		break;
	}
	return variable2;
}

void Parse_FUNC_DEFINITIONS(){

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	firstArr[0] = TOKEN_KW_VOID;
	firstArr[1] = TOKEN_KW_REAL;
	firstArr[2] = TOKEN_KW_INTEGER;


	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_EOF;
	

	Token *currentNode = next_token();

	if (!currentNode)
	{
		return;
	}
	fprintf(yyout1, "Rule(FUNC_DEFINITIONS ->  FUNC_DEFINITION FUNC_DEFINITIONS_TAG)\n");

	switch (currentNode->kind)
	{

	case TOKEN_KW_VOID:
	case TOKEN_KW_INTEGER:
	case TOKEN_KW_REAL:
	{
		currentNode = back_token();
		Parse_FUNC_DEFINITION();
		Parse_FUNC_DEFINITIONS_TAG();
		break;
	}

	default:
		handle_error(followArr, firstArr, 1, 3, currentNode, 0);
		break;
	}
}

void Parse_FUNC_DEFINITIONS_TAG(){

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	firstArr[0] = TOKEN_KW_VOID;
	firstArr[1] = TOKEN_KW_REAL;
	firstArr[2] = TOKEN_KW_INTEGER;


	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_EOF;

	Token *currentNode = next_token();

	if (!currentNode)
	{
		return;
	}



	switch (currentNode->kind)
	{

	case TOKEN_KW_VOID:
	case TOKEN_KW_INTEGER:
	case TOKEN_KW_REAL:
		fprintf(yyout1, "Rule( FUNC_DEFINITIONS_TAG -> FUNC_DEFINITION FUNC_DEFINITIONS_TAG )\n");
		currentNode = back_token();
		Parse_FUNC_DEFINITION();
		Parse_FUNC_DEFINITIONS_TAG();
		break;

	case TOKEN_EOF:
		fprintf(yyout1, "Rule( FUNC_DEFINITIONS_TAG -> EPSILON ) \n");
		currentNode = back_token();
		break;

	default:
		handle_error(followArr, firstArr, 1, 3, currentNode, 1);
		break;
	}
}

void Parse_FUNC_DEFINITION(){
	element_attr *returnedType, *paramDef, *id_table_enrty;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	firstArr[0] = TOKEN_KW_VOID;
	firstArr[1] = TOKEN_KW_REAL;
	firstArr[2] = TOKEN_KW_INTEGER;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 4);
	followArr[0] = TOKEN_KW_VOID;
	followArr[1] = TOKEN_KW_REAL;
	followArr[2] = TOKEN_KW_INTEGER;
	followArr[3] = TOKEN_EOF;

	Token *currentNode = next_token();

	if (!currentNode)
	{
		return;
	}

	fprintf(yyout1, "Rule( FUNC_DEFINITION ->  RETURNED_TYPE id ( PARAM_DEFINITIONS ) BLOCK  )\n");

	switch (currentNode->kind)
	{

	case TOKEN_KW_VOID:
	case TOKEN_KW_INTEGER:
	case TOKEN_KW_REAL:

		currentNode = back_token();
		returnedType = Parse_RETURNED_TYPE();
		match(TOKEN_ID);
		id_table_enrty = insert(curr_table, currentNode->lexeme);
		match(TOKEN_ROUND_BRACKET_LEFT);
		paramDef = Parse_PARAM_DEFINITIONS();
		if (!id_table_enrty) {
			fprintf(yyout2, "Error: Duplicated id: %s, at line: %d.\n", currentNode->lexeme, currentNode->lineNumber);
		}
		else {
			set_id_type(id_table_enrty, FUNCTION);
			set_id_returned_type(id_table_enrty, returnedType->r_type);
			set_id_param_number(id_table_enrty, paramDef->param_number);
			set_id_param_list(id_table_enrty, paramDef->param_list);
		}
		match(TOKEN_ROUND_BRACKET_RIGHT);
		Parse_BLOCK(returnedType);
		break;

	default:
		handle_error(followArr, firstArr, 4, 3, currentNode, 0);
		break;
	}
}

element_attr* Parse_RETURNED_TYPE(){

	element_attr* returnedType, *type;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	firstArr[0] = TOKEN_KW_REAL;
	firstArr[1] = TOKEN_KW_INTEGER;
	firstArr[2] = TOKEN_KW_VOID;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_ID;

	Token *currentNode = next_token();
	returnedType = init_element_attr();


	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_KW_INTEGER:

	{
		fprintf(yyout1, "Rule(RETURNED_TYPE ->  TYPE)\n");
		currentNode = back_token();
		type = Parse_TYPE();
		returnedType->r_type = ((RTypes)(type->type));
		break;
	}
	case TOKEN_KW_REAL:
	{
		fprintf(yyout1, "Rule(RETURNED_TYPE ->  TYPE)\n");
		currentNode = back_token();
		type = Parse_TYPE();
		returnedType->r_type = ((RTypes)(type->type));
		break;
	}
	case TOKEN_KW_VOID:
	{
		fprintf(yyout1, "Rule(RETURNED_TYPE ->  void )\n");
		returnedType->r_type = r_VOID;

		break;
	}
	
	default:
		handle_error(followArr, firstArr, 1, 3, currentNode, 0);
		break;
	}
	return returnedType;
}

element_attr* Parse_PARAM_DEFINITIONS(){
	element_attr* paramDef, *varDefS;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	firstArr[0] = TOKEN_KW_REAL;
	firstArr[1] = TOKEN_KW_INTEGER;
	
	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_ROUND_BRACKET_RIGHT;
	
	Token *currentNode = next_token();
	paramDef = init_element_attr();


	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_KW_INTEGER:
	case TOKEN_KW_REAL:
	{
		fprintf(yyout1, "Rule(PARAM_DEFINITIONS -> VAR_DEFINITIONS)\n");

		currentNode = back_token();
		varDefS = Parse_VAR_DEFINITIONS();
		paramDef->param_number = varDefS->param_number;
		paramDef->param_list = slist_create();
		slist_add_slist(paramDef->param_list, varDefS->param_list);
		break;
	}

	case TOKEN_ROUND_BRACKET_RIGHT:
	{
		fprintf(yyout1, "Rule(PARAM_DEFINITIONS -> EPSILON ')' )\n");
		currentNode = back_token();
		paramDef->param_number = 0;
		paramDef->param_list = NULL;
		break;
	}

	default:
		handle_error(followArr, firstArr, 1, 2, currentNode, 1);
		break;
	}
	return paramDef;

}

void Parse_STATEMENTS(element_attr* e){
	element_attr* statements, *statement, *statements2;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	firstArr[0] = TOKEN_KW_RETURN;
	firstArr[1] = TOKEN_ID;
	firstArr[2] = TOKEN_CURLY_BRACKET_LEFT;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	followArr[0] = TOKEN_CURLY_BRACKET_RIGHT;
	followArr[1] = TOKEN_KW_END;


	Token *currentNode = next_token();
	statements = init_element_attr();
	statements->r_type = e->r_type;

	if (!currentNode)
	{
		return;
	}

	fprintf(yyout1, "Rule(STATEMENTS ->  STATEMENT ; STATEMENTS_TAG  )\n");

	switch (currentNode->kind)
	{

	case TOKEN_KW_RETURN:
	case TOKEN_ID:
	case TOKEN_CURLY_BRACKET_LEFT:
	{
	currentNode = back_token();
	Parse_STATEMENT(statements);
	match(TOKEN_SEMICOLON);
	Parse_STATEMENTS_TAG(statements);
		
		break;
	}
	
	default:
		handle_error(followArr, firstArr, 2, 3, currentNode, 0);
		break;
	}
}

void Parse_STATEMENTS_TAG(element_attr* e){
	element_attr* statements, *statements2;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	firstArr[0] = TOKEN_KW_RETURN;
	firstArr[1] = TOKEN_ID;
	firstArr[2] = TOKEN_CURLY_BRACKET_LEFT;
		
	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 2);
	followArr[0] = TOKEN_CURLY_BRACKET_RIGHT;
	followArr[1] = TOKEN_KW_END;
	
	Token *currentNode = next_token();
	statements2 = init_element_attr();
	statements2->r_type = e->r_type;

	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_KW_RETURN:
	case TOKEN_ID:
	case TOKEN_CURLY_BRACKET_LEFT:
	{

		fprintf(yyout1, "Rule(STATEMENTS_TAG ->  STATEMENTS )\n");
		currentNode = back_token();
		Parse_STATEMENTS(statements2);
		break;
	}
	case TOKEN_CURLY_BRACKET_RIGHT:
	{

		fprintf(yyout1, "Rule(STATEMENTS_TAG ->  EPSILON  '}')\n");
		currentNode = back_token();
		break;
	}

	case TOKEN_KW_END:
	{
		fprintf(yyout1, "Rule(STATEMENTS_TAG ->  EPSILON 'end')\n");
		currentNode = back_token();
		break;
	}

	default:
		handle_error(followArr, firstArr, 2, 3, currentNode, 1);
		break;
	}
}

void Parse_STATEMENT(element_attr* e){
		
	element_attr* statement, *statement2, *statement3, *id_table_entry;
	int line;
	IdTypes id_type;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	firstArr[0] = TOKEN_KW_RETURN;
	firstArr[1] = TOKEN_ID;
	firstArr[2] = TOKEN_CURLY_BRACKET_LEFT;
	
	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;
		
	Token *currentNode = next_token();
	statement = init_element_attr();
	statement->r_type = e->r_type;

	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_KW_RETURN:
	{
		fprintf(yyout1, "Rule(STATEMENT ->  return STATEMENT_TAG  )\n");
		statement2 = Parse_STATEMENT_TAG();
		if (statement->r_type != statement2->type)
			fprintf(yyout2, "Error: wrong return type at line:%d.\n", currentNode->lineNumber);
		break;
	}
	case TOKEN_ID:
	{
		fprintf(yyout1, "Rule(STATEMENT ->  id STATEMENT_TAG2 )\n");
		line = currentNode->lineNumber;
		id_table_entry = find(curr_table, currentNode->lexeme, line);
		statement3 = Parse_STATEMENT_TAG2();
		if (id_table_entry) {
			id_type = get_id_type(id_table_entry);
			if (statement3->type == NULL_type) {
				if (id_type != FUNCTION && statement3->param_number > -1) {
					fprintf(yyout2, "Error: id: %s, at line: %d is not a function.\n", id_table_entry->id_name, line);
				}
			}
			else if (statement3->type != id_type && id_type != REAL_type) {
				fprintf(yyout2, "Error: wrong types in assignment, at line:%d.\n", currentNode->lineNumber);
			}
		}
		break;
	}
	case TOKEN_CURLY_BRACKET_LEFT:
	{
				fprintf(yyout1, "Rule(STATEMENT ->  BLOCK )\n");
		currentNode = back_token();
		Parse_BLOCK(statement);
		break;
	}

	default:
		handle_error(followArr, firstArr, 1, 3, currentNode, 0);
		break;
	}
}

element_attr* Parse_STATEMENT_TAG(){
	element_attr* statement2, *expression;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	firstArr[0] = TOKEN_INT_NUMBER;
	firstArr[1] = TOKEN_REAL_NUMBER;
	firstArr[2] = TOKEN_ID;
		
	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *currentNode = next_token();
	statement2 = init_element_attr();


	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_INT_NUMBER:
	case TOKEN_REAL_NUMBER:
	case TOKEN_ID:
	{
		fprintf(yyout1, "Rule(STATEMENT_TAG ->  EXPRESSION)\n");
		currentNode = back_token();
		expression = Parse_EXPRESSION();
		statement2->type = expression->type;
		break;
	}

	case TOKEN_SEMICOLON:
	{
		fprintf(yyout1, "Rule(STATEMENTS_TAG ->  EPSILON ';' )\n");
		currentNode = back_token();
		statement2->type = VOID_type;

		break;
	}

	default:
		handle_error(followArr, firstArr, 1, 3, currentNode, 1);
		break;
	}
	return statement2;
}

element_attr* Parse_STATEMENT_TAG2(){
	element_attr* statement3, *expression, *paramList, *variable2;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	firstArr[0] = TOKEN_ASSIGNMENT;
	firstArr[1] = TOKEN_ROUND_BRACKET_LEFT;
	firstArr[2] = TOKEN_SQUARE_BRACKET_LEFT;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *currentNode = next_token();
	statement3 = init_element_attr();


	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_ASSIGNMENT:
	case TOKEN_SQUARE_BRACKET_LEFT:
	{
		fprintf(yyout1, "Rule(STATEMENT_TAG2 -> VARIABLE_TAG = EXPRESSION)\n");
		currentNode = back_token();
		variable2= Parse_VARIABLE_TAG();
		match(TOKEN_ASSIGNMENT);
		expression = Parse_EXPRESSION();
		statement3->type = expression->type;
		statement3->size = variable2->size;
		statement3->param_number = -1;
		statement3->param_list = NULL;
		break;
	}

	case TOKEN_ROUND_BRACKET_LEFT:
	{
		fprintf(yyout1, "Rule(STATEMENT_TAG2 -> | ( PARAMETERS_LIST ) )\n");
		paramList= Parse_PARAMETERS_LIST();
		statement3->type = paramList->type;
		statement3->size = -1;
		statement3->param_number = paramList->param_number;
		statement3->param_list = slist_create();
		slist_add_slist(statement3->param_list, paramList->param_list);
		match(TOKEN_ROUND_BRACKET_RIGHT);
		break;
	}

	default:
		handle_error(followArr, firstArr, 1, 3, currentNode, 0);
		break;
	}
	return statement3;
}

void Parse_BLOCK(element_attr* e){
	element_attr* b;
	b = init_element_attr();
	b->r_type = e->r_type;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_CURLY_BRACKET_LEFT;
		
	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 5);
	followArr[0] = TOKEN_SEMICOLON;
	followArr[1] = TOKEN_KW_VOID;
	followArr[2] = TOKEN_KW_INTEGER;
	followArr[3] = TOKEN_KW_REAL;
	followArr[4] = TOKEN_EOF;

	
	Token *currentNode = next_token();

	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_CURLY_BRACKET_LEFT:
	{
		
		fprintf(yyout1, "Rule(BLOCK ->  { VAR_DEFINITIONS ; STATEMENTS })\n");
		curr_table = make_table(curr_table);
		Parse_VAR_DEFINITIONS();
		match(TOKEN_SEMICOLON);
		Parse_STATEMENTS(b);
		curr_table = pop_table(curr_table);
		match(TOKEN_CURLY_BRACKET_RIGHT);


		break;
	}
	default:
		handle_error(followArr, firstArr, 5, 1, currentNode, 0);
		break;
	}
}

element_attr* Parse_PARAMETERS_LIST(){
	element_attr* paramList, *varList;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	firstArr[0] = TOKEN_ID;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_ROUND_BRACKET_RIGHT;
	
	Token *currentNode = next_token();
	paramList = init_element_attr();


	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_ID:
	{
		fprintf(yyout1, "Rule(PARAMETERS_LIST -> VARIABLES_LIST)\n");
		currentNode = back_token();
		varList = Parse_VARIABLES_LIST(paramList);
		paramList->param_number = varList->param_number;
		paramList->param_list = slist_create();
		slist_add_slist(paramList->param_list, varList->param_list);
		break;
	}
	case TOKEN_ROUND_BRACKET_RIGHT:
	{
		fprintf(yyout1, "Rule(PARAMETERS_LIST -> EPSILON ')' )\n");
		currentNode = back_token();
		paramList->type = VOID_type;
		paramList->param_number = 0;
		paramList->param_list = NULL;
		break;
	}

	default:
		handle_error(followArr, firstArr, 1, 1, currentNode, 1);
		break;
	}
	return paramList;
}

element_attr* Parse_EXPRESSION(){
	element_attr* expression, *expression2, *id_table_entry;
	int id_size, line;
	IdTypes	id_type;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 3);
	firstArr[0] = TOKEN_ID;
	firstArr[1] = TOKEN_INT_NUMBER;
	firstArr[2] = TOKEN_REAL_NUMBER;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *currentNode = next_token();
	expression = init_element_attr();


	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_ID:
	{
		fprintf(yyout1, "Rule(EXPRESSION ->  id EXPRESSION_TAG )\n");
		line = currentNode->lineNumber;
		id_table_entry = find(curr_table, currentNode->lexeme, line);
		expression2 = Parse_EXPRESSION_TAG();
		if (id_table_entry)
		{
			id_type = get_id_type(id_table_entry);
			id_size = get_id_size(id_table_entry);
			if (expression2->size == 0) {
				if (id_type == INTEGER_type && expression2->type == INTEGER_type) {
					expression->type = INTEGER_type;
				}
				else if (id_type == REAL_type || expression2->type == REAL_type) {
					expression->type = REAL_type;
				}
				else if (expression2->type == ERROR_type) {
					expression->type = ERROR_type;
				}
			}
			else if (expression2->size > 0) {
				if (expression2->size < id_size) {
					expression->type = id_type;
				}
				else {
					fprintf(yyout2, "array: %s index out of bounds, at line: %d.\n", id_table_entry->id_name, line);
					expression->type = ERROR_type;
				}
			}
		}
		break;
	}
	case TOKEN_INT_NUMBER:
	{
		fprintf(yyout1, "Rule(EXPRESSION ->  int_number )\n");
		expression->type = INTEGER_type;

		break;
	}

	case TOKEN_REAL_NUMBER:
	{
		fprintf(yyout1, "Rule(EXPRESSION -> real_number )\n");
		expression->type = REAL_type;

		break;
	}

	default:
		handle_error(followArr, firstArr, 1, 3, currentNode, 0);
		break;
	}
	return expression;

}

element_attr* Parse_EXPRESSION_TAG(){
	element_attr* expression, *expression2, *variable2;

	eTOKENS *firstArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 4);
	firstArr[0] = TOKEN_MULTIPLICATION;
	firstArr[1] = TOKEN_DIVISION;
	firstArr[2] = TOKEN_SQUARE_BRACKET_LEFT;
	firstArr[3] = TOKEN_ASSIGNMENT;

	eTOKENS *followArr = (eTOKENS *)malloc(sizeof(eTOKENS) * 1);
	followArr[0] = TOKEN_SEMICOLON;

	Token *currentNode = next_token();
	expression2 = init_element_attr();


	if (!currentNode)
	{
		return;
	}

	switch (currentNode->kind)
	{

	case TOKEN_MULTIPLICATION:
	case TOKEN_ASSIGNMENT:
	case TOKEN_DIVISION:
	{
		fprintf(yyout1, "Rule(EXPRESSION_TAG -> ar_op EXPRESSION )\n");
		expression= Parse_EXPRESSION();
		expression2->size = expression->size;

		break;
	}

	case TOKEN_SQUARE_BRACKET_LEFT:
	{
		fprintf(yyout1, "Rule(EXPRESSION_TAG ->  VARIABLE_TAG )\n");
		currentNode = back_token();
		variable2= Parse_VARIABLE_TAG();
		expression2->size = variable2->size;

		break;
	}
	case TOKEN_SEMICOLON:
	{
		fprintf(yyout1, "Rule(EXPRESSION_TAG -> EPSILON ';' )\n");
		currentNode = back_token();
		expression2->size = 0;

		break;
	}

	default:
		handle_error(followArr, firstArr, 1, 4, currentNode, 1);
		break;
	}
	return expression2;

}
