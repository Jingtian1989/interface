#include <stddef.h>
#include "assert.h"
#include "mem.h"
#include "stack.h"

#define T stack_t

struct T 
{
	int count;
	struct elem 
	{
		void *x;
		struct elem *link;
	} *head;
};

T stack_new(void)
{
	T stk;
	NEW(stk);
	stk->count = 0;
	stk->head  = NULL;
	return stk; 
}


int stack_empty(T s)
{
	assert(s);
	return s->count == 0;
}


void stack_push(T s, void *x)
{
	struct elem *t;
	
	assert(s);
	NEW(t);
	t->x = x;
	t->link = s->head;
	s->head = t;
	s->count++;
}


void stack_pop(T s)
{
	void *x;
	struct elem *t;

	assert(s);
	assert(s->count > 0);
	t = s->head;
	s->head = t->link;
	s->count--;
	x = t->x;
	FREE(t);
	return x;
}

void stack_free(T *s)
{
	struct elem *t, *u;

	assert(s);
	for (t = (*s)->head; t; t = u)
	{
		u = t->link;
		FREE(t);
	}
	FREE(*s);
}