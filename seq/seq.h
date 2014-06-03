#ifndef __SEQ_H__
#define __SEQ_H__

#define T seq_t
typedef struct T *T;

extern T seq_new(int hint);
extern T seq_seq(void *x, ...);
extern int seq_length(T seq);
extern void seq_free(T *seq);
extern void *seq_get(T seq, int i);
extern void *seq_put(T seq, int i, void *x);
extern void *seq_addlo(T seq, void *x);
extern void *seq_addhi(T seq, void *x);
extern void *seq_rmlo(T seq);
extern void *seq_rmhi(T seq);


#undef T
#endif