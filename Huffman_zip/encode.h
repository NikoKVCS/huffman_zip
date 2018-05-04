#pragma once

class Encode {

private:

	BYTE * dest_data = NULL;
	int dest_data_size = 0;

	void write_to_date(long index, int value);


	BYTE * dest_data2 = NULL;
	int dest_data_size2 = 0;

	void write_to_date2(long index, int value);

public:
	BYTE * encode(huffman *huff, BYTE * src, long src_size, long *dst_size);
	BYTE *decode(huffman *huff, BYTE * src, long src_size, long bit_total, long *dst_size);
};
