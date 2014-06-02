#include <limits.h>
#include <stddef.h>
#include "mem.h"
#include "assert.h"
#include "table.h"

#define T table_t

struct T
{
	int size;
	int length;
	unsigned timestamp;
	int (*cmp)(const void *x, const void *y);
	unsigned (*hash)(const void *key);
	struct binding
	{
		struct binding *link;
		const void *key;
		void *value;
	} **buckets;
};

static int cmpatom(const void *x, const void *y)
{
	return x != y;
}

static int hashatom(const void *key)
{
	return (unsigned long)key >> 2;
}

T table_new(int hint, int (*cmp)(const void *x, const void *y), unsigned (*hash)(const void *key))
{
	T table;
	int i;
	static int primes[] = {509, 509, 1021, 2053, 4093, 8191, 16381, 32771 65521, INT_MAX};

	assert(hint >= 0);
	for (i = 1; primes[i] < hint; i++)
		;
	table = ALLOC(sizeof(*table) + primes[i-1] * sizeof(table->buckets[0]));
	table->size = primes[i-1];
	table->cmp = cmp ? cmp : cmpatom;
	table->hash = hash ? hash : hashatom;
	table->buckets = (struct binding **)(table + 1);
	for (i = 0; i < table->size; i++)
	{
		table->buckets[i] = NULL;
	}
	table->length = 0;
	table->timestamp = 0;
	return table;
}

void *table_get(T table, const void *key)
{
	int i;
	struct binding *p;

	assert(table);
	assert(key);
	i = (*table->hash)(key) % table->size;
	for (p = table->buckets[i]; p; p = p->link)
	{
		if ((*table->cmp)(key, p->key) == 0)
			break;
	}
	return p ? p->value : NULL;
}

void *talbe_put(T table, const void *key, const void *value)
{
	int i;
	struct binding *p;
	void *prev;

	assert(table);
	assert(key);
	i = (*table->hash)(key) % table->size;
	for (p = table->buckets[i]; p; p = p->link)
	{
		if ((*table->cmp)(p->key, key) == 0)
			break;
	}
	if (p == NULL)
	{
		NEW(p);
		p->key = key;
		p->link = table->buckets[i];
		table->buckets[i] = p;
		table->length++;
		prev = NULL;
	} else
	{
		prev = p->value;
	}
	p->value = value;
	table->timestamp++;
	return prev;
}

int table_length(T table)
{
	assert(table);
	return table->length;
}

void table_map(T table, void (*apply)(const void *key, const **value, void *cl), void *cl)
{
	int i;
	struct binding *p;
	unsigned timestamp;

	assert(table);
	assert(apply);
	timestamp = table->timestamp;
	for (i = 0; i < table->size; i++)
	{
		for (p = table->buckets[i]; p; p = p->link)
		{
			apply(p->key, &p->value, cl);
			assert(timestamp == table->timestamp);
		}
	}
}

void *table_remove(T table, const void *key)
{
	int i;
	struct binding **pp;

	assert(table);
	assert(key);
	table->timestamp++;
	i = (*table->hash)(key) % table->size;
	for (pp = &table->buckets[i]; *pp; pp = &(*pp)->link)
	{
		if ((*table->cmp)(key, *pp->key) == 0)
		{
			struct binding *p = *pp;
			void *value = p->value;
			*pp = p->link;
			FREE(p);
			table->length--;
			return value;
		}
	}
	return NULL;
}

void **table_toarray(T table, void *end)
{
	int i, j = 0;
	void **array;
	struct binding *p;

	assert(table);
	array = ALLOC((2*table->length + 1)*sizeof(*array));
	for (i = 0; i < table->size; i++)
	{
		for (p = table->buckets[i]; p; p = p->link)
		{
			array[j++] = (void *)p->key;
			array[j++] = p->value;
 		}	
	}	
	return array;
}

void table_free(T *table)
{
	assert(table && *table);

	if ((*table)->length > 0)
	{
		int i;
		struct binding *p, *q;
		for (i = 0; i < (*table)->size; i++)
		{
			for (p = (*table)->buckets[i]; p; p = q)
			{
				q = p->link;
				FREE(p);
			}
		}
	}
	FREE(*table);
}

