#ifndef CAT_H
#define CAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TK_EOF,
    TK_ID,
    TK_INT,
    TK_FLOAT,
    TK_STRING,
    TK_ASSIGN,
    TK_PLUS,
    TK_MINUS,
    TK_ASTERISK,
    TK_SLASH,
    TK_LPAREN,
    TK_RPAREN,
    TK_SEMI,
    TK_PRINT,
    TK_WHILE,
    TK_IF,
    TK_ELSE,
    TK_BOOL,
} TokenType;

typedef struct {
    TokenType type;
    char* text;
    union {
        long long ival;
        double fval;
        int bval;
    } value;
} Token;

typedef struct {
    const char* src;
    int pos;
    int len;
    Token* tokens;
    int ntokens;
} Lexer;

typedef struct Env {
    char* name;
    double num;
    int is_bool;
    int val_bool;
    struct Env* next;
} Env;

int cat_run_file(const char* path);

#endif
