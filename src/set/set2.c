#include "set2.h"
#include "mem.h"

#define T set2_t

struct T
{
	struct node
	{
		struct node *head, *next;
		int key;
	} *head, *tail;
};


T set_make(int key)
{
	T set;
	struct node *node;

	NEW0(set);
	NEW0(node);
	set->head = node;
	set->tail = node;
	node->head = node;
	node->next = next;
	node->key = key;
	return set;
}


T set_union(T set1, T set2)
{
	struct node *ptr;
	ptr = set1->tail;
	ptr->next = set2->head;

	for (ptr = set2->head; ptr; ptr = ptr->next)
	{
		ptr->head = set1->head;
	}
	return set1;
}
