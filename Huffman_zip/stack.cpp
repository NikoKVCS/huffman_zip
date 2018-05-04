#include "stdio.h"
#include "stack.h"

bool CStack:: stackEmpty() {
	if (NULL == m_top)
		return true;
	return false;
}

void CStack::push(void *p) {
	if (NULL == m_top) {
		stack_node_t *node = new stack_node_t;
		node->item = p;
		node->next = NULL;
		node->prev = NULL;

		m_top = node;
	}
	else {

		stack_node_t *node = new stack_node_t;
		node->item = p;
		node->next = NULL;
		node->prev = m_top;

		m_top->next = node;
		m_top = node;
	}

}

void * CStack:: pop() {
	if (m_top == NULL) {
		return NULL;
	}
	stack_node_t *q = m_top;
	void *data = q->item;

	m_top = m_top->prev;

	delete q;
	return data;
}