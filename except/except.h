#ifndef __EXCEPT_H__
#define __EXCEPT_H__

#include <setjmp.h>

#define T except_t
#define RAISE(e) except_raise(&(e), __FILE__, __LINE__)
#define RERAISE(e) except_raise(except_frame.exception, except_frame.file, except_frame.line)
#define RETURN switch(except_stack = except_stack->prev, 0) default : return

#define TRY do { \
	volatile int except_flag; \ 
 	except_frame except_frame; \
 	except_frame.prev = except_stack; \
 	except_stack = &except_frame; \
 	except_flag = setjmp(except_frame.env); \
 	if (except_flag == except_entered) \
 	{


#define EXCEPT(e) \
		if (except_flag == except_handled) \
			except_stack = except_stack->prev; \
	} else if(except_frame.exception == &(e)) { \
		except_flag = except_handled;


#define ELSE \
		if (except_flag == except_entered) \
			except_stack = except_stack->prev; \
	} else { \
		except_flag = except_handled;


#define FINALLY \
		if (except_flag == except_handled) \
			except_stack = except_stack->prev; \
	} { \
		if (except_flag == except_entered) \
			except_flag = except_finalized;	

#define END_TRY \
		if (except_flag == except_handled) \
			except_stack = except_stack->prev; \
		} if (except_flag == except_raised) RERAISE; \
	} while(0)



typedef struct T 
{
	const char *reason;
} T;

typedef struct except_frame
{
	struct except_frame *prev;
	jmp_buf env;
	const char *file;
	int line;
	const T *exception;
} except_frame;

enum {except_entered = 0, except_raised, except_handled, except_finalized};

extern except_frame *except_stack;
extern const except_t assert_failed;

extern void except_raise(const T *e, const char *file, int line);

extern void except_raise_dbg(const T *e, const char *file, int line);


#undef T
#endif