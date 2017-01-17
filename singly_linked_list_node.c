#include<stdio.h>
#include<stdlib.h>
#include"singly_linked_list_node.h"

static void sll_all(Node * node,void (* funct)(Node * node));

static void node_delete(Node * node);

static void node_shown(Node * node);

int sll_insert(Node ** rootp,Item new_value)
{
	Node * current,* newnode;
	while((current=*rootp)!=NULL && current->item.start < new_value.start)
		rootp=&current->link;
	newnode=(Node *)malloc(sizeof(Node));
	if(newnode==NULL)
		return FALSE;
	newnode->item=new_value;
	newnode->link=current;
	*rootp=newnode;
	return TRUE;
}

int sll_copy(Node ** source, Node ** dest)
{
	Node * current, * newnode;
	current = *source;
	while(NULL != current)
	{
		newnode = (Node *)malloc(sizeof(Node));
		if(NULL == newnode)
			return FALSE;
		newnode->item = current->item;
		newnode->link = NULL;
		*dest = newnode;
		dest = &newnode->link;
	}
	return TRUE;
}

int count_list(Node const **rootp)
{
	int count=0;
	Node * current=*rootp;
	while(current!=NULL)
	{
		count++;
		current=current->link;
	}
	return count;
}

Node * seek_item(Node const ** rootp,Item item)
{
	Node * current;
	current=*rootp;
	while(current!=NULL&&current->item.start!=item.start&&current->item.end!=item.end)
		current=current->link;
	return current;
}

Node * sll_reverse(Node const ** rootp)
{
	Node * root,*current,*previous,*temp;
	current=*rootp;
	previous=NULL; 
	while(current!=NULL)
	{
		temp=current->link;
		current->link=previous;
		previous=current;
		current=temp;
	}
	root=previous;
	return root;
}

int sll_remove(Node ** rootp,Item item)
{
	Node * current,* temp;
	while((current=*rootp)!=NULL&&current->item.start!=item.start)
		rootp=&current->link;
	if(current==NULL)  
		return FALSE;
	temp=current->link; 
	free(current);
	*rootp=temp;
	return TRUE;
}

void sll_shown(Node const ** rootp)
{
	sll_all(*rootp,node_shown);
}

void sll_delete(Node ** rootp)
{
	sll_all(*rootp,node_delete);
}

static void sll_all(Node * node,void (* funct)(Node * node))
{
	Node * current,* temp;
	current=node;
	if (node != NULL)
	{
		temp = current->link;
		funct(current);
		sll_all(temp, funct);
	}
}

static void node_delete(Node * node)
{
	free(node);
}

static void node_shown(Node * node)
{
	printf("%d %d %f\n",node->item.start,node->item.end,node->item.copynumber);
}
