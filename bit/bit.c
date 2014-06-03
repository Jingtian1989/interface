#include <stdarg.h>
#include <string.h>
#include "assert.h"
#include "bit.h"
#include "mem.h"

#define T bit_t

struct T
{
	int length;
	unsigned char *bytes;
	unsigned long *words;
};

#define BPW (8*sizeof(unsigned long))

#define nwords(len) ((((len) + BPW - 1)&(~(BPW-1)))/BPW)

T bit_new(int length)
{
	T set;

	assert(length >= 0);
	NEW(set);
	if (length > 0)
		set->words = CALLOC(nwords(length), sizeof(unsigned long));
	else
		set->words = NULL;

	set->bytes = (unsigned char *)set->words;
	set->length = length;
	return set;
}

void bit_free(T *set)
{
	assert(set && *set);
	FREE((*set)->words);
	FREE(*set);
}

#define nbytes(len) ((((len) + 8 - 1) & ~(8-1)) / 8)

int bit_length(T set)
{
	int length = 0, n;
	static char count[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};

	assert(set);
	for (n = nbytes(set->length); --n >= 0; )
	{
		unsigned char c = set->bytes[n];
		length += count[c & 0xf] + count[c >> 4];
	}
	return length;
}

int bit_get(T set, int n)
{
	assert(0 <= n && n < set->length);
	return ((set->bytes[n/8] >> (n % 8)) & 1);
}

int bit_put(T set, int n, int bit)
{
	int prev;

	assert(set);
	assert(bit == 0 || bit == 1);
	assert(0 <= n && n < set->length);
	prev = ((set->bytes[n/8] >> (n % 8)) & 1);
	if (bit == 1)
		set->bytes[n/8] |= (1 << (n % 8));
	else
		set->bytes[n/8] &= (~(1<<(n % 8)));
}