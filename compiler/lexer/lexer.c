/* LAMC Compiler - Lexer Implementation */

#include "lexer.h"
#include <string.h>
#include <ctype.h>

void lexer_init(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
    lexer->column = 1;
}

bool is_at_end(Lexer* lexer) {
    return *lexer->current == '\0';
}

char advance(Lexer* lexer) {
    lexer->column++;
    return *lexer->current++;
}

char peek(Lexer* lexer) {
    return *lexer->current;
}

char peek_next(Lexer* lexer) {
    if (is_at_end(lexer)) return '\0';
    return lexer->current[1];
}

bool match(Lexer* lexer, char expected) {
    if (is_at_end(lexer)) return false;
    if (*lexer->current != expected) return false;
    lexer->current++;
    lexer->column++;
    return true;
}

static void skip_whitespace(Lexer* lexer) {
    for (;;) {
        char c = peek(lexer);
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance(lexer);
                break;
            case '\n':
                lexer->line++;
                lexer->column = 0;
                advance(lexer);
                break;
            case '/':
                if (peek_next(lexer) == '/') {
                    // Line comment
                    while (peek(lexer) != '\n' && !is_at_end(lexer)) {
                        advance(lexer);
                    }
                } else if (peek_next(lexer) == '*') {
                    // Block comment
                    advance(lexer);  // consume '/'
                    advance(lexer);  // consume '*'
                    while (!is_at_end(lexer)) {
                        if (peek(lexer) == '*' && peek_next(lexer) == '/') {
                            advance(lexer);
                            advance(lexer);
                            break;
                        }
                        if (peek(lexer) == '\n') {
                            lexer->line++;
                            lexer->column = 0;
                        }
                        advance(lexer);
                    }
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

static Token make_current_token(Lexer* lexer, TokenType type) {
    return make_token(type, lexer->start, 
                     (size_t)(lexer->current - lexer->start),
                     lexer->line, lexer->column);
}

static TokenType check_keyword(Lexer* lexer, int start, int length, 
                               const char* rest, TokenType type) {
    if (lexer->current - lexer->start == start + length &&
        memcmp(lexer->start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TokenType identifier_type(Lexer* lexer) {
    switch (lexer->start[0]) {
        case 'b': return check_keyword(lexer, 1, 4, "reak", TOKEN_BREAK);
        case 'c':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'a': return check_keyword(lexer, 2, 3, "tch", TOKEN_CATCH);
                    case 'l': return check_keyword(lexer, 2, 3, "ass", TOKEN_CLASS);
                    case 'o': return check_keyword(lexer, 2, 6, "ntinue", TOKEN_CONTINUE);
                }
            }
            break;
        case 'e':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'l': return check_keyword(lexer, 2, 2, "se", TOKEN_ELSE);
                    case 'x': return check_keyword(lexer, 2, 4, "port", TOKEN_EXPORT);
                }
            }
            break;
        case 'f':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'a': return check_keyword(lexer, 2, 3, "lse", TOKEN_FALSE);
                    case 'i': return check_keyword(lexer, 2, 5, "nally", TOKEN_FINALLY);
                    case 'o': return check_keyword(lexer, 2, 1, "r", TOKEN_FOR);
                    case 'u': return check_keyword(lexer, 2, 2, "nc", TOKEN_FUNC);
                }
            }
            break;
        case 'i':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'f': return check_keyword(lexer, 2, 0, "", TOKEN_IF);
                    case 'm': return check_keyword(lexer, 2, 4, "port", TOKEN_IMPORT);
                    case 'n': return check_keyword(lexer, 2, 0, "", TOKEN_IN);
                }
            }
            break;
        case 'l': return check_keyword(lexer, 1, 3, "oop", TOKEN_LOOP);
        case 'r': return check_keyword(lexer, 1, 5, "eturn", TOKEN_RETURN);
        case 't':
            if (lexer->current - lexer->start > 1) {
                switch (lexer->start[1]) {
                    case 'h': return check_keyword(lexer, 2, 2, "is", TOKEN_THIS);
                    case 'r':
                        if (lexer->current - lexer->start > 2) {
                            switch (lexer->start[2]) {
                                case 'u': return check_keyword(lexer, 3, 1, "e", TOKEN_TRUE);
                                case 'y': return check_keyword(lexer, 3, 0, "", TOKEN_TRY);
                            }
                        }
                        break;
                }
            }
            break;
        case 'w': return check_keyword(lexer, 1, 4, "hile", TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier(Lexer* lexer) {
    while (isalnum(peek(lexer)) || peek(lexer) == '_') {
        advance(lexer);
    }
    return make_current_token(lexer, identifier_type(lexer));
}

static Token number(Lexer* lexer) {
    while (isdigit(peek(lexer))) {
        advance(lexer);
    }
    
    // Look for decimal point
    if (peek(lexer) == '.' && isdigit(peek_next(lexer))) {
        advance(lexer);  // consume '.'
        while (isdigit(peek(lexer))) {
            advance(lexer);
        }
        return make_current_token(lexer, TOKEN_FLOAT);
    }
    
    return make_current_token(lexer, TOKEN_INT);
}

static Token string(Lexer* lexer, char quote) {
    while (peek(lexer) != quote && !is_at_end(lexer)) {
        if (peek(lexer) == '\n') {
            lexer->line++;
            lexer->column = 0;
        }
        if (peek(lexer) == '\\') {
            advance(lexer);  // skip escape character
        }
        advance(lexer);
    }
    
    if (is_at_end(lexer)) {
        return error_token("Unterminated string", lexer->line, lexer->column);
    }
    
    advance(lexer);  // closing quote
    return make_current_token(lexer, TOKEN_STRING);
}

Token lexer_next_token(Lexer* lexer) {
    skip_whitespace(lexer);
    
    lexer->start = lexer->current;
    
    if (is_at_end(lexer)) {
        return make_current_token(lexer, TOKEN_EOF);
    }
    
    char c = advance(lexer);
    
    // Identifiers and keywords
    if (isalpha(c) || c == '_') {
        return identifier(lexer);
    }
    
    // Numbers
    if (isdigit(c)) {
        return number(lexer);
    }
    
    switch (c) {
        // Single character tokens
        case '(': return make_current_token(lexer, TOKEN_LEFT_PAREN);
        case ')': return make_current_token(lexer, TOKEN_RIGHT_PAREN);
        case '{': return make_current_token(lexer, TOKEN_LEFT_BRACE);
        case '}': return make_current_token(lexer, TOKEN_RIGHT_BRACE);
        case '[': return make_current_token(lexer, TOKEN_LEFT_BRACKET);
        case ']': return make_current_token(lexer, TOKEN_RIGHT_BRACKET);
        case ',': return make_current_token(lexer, TOKEN_COMMA);
        case ':': return make_current_token(lexer, TOKEN_COLON);
        case '+': return make_current_token(lexer, TOKEN_PLUS);
        case '%': return make_current_token(lexer, TOKEN_PERCENT);
        case '^': return make_current_token(lexer, TOKEN_CARET);
        case '~': return make_current_token(lexer, TOKEN_TILDE);
        
        // Two character tokens
        case '-':
            return make_current_token(lexer, 
                match(lexer, '>') ? TOKEN_ARROW : TOKEN_MINUS);
        
        case '*': return make_current_token(lexer, TOKEN_STAR);
        case '/': return make_current_token(lexer, TOKEN_SLASH);
        
        case '!':
            return make_current_token(lexer,
                match(lexer, '=') ? TOKEN_NOT_EQUAL : TOKEN_NOT);
        
        case '=':
            return make_current_token(lexer,
                match(lexer, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        
        case '<':
            return make_current_token(lexer,
                match(lexer, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        
        case '>':
            return make_current_token(lexer,
                match(lexer, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        
        case '&':
            return make_current_token(lexer,
                match(lexer, '&') ? TOKEN_AND : TOKEN_AMPERSAND);
        
        case '|':
            return make_current_token(lexer,
                match(lexer, '|') ? TOKEN_OR : TOKEN_PIPE);
        
        case '.':
            if (match(lexer, '.')) {
                return make_current_token(lexer,
                    match(lexer, '=') ? TOKEN_DOT_DOT_EQUAL : TOKEN_DOT_DOT);
            }
            return make_current_token(lexer, TOKEN_DOT);
        
        // Strings
        case '"':
        case '\'':
            return string(lexer, c);
    }
    
    return error_token("Unexpected character", lexer->line, lexer->column);
}
