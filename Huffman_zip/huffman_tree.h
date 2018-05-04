#pragma once

#include "WeightTable.h"
#include "math.h"
#include "stack.h"

typedef struct tree_node_s {
	int value;
	struct tree_node_s  * rchild;
	struct tree_node_s  * lchild;
	struct tree_node_s  * parent;

	LINT weight;

}tree_node_t;

class huffman {
private:
	tree_node_t * code_group[256];
	tree_node_t * root = NULL;
public:
	tree_node_t * createHuffmanTree(WeightTable *table);
	CStack* getHuffmanCode(int buff);
	tree_node_t *getRoot();
};
