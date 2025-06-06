#ifndef KITE_CTX_H
#define KITE_CTX_H
#include "ast.h"
#include <stdio.h>

#define dctx_printf(...)
#define dctx_puts(...)

char *copy_string(const char *other);
typedef struct ctx_t ctx_t;

enum obj_type_t
{
	ot_num, ot_str, ot_nil,
	ot_fun, ot_obj, ot_arr,
	ot_nat, //   TODO!   //
};

#define obj_header_t enum obj_type_t type;
typedef struct { obj_header_t; } obj_t;
#define obj_typedef(name, ...) \
	typedef struct { obj_header_t; __VA_ARGS__; } obj_##name##_t;

obj_typedef(num, double num;);
obj_typedef(str, char *str;);
obj_typedef(nat, obj_t *(*nat)(ctx_t*, size_t, obj_t**););
obj_typedef(fun, size_t count; char **args; node_t *body; ctx_t *ctx);
typedef struct t_001 { obj_t *l, *r; struct t_001 *n; } obj_obj_pair_t;
obj_obj_pair_t *new_pair(obj_t *l, obj_t *r, obj_obj_pair_t *n)
{
	obj_obj_pair_t *p = malloc(sizeof(obj_obj_pair_t));
	p->l = l;
	p->r = r;
	p->n = n;
	return p;
}
obj_typedef(obj, size_t count; obj_obj_pair_t *first;);

typedef struct
{
	char  *name;
	obj_t *value;
	bool  exported;
} var_t;

struct ctx_t
{
	size_t count, obj_count;
	var_t  *vars;
	obj_t  **pool;
	ctx_t  *parent;
	bool   in_loop;
};

void print_obj(obj_t *o);
void add_obj(ctx_t *ctx, obj_t *o)
{
    ctx->pool = realloc(ctx->pool, ++ctx->obj_count * sizeof(obj_t*));
    ctx->pool[ctx->obj_count - 1] = o;
}



// OBJ //

bool obj_is_true(obj_t *o)
{
    if(!o) return false;
    switch (o->type) {
		case ot_nil: return false;
		case ot_num: return ((obj_num_t*)o)->num;
		case ot_str: return ((obj_str_t*)o)->str && strlen(((obj_str_t*)o)->str);
		default: return false;
	}
}
#define alloc_obj(type) malloc(sizeof(obj_##type##_t))
obj_t *copy_obj(ctx_t *ctx, obj_t *o);

obj_t *create_nil_obj(ctx_t *ctx)
{
	obj_t *o = malloc(sizeof(obj_t));
	o->type = ot_nil;
	add_obj(ctx, (obj_t*)o);
	return (obj_t*)o;
}

obj_t *create_num_obj(ctx_t *ctx, double value)
{
	obj_num_t *o = alloc_obj(num);
	o->type = ot_num;
	o->num = value;
	add_obj(ctx, (obj_t*)o);
	return (obj_t*)o;
}

obj_t *create_str_obj(ctx_t *ctx, const char *value)
{
	obj_str_t *o = alloc_obj(str);
	o->type = ot_str;
	o->str = copy_string(value);
	add_obj(ctx, (obj_t*)o);
	return (obj_t*)o;
}

obj_t *create_fun_obj(ctx_t *ctx, size_t count, char **args, node_t *body, ctx_t *bctx)
{
	obj_fun_t *o = alloc_obj(fun);
	o->type  = ot_fun;
	o->count = count;
	o->body  = body;
	o->ctx   = bctx;
	o->args  = malloc(sizeof(char*) * count);
	for(size_t i = 0; i < count; ++i)
		o->args[i] = copy_string(args[i]);
	add_obj(ctx, (obj_t*)o);
	return (obj_t*)o;
}

obj_t *create_nat_obj(ctx_t *ctx, obj_t *(*nat)(ctx_t*, size_t, obj_t**))
{
	obj_nat_t *o = alloc_obj(nat);
	o->type = ot_nat;
	o->nat = nat;
	add_obj(ctx, (obj_t*)o);
	return (obj_t*)o;
}

obj_t *create_obj_obj(ctx_t *ctx, ctx_t *from)
{
	dctx_puts("create obj obj");
	dctx_printf("ctx: 0x%x, from: 0x%x\n", ctx, from);
	obj_obj_t *o = alloc_obj(obj);
	o->type = ot_obj;
	o->count = from->count;
	dctx_printf("count: %d\n", o->count);

	obj_obj_pair_t **p = &(o->first);
	for(size_t i = 0; i < from->count; ++i)
	{
		if(!from->vars[i].exported) continue;
		*p = new_pair(
			create_str_obj(ctx, from->vars[i].name),
			copy_obj(ctx, from->vars[i].value),
			NULL);
		dctx_printf("'%s' -> i: %d, p: 0x%x\n", from->vars[i].name, i, *p);
		p = &(*p)->n;
	}
	add_obj(ctx, (obj_t*)o);
	return (obj_t*)o;
}

// TODO: Can we just move memory to the new context loc?
obj_t *copy_obj(ctx_t *ctx, obj_t *o)
{
	if(!o) return NULL;
	switch (o->type) {
		case ot_nil: return create_nil_obj(ctx);
		case ot_num: return create_num_obj(ctx, ((obj_num_t*)o)->num);

		case ot_str:
			return create_str_obj(ctx, copy_string(((obj_str_t*)o)->str));
		case ot_nat:
			return create_nat_obj(ctx, ((obj_nat_t*)o)->nat);
		case ot_fun:
			return create_fun_obj(ctx,
				((obj_fun_t*)o)->count,
				((obj_fun_t*)o)->args,
				((obj_fun_t*)o)->body,
				((obj_fun_t*)o)->ctx);
		default:
            fprintf(stderr, "error: eval -> copy obj not implemented\n");
			return NULL;
	}
}

void free_obj(obj_t *o)
{
	if(!o) return;
	switch (o->type) {
		case ot_nil:
		case ot_nat:
		case ot_num: break;
		case ot_str:
			free(((obj_str_t*)o)->str);
			break;
		case ot_fun:
			for(size_t i = 0; i < ((obj_fun_t*)o)->count; ++i)
				free(((obj_fun_t*)o)->args[i]);
			free(((obj_fun_t*)o)->args);
			break;
		case ot_obj:
			break;
		default:
            fprintf(stderr, "error: eval -> free obj %d not implemented\n", o->type);
			return;
	}
	free(o);
}

// value returned is a constant.
char *string_type(char type)
{
    switch(type) {
        case ot_nil: return "nil";
        case ot_fun: return "fun";
        case ot_nat: return "native";
        case ot_num: return "number";
        case ot_str: return "string";
        case ot_arr: return "array";
        case ot_obj: return "map";
        default:
            fprintf(stderr, "error: eval -> string type not implemented\n");
			return NULL;
    }
}

int string_obj(char *buf, size_t bufsz, obj_t *o)
{
	if(!o) return 0;
    switch(o->type) {
        case ot_nil: return snprintf(buf, bufsz, "[nil]"); break;
        case ot_fun: return snprintf(buf, bufsz, "[function}"); break;
        case ot_nat: return snprintf(buf, bufsz, "[native]"); break;
        case ot_num:
			return snprintf(buf, bufsz, "%f", ((obj_num_t*)o)->num); break;
        case ot_str: return snprintf(buf, bufsz, "%s", ((obj_str_t*)o)->str); break;
        default:
            fprintf(stderr, "error: eval -> string obj %d not implemented\n", o->type);
			return snprintf(buf, bufsz, "[ERROR]: INTERNAL (FATAL) ERROR");
			break;
    }
}

void fprint_obj(FILE *f, obj_t *o)
{
	if(!o) return;
	int sz = string_obj(NULL, 0, o);
	if(sz == 0) return;
    char str[sz + 1];
	string_obj(str, sz + 1, o);
	fprintf(f, "%s", str);
}

void print_obj(obj_t *o)
{
	fprint_obj(stdout, o);
}

/* Beware that create_var allocates
   the name and copies from the argument.
   (see free_var) */
var_t create_var(char *name, obj_t *value, bool exp)
{
	var_t v = (var_t){ copy_string(name), value, exp };
	return v;
};

/* Frees the variable name. */
void free_var(var_t *var)
{ free(var->name); };

ctx_t create_context(ctx_t *parent)
{
    return (ctx_t)
    {
        0,
        0,
        malloc(0),
        malloc(0),
        parent,
        false
    };
}

void free_context(ctx_t *ctx)
{
	for(size_t i = 0; i < ctx->count; ++i)
	{
		free_var(&ctx->vars[i]);
	}
    for(size_t i = 0; i < ctx->obj_count; ++i)
	{
		if(ctx->pool[i])
		{
			free_obj(ctx->pool[i]);
			ctx->pool[i] = NULL;
		}
	}
	free(ctx->vars);
	free(ctx->pool);
	ctx->count = 0;
}

ctx_t *context_get_loop(ctx_t *ctx)
{
	if(ctx->in_loop)
		return ctx;
	if(ctx->parent)
		return context_get_loop(ctx->parent);
	return NULL;
}

var_t *get_var(ctx_t *ctx, char *name)
{
	for(size_t i = 0; i < ctx->count; ++i)
		if(strcmp(ctx->vars[i].name, name) == 0)
			return &ctx->vars[i];
	if(ctx->parent)
		return get_var(ctx->parent, name);
	return NULL;
}


void add_var(ctx_t *ctx, var_t var)
{
	for(size_t i = 0; i < ctx->count; ++i)
		if(strcmp(ctx->vars[i].name, var.name) == 0)
		{
			fprintf(stderr, "variable with name \"%s\" already exists.\n", var.name);
			return;
		}
	ctx->vars = realloc(ctx->vars, ++ctx->count * sizeof(var_t));
	ctx->vars[ctx->count - 1] = var;
}



#endif /* KITE_CTX_H */
