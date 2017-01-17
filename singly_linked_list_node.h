#ifndef SINGLY_LINKED_LIST_NODE_H
#define SINGLY_LINKED_LIST_NODE_H
#define FALSE 0
#define TRUE 1

typedef struct
{
	long start;
	long end;
	double copynumber;
}Item;  

typedef struct NODE{
		struct NODE * link;  
		Item item;
}Node;

int sll_insert(Node ** rootp,Item new_value);

int count_list(Node const ** rootp);

Node * seek_item(Node const ** rootp,Item item);

Node * sll_reverse(Node const ** rootp);

int sll_remove(Node ** rootp,Item item);

void sll_shown(Node const ** rootp);

void sll_delete(Node ** rootp);

#endif
