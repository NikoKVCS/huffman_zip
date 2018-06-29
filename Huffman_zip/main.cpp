#include "stdio.h"
#include<io.h> 
#include <windows.h>
#include "WeightTable.h"
#include "base.h"
#include "huffman_tree.h"
#include "stack.h"
#include "encode.h"

WeightTable table;

/*

ʵ���ܽ� 
�ڴ������������Ľ׶�
minest �� second_minest һ��Ҫ����Ϊ���ֵ
����Ҫѡ�ü���� int ��Ȼ Ȩֵ���ۼƽ�������

*/


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

void write_file(const char *filename, BYTE* data, long size) {

	FILE * file = fopen(filename, "wb");
	if (file != NULL) {

		fwrite(data, 1, size, file);
		fclose(file);
		return;
	}

	return;
}

typedef struct file_header_s {
	long bit_total = 0;
	long size = 0;
}file_header_t;

void write_compressed_file(const char *filename, LINT *weight, BYTE *data, file_header_t header) {
	FILE *file = fopen(filename, "wb");
	if (file != NULL) {

		fwrite(&header, sizeof(header), 1, file);
		fwrite(weight, sizeof(LINT), 256, file);
		fwrite(data, 1, header.size, file);

		fclose(file);
	}
}

BYTE * read_compressed_file(const char *filename, long *size) {
	FILE *file = fopen(filename, "rb");
	if (NULL != file) {
		file_header_t * header = (file_header_t*)malloc(sizeof(file_header_t));
		fread(header, sizeof(header), 1, file);

		LINT weight[256];
		fread(weight, sizeof(LINT), 256, file);

		BYTE * file_date = (BYTE*)malloc(header->size);
		fread(file_date, sizeof(BYTE), header->size, file);

		WeightTable table;
		table.setdata(weight);
		huffman huff;
		huff.createHuffmanTree(&table);
		Encode decoder;

		return decoder.decode(&huff, file_date, header->size, header->bit_total, size);
	}

	return NULL;
}

int main() {
			
	printf("����1ѹ��,����2��ѹ\n");
	int input = 0;
	scanf("%d", &input);

	switch (input) {
	case 1: {

		printf("���뱻ѹ�����ļ���ַ:\n");
		char szPath[128];
		scanf("%s", szPath);

		BYTE * data;
		long size;
		read_file(szPath, &data, &size);

		for (int i = 0; i < size; i++) {
			table.addWeight(data[i]);
		}

		huffman huff;
		Encode encoder;

		long dst_size = 0;
		tree_node_t *p = huff.createHuffmanTree(&table);
		BYTE * encoded_data = encoder.encode(&huff, data, size, &dst_size);

		free(data);

		file_header_t header;
		header.bit_total = dst_size;
		header.size = dst_size / 8 + (dst_size % 8 == 0 ? 0 : 1);

		printf("����ѹ������ļ���ַ:");
		char szPath2[128];
		scanf("%s", szPath2);

		write_compressed_file(szPath2, table.getdata(), encoded_data, header);

		free(encoded_data);

		printf("�Ѿ����");


		break; 
	}
	case 2: {
		printf("�������ѹ���ļ���ַ:\n");
		char szPath3[128];
		scanf("%s", szPath3);

		long size2 = 0;
		BYTE *decompressed_data = read_compressed_file(szPath3, &size2);

		printf("�����ѹ����ļ���ַ:");
		char szPath4[128];
		scanf("%s", szPath4);

		write_file(szPath4, decompressed_data, size2);


		printf("�Ѿ����");


		break;
	}

		
	}

	return 0;
}