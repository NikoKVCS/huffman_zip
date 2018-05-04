#include "stdio.h"
#include<io.h> 
#include <windows.h>
#include "WeightTable.h"
#include "base.h"

WeightTable table;

/*

实验总结 
在创建哈夫曼树的阶段
minest 和 second_minest 一定要设置为最大值
并且要选用极大的 int 不然 权值的累计将不够用

*/

typedef struct tree_node_s {
	int value;
	struct tree_node_s  * rchild;
	struct tree_node_s  * lchild;
	struct tree_node_s  * parent;

	INT weight;
}tree_node_t;

long getFileLength(const char *filename) {
	int handle;
	handle = _open(filename, 0x0100);
	long length = _filelength(handle);
	_close(handle);
	return length;
}

void read_file(const char *filename, BYTE** data, long *size) {

	*size = getFileLength(filename);
	*data = (BYTE *)malloc(*size);

	FILE * file = fopen(filename, "rb");
	if (file != NULL) {
		
		fread(*data, 1, *size, file);
		fclose(file);
		return;
	}

	*size = 0;
	*data = NULL;
	return;
}

tree_node_t * createHuffmanTree() {

	tree_node_t *tree_group[256];
	INT *_data = table.getdata();

	for (int i = 0; i < 256; i++) {
		tree_group[i] = 
		tree_group[i].parent = NULL;
		tree_group[i].lchild = NULL;
		tree_group[i].rchild = NULL;
		tree_group[i].value = i;
		tree_group[i].weight = _data[i];
	}

	tree_node_t * head;

	while (true) {

		INT minest;
		minest = -((1 << (sizeof(INT) * 8 - 1)) + 1);
		int position_of_minest = 0;

		INT second_minest;
		second_minest = -((1 << (sizeof(INT) * 8 - 1)) + 1);
		int position_of_second_minest = 0;

		int j = sizeof(INT);

		int tree_total = 0;
		int final_position = -1;

		for (int i = 0; i < 256; i++) {
			tree_node_t * tree = &tree_group[i];
			if (tree->weight <= 0)
				continue;

			tree_total++;
			final_position = i;

			if (tree->weight < minest) {
				minest = tree->weight;
				position_of_minest = i;
			}
			else if (tree->weight < second_minest) {
				second_minest = tree->weight;
				position_of_second_minest = i;
			}
		}

		if (1 == tree_total) {
			head = (tree_node_t*)malloc(sizeof(tree_node_t));
			memcpy(head, &tree_group[final_position], sizeof(tree_node_t));
			break;
		}

		tree_node_t * new_tree = (tree_node_t*)malloc(sizeof(tree_node_t));

		tree_node_t * _lchild = (tree_node_t*)malloc(sizeof(tree_node_t));
		memcpy(_lchild, &tree_group[position_of_minest], sizeof(tree_node_t));
		tree_group[position_of_minest].weight = 0;
		new_tree->lchild = _lchild;

		tree_node_t * _rchild = (tree_node_t*)malloc(sizeof(tree_node_t));
		memcpy(_rchild, &tree_group[position_of_second_minest], sizeof(tree_node_t));
		tree_group[position_of_second_minest].weight = 0;
		new_tree->rchild = _rchild;

		new_tree->value = -1;
		new_tree->weight = minest + second_minest;

		memcpy(&tree_group[position_of_minest], new_tree, sizeof(tree_node_t));
		free(new_tree);
	}
	
	return head;
}



int main() {
	BYTE * data;
	long size;
	read_file("test.jpg", &data, &size);

	for (int i = 0; i < size; i++) {
		table.addWeight(data[i]);
	}

	createHuffmanTree();

	return 0;
}