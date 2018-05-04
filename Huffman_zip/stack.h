#pragma once

typedef struct stack_node_s{
	void *item;
	stack_node_s *next;
	stack_node_s *prev;

} stack_node_t;

class CStack {
public:
	void push(void *);
	void * pop();
	bool stackEmpty();

private:
	stack_node_t *m_top = NULL;
};