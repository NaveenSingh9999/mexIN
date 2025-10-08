/* LAMC Compiler - Token Definitions
 * Part of the LAMC (Low-level Advanced Machine Code) Compiler
 * Written in C for independence and performance
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

// Token types for LAMC language
typedef enum {
    // Literals
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_CHAR,
    TOKEN_TRUE,
    TOKEN_FALSE,
    
    // Identifiers and Keywords
    TOKEN_IDENTIFIER,
    TOKEN_FUNC,
    TOKEN_RETURN,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_IN,
    TOKEN_LOOP,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_IMPORT,
    TOKEN_EXPORT,
    TOKEN_CLASS,
    TOKEN_THIS,
    TOKEN_TRY,
    TOKEN_CATCH,
    TOKEN_FINALLY,
    
    // Operators
    TOKEN_PLUS,           // +
    TOKEN_MINUS,          // -
    TOKEN_STAR,           // *
    TOKEN_SLASH,          // /
    TOKEN_PERCENT,        // %
    TOKEN_EQUAL,          // =
    TOKEN_EQUAL_EQUAL,    // ==
    TOKEN_NOT_EQUAL,      // !=
    TOKEN_LESS,           // <
    TOKEN_GREATER,        // >
    TOKEN_LESS_EQUAL,     // <=
    TOKEN_GREATER_EQUAL,  // >=
    TOKEN_AND,            // &&
    TOKEN_OR,             // ||
    TOKEN_NOT,            // !
    TOKEN_AMPERSAND,      // &
    TOKEN_PIPE,           // |
    TOKEN_CARET,          // ^
    TOKEN_TILDE,          // ~
    TOKEN_DOT_DOT,        // ..
    TOKEN_DOT_DOT_EQUAL,  // ..=
    
    // Delimiters
    TOKEN_LEFT_PAREN,     // (
    TOKEN_RIGHT_PAREN,    // )
    TOKEN_LEFT_BRACE,     // {
    TOKEN_RIGHT_BRACE,    // }
    TOKEN_LEFT_BRACKET,   // [
    TOKEN_RIGHT_BRACKET,  // ]
    TOKEN_COMMA,          // ,
    TOKEN_COLON,          // :
    TOKEN_DOT,            // .
    TOKEN_ARROW,          // ->
    
    // Special
    TOKEN_NEWLINE,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    const char* start;  // Points to start of lexeme in source
    size_t length;      // Length of lexeme
    int line;           // Line number
    int column;         // Column number
} Token;

// Function to get string representation of token type
const char* token_type_to_string(TokenType type);

// Function to create a token
Token make_token(TokenType type, const char* start, size_t length, int line, int column);

// Function to create an error token
Token error_token(const char* message, int line, int column);

#endif // TOKEN_H
