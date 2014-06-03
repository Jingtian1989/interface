#ifndef __ARRAY_H__
#define __ARRAY_H__

#define T array_t
typedef struct T *T;

extern T 	array_new		(int length, int size);
extern void array_free		(T *array);
extern int 	array_length	(T array);
extern int 	array_size		(T array);
extern void *array_get		(T array, int i);
extern void *array_put		(T array, int i, void *elem);
extern void array_resize	(T array, int length);
extern T 	array_copy		(T array, int length);

#undef T
#endif