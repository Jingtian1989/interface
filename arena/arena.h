#ifndef __ARENA_H__
#define __ARENA_H__

#include "except.h"

#define T arena_t

typedef struct T *T;

extern const except_t arena_newfailed;
extern const except_t arena_failed;

extern 	T 		 arena_new		(void);
extern	void 	 arena_dispose	(T *ap);

extern	void 	*arena_alloc 	(T arena, long nbytes, const char *file, int line);
extern	void 	*arena_calloc	(T arena, long count, long nbytes, const char *file, int line);
extern 	void 	 arena_free		(T arena);

#undef T

#endif
