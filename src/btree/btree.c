#include <mem.h>
#include "btree.h"

#define T btree_t

struct T 
{
	int t;
	struct node 
	{
		int is_leaf;
		int count;

		int *keys;
		struct node **childs;

		T tree;
	} *root;
};
static void disk_write(struct node *n){}
static void disk_read(struct node *n){}

static struct node *alloc_node(int t)
{
	int i, max;
	struct node *node;
	
	max = t << 1;
	node = ALLOC(sizeof(*node));
	node->is_leaf = 1;
	node->count = 0;
	node->tree = tree;
	node->node = (struct node **)ALLOC(max * sizeof(struct node *));
	for (i = 0; i < max; i++)
		node->node[i] = NULL;
	node->keys = (int *)ALLOC((max-1) * sizeof(int));
}

T btree_create(int t)
{
	T btree;
	struct node *root;
	NEW0(btree);
	root = alloc_node(t);
	disk_write(root);
	btree->root = root;
	btree->t = t;
	return btree;
}
//y = x->childs[i]
static btree_split_child(struct node *x, int i, struct node *y)
{
	int j, t;
	T tree;
	struct node *z;
	tree = x->tree;
	t = tree->t;
	z = alloc_node(tree->t);
	z->is_leaf = y->is_leaf;
	z->count = tree->t - 1;
	for (j = 0; j < t; j++)
	{
		z->keys[j] = y->keys[t + j];
	}
	if (!y->is_leaf)
	{
		for (j = 0; j < t; j++)
		{
			z->childs[j] = y->childs[t + j];
			y->childs[t + j] = NULL;
		}
	}
	y->count = t - 1;
	for (j = x->count + 1; j > i; j--)
	{
		x->childs[j + 1] = x->childs[j];
	}
	x->childs[i + 1] = z;
	for (j = x->count; j >= i; j--)
	{
		x->keys[j + 1] = x->keys[i];
	}
	x->keys[i] = y->keys[t];
	x->count++;
	disk_write(y);
	disk_write(z);
	disk_write(x);
}

static void btree_insert_nofull(struct node *node, int key)
{
	int i = node->count;
	int t = node->tree->t;
	int max = t << 1;

	if (node->is_leaf)
	{
		while (i >= 0 && key < node->key[i])
			i--;
		node->key[i + 1] = k;
		node->count++;
		disk_write(node);
	}else
	{
		while (i >= 0 && key < node->key[i])
			i--;
		disk_read(node->childs[i]);
		if (node->childs[i]->count == max - 1)
		{
			btree_split_child(node, i, node->childs[i]);
			if (key > node->keys[i])
				i++;
		}
		btree_insert_nofull(node->childs[i]);
	}
}

void btree_insert(T tree, int key)
{
	int t = tree->t;
	int max = t << 1;
	struct node *root, node;
	root = tree->root;
	if (root->count == max - 1)
	{
		node = alloc_node(t);
		node->is_leaf = 0;
		node->count = 0;
		node->childs[0] = root;
		btree_split_child(node, 0, root);
		root = node;
	}
	btree_insert_nofull(root, key);
}


