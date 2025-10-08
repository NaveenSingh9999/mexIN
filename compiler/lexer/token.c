/* LAMC Compiler - Token Implementation */

#include "token.h"
#include <string.h>

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_STRING: return "STRING";
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_FUNC: return "FUNC";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_IN: return "IN";
        case TOKEN_LOOP: return "LOOP";
        case TOKEN_BREAK: return "BREAK";
        case TOKEN_CONTINUE: return "CONTINUE";
        case TOKEN_IMPORT: return "IMPORT";
        case TOKEN_EXPORT: return "EXPORT";
        case TOKEN_CLASS: return "CLASS";
        case TOKEN_THIS: return "THIS";
        case TOKEN_TRY: return "TRY";
        case TOKEN_CATCH: return "CATCH";
        case TOKEN_FINALLY: return "FINALLY";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_STAR: return "STAR";
        case TOKEN_SLASH: return "SLASH";
        case TOKEN_PERCENT: return "PERCENT";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_LESS: return "LESS";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_LESS_EQUAL: return "LESS_EQUAL";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT";
        case TOKEN_AMPERSAND: return "AMPERSAND";
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_CARET: return "CARET";
        case TOKEN_TILDE: return "TILDE";
        case TOKEN_DOT_DOT: return "DOT_DOT";
        case TOKEN_DOT_DOT_EQUAL: return "DOT_DOT_EQUAL";
        case TOKEN_LEFT_PAREN: return "LEFT_PAREN";
        case TOKEN_RIGHT_PAREN: return "RIGHT_PAREN";
        case TOKEN_LEFT_BRACE: return "LEFT_BRACE";
        case TOKEN_RIGHT_BRACE: return "RIGHT_BRACE";
        case TOKEN_LEFT_BRACKET: return "LEFT_BRACKET";
        case TOKEN_RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_COLON: return "COLON";
        case TOKEN_DOT: return "DOT";
        case TOKEN_ARROW: return "ARROW";
        case TOKEN_NEWLINE: return "NEWLINE";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

Token make_token(TokenType type, const char* start, size_t length, int line, int column) {
    Token token;
    token.type = type;
    token.start = start;
    token.length = length;
    token.line = line;
    token.column = column;
    return token;
}

Token error_token(const char* message, int line, int column) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = strlen(message);
    token.line = line;
    token.column = column;
    return token;
}
