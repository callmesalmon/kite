#ifndef KITE_STDLIB_H
#define KITE_STDLIB_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ctx.h"

void stdlib_error_arg_count(const char *from, size_t expected, size_t got)
{
    fprintf(stderr, "error: %s -> expected %zu argument%s, but got %zu\n",
        from,
        expected,
        expected == 1 ? "" : "s", got);
}

/* No more than 256 types i suppose :/ */
void stdlib_error_arg_type(const char *from, char expected, char got)
{
    fprintf(stderr, "error: %s -> expected %s, but got %s\n",
        from, string_type(expected), string_type(got));
}

obj_t *stdlib_decl_tostring(ctx_t *ctx, size_t count, obj_t *args[])
{
    if(count != 1)
    {
        stdlib_error_arg_count("tostring()", 1, count);
        return NULL;
    }
	int sz = string_obj(NULL, 0, args[0]);
	if(sz == 0) return create_str_obj(ctx, "");
    char str[sz + 1];
	string_obj(str, sz + 1, args[0]);
    return create_str_obj(ctx, str);
}

obj_t *stdlib_decl_write(ctx_t *ctx, size_t count, obj_t *args[])
{
    if(count != 1)
    {
        stdlib_error_arg_count("write()", 1, count);
        return NULL;
    }
    if(args[0]->type != ot_str)
    {
        stdlib_error_arg_type("write()", ot_str, args[0]->type);
        return NULL;
    }

    printf("%s", ((obj_str_t*)args[0])->str);
    return NULL;
}

obj_t *stdlib_decl_writeln(ctx_t *ctx, size_t count, obj_t *args[])
{
    if(count != 1)
    {
        stdlib_error_arg_count("writeln()", 1, count);
        return NULL;
    }
    if(args[0]->type != ot_str)
    {
        stdlib_error_arg_type("writeln()", ot_str, args[0]->type);
        return NULL;
    }

    printf("%s\n", ((obj_str_t*)args[0])->str);
    return NULL;
}

void stdlib_context(ctx_t *ctx)
{
    if(!ctx) return;
    add_var(ctx, create_var("write", create_nat_obj(ctx, stdlib_decl_write), false));
    add_var(ctx, create_var("writeln", create_nat_obj(ctx, stdlib_decl_writeln), false));
    add_var(ctx, create_var("tostring", create_nat_obj(ctx, stdlib_decl_tostring), false));
}

#endif /* KITE_STDLIB_H */
