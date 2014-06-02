#ifndef __TABLE_H__
#define __TABLE_H__

#define T table_t

typedef struct T *T;


extern 	T 		table_new		(int hint, int (*cmp)(const void *x, const void *y), unsigned (*hash)(const void *key));
extern 	int 	table_length	(T table);
extern	void   *table_put		(T table, const void *key, void *value);
extern 	void   *table_get		(T table, const void *key);
extern 	void   *table_remove	(T table, const void *key);
extern 	void   *table_map 		(T table, void (*apply)(const void *key, void **value, void *cl), void *cl);
extern  void  **table_toarray	(T table, void *end);

#undef T

#endif 