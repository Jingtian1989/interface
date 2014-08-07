#ifndef __BTEE_H__
#define __BTEE_H__

#define T btree_t
typedef struct T *T;

extern T btree_create(void);
extern T btree_insert(T tree, int key);

#undef T
#endif 