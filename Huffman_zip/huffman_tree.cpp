#include "base.h"
#include "stdio.h"
#include "huffman_tree.h"
#include "WeightTable.h"
#include "stack.h"


tree_node_t * huffman::createHuffmanTree(WeightTable *table) {

	tree_node_t *tree_group[256];
	LINT *_data = table->getdata();

	for (int i = 0; i < 256; i++) {
		tree_group[i] = (tree_node_t *)malloc(sizeof(tree_node_t));
		tree_group[i]->parent = NULL;
		tree_group[i]->lchild = NULL;
		tree_group[i]->rchild = NULL;
		tree_group[i]->value = i;
		tree_group[i]->weight = _data[i];

		code_group[i] = tree_group[i];
	}

	tree_node_t * head;

	while (true) {

		LINT minest;
		minest = -((1 << (sizeof(LINT) * 8 - 1)) + 1);
		int position_of_minest = -1;

		LINT second_minest;
		second_minest = -((1 << (sizeof(LINT) * 8 - 1)) + 1);
		int position_of_second_minest = -1;

		int tree_total = 0;
		int final_position = -1;

		for (int i = 0; i < 256; i++) {

			if (NULL == tree_group[i])
				continue;

			tree_node_t * tree = tree_group[i];

			if (tree->weight <= 0)
				continue;

			tree_total++;
			final_position = i;

			if (tree->weight < minest) {
				minest = tree->weight;
				position_of_minest = i;
			}

		}



		for (int i = 0; i < 256; i++) {

			if (NULL == tree_group[i])
				continue;

			if (position_of_minest == i)
				continue;

			tree_node_t * tree = tree_group[i];

			if (tree->weight <= 0)
				continue;


			if (tree->weight < second_minest) {
				second_minest = tree->weight;
				position_of_second_minest = i;
			}

		}


		if (1 == tree_total) {
			head = tree_group[final_position];
			break;
		}

		tree_node_t * new_tree = (tree_node_t*)malloc(sizeof(tree_node_t));
		new_tree->lchild = tree_group[position_of_minest];
		new_tree->rchild = tree_group[position_of_second_minest];
		new_tree->value = -1;
		new_tree->weight = minest + second_minest;
		new_tree->parent = NULL;

		new_tree->lchild->parent = new_tree;
		new_tree->rchild->parent = new_tree;

		tree_group[position_of_minest] = new_tree;
		tree_group[position_of_second_minest] = NULL;
	}

	root = head;
	return head;
}

CStack* huffman::getHuffmanCode(int buff) {

	tree_node_t * tree = code_group[buff];
	struct tree_node_s  *node = tree->parent;
	struct tree_node_s  *child = tree;

	CStack *result = new CStack;

	while (node) {
		if (node->lchild == child) {
			int *path = (int *)malloc(sizeof(int));
			*path = 0;
			result->push(path);
		}
		else if (node->rchild == child) {
			int *path = (int *)malloc(sizeof(int));
			*path = 1;
			result->push(path);
		}
		child = node;
		node = node->parent;
	}

	return result;
}


tree_node_t *huffman::getRoot() {
	return root;
}