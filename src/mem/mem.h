#ifndef __MEM_H__
#define __MEM_H__
#include "except.h"


#define ALLOC(nbytes) \
	mem_alloc((nbytes), __FILE__, __LINE__)

#define CALLOC(count, nbytes) \
	mem_calloc((count), (nbytes), __FILE__, __LINE__)

#define NEW(p) ((p) = ALLOC((long)sizeof(*p)))

#define NEW0(p) ((p) = CALLOC(1, (long)sizeof(*p)))

#define FREE(ptr) ((void)(mem_free(ptr, __FILE__, __LINE__)), ptr = 0)

#define RESIZE(ptr, nbytes) ((ptr) = mem_resize((ptr), \
	(nbytes), __FILE__, __LINE__))

extern const except_t mem_failed;

extern void *mem_alloc(long nbytes, const char *file, int line);
extern void *mem_calloc(long count, long nbytes, const char *file, int line);
extern void *mem_resize(void *ptr, long nbytes, const char *file, int line);
extern void  mem_free(void *ptr, const char *file, int line);
extern void  mem_log(FILE *log);
extern void  mem_leak(void (*apply)(void *ptr, long size, const char *file, int line, void *cl), void *cl)

#endif