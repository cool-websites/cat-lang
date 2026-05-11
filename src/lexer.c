#include "../include/cat.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

static void add_token(Lexer* l, TokenType type, const char* text, long long ival, double fval, int bval) {
    Token* t = &l->tokens[l->ntokens++];
    t->type = type;
    t->text = text ? strdup(text) : NULL;
    t->value.ival = ival;
    t->value.fval = fval;
    t->value.bval = bval;
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static bool is_id_char(char c) {
    return isalnum(c) || c == '_';
}

static void lex_string(Lexer* l) {
    int start = l->pos + 1;
    while (l->pos < l->len && l->src[l->pos] != '"' && l->src[l->pos] != '\n')
        l->pos++;

    int len = l->pos - start;
    if (l->pos < l->len && l->src[l->pos] == '"') {
        char* str = malloc(len + 1);
        memcpy(str, l->src + start, len);
        str[len] = '\0';
        add_token(l, TK_STRING, str, 0, 0, 0);
        free(str);
        l->pos++;
    } else {
        fprintf(stderr, "Unclosed string\n");
        exit(1);
    }
}

static void lex_number(Lexer* l) {
    int start = l->pos;
    int dot = 0;

    while (l->pos < l->len && (is_digit(l->src[l->pos]) || l->src[l->pos] == '.')) {
        if (l->src[l->pos] == '.') {
            if (dot) break;
            dot = 1;
        }
        l->pos++;
    }

    int len = l->pos - start;
    char* numstr = malloc(len + 1);
    memcpy(numstr, l->src + start, len);
    numstr[len] = '\0';

    if (dot) {
        add_token(l, TK_FLOAT, NULL, 0, atof(numstr), 0);
    } else {
        add_token(l, TK_INT, NULL, atoll(numstr), 0, 0);
    }

    free(numstr);
}

static void lex_identifier(Lexer* l) {
    int start = l->pos;

    while (l->pos < l->len && is_id_char(l->src[l->pos]))
        l->pos++;

    int len = l->pos - start;
    char* id = malloc(len + 1);
    memcpy(id, l->src + start, len);
    id[len] = '\0';

    if (strcmp(id, "print") == 0) {
        add_token(l, TK_PRINT, id, 0, 0, 0);
    } else if (strcmp(id, "while") == 0) {
        add_token(l, TK_WHILE, id, 0, 0, 0);
    } else if (strcmp(id, "if") == 0) {
        add_token(l, TK_IF, id, 0, 0, 0);
    } else if (strcmp(id, "else") == 0) {
        add_token(l, TK_ELSE, id, 0, 0, 0);
    } else if (strcmp(id, "true") == 0 || strcmp(id, "false") == 0) {
        add_token(l, TK_BOOL, id, 0, 0, strcmp(id, "true") == 0);
    } else {
        add_token(l, TK_ID, id, 0, 0, 0);
    }

    free(id);
}

void lex(Lexer* l) {
    while (l->pos < l->len) {
        char c = l->src[l->pos];

        if (isspace((unsigned char)c)) {
            l->pos++;
            continue;
        }

        if (c == '"') {
            l->pos++;
            lex_string(l);
            continue;
        }

        if (is_digit(c)) {
            lex_number(l);
            continue;
        }

        if (isalpha((unsigned char)c) || c == '_') {
            lex_identifier(l);
            continue;
        }

        if (c == '=') {
            l->pos++;
            add_token(l, TK_ASSIGN, NULL, 0, 0, 0);
            continue;
        }

        if (c == '+') {
            l->pos++;
            add_token(l, TK_PLUS, NULL, 0, 0, 0);
            continue;
        }

        if (c == '-') {
            l->pos++;
            add_token(l, TK_MINUS, NULL, 0, 0, 0);
            continue;
        }

        if (c == '*') {
            l->pos++;
            add_token(l, TK_ASTERISK, NULL, 0, 0, 0);
            continue;
        }

        if (c == '/') {
            l->pos++;
            add_token(l, TK_SLASH, NULL, 0, 0, 0);
            continue;
        }

        if (c == '(') {
            l->pos++;
            add_token(l, TK_LPAREN, NULL, 0, 0, 0);
            continue;
        }

        if (c == ')') {
            l->pos++;
            add_token(l, TK_RPAREN, NULL, 0, 0, 0);
            continue;
        }

        if (c == ';') {
            l->pos++;
            add_token(l, TK_SEMI, NULL, 0, 0, 0);
            continue;
        }

        fprintf(stderr, "Unexpected char: %c\n", c);
        exit(1);
    }

    add_token(l, TK_EOF, NULL, 0, 0, 0);
}