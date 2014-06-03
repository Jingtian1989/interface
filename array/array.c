#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"
#define T array_t


T array_new(int length, int size)
{
	T array;
	
	NEW(array);
	if (length > 0)
		array_rep_init(array, length, size, CALLOC(length, size));
	else
		array_rep_init(array, length, size, NULL);

	return array;
}

void array_rep_init(T array, int length, int size, void *ary)
{
	assert(array);
	assert((ary && length > 0) || (ary== NULL && length == 0));
	assert(size > 0);
	array->length = length;
	array->size = size;
	array->array = ary;
}

void array_free(T *array)
{
	assert(array && *array);
	FREE((*array)->array);
	FREE(*array);
}


void *array_get(T array, int i)
{
	assert(array);
	assert(i >= 0 && i < array->length);
	return array->array + i * array->size;
}

void *array_put(T array, int i, void *elem)
{
	assert(array);
	assert(i >= 0 && i < array->length);
	assert(elem);
	memcpy(array->array + i * array->size, elem, array->size);
	return elem;
}

int array_length(T array)
{
	assert(array);
	return array->length;
}

int array_size(T array)
{
	assert(array);
	return array->size;
}

void array_resize(T array, int length)
{
	assert(array);
	assert(length >= 0);
	if (length == 0)
		FREE(array->array);
	else if (array->length == 0)
		array->array = ALLOC(length * array->size);
	else
		RESIZE(array->array, length * array->size);
	array->length = length;
}


T array_copy(T array, int length)
{
	T copy;

	assert(array);
	assert(length >= 0);
	copy = array_new(length, array->size);
	if (copy->length >= array->length && array->length > 0)
		memcpy(copy->array, array->array, array->length * array->size);
	else if (array->length > copy->length && copy->length > 0)
		memcpy(copy->array, array->array, copy->length * array->size);
	return copy;
}