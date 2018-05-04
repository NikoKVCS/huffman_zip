#include "stdio.h"
#include<io.h> 
#include <windows.h>
#include "WeightTable.h"
#include "base.h"
#include "huffman_tree.h"
#include "stack.h"
#include "encode.h"


void Encode::write_to_date(long index, int value) {

	if (0 == dest_data_size || NULL == dest_data) {
		dest_data = (BYTE*)malloc(1024);
		dest_data_size = 1024;
	}

	long i = index / 8;
	int j = index % 8;

	if (i >= dest_data_size) {
		dest_data = (BYTE*)realloc(dest_data, dest_data_size + 1024);
		dest_data_size += 1024;
	}

	if (1 == value) {
		dest_data[i] |= (1 << j);
	}
	else {
		dest_data[i] &= ((1 << j) ^ 255);
	}
}


void Encode::write_to_date2(long index, int value) {

	if (0 == dest_data_size2 || NULL == dest_data2) {
		dest_data2 = (BYTE*)malloc(1024);
		dest_data_size2 = 1024;
	}

	if (index >= dest_data_size2) {
		dest_data2 = (BYTE*)realloc(dest_data2, dest_data_size2 + 1024);
		dest_data_size2 += 1024;
	}

	(dest_data2[index]) = value;

}


BYTE * Encode::encode(huffman *huff, BYTE * src, long src_size, long *dst_size) {

	*dst_size = 0;

	if (dest_data != NULL) {
		dest_data_size = 0;
		dest_data = NULL;
	}

	for (int i = 0; i < src_size; i++) {
		CStack* path = huff->getHuffmanCode((int)src[i]);
		while (!path->stackEmpty()){
			int *bin = (int *)path->pop();
			(*dst_size)++;
			write_to_date((*dst_size) - 1, *bin);
			free(bin);

		}
		free(path);
	}

	return dest_data;
}


BYTE * Encode::decode(huffman *huff, BYTE * src, long src_size, long bit_total, long *dst_size) {

	tree_node_t* tree_root = huff->getRoot();
	tree_node_t* node = tree_root;

	*dst_size = 0;

	if (dest_data2 != NULL) {
		dest_data_size2 = 0;
		dest_data2 = NULL;
	}


	long jjj = 0;
	
	for (int i = 0; i < bit_total; i++) {

		int q = i / 8;
		int w = i % 8;

		int n = (src[q] & (1 << w)) > 0;

		if (NULL == node) {
			node = tree_root;
			continue;
		}
		
		if (1 == n) {
			node = node->rchild;
			if (NULL == node) {
				node = tree_root;
				continue;
			}
			else if (node->value >= 0) {
				write_to_date2(jjj, node->value);
				(*dst_size)++;
				jjj++;
				node = tree_root;
			}
			else {

			}
		}
		else {
			node = node->lchild;
			if (NULL == node) {
				node = tree_root;
				continue;
			}
			else if (node->value >= 0) {
				write_to_date2(jjj, node->value);
				(*dst_size)++;
				jjj++;
				node = tree_root;
			}
			else {

			}
		}
	}
	return dest_data2;
}