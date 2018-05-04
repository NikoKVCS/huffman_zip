#pragma once
#include "windows.h"
#include "malloc.h"
#include "base.h"

class WeightTable {

private:
	LINT weight[256]; // Ȩֵ�ڵ㼯��

public:

	WeightTable() {
		memset(weight, 0, 256 * sizeof(long));
	}

	void addWeight(BYTE c) {
		int position = (int)c;
		weight[position] ++;
	}

	LINT *getdata() {
		return weight;
	}

	void setdata(LINT *w) {
		for (int i = 0; i < 256; i++) {
			weight[i] = w[i];
		}
	}
};