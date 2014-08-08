#ifndef __SET2_H__
#define __SET2_H__

#define T set2_t
typedef struct T *T;

extern T set_make(int key);
extern void set_union(T set1, T set2);

#undef T
#endif 