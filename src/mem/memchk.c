#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "except.h"
#include "mem.h"

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


#define hash(p, t) (((unsigned long)(p)>>3) & \
	(sizeof(t)/sizeof((t)[0]))-1)

#define NDESCRIPTORS 512

#define NALLOC ((4096 + sizeof(union align) - 1) / \
	(sizeof(union align))) * (sizeof(union align))

#define NELEMS(t) ((sizeof(t))/(sizeof((t)[0])))

const except_t mem_failed = {"allocation failed"};

static struct descriptor 
{
	struct descriptor *free;
	struct descriptor *link;
	const void *ptr;
	long size;
	const char *file;
	int line;
} *htab[2048];


static struct descriptor freelist = {&freelist};

static FILE *logger;


void mem_free(void *ptr, const char *file, int line)
{
	if (ptr)
	{
		struct descriptor *bp;
		if (((unsigned long)ptr)%(sizeof(union align)) != 0 || (bp = find(ptr)) == NULL ||
			bp->free)
		{
			if (logger != NULL)
			{
				fprintf(logger, "** freeing free memory\n");
				fprintf(logger, "mem_free(%p) called from %s:%d\n",ptr, file,line);
				fprintf(logger, "this block is %d bytes long and was allocated from %s:%d\n",bp->size, bp->file, bp->line);
			} else
				except_raise(&assert_failed, file, line);
		} else
		{
			bp->free = freelist.free;
			freelist.free = bp;
		}
	}
}


void *mem_resize(void *ptr, long nbytes, const char *file, int line)
{
	struct descriptor *bp;
	void *newptr;

	assert(ptr);
	assert(nbytes > 0);
	if (((unsigned long)ptr)%(sizeof(union align)) != 0 || (bp = find(ptr)) == NULL ||
		bp->free)
	{
		if (logger != NULL)
		{
			fprintf(logger, "** resizing unallocated memory\n");
			fprintf(logger, "mem_resize(%p, %ld) called from %s:%d\n", ptr, nbytes, file, line);
		} else
			except_raise(&assert_failed, file, line);

	} else {
		newptr = mem_alloc(nbytes, file, line);
		memcpy(newptr, ptr, nbytes < bp->size ? nbytes : bp->size);
		mem_free(ptr);
		return newptr;
	}
		
	
}

void *mem_calloc(long count, long nbytes, const char *file, int line)
{
	void *ptr;

	assert(count > 0);
	assert(nbytes > 0);
	ptr = mem_alloc(count * nbytes, file, line);
	memset(ptr, '\0', count * nbytes);
	return ptr;
}

static struct descriptor *find(const void *ptr)
{
	struct descriptor *bp = htab[hash(ptr, htab)];

	while (bp && bp->ptr != ptr)
		bp = bp->link;
	return bp;
}

static struct descriptor *dalloc(void *ptr, long size, const char *file, int line)
{
	static struct descriptor *avail;
	static int nleft;

	if (nleft <= 0)
	{
		avail = malloc(NDESCRIPTORS * sizeof(*avail));
		if (avail == NULL)
			return NULL;
		nleft = NDESCRIPTORS;	
	}

	avail->ptr = ptr;
	avail->size = size;
	avail->file = file;
	avail->line = line;
	avail->free = avail->link = NULL;
	nleft--;
	return avail++;
}


void *mem_alloc(long nbytes, const char *file, int line)
{
	struct descriptor *bp;
	void *ptr;

	assert(nbytes > 0);
	nbytes = ((nbytes + sizeof(union align) -1)/(sizeof(union align))) * (sizeof(union align));
	for (bp = freelist.free; bp; bp = bp->free)
	{
		if (bp->size > nbytes)
		{	
			bp->size -= nbytes;
			ptr = (char *)bp->ptr + bp->size;
			if ((bp = dalloc(ptr, nbytes, file, line)) != NULL)
			{
				unsigned h = hash(ptr, htab);
				bp->link = htab[h];
				htab[h] = bp;
				return ptr;
			} else
			{
				if (file == NULL)
					RAISE(mem_failed);
				else
					except_raise(&mem_failed, file, line);
			}

		}

		if (bp == &freelist)
		{
			struct descriptor *newptr;
			if ((ptr = malloc(nbytes + NALLOC)) == NULL || (newptr = dalloc(ptr, nbytes + NALLOC, __FILE__, __LINE__)) == NULL)
			{
				if (file == NULL)
					RAISE(mem_failed);
				else
					except_raise(&mem_failed, file, line);
			}
			newptr->free = freelist.free;
			freelist.free = newptr;
		}
	}
	assert(0);
	return NULL;
}


void mem_log(FILE *log)
{
	logger = log;
}

void mem_leak(void (*apply)(void *ptr, long size, const char *file, int line, void *cl), void *cl)
{
	struct descriptor *bp;
	int i;

	for (i = 0; i < NELEMS(htab); i++)
	{
		for (bp = htab[i]; bp; bp->link)
		{
			apply(bp->ptr, bp->size, bp->file, bp->line, cl);
		}
	}
}


static void inuse(void *ptr, long size, const char *file, int line, void *cl)
{
	FILE *logger = (FILE *)cl;

	fprintf(logger, "** memory in use at %p\n", ptr);
	fprintf(logger, "this block is %ld bytes long and was allocated from %s:%d\n", size, file, line);
}
