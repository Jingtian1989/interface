#ifndef __STACK_H__
#define __STACK_H__

#define T stack_t
typedef struct T *T;

extern T 	 stack_new	(void);
extern int 	 stack_empty(T s);
extern void  stack_push	(T s, void *x);
extern void *stack_pop	(T s);
extern void  stack_free (T *s);

#undef T

#endif