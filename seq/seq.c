#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "assert.h"
#include "seq.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"



#define T seq_t

#define SEQI(seq, i) ((void **)seq->array.array[(seq->head+i)%seq->array.length])

struct T
{
	struct array_t array;
	int length;
	int head;
};

T seq_new(int hint)
{
	T seq;

	assert(hint >= 0);
	NEW0(seq);
	if (hint == 0)
		hint = 16;
	array_rep_init(&seq->array, hint, sizeof(void *), ALLOC(hint * sizeof(void *)));
	return seq;
}

T seq_seq(void *x, ...)
{
	va_list ap;
	T seq = seq_new(0);

	va_start(ap, x);
	for(; x; x = va_arg(ap, void *))
		seq_addhi(seq, x);
	va_end(ap);
	return seq;
}


void seq_free(T *seq)
{
	assert(seq && *seq);
	assert((void *)*seq == (void *)&(*seq)->array);
	array_free((array_t *)seq);
}

int seq_length(T seq)
{
	assert(seq);
	return seq->length;
}

void *seq_get(T seq, int i)
{
	void *x;

	assert(seq);
	assert(i >= 0 && i < seq->length);
	x = SEQI(seq, i);
	return x;
}

void *seq_put(T seq, int i, void *x)
{
	void *prev;

	assert(seq);
	assert(i >= 0 && i < seq->length);
	prev = SEQI(seq, i);
	SEQI(seq, i) = x;
	return prev;
}



void *seq_rmhi(T seq)
{
	int i;

	assert(seq);
	assert(seq->length > 0);
	i = --seq->length;
	return SEQI(seq, i);
}

void *seq_rmlo(T seq)
{
	int i = 0;
	void *x;
	
	assert(seq);
	assert(seq->length > 0);
	x = SEQI(seq, i);
	seq->head = (seq->head + 1) % seq->array.length;
	seq->length--;
	return x;
}

static void expand(T seq)
{
	int n = seq->array.length;
	array_resize(&seq->array, 2*n);
	if (seq->head > 0)
	{
		void **old = &((void **)seq->array.array)[seq->head];
		memcpy(old + n, (n - seq->head) * sizeof(void *x));
		seq->head += n;
	}
}


void *seq_addlo(T seq, void *x)
{
	int i = 0;

	assert(seq);
	if (seq->length == seq->array.length)
		expand(seq);
	if (--seq->head < 0)
		seq->head = seq->array.length - 1;
	seq->length++;
	SEQI(seq, i) = x;
	return x;
}

void *seq_addhi(T seq, void *x)
{
	int i;

	assert(seq);
	if (seq->length == seq->array.length)
		expand(seq);
	i = seq->length;
	seq->length++;
	SEQI(seq, i) = x;
	return x;
}


