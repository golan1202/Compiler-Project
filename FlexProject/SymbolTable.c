#include "SymbolTable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern FILE *yyout2;

SymbolTable * make_table(SymbolTable * current_table) {
	int i;
	SymbolTable * new_table;
	new_table = (SymbolTable *)malloc(sizeof(SymbolTable));
	new_table->father = current_table;
	for (i = 0; i<SIZE; i++) {
		new_table->items[i] = NULL;
	}

	return new_table;
}


SymbolTable * pop_table(SymbolTable * current_table) {
	SymbolTable * temp = current_table;
	current_table = current_table->father;
	free(temp);
	return current_table;
}
//Inserts a symbol to table 
element_attr* insert(SymbolTable * current_table, char * id_name) {

	int bin = 0;
	element_attr *result = NULL;
	element_attr *next = NULL;
	element_attr *last = NULL;

	//case: id already exists
	if (lookup(current_table, id_name) != NULL) return NULL;
	bin = hash(id_name);
	next = current_table->items[bin];

	while (next != NULL && next->id_name != NULL && strcmp(id_name, next->id_name) > 0)
	{
		last = next;
		next = next->next;
	}

	result = (element_attr*)malloc(sizeof(element_attr));

	if (next == current_table->items[bin]) {
		result->next = next;
		current_table->items[bin] = result;
	}
	else if (next == NULL) {
		last->next = result;
	}
	else {
		result->next = next;
		last->next = result;
	}

	result->id_name = _strdup(id_name);
	return result;

}

//lookUp and see if exist: Returns a pointer to the requested id else Return NULL.
element_attr* lookup(SymbolTable * current_table, char * id_name) {
	int i;
	element_attr *p;
	i = hash(id_name);
	for (p = current_table->items[i]; p != NULL; p = p->next)
	{
		if (strcmp(id_name, p->id_name) == 0)
		{
			return p;
		}
	}
	return NULL;
}
element_attr* find(SymbolTable * current_table, char * id_name, int line_number) {
	SymbolTable* tab = current_table;
	element_attr* id_entry;
	while (tab != NULL)
	{
		id_entry = lookup(tab, id_name);
		if (id_entry != NULL)
			return (id_entry);
		else
			tab = tab->father;
	}
	fprintf(yyout2, "Error: undeclared id %s, at line: %d \n", id_name, line_number);
	return NULL;
}
void set_id_type(element_attr* current_item, IdTypes type) {
	current_item->type = type;
}
IdTypes get_id_type(element_attr* current_item) {
	return current_item->type;
}
void set_id_returned_type(element_attr* current_item, RTypes r_type) {
	current_item->r_type = r_type;
}
RTypes get_id_returned_type(element_attr* current_item) {
	return current_item->r_type;
}
void set_id_size(element_attr* current_item, int size) {
	current_item->size = size;
}
int get_id_size(element_attr* current_item) {
	return current_item->size;
}
void set_id_param_number(element_attr* current_item, int param_number) {
	current_item->param_number = param_number;
}
int get_id_param_number(element_attr* current_item) {
	return current_item->param_number;
}
void set_id_param_list(element_attr* current_item, slist* param_list) {
	current_item->param_list = slist_create();
	slist_add_slist(current_item->param_list, param_list);

}
slist* get_id_param_list(element_attr* current_item)
{
	return current_item->param_list;
}

// Convert string to apropriate cell in array.
unsigned int hash(char *str)
{
	unsigned int un_h = 0;
	for (; *str; str++)
		un_h = MULTIPLY * un_h + *str;
	return un_h % SIZE;
}

static snode * snode_create(var_attr * data)
{
	snode * node = (snode*)malloc(sizeof(snode));
	if (node) {
		node->data = data;
		node->next = NULL;
	}

	return node;
}

slist * slist_create(void)
{
	slist * list = (slist *)malloc(sizeof(slist));
	if (list) {
		list->head = NULL;
		list->tail = NULL;
	}

	return list;
}

void slist_empty(slist * list)
{
	snode * node, *temp;
	node = list->head;
	while (node != NULL) {
		temp = node->next;
		free(node);
		node = temp;
	}
}

void slist_delete(slist * list)
{
	if (list) {
		slist_empty(list);
		free(list);
	}
}

void slist_add_tail(slist * list, var_attr * data)
{
	snode * node = snode_create(data);
	if (!list->head) {
		/* Adding the first node */
		list->head = node;
		list->tail = node;
	}
	else {
		list->tail->next = node;
		list->tail = node;
	}
}

void slist_add_slist(slist * list1, slist * list2)
{
	if (list2 && list2->head)
		slist_add_tail(list1, list2->head->data);
}

int get_int_value(char* str) {
	int num = 0, i, j, len;
	len = strlen(str);
	for (i = 0; i<len; i++)
	{
		num = num * 10 + (str[i] - '0');
	}
	return num;
}

element_attr* init_element_attr() {
	element_attr* elm = (element_attr*)malloc(sizeof(element_attr));
	elm->id_name = NULL;
	elm->next = NULL;
	elm->param_list = NULL;
	elm->param_number = -1;
	elm->r_type = r_VOID;
	elm->type = NULL_type;
	elm->size = -1;
}