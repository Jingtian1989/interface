#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "except.h"
#include "arena.h"

#define T arena_t

const except_t arena_newfailed = {"arena creation failed"};
const except_t arena_failed = {"arena allocation failed"};


struct T 
{
	T prev;
	char *avail;
	char *limit;
};

union align
{
	int i;
	long l;
	long *lp;
	void *p;
	void (*fp)(void);
	float f;
	double d;
	long double ld;
};

union header
{
	struct T b;
	union align a;
};


T arena_new(void)
{
	T arena = malloc(sizeof(*arena));
	if (arena == NULL)
		RAISE(arena_newfailed);
	arena->prev = NULL;
	arena->avail = arena->limit = NULL;
	return arena;
}


void arena_dispose(T *ap)
{
	assert(ap && *ap);
	arena_free(*ap);
	free(*ap);
	*ap = NULL;
}


void *arena_alloc(T arena, long nbytes, const char *file, int line)
{
	assert(arena);
	assert(nbytes > 0);
	nbytes = ((nbytes + sizeof(union align) - 1) / (sizeof(union align))) * (sizeof(union align));
	while (nbytes > arena->limit - arena->avail)
	{
		T ptr;
		char *limit;
		if ((ptr = freechunks) != NULL)
		{
			freechunks = freechunks->prev;
			nfree--;
			limit = ptr->limit;
		} else
		{
			long m = sizeof(union header) + nbytes + 10 * 1024;
			ptr = malloc(m);
			if (ptr == NULL)
			{
				if (file == NULL)
					RAISE(arena_failed);
				else
					except_raise(&arena_failed, file, line);
			}
			limit = (char *) ptr + m;
		}
		*ptr = *arena;
		arena->avail = (char *)((union header *)ptr + 1);
		arena->limit = limit;
		arena->prev  = ptr;
	}
}

void *arena_calloc(T arena, long count, long nbytes, const char *file, int line)
{
	void *ptr;

	assert(count > 0);
	ptr = arena_alloc(arena, count * nbytes, file, line);
	memset(ptr, '\0', count * nbytes);
	return ptr;
}

void arena_free(T arena)
{
	assert(arena);
	while(arena->prev)
	{
		struct T tmp = *arena->prev;
		if (nfree < THRESHOLD)
		{
			arena->prev->prev = freechunks;
			freechunks = arena->prev;
			nfree++;
			freechunks->limit = arena->limit;
		} else
		{
			free(arena->prev);
		}
		*arena = tmp;
	}
	assert(arena->limit == NULL);
	assert(arena->avail == NULL);
}