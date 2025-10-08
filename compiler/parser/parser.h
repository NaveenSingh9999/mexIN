/* LAMC Compiler - Parser Interface
 * Converts tokens into Abstract Syntax Tree
 * Production-ready recursive descent parser
 * Copyright (c) 2025 Naveen Singh
 */

#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include <stdbool.h>

/* Parser state */
typedef struct {
    Lexer* lexer;           /* Lexer for token generation */
    Token current;          /* Current token */
    Token previous;         /* Previous token */
    bool had_error;         /* Error flag */
    bool panic_mode;        /* Panic mode for error recovery */
} Parser;

/* Parser initialization and cleanup */
void parser_init(Parser* parser, Lexer* lexer);
void parser_free(Parser* parser);

/* Main parsing entry point */
AstNode* parser_parse(Parser* parser);

/* Expression parsing */
AstNode* parser_parse_expression(Parser* parser);
AstNode* parser_parse_primary(Parser* parser);

/* Statement parsing */
AstNode* parser_parse_statement(Parser* parser);
AstNode* parser_parse_declaration(Parser* parser);

/* Helper functions */
void parser_advance(Parser* parser);
bool parser_check(Parser* parser, TokenType type);
bool parser_match(Parser* parser, TokenType type);
Token parser_expect(Parser* parser, TokenType type, const char* message);
Token parser_peek(Parser* parser);
Token parser_previous(Parser* parser);

/* Error handling */
void parser_error_at_current(Parser* parser, const char* message);
void parser_error(Parser* parser, const char* message);
void parser_error_at(Parser* parser, Token* token, const char* message);
void parser_synchronize(Parser* parser);

/* Utility */
bool parser_is_at_end(Parser* parser);

#endif /* PARSER_H */
