#ifndef __BIT_H__
#define __BIT_H__

#define T bit_t

typedef struct T *T;

extern T bit_new(int length);
extern int bit_length(T set);
extern int bit_count(T set);
extern void bit_free(T *set);
extern int bit_get(T set, int n);
extern int bit_put(T set, int n, int bit);
extern void bit_clear(T set, int lo, int hi);
extern void bit_set(T set, int lo, int hi);
extern void bit_not(T set, int lo, int hi);
extern void bit_lt(T s, T t);
extern void bit_eq(T s, T t);
extern void bit_leq(T s, T t);
extern void bit_map(T set, void (*apply)(int n, int bit, void *cl), void *cl);
extern void bit_uion(T s, T t);
extern void bit_inter(T s, T t);
extern void bit_minus(T s, T t);
extern void bit_diff(T s, T t);

#undef T
#endif