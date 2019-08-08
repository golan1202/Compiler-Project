#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#define SIZE 10000
#define MULTIPLY 45


typedef enum idTypes{
	REAL_type,INTEGER_type, VOID_type, INTEGER_ARRAY_type, REAL_ARRAY_type,FUNCTION,NULL_type,ERROR_type
}IdTypes;

typedef enum rTypes{
	r_REAL,r_INTEGER,r_VOID
}RTypes;

struct var_attr{
	char *id_name;
	IdTypes type;
	int size;
};
typedef struct var_attr var_attr;

struct snode {
    var_attr * data;
    struct snode * next;
};
typedef struct snode snode;
 
struct slist {
    snode * head;
    snode * tail;
};
typedef struct slist slist;
 
struct element_attr{
	char *id_name;
	int size;
	int param_number;
	IdTypes type;
	RTypes r_type;
	slist * param_list;
	struct element_attr * next;
};
typedef struct element_attr element_attr;

typedef struct symbolTable{
	element_attr * items[SIZE];
	struct symbolTable * father;
}SymbolTable;


SymbolTable * make_table(SymbolTable * current_table);
SymbolTable * pop_table(SymbolTable * current_table);
element_attr* insert(SymbolTable * current_table, char * id_name);
element_attr* lookup(SymbolTable * current_table, char * id_name);
element_attr* find(SymbolTable * current_table, char * id_name, int line_number);
void set_id_type(element_attr* current_item, IdTypes type);
IdTypes get_id_type(element_attr* current_item);
void set_id_returned_type(element_attr* current_item, RTypes r_type);
RTypes get_id_returned_type(element_attr* current_item);
void set_id_size(element_attr* current_item, int size);
int get_id_size(element_attr* current_item);
void set_id_param_number(element_attr* current_item, int param_number);
int get_id_param_number(element_attr* current_item);
void set_id_param_list(element_attr* current_item, slist* param_list);
slist* get_id_param_list(element_attr* current_item);
unsigned int hash(char *str);

slist * slist_create();
void slist_empty(slist * list);
void slist_delete(slist * list);
void slist_add_tail(slist * list, var_attr * data);
void slist_add_slist(slist * list1, slist * list2);
int get_int_value(char* str);
element_attr* init_element_attr();


#endif