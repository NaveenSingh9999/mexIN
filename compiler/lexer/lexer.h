/* LAMC Compiler - Lexer (Tokenizer)
 * Converts source code into tokens
 */

#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdbool.h>

// Lexer state
typedef struct {
    const char* start;      // Start of current token
    const char* current;    // Current character
    int line;               // Current line number
    int column;             // Current column number
} Lexer;

// Initialize the lexer with source code
void lexer_init(Lexer* lexer, const char* source);

// Get the next token
Token lexer_next_token(Lexer* lexer);

// Helper functions
bool is_at_end(Lexer* lexer);
char advance(Lexer* lexer);
char peek(Lexer* lexer);
char peek_next(Lexer* lexer);
bool match(Lexer* lexer, char expected);

#endif // LEXER_H
