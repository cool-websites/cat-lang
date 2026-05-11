#include "../include/cat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Env* env = NULL;

static void env_set(const char* name, double num, int is_bool, int val_bool) {
    Env* e = malloc(sizeof(Env));
    e->name = strdup(name);
    e->num = num;
    e->is_bool = is_bool;
    e->val_bool = val_bool;
    e->next = env;
    env = e;
}

static Env* env_get(const char* name) {
    Env* e = env;
    while (e) {
        if (strcmp(e->name, name) == 0)
            return e;
        e = e->next;
    }
    return NULL;
}

static double eval(Lexer* l, int* pos);

static double eval_primary(Lexer* l, int* pos) {
    if (*pos >= l->ntokens)
        return 0;

    Token* t = &l->tokens[*pos];

    if (t->type == TK_INT) {
        *pos += 1;
        return (double)t->value.ival;
    }
    if (t->type == TK_FLOAT) {
        *pos += 1;
        return t->value.fval;
    }
    if (t->type == TK_BOOL) {
        *pos += 1;
        return (double)t->value.bval;
    }
    if (t->type == TK_ID) {
        Env* e = env_get(t->text);
        *pos += 1;
        if (e) {
            if (e->is_bool)
                return (double)e->val_bool;
            return e->num;
        }
        fprintf(stderr, "Undefined variable: %s\n", t->text);
        exit(1);
    }

    return 0;
}

static double eval(Lexer* l, int* pos) {
    double lhs = eval_primary(l, pos);

    if (*pos >= l->ntokens)
        return lhs;

    Token* t = &l->tokens[*pos];

    if (t->type == TK_PLUS) {
        *pos += 1;
        return lhs + eval_primary(l, pos);
    }
    if (t->type == TK_MINUS) {
        *pos += 1;
        return lhs - eval_primary(l, pos);
    }
    if (t->type == TK_ASTERISK) {
        *pos += 1;
        return lhs * eval_primary(l, pos);
    }
    if (t->type == TK_SLASH) {
        *pos += 1;
        double rhs = eval_primary(l, pos);
        if (rhs == 0.0) {
            fprintf(stderr, "Divide by zero\n");
            exit(1);
        }
        return lhs / rhs;
    }

    return lhs;
}

static void run_stmt(Lexer* l, int* pos) {
    if (*pos >= l->ntokens)
        return;

    Token* t = &l->tokens[*pos];

    if (t->type == TK_PRINT) {
        *pos += 1;

        if (*pos < l->ntokens && l->tokens[*pos].type == TK_LPAREN) {
            *pos += 1;

            int p = *pos;
            double val = eval(l, &p);
            *pos = p;

            if (*pos < l->ntokens && l->tokens[*pos].type == TK_RPAREN)
                *pos += 1;

            printf("%g\n", val);
        } else {
            fprintf(stderr, "Expected (\n");
            exit(1);
        }
    } else if (t->type == TK_ID) {
        char* name = strdup(t->text);
        *pos += 1;

        if (*pos < l->ntokens && l->tokens[*pos].type == TK_ASSIGN) {
            *pos += 1;

            int p = *pos;
            double val = eval(l, &p);
            *pos = p;

            env_set(name, val, 0, 0);
        } else {
            fprintf(stderr, "Expected = after identifier\n");
            exit(1);
        }

        free(name);
    } else if (t->type == TK_EOF) {
        return;
    } else {
        fprintf(stderr, "Unexpected token in statement\n");
        exit(1);
    }
}

static void run_stmts(Lexer* l, int* pos) {
    while (*pos < l->ntokens && l->tokens[*pos].type != TK_EOF) {
        run_stmt(l, pos);

        if (*pos < l->ntokens && l->tokens[*pos].type == TK_SEMI)
            *pos += 1;
    }
}

int cat_run_file(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Cannot open %s\n", path);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* src = malloc(size + 1);
    fread(src, 1, size, f);
    src[size] = '\0';
    fclose(f);

    Lexer lexer;
    lexer.src = src;
    lexer.pos = 0;
    lexer.len = (int)size;
    lexer.ntokens = 0;
    lexer.tokens = malloc(1024 * sizeof(Token));

    lex(&lexer);

    int pos = 0;
    run_stmts(&lexer, &pos);

    free(lexer.tokens);
    free(src);

    return 0;
}