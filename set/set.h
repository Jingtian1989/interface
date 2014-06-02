#ifndef __SET_H__
#define __SET_H__

#define T set_t
typedef struct T *T;

extern T 	set_new			(int hint, int (*cmp)(const void *x, const void *y), unsigned (*hash)(const void *x));
extern T 	set_free		(T *set);
extern int 	set_length		(T set);
extern int 	set_member 		(T set, void *member);
extern void set_put 		(T set, void *member);
extern void set_remove 		(T set, void *member);
extern void set_map 		(T set, void (*apply)(const void *member, void *cl), void *cl);
extern void **set_toarray	(T set);
extern T 	set_union 		(T s, T t);
extern T 	set_inter 		(T s, T t);
extern T 	set_minus 		(T s, T t);
extern T 	set_diff		(T s, T t);

#undef T
#endif 