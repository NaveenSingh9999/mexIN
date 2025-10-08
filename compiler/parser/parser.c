/* LAMC Compiler - Parser Implementation
 * Production-ready recursive descent parser
 * Copyright (c) 2025 Naveen Singh
 */

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Helper to duplicate a substring */
static char* string_dup_n(const char* str, size_t len) {
    char* result = (char*)malloc(len + 1);
    if (!result) return NULL;
    memcpy(result, str, len);
    result[len] = '\0';
    return result;
}

/* ===== Initialization ===== */

void parser_init(Parser* parser, Lexer* lexer) {
    parser->lexer = lexer;
    parser->had_error = false;
    parser->panic_mode = false;
    
    /* Prime the parser with the first token */
    parser_advance(parser);
}

void parser_free(Parser* parser) {
    /* Nothing to free for now */
    (void)parser;
}

/* ===== Helper Functions ===== */

void parser_advance(Parser* parser) {
    parser->previous = parser->current;
    
    for (;;) {
        parser->current = lexer_next_token(parser->lexer);
        
        if (parser->current.type != TOKEN_ERROR) break;
        
        parser_error_at_current(parser, parser->current.start);
    }
}

bool parser_check(Parser* parser, TokenType type) {
    return parser->current.type == type;
}

bool parser_match(Parser* parser, TokenType type) {
    if (!parser_check(parser, type)) return false;
    parser_advance(parser);
    return true;
}

Token parser_expect(Parser* parser, TokenType type, const char* message) {
    if (parser->current.type == type) {
        Token token = parser->current;
        parser_advance(parser);
        return token;
    }
    
    parser_error_at_current(parser, message);
    return parser->current;
}

Token parser_peek(Parser* parser) {
    return parser->current;
}

Token parser_previous(Parser* parser) {
    return parser->previous;
}

bool parser_is_at_end(Parser* parser) {
    return parser->current.type == TOKEN_EOF;
}

/* ===== Error Handling ===== */

void parser_error_at(Parser* parser, Token* token, const char* message) {
    if (parser->panic_mode) return;
    parser->panic_mode = true;
    parser->had_error = true;
    
    fprintf(stderr, "[Line %d, Column %d] Error", token->line, token->column);
    
    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        /* Nothing */
    } else {
        fprintf(stderr, " at '%.*s'", (int)token->length, token->start);
    }
    
    fprintf(stderr, ": %s\n", message);
}

void parser_error_at_current(Parser* parser, const char* message) {
    parser_error_at(parser, &parser->current, message);
}

void parser_error(Parser* parser, const char* message) {
    parser_error_at(parser, &parser->previous, message);
}

void parser_synchronize(Parser* parser) {
    parser->panic_mode = false;
    
    while (parser->current.type != TOKEN_EOF) {
        /* Synchronize at statement boundaries */
        if (parser->previous.type == TOKEN_NEWLINE) return;
        
        switch (parser->current.type) {
            case TOKEN_FUNC:
            case TOKEN_IF:
            case TOKEN_WHILE:
            case TOKEN_FOR:
            case TOKEN_LOOP:
            case TOKEN_RETURN:
            case TOKEN_IMPORT:
            case TOKEN_CLASS:
                return;
            default:
                ; /* Do nothing */
        }
        
        parser_advance(parser);
    }
}

/* ===== Primary Expression Parsing ===== */

AstNode* parser_parse_primary(Parser* parser) {
    /* Check for empty expression in parens or brackets */
    if (parser->current.type == TOKEN_RIGHT_PAREN || 
        parser->current.type == TOKEN_RIGHT_BRACKET ||
        parser->current.type == TOKEN_EOF) {
        return NULL;
    }
    
    /* Integer literal */
    if (parser_match(parser, TOKEN_INT)) {
        Token token = parser->previous;
        char* num_str = string_dup_n(token.start, token.length);
        long value = strtol(num_str, NULL, 10);
        free(num_str);
        return ast_create_literal_int(value, token.line, token.column);
    }
    
    /* Float literal */
    if (parser_match(parser, TOKEN_FLOAT)) {
        Token token = parser->previous;
        char* num_str = string_dup_n(token.start, token.length);
        double value = strtod(num_str, NULL);
        free(num_str);
        return ast_create_literal_float(value, token.line, token.column);
    }
    
    /* String literal */
    if (parser_match(parser, TOKEN_STRING)) {
        Token token = parser->previous;
        /* Remove quotes from string */
        char* str_value = string_dup_n(token.start + 1, token.length - 2);
        AstNode* node = ast_create_literal_string(str_value, token.line, token.column);
        free(str_value);
        return node;
    }
    
    /* Boolean literals */
    if (parser_match(parser, TOKEN_TRUE)) {
        Token token = parser->previous;
        return ast_create_literal_bool(true, token.line, token.column);
    }
    
    if (parser_match(parser, TOKEN_FALSE)) {
        Token token = parser->previous;
        return ast_create_literal_bool(false, token.line, token.column);
    }
    
    /* Identifier */
    if (parser_match(parser, TOKEN_IDENTIFIER)) {
        Token token = parser->previous;
        char* name = string_dup_n(token.start, token.length);
        AstNode* node = ast_create_identifier(name, token.line, token.column);
        free(name);
        return node;
    }
    
    /* Grouped expression */
    if (parser_match(parser, TOKEN_LEFT_PAREN)) {
        AstNode* expr = parser_parse_expression(parser);
        if (!expr) {
            parser_error(parser, "Expected expression after '('");
            return NULL;
        }
        parser_expect(parser, TOKEN_RIGHT_PAREN, "Expected ')' after expression");
        return expr;
    }
    
    /* Array literal */
    if (parser_match(parser, TOKEN_LEFT_BRACKET)) {
        Token start_token = parser->previous;
        AstList* elements = ast_list_create();
        
        if (!parser_check(parser, TOKEN_RIGHT_BRACKET)) {
            do {
                AstNode* element = parser_parse_expression(parser);
                ast_list_append(elements, element);
            } while (parser_match(parser, TOKEN_COMMA));
        }
        
        parser_expect(parser, TOKEN_RIGHT_BRACKET, "Expected ']' after array elements");
        return ast_create_array(elements, start_token.line, start_token.column);
    }
    
    parser_error_at_current(parser, "Expected expression");
    return NULL;
}

/* ===== Expression Parsing (Pratt Parser - Simple Version) ===== */

/* Forward declarations for expression parsing */
static AstNode* parse_unary(Parser* parser);
static AstNode* parse_factor(Parser* parser);
static AstNode* parse_term(Parser* parser);
static AstNode* parse_comparison(Parser* parser);
static AstNode* parse_equality(Parser* parser);
static AstNode* parse_logical_and(Parser* parser);
static AstNode* parse_logical_or(Parser* parser);

/* Forward declarations for statement parsing */
static AstNode* parse_if_statement(Parser* parser);
static AstNode* parse_while_statement(Parser* parser);
static AstNode* parse_for_statement(Parser* parser);
static AstNode* parse_loop_statement(Parser* parser);
static AstNode* parse_return_statement(Parser* parser);
static AstNode* parse_block_statement(Parser* parser);
static AstNode* parse_function_declaration(Parser* parser);

/* Postfix operators (call, index, member) */
static AstNode* parse_postfix(Parser* parser) {
    AstNode* expr = parser_parse_primary(parser);
    
    for (;;) {
        /* Function call */
        if (parser_match(parser, TOKEN_LEFT_PAREN)) {
            AstList* args = ast_list_create();
            
            if (!parser_check(parser, TOKEN_RIGHT_PAREN)) {
                do {
                    AstNode* arg = parser_parse_expression(parser);
                    ast_list_append(args, arg);
                } while (parser_match(parser, TOKEN_COMMA));
            }
            
            Token paren = parser_expect(parser, TOKEN_RIGHT_PAREN, "Expected ')' after arguments");
            expr = ast_create_call(expr, args, paren.line, paren.column);
        }
        /* Array indexing */
        else if (parser_match(parser, TOKEN_LEFT_BRACKET)) {
            AstNode* index = parser_parse_expression(parser);
            Token bracket = parser_expect(parser, TOKEN_RIGHT_BRACKET, "Expected ']' after index");
            expr = ast_create_index(expr, index, bracket.line, bracket.column);
        }
        /* Member access */
        else if (parser_match(parser, TOKEN_DOT)) {
            Token member = parser_expect(parser, TOKEN_IDENTIFIER, "Expected property name after '.'");
            char* member_name = string_dup_n(member.start, member.length);
            expr = ast_create_member(expr, member_name, member.line, member.column);
            free(member_name);
        }
        else {
            break;
        }
    }
    
    return expr;
}

/* Continue parsing postfix operations on an already-parsed expression */
static AstNode* parse_postfix_continue(Parser* parser, AstNode* expr) {
    for (;;) {
        /* Function call */
        if (parser_match(parser, TOKEN_LEFT_PAREN)) {
            AstList* args = ast_list_create();
            
            if (!parser_check(parser, TOKEN_RIGHT_PAREN)) {
                do {
                    AstNode* arg = parser_parse_expression(parser);
                    ast_list_append(args, arg);
                } while (parser_match(parser, TOKEN_COMMA));
            }
            
            Token paren = parser_expect(parser, TOKEN_RIGHT_PAREN, "Expected ')' after arguments");
            expr = ast_create_call(expr, args, paren.line, paren.column);
        }
        /* Array indexing */
        else if (parser_match(parser, TOKEN_LEFT_BRACKET)) {
            AstNode* index = parser_parse_expression(parser);
            Token bracket = parser_expect(parser, TOKEN_RIGHT_BRACKET, "Expected ']' after index");
            expr = ast_create_index(expr, index, bracket.line, bracket.column);
        }
        /* Member access */
        else if (parser_match(parser, TOKEN_DOT)) {
            Token member = parser_expect(parser, TOKEN_IDENTIFIER, "Expected property name after '.'");
            char* member_name = string_dup_n(member.start, member.length);
            expr = ast_create_member(expr, member_name, member.line, member.column);
            free(member_name);
        }
        else {
            break;
        }
    }
    
    return expr;
}

/* Unary operators: -, !, ~ */
static AstNode* parse_unary(Parser* parser) {
    if (parser_match(parser, TOKEN_MINUS)) {
        Token op = parser->previous;
        AstNode* operand = parse_unary(parser);
        return ast_create_unary(OP_NEG, operand, op.line, op.column);
    }
    
    if (parser_match(parser, TOKEN_NOT)) {
        Token op = parser->previous;
        AstNode* operand = parse_unary(parser);
        return ast_create_unary(OP_NOT, operand, op.line, op.column);
    }
    
    if (parser_match(parser, TOKEN_TILDE)) {
        Token op = parser->previous;
        AstNode* operand = parse_unary(parser);
        return ast_create_unary(OP_BIT_NOT, operand, op.line, op.column);
    }
    
    return parse_postfix(parser);
}

/* Multiplicative operators: *, /, % */
static AstNode* parse_factor(Parser* parser) {
    AstNode* expr = parse_unary(parser);
    
    while (parser_match(parser, TOKEN_STAR) || 
           parser_match(parser, TOKEN_SLASH) || 
           parser_match(parser, TOKEN_PERCENT)) {
        Token op = parser->previous;
        AstNode* right = parse_unary(parser);
        
        BinaryOp bin_op;
        if (op.type == TOKEN_STAR) bin_op = OP_MUL;
        else if (op.type == TOKEN_SLASH) bin_op = OP_DIV;
        else bin_op = OP_MOD;
        
        expr = ast_create_binary(bin_op, expr, right, op.line, op.column);
    }
    
    return expr;
}

/* Additive operators: +, - */
static AstNode* parse_term(Parser* parser) {
    AstNode* expr = parse_factor(parser);
    
    while (parser_match(parser, TOKEN_PLUS) || parser_match(parser, TOKEN_MINUS)) {
        Token op = parser->previous;
        AstNode* right = parse_factor(parser);
        
        BinaryOp bin_op = (op.type == TOKEN_PLUS) ? OP_ADD : OP_SUB;
        expr = ast_create_binary(bin_op, expr, right, op.line, op.column);
    }
    
    return expr;
}

/* Comparison operators: <, >, <=, >= */
static AstNode* parse_comparison(Parser* parser) {
    AstNode* expr = parse_term(parser);
    
    while (parser_match(parser, TOKEN_LESS) || 
           parser_match(parser, TOKEN_GREATER) ||
           parser_match(parser, TOKEN_LESS_EQUAL) ||
           parser_match(parser, TOKEN_GREATER_EQUAL)) {
        Token op = parser->previous;
        AstNode* right = parse_term(parser);
        
        BinaryOp bin_op;
        switch (op.type) {
            case TOKEN_LESS: bin_op = OP_LT; break;
            case TOKEN_GREATER: bin_op = OP_GT; break;
            case TOKEN_LESS_EQUAL: bin_op = OP_LE; break;
            case TOKEN_GREATER_EQUAL: bin_op = OP_GE; break;
            default: bin_op = OP_LT; /* Should never happen */
        }
        
        expr = ast_create_binary(bin_op, expr, right, op.line, op.column);
    }
    
    return expr;
}

/* Equality operators: ==, != */
static AstNode* parse_equality(Parser* parser) {
    AstNode* expr = parse_comparison(parser);
    
    while (parser_match(parser, TOKEN_EQUAL_EQUAL) || parser_match(parser, TOKEN_NOT_EQUAL)) {
        Token op = parser->previous;
        AstNode* right = parse_comparison(parser);
        
        BinaryOp bin_op = (op.type == TOKEN_EQUAL_EQUAL) ? OP_EQ : OP_NE;
        expr = ast_create_binary(bin_op, expr, right, op.line, op.column);
    }
    
    return expr;
}

/* Logical AND: && */
static AstNode* parse_logical_and(Parser* parser) {
    AstNode* expr = parse_equality(parser);
    
    while (parser_match(parser, TOKEN_AND)) {
        Token op = parser->previous;
        AstNode* right = parse_equality(parser);
        expr = ast_create_binary(OP_AND, expr, right, op.line, op.column);
    }
    
    return expr;
}

/* Logical OR: || */
static AstNode* parse_logical_or(Parser* parser) {
    AstNode* expr = parse_logical_and(parser);
    
    while (parser_match(parser, TOKEN_OR)) {
        Token op = parser->previous;
        AstNode* right = parse_logical_and(parser);
        expr = ast_create_binary(OP_OR, expr, right, op.line, op.column);
    }
    
    return expr;
}

AstNode* parser_parse_expression(Parser* parser) {
    return parse_logical_or(parser);
}

/* ===== Statement Parsing Helpers ===== */

/* Parse a block of statements (for function bodies, control flow, etc.) */
static AstNode* parse_block_statement(Parser* parser) {
    Token brace = parser_expect(parser, TOKEN_LEFT_BRACE, "Expected '{' to begin block");
    
    AstList* statements = ast_list_create();
    
    while (!parser_check(parser, TOKEN_RIGHT_BRACE) && !parser_is_at_end(parser)) {
        AstNode* stmt = parser_parse_statement(parser);
        if (stmt) {
            ast_list_append(statements, stmt);
        }
    }
    
    parser_expect(parser, TOKEN_RIGHT_BRACE, "Expected '}' after block");
    
    return ast_create_block(statements, brace.line, brace.column);
}

/* Parse if statement: if condition { ... } else if condition { ... } else { ... } */
static AstNode* parse_if_statement(Parser* parser) {
    Token if_token = parser->previous;
    
    /* Parse condition */
    AstNode* condition = parser_parse_expression(parser);
    if (!condition) {
        parser_error(parser, "Expected condition in if statement");
        return NULL;
    }
    
    /* Parse then branch */
    AstNode* then_branch = NULL;
    if (parser_check(parser, TOKEN_LEFT_BRACE)) {
        then_branch = parse_block_statement(parser);
    } else {
        /* Single statement without braces */
        then_branch = parser_parse_statement(parser);
    }
    
    /* Parse else branch (optional) */
    AstNode* else_branch = NULL;
    if (parser_match(parser, TOKEN_ELSE)) {
        if (parser_check(parser, TOKEN_IF)) {
            /* else if - parse as another if statement */
            parser_advance(parser);
            else_branch = parse_if_statement(parser);
        } else if (parser_check(parser, TOKEN_LEFT_BRACE)) {
            else_branch = parse_block_statement(parser);
        } else {
            /* Single statement without braces */
            else_branch = parser_parse_statement(parser);
        }
    }
    
    return ast_create_if(condition, then_branch, else_branch, if_token.line, if_token.column);
}

/* Parse while loop: while condition { ... } */
static AstNode* parse_while_statement(Parser* parser) {
    Token while_token = parser->previous;
    
    /* Parse condition */
    AstNode* condition = parser_parse_expression(parser);
    if (!condition) {
        parser_error(parser, "Expected condition in while statement");
        return NULL;
    }
    
    /* Parse body */
    AstNode* body = NULL;
    if (parser_check(parser, TOKEN_LEFT_BRACE)) {
        body = parse_block_statement(parser);
    } else {
        /* Single statement without braces */
        body = parser_parse_statement(parser);
    }
    
    return ast_create_while(condition, body, while_token.line, while_token.column);
}

/* Parse for loop: for i in 0..10 { ... } or for item in array { ... } */
static AstNode* parse_for_statement(Parser* parser) {
    Token for_token = parser->previous;
    
    /* Parse iterator variable(s) */
    Token var = parser_expect(parser, TOKEN_IDENTIFIER, "Expected variable name in for loop");
    char* var_name = string_dup_n(var.start, var.length);
    
    /* Check for index variable: for i, item in array */
    char* index_var = NULL;
    if (parser_match(parser, TOKEN_COMMA)) {
        index_var = var_name;
        Token value_var = parser_expect(parser, TOKEN_IDENTIFIER, "Expected value variable after ','");
        var_name = string_dup_n(value_var.start, value_var.length);
    }
    
    /* Expect 'in' keyword */
    parser_expect(parser, TOKEN_IN, "Expected 'in' in for loop");
    
    /* Parse iterable expression */
    AstNode* iterable = parser_parse_expression(parser);
    if (!iterable) {
        parser_error(parser, "Expected iterable expression in for loop");
        free(var_name);
        if (index_var) free(index_var);
        return NULL;
    }
    
    /* Parse body */
    AstNode* body = NULL;
    if (parser_check(parser, TOKEN_LEFT_BRACE)) {
        body = parse_block_statement(parser);
    } else {
        body = parser_parse_statement(parser);
    }
    
    AstNode* result = ast_create_for(var_name, iterable, body, index_var,
                                     for_token.line, for_token.column);
    
    free(var_name);
    if (index_var) free(index_var);
    
    return result;
}

/* Parse infinite loop: loop { ... } */
static AstNode* parse_loop_statement(Parser* parser) {
    Token loop_token = parser->previous;
    
    /* Parse body */
    AstNode* body = NULL;
    if (parser_check(parser, TOKEN_LEFT_BRACE)) {
        body = parse_block_statement(parser);
    } else {
        body = parser_parse_statement(parser);
    }
    
    return ast_create_loop(body, loop_token.line, loop_token.column);
}

/* Parse return statement: return or return expr */
static AstNode* parse_return_statement(Parser* parser) {
    Token return_token = parser->previous;
    
    AstNode* value = NULL;
    
    /* Check if there's a return value */
    if (!parser_check(parser, TOKEN_RIGHT_BRACE) && 
        !parser_check(parser, TOKEN_EOF) &&
        !parser_is_at_end(parser)) {
        /* Try to parse return value - but be careful about newlines */
        /* For now, we'll assume if the next token is on the same line, it's the return value */
        value = parser_parse_expression(parser);
    }
    
    return ast_create_return(value, return_token.line, return_token.column);
}

/* Parse function declaration: func name(params) { ... } or func name(params) -> type { ... } */
static AstNode* parse_function_declaration(Parser* parser) {
    Token func_token = parser->previous;
    
    /* Parse function name */
    Token name_token = parser_expect(parser, TOKEN_IDENTIFIER, "Expected function name");
    char* func_name = string_dup_n(name_token.start, name_token.length);
    
    /* Parse parameter list */
    parser_expect(parser, TOKEN_LEFT_PAREN, "Expected '(' after function name");
    
    AstList* params = ast_list_create();
    
    if (!parser_check(parser, TOKEN_RIGHT_PAREN)) {
        do {
            Token param = parser_expect(parser, TOKEN_IDENTIFIER, "Expected parameter name");
            char* param_name = string_dup_n(param.start, param.length);
            
            /* Check for type annotation: param: type */
            char* param_type = NULL;
            if (parser_match(parser, TOKEN_COLON)) {
                Token type_token = parser_expect(parser, TOKEN_IDENTIFIER, "Expected parameter type");
                param_type = string_dup_n(type_token.start, type_token.length);
            }
            
            /* Create parameter object */
            Parameter* param_obj = ast_create_parameter(param_name, param_type, NULL);
            ast_list_append(params, param_obj);
            
            free(param_name);
            if (param_type) free(param_type);
            
        } while (parser_match(parser, TOKEN_COMMA));
    }
    
    parser_expect(parser, TOKEN_RIGHT_PAREN, "Expected ')' after parameters");
    
    /* Check for return type annotation: -> type */
    char* return_type = NULL;
    if (parser_match(parser, TOKEN_ARROW)) {
        Token type_token = parser_expect(parser, TOKEN_IDENTIFIER, "Expected return type");
        return_type = string_dup_n(type_token.start, type_token.length);
    }
    
    /* Parse function body */
    AstNode* body = parse_block_statement(parser);
    
    AstNode* func = ast_create_function(func_name, params, body, return_type,
                                        func_token.line, func_token.column);
    
    free(func_name);
    if (return_type) free(return_type);
    
    return func;
}

/* ===== Statement Parsing (Basic) ===== */

AstNode* parser_parse_statement(Parser* parser) {
    /* Variable declaration: identifier = expr OR identifier: type = expr */
    if (parser_check(parser, TOKEN_IDENTIFIER)) {
        /* We need to peek ahead to distinguish:
         * - "x = 42" (variable declaration)
         * - "x: int = 42" (typed variable declaration)
         * - "print(...)" (expression statement with function call)
         */
        
        /* Since we can't easily backtrack with streaming lexer,
         * we'll handle this by checking the NEXT token after identifier.
         * This requires a two-token lookahead which we can hack with
         * saving state temporarily.
         */
        Token name_token = parser->current;
        parser_advance(parser);
        
        /* Check for type annotation (x: type = value) */
        if (parser_check(parser, TOKEN_COLON)) {
            parser_advance(parser);
            Token type_token = parser_expect(parser, TOKEN_IDENTIFIER, "Expected type name");
            char* type_name = string_dup_n(type_token.start, type_token.length);
            
            parser_expect(parser, TOKEN_EQUAL, "Expected '=' after type");
            AstNode* init = parser_parse_expression(parser);
            
            char* var_name = string_dup_n(name_token.start, name_token.length);
            AstNode* var_decl = ast_create_var_decl(var_name, type_name, init, 
                                                     name_token.line, name_token.column);
            free(var_name);
            free(type_name);
            return var_decl;
        }
        /* Simple assignment: x = value (treat as variable declaration) */
        else if (parser_check(parser, TOKEN_EQUAL)) {
            parser_advance(parser);
            AstNode* value = parser_parse_expression(parser);
            
            char* var_name = string_dup_n(name_token.start, name_token.length);
            AstNode* var_decl = ast_create_var_decl(var_name, NULL, value,
                                                     name_token.line, name_token.column);
            free(var_name);
            return var_decl;
        }
        else {
            /* It's an expression like "print(...)" or "foo.bar" 
             * We already consumed the identifier, so create it manually
             * and then continue parsing postfix expressions
             */
            char* id_name = string_dup_n(name_token.start, name_token.length);
            AstNode* expr = ast_create_identifier(id_name, name_token.line, name_token.column);
            free(id_name);
            
            /* Now parse any postfix operations (calls, indexing, member access) */
            expr = parse_postfix_continue(parser, expr);
            
            /* Parse any binary operations that follow */
            /* This is a simplification - ideally we'd restart the precedence climb */
            /* For now, just return as expression statement */
            return ast_create_expr_stmt(expr, expr->line, expr->column);
        }
    }
    
    /* If statement */
    if (parser_match(parser, TOKEN_IF)) {
        return parse_if_statement(parser);
    }
    
    /* While loop */
    if (parser_match(parser, TOKEN_WHILE)) {
        return parse_while_statement(parser);
    }
    
    /* For loop */
    if (parser_match(parser, TOKEN_FOR)) {
        return parse_for_statement(parser);
    }
    
    /* Infinite loop */
    if (parser_match(parser, TOKEN_LOOP)) {
        return parse_loop_statement(parser);
    }
    
    /* Return statement */
    if (parser_match(parser, TOKEN_RETURN)) {
        return parse_return_statement(parser);
    }
    
    /* Break statement */
    if (parser_match(parser, TOKEN_BREAK)) {
        Token tok = parser->previous;
        return ast_create_break(tok.line, tok.column);
    }
    
    /* Continue statement */
    if (parser_match(parser, TOKEN_CONTINUE)) {
        Token tok = parser->previous;
        return ast_create_continue(tok.line, tok.column);
    }
    
    /* Default: expression statement */
    AstNode* expr = parser_parse_expression(parser);
    if (!expr) {
        return NULL;
    }
    return ast_create_expr_stmt(expr, expr->line, expr->column);
}

AstNode* parser_parse_declaration(Parser* parser) {
    /* Function declaration */
    if (parser_match(parser, TOKEN_FUNC)) {
        return parse_function_declaration(parser);
    }
    
    /* Otherwise, parse as statement */
    return parser_parse_statement(parser);
}

/* ===== Main Parse Function ===== */

AstNode* parser_parse(Parser* parser) {
    AstList* declarations = ast_list_create();
    
    while (!parser_is_at_end(parser)) {
        AstNode* decl = parser_parse_declaration(parser);
        if (decl) {
            ast_list_append(declarations, decl);
        }
        
        if (parser->panic_mode) {
            parser_synchronize(parser);
        }
    }
    
    if (parser->had_error) {
        /* Free declarations and return NULL on error */
        for (size_t i = 0; i < declarations->count; i++) {
            ast_free_node((AstNode*)declarations->items[i]);
        }
        ast_list_free(declarations);
        return NULL;
    }
    
    return ast_create_program(declarations);
}
