#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include "assert.h"
#include "except.h"

#define T except_t

except_frame *except_stack = NULL;


static void except_backtrace()
{
	int j, nptrs;

	#define SIZE 100
	void *buffer[SIZE];
	char **strings;
	nptrs = backtrace(buffer, SIZE);
	strings = backtrace_symbols(buffer, nptrs);
	for (j = 0; j < nptrs; j++)
		fprintf(stderr, "%s\n", strings[j]);
	free(strings);
}


static void except_uncaught(const T *e, const char *file, int line)
{
	fprintf(stderr, "Uncaught exception");
	if (e->reason)
		fprintf(stderr, " %s\n", e->reason);
	else
		fprintf(stderr, " at 0x%p", e);
	if (file && line > 0)
		fprintf(stderr, " raised at %s:%d\n", file, line);

	except_backtrace();
	fprintf(stderr, "aborting...\n");
	fflush(stderr);
	
}

void except_raise(const T *e, const char *file, int line)
{
	except_frame *p = except_stack;

	assert(e);
	if (p == NULL)
	{
		except_uncaught(e, file, line);
		abort();
	}
	p->exception = e;
	p->file = file;
	p->line = line;
	except_stack = except_stack->prev;
	longjmp(p->env, except_raised);
}
