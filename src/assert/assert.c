#include "assert.h"

const except_t assert_failed = {"assert failed"};

void (assert)(int e)
{
	assert(e);
}