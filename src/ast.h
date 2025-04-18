#ifndef KITE_AST_H
#define KITE_AST_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define alloc_node(x) malloc(sizeof(node_##x##_t))

#define dast_printf(...)
#define dast_puts(...)

char *copy_string(const char *other) {
	if(!other) return NULL;
	size_t len = strlen(other) + 1;
	char *s = (char *)malloc(len);
	memcpy(s, other, len);
	return s;
}

typedef enum {
	nt_num, nt_var, nt_str,
	nt_bin, nt_let, nt_seq,
	nt_set, nt_iff, nt_for,
	nt_brk, nt_ret, nt_cll,
	nt_fun, nt_get, nt_imp,
} node_type_t;

typedef struct {
	node_type_t type;
	bool allow_free;
}  node_t;

void free_node(node_t *node);
void free_node_all(node_t *node);
void free_node_imp(node_t *node, bool free_all);

typedef struct {
	node_t node;
	double value;
} node_num_t;

node_num_t *create_num_node(double value) {
	dast_printf("create num node %f\n", value);
	node_num_t *n = (node_num_t*)alloc_node(num);
	n->node.type = nt_num;
	n->node.allow_free = true;
	n->value = value;
	dast_puts("create num node ret");
	return n;
}

typedef struct {
	node_t node;
	char *value;
} node_var_t;

node_var_t *create_var_node(char *value) {
	node_var_t *n = (node_var_t*)alloc_node(var);
	n->node.type = nt_var;
	n->node.allow_free = true;
	n->value = copy_string(value);
	return n;
}

typedef struct {
	node_t node;
	char *value;
} node_imp_t;

node_imp_t *create_imp_node(char *value) {
	node_imp_t *n = (node_imp_t*)alloc_node(var);
	n->node.type = nt_imp;
	n->node.allow_free = true;
	n->value = copy_string(value);
	return n;
}

typedef struct {
	node_t node;
	node_t *lhs, *rhs;
	enum
	{
		bin_op_add,
		bin_op_sub,
		bin_op_mul,
		bin_op_div,
		bin_op_mod,
		bin_op_and,
		bin_op_cor,
		bin_op_ieq,
		bin_op_neq,
		bin_op_grt,
		bin_op_lst,
		bin_op_gte,
		bin_op_lte,
	} value;
} node_bin_t;

node_bin_t *create_bin_node(char op, node_t *lhs, node_t *rhs) {
	node_bin_t *n = (node_bin_t*)alloc_node(bin);
	n->node.type = nt_bin;
	n->node.allow_free = true;
	n->lhs = lhs; n->rhs = rhs;
	n->value = op;
	return n;
}

typedef struct {
	node_t node;
	char *name;
	bool exported;
	node_t *value;
} node_let_t;

node_let_t *create_let_node(char *name, node_t *value, bool exported) {
	node_let_t *n = (node_let_t*)alloc_node(let);
	n->node.type = nt_let;
	n->node.allow_free = true;
	n->value = value;
	n->name = copy_string(name);
	n->exported = exported;
	if(exported && n->value->type == nt_fun) n->value->allow_free = false;
	return n;
}

typedef struct {
	node_t node;
	node_t *cond, *body, *otherwise;
} node_iff_t;

node_iff_t *create_iff_node(node_t *cond, node_t *body, node_t *otherwise) {
	node_iff_t *n = (node_iff_t*)alloc_node(iff);
	n->node.type = nt_iff;
	n->node.allow_free = true;
	n->cond = cond;
	n->body = body;
	n->otherwise = otherwise;
	return n;
}

typedef struct {
	node_t node;
	node_t *cond, *body;
} node_for_t;

node_for_t *create_for_node(node_t *cond, node_t *body) {
	node_for_t *n = (node_for_t*)alloc_node(for);
	n->node.type = nt_for;
	n->node.allow_free = true;
	n->cond = cond;
	n->body = body;
	return n;
}

typedef struct {
	node_t node;
} node_brk_t;

node_brk_t *create_brk_node() {
	node_brk_t *n = (node_brk_t*)alloc_node(brk);
	n->node.type = nt_brk;
	n->node.allow_free = true;
	return n;
}

typedef struct {
	node_t node;
	node_t *value;
} node_ret_t;

node_ret_t *create_ret_node(node_t *value) {
	node_ret_t *n = (node_ret_t*)alloc_node(ret);
	n->node.type = nt_ret;
	n->node.allow_free = true;
	n->value = value;
	return n;
};

typedef struct {
	node_t node;
	node_t *value;
	size_t count;
	char *args[];
} node_fun_t;

node_fun_t *create_fun_node(node_t *value, size_t count, char *args[]) {
	node_fun_t *n = (node_fun_t*)malloc(sizeof(node_fun_t) + count * sizeof(char*));
	n->node.type = nt_fun;
	n->node.allow_free = true;
	n->value = value;
	n->count = count;
	for(size_t i = 0; i < n->count; ++i)
		n->args[i] = copy_string(args[i]);
	return n;
}

typedef struct {
	node_t node;
	node_t *value;
	size_t count;
	node_t *args[];
} node_cll_t;

node_cll_t *create_cll_node(node_t *value, size_t count, node_t *args[]) {
	node_cll_t *n = (node_cll_t*)malloc(sizeof(node_cll_t) + count * sizeof(char*));
	n->node.type = nt_cll;
	n->node.allow_free = true;
	n->value = value;
	n->count = count;
	for(size_t i = 0; i < n->count; ++i)
		n->args[i] = args[i];
	return n;
}

typedef struct {
	node_t node;
	char *name;
	node_t *value;
} node_set_t;

node_set_t *create_set_node(char *name, node_t *value) {
	node_set_t *n = (node_set_t*)alloc_node(set);
	n->node.type = nt_set;
	n->node.allow_free = true;
	n->value = value;
	n->name = copy_string(name);
	return n;
}

typedef struct {
	node_t node;
	node_t *from;
	node_t *value;
} node_get_t;

node_get_t *create_get_node(node_t *from, node_t *value) {
	node_get_t *n = (node_get_t*)alloc_node(get);
	dast_printf("create_get_node: 0x%x\n", n);
	dast_printf("val: '%s'\n", ((node_var_t*)value)->value);
	n->node.type = nt_get;
	n->node.allow_free = true;
	n->from = from;
	n->value = value;
	return n;
}

typedef struct {
	node_t node;
	bool new;
	struct node_seq_cell_t
	{
		node_t *value;
		struct node_seq_cell_t *next;
	} *head, *tail;
} node_seq_t;

node_seq_t *create_seq_node(bool new) {
	node_seq_t *n = alloc_node(seq);
	n->node.type = nt_seq;
	n->node.allow_free = true;
	n->head = NULL;
	n->tail = NULL;
	n->new = new;
	return n;
}

void node_seq_add_node(node_seq_t *seq, node_t *node) {
	struct node_seq_cell_t *cell = malloc(sizeof(struct node_seq_cell_t));
	cell->value = node;
	cell->next = NULL;
	if(seq->tail == NULL)
	{
		seq->head = seq->tail = cell;
	}
	else
	{
		seq->tail->next = cell;
		seq->tail = cell;
	}
}

void node_seq_del_node(node_seq_t *seq, bool free_all) {
	struct node_seq_cell_t *tmp = seq->head;
	seq->head = seq->head->next;
	free_node_imp(tmp->value, free_all);
	free(tmp);
}

void node_seq_del_all(node_seq_t *seq, bool free_all) {
	while(seq->head)
		node_seq_del_node(seq, free_all);
	seq->tail = NULL;
}

void free_node(node_t *node) { free_node_imp(node, false); }
void free_node_all(node_t *node) { free_node_imp(node, true); }

void free_node_imp(node_t *node, bool free_all) {
	dast_puts("FREE NODE");
	if(!node) return;
	if(!free_all && !node->allow_free) return;
	switch(node->type)
	{
		case nt_num:
		case nt_brk: break;

		case nt_ret:
		case nt_var:
		{
			node_var_t *n = (node_var_t*)node;
			free(n->value);
			n->value = "{freed}";
			break;
		}
		case nt_imp:
		{
			node_imp_t *n = (node_imp_t*)node;
			free(n->value);
			break;
		}
		case nt_seq:
		{
			node_seq_t *n = (node_seq_t*)node;
			node_seq_del_all(n, free_all);
			break;
		}
		case nt_iff:
		{
			node_iff_t *n = (node_iff_t*)node;
			free_node_imp(n->cond, free_all); n->cond = NULL;
			free_node_imp(n->body, free_all); n->body = NULL;
			free_node_imp(n->otherwise, free_all); n->otherwise = NULL;
			break;
		}
		case nt_for:
		{
			node_for_t *n = (node_for_t*)node;
			free_node_imp(n->cond, free_all); n->cond = NULL;
			free_node_imp(n->body, free_all); n->body = NULL;
			break;
		}
		case nt_let:
		{
			node_let_t *n = (node_let_t*)node;
			free(n->name);
			free_node_imp(n->value, free_all); n->value = NULL;
			break;
		}
		case nt_fun:
		{
			node_fun_t *n = (node_fun_t*)node;
			free_node_imp(n->value, free_all); n->value = NULL;
			for(size_t i = 0; i < n->count; ++i)
				free(n->args[i]);
			break;
		}
		case nt_cll:
		{
			node_cll_t *n = (node_cll_t*)node;
			free_node_imp(n->value, free_all); n->value = NULL;
			for(size_t i = 0; i < n->count; ++i)
			{
				free_node_imp(n->args[i], free_all);
				n->args[i] = NULL;
			}
			break;
		}
		case nt_set:
		{
			node_set_t *n = (node_set_t*)node;
			free(n->name); n->name = "{freed}";
			free_node_imp(n->value, free_all); n->value = NULL;
			break;
		}
		case nt_get:
		{
			node_get_t *n = (node_get_t*)node;
			free_node_imp(n->from, free_all); n->from = NULL;
			free_node_imp(n->value, free_all); n->value = NULL;
			dast_printf("freeing get node at 0x%x\n", node);
			break;
		}
		case nt_bin:
		{
			node_bin_t *n = (node_bin_t*)node;
			free_node_imp(n->lhs, free_all); n->lhs = NULL;
			free_node_imp(n->rhs, free_all); n->rhs = NULL;
			break;
		}
		default:
			fprintf(stderr, "error: free -> not implemented %d.\n", node->type);
			break;
	}
	free(node);
}

#endif /* KITE_AST_H */
