#ifndef __ARRAY_REP_H__
#define __ARRAY_REP_H__

#define T array_t
struct T
{
	int length;
	int size;
	char *array;
};

extern void array_rep_init(T array, int length, int size, void *ary);

#undef T
#endif