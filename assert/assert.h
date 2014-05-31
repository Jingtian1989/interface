#undef assert
#ifdef NDEBUG
#define assert(e) ((void)(0))
#include "except.h"
extern void assert(int e);
#define assert(e) ((void)((e)||(RAISE(assert_failed), 0)))
#endif