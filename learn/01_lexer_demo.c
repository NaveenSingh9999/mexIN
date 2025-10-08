/* ============================================
 * LAMC Compiler - Phase 1: Lexer Demo
 * Educational demonstration of tokenization
 * ============================================
 * 
 * This program shows how the LAMC lexer works:
 * 1. Reads source code character by character
 * 2. Identifies tokens (keywords, operators, etc.)
 * 3. Tracks position for error reporting
 * 
 * Author: Naveen Singh
 * Date: October 8, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../compiler/lexer/lexer.h"
#include "../compiler/lexer/token.h"

/* ANSI color codes for pretty output */
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"

/* Print a section header */
void print_header(const char* title) {
    printf("\n");
    printf(COLOR_BOLD COLOR_CYAN);
    printf("========================================\n");
    printf("  %s\n", title);
    printf("========================================\n");
    printf(COLOR_RESET);
}

/* Get color for token type */
const char* get_token_color(TokenType type) {
    switch (type) {
        case TOKEN_FUNC:
        case TOKEN_RETURN:
        case TOKEN_IF:
        case TOKEN_ELSE:
        case TOKEN_WHILE:
        case TOKEN_FOR:
        case TOKEN_LOOP:
            return COLOR_MAGENTA;  /* Keywords */
        
        case TOKEN_INT:
        case TOKEN_FLOAT:
            return COLOR_YELLOW;   /* Numbers */
        
        case TOKEN_STRING:
            return COLOR_GREEN;    /* Strings */
        
        case TOKEN_IDENTIFIER:
            return COLOR_CYAN;     /* Identifiers */
        
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_STAR:
        case TOKEN_SLASH:
        case TOKEN_EQUAL:
        case TOKEN_EQUAL_EQUAL:
        case TOKEN_NOT_EQUAL:
        case TOKEN_LESS:
        case TOKEN_GREATER:
            return COLOR_RED;      /* Operators */
        
        default:
            return COLOR_RESET;
    }
}

/* Demonstrate lexer on a code snippet */
void demonstrate_lexer(const char* code_snippet, const char* description) {
    print_header(description);
    
    printf(COLOR_BOLD "Source Code:" COLOR_RESET "\n");
    printf("  %s\n\n", code_snippet);
    
    printf(COLOR_BOLD "Tokens Generated:" COLOR_RESET "\n");
    printf("  %-15s %-20s %-10s\n", "TYPE", "LEXEME", "POSITION");
    printf("  %s\n", "-------------------------------------------------------");
    
    /* Initialize lexer */
    Lexer lexer;
    lexer_init(&lexer, code_snippet);
    
    /* Tokenize */
    Token token;
    int token_count = 0;
    do {
        token = lexer_next_token(&lexer);
        
        if (token.type != TOKEN_EOF) {
            const char* color = get_token_color(token.type);
            printf("  %s%-15s" COLOR_RESET " ", color, token_type_to_string(token.type));
            printf("'%.*s'", (int)token.length, token.start);
            
            /* Pad for alignment */
            int padding = 20 - (int)token.length - 2;
            for (int i = 0; i < padding; i++) printf(" ");
            
            printf("(%d:%d)\n", token.line, token.column);
            token_count++;
        }
        
        if (token.type == TOKEN_ERROR) {
            printf(COLOR_RED "  Error: %.*s" COLOR_RESET "\n", 
                   (int)token.length, token.start);
            break;
        }
        
    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);
    
    printf("\n");
    printf(COLOR_GREEN "  ✓ Generated %d tokens" COLOR_RESET "\n", token_count);
}

/* Interactive mode - tokenize a file */
void tokenize_file(const char* filename) {
    print_header("Tokenizing LAMC File");
    
    printf("File: %s\n\n", filename);
    
    /* Read file */
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf(COLOR_RED "Error: Could not open file '%s'\n" COLOR_RESET, filename);
        return;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    
    char* source = (char*)malloc(file_size + 1);
    if (!source) {
        printf(COLOR_RED "Error: Memory allocation failed\n" COLOR_RESET);
        fclose(file);
        return;
    }
    
    fread(source, 1, file_size, file);
    source[file_size] = '\0';
    fclose(file);
    
    /* Show source code */
    printf(COLOR_BOLD "Source Code:" COLOR_RESET "\n");
    printf("---\n%s\n---\n\n", source);
    
    /* Tokenize */
    Lexer lexer;
    lexer_init(&lexer, source);
    
    printf(COLOR_BOLD "Token Stream:" COLOR_RESET "\n");
    printf("%-5s %-15s %-25s %-10s\n", "#", "TYPE", "LEXEME", "POSITION");
    printf("----------------------------------------------------------------\n");
    
    Token token;
    int count = 0;
    do {
        token = lexer_next_token(&lexer);
        
        if (token.type != TOKEN_EOF) {
            const char* color = get_token_color(token.type);
            
            printf("%-5d ", ++count);
            printf("%s%-15s" COLOR_RESET " ", color, token_type_to_string(token.type));
            
            /* Print lexeme (truncate if too long) */
            if (token.length > 20) {
                printf("'%.*s...'", 17, token.start);
                printf("%*s", 25 - 20, "");
            } else {
                printf("'%.*s'", (int)token.length, token.start);
                printf("%*s", (int)(25 - token.length - 2), "");
            }
            
            printf("(%d:%d)\n", token.line, token.column);
        }
        
    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);
    
    printf("\n");
    printf(COLOR_GREEN "✓ Successfully tokenized %d tokens from %s\n" COLOR_RESET, 
           count, filename);
    
    free(source);
}

int main(int argc, char* argv[]) {
    printf(COLOR_BOLD COLOR_BLUE);
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   LAMC Compiler - Lexer Demo          ║\n");
    printf("║   Phase 1: Tokenization                ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(COLOR_RESET);
    
    /* Demo 1: Simple variable */
    demonstrate_lexer(
        "x = 42",
        "Example 1: Variable Assignment"
    );
    
    /* Demo 2: Function call */
    demonstrate_lexer(
        "print(\"Hello, LAMC!\")",
        "Example 2: Function Call"
    );
    
    /* Demo 3: Arithmetic expression */
    demonstrate_lexer(
        "(5 + 3) * 2 - 1",
        "Example 3: Arithmetic Expression"
    );
    
    /* Demo 4: Control flow */
    demonstrate_lexer(
        "if x > 10\n    print(\"big\")",
        "Example 4: If Statement"
    );
    
    /* Demo 5: Function definition */
    demonstrate_lexer(
        "func add(a, b)\n    return a + b",
        "Example 5: Function Definition"
    );
    
    /* Demo 6: Array and operators */
    demonstrate_lexer(
        "arr = [1, 2, 3]\nresult = arr[0] == 1",
        "Example 6: Arrays and Comparison"
    );
    
    /* If file provided, tokenize it */
    if (argc > 1) {
        tokenize_file(argv[1]);
    }
    
    /* Summary */
    print_header("What You Learned");
    printf("  • The lexer breaks source code into " COLOR_BOLD "tokens" COLOR_RESET "\n");
    printf("  • Tokens include: " COLOR_MAGENTA "keywords" COLOR_RESET ", ");
    printf(COLOR_CYAN "identifiers" COLOR_RESET ", ");
    printf(COLOR_YELLOW "numbers" COLOR_RESET ", ");
    printf(COLOR_GREEN "strings" COLOR_RESET ", ");
    printf(COLOR_RED "operators" COLOR_RESET "\n");
    printf("  • Position tracking helps with " COLOR_BOLD "error messages" COLOR_RESET "\n");
    printf("  • This is the " COLOR_BOLD "first phase" COLOR_RESET " of compilation\n");
    printf("\n");
    
    printf(COLOR_GREEN COLOR_BOLD "✓ Phase 1 (Lexer) is complete and production-ready!\n" COLOR_RESET);
    printf("  Next: Phase 2 (Parser) - Building the Abstract Syntax Tree\n\n");
    
    return 0;
}
