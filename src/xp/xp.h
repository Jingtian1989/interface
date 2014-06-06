#ifndef __XP_H__
#define __XP_H__

#define T xp_t
typedef unsigned char *T;

extern int xp_add(int n, T z, T x, T y, int carray);
extern int xp_sub(int n, T z, T x, T y, int borrow);
extern int xp_mul(T z, int n, T x, int m, T y);
extern int xp_sum(int n, T z, T x, int y);
extern int xp_diff(int n, T z, T x, int y);
extern int xp_neg(int n, T z, T x, int carray);
extern int xp_product(int n, T z, T x, int y);
extern int xp_quotient(int n, T z, T x, int y);
extern void xp_lshift(int n, T z, int m, T x,int s, int fill);
extern void xp_rshift(int n, T z, int m, T x, int s, int fill);

extern unsigned long xp_fromint(int n, T z, unsigned long u);
extern unsigned long xp_toint(int n, T z);

#endif