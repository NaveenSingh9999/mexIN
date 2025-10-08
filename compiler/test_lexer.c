/* LAMC Compiler - Lexer Test Program */

#include <stdio.h>
#include <stdlib.h>
#include "lexer/lexer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source-file>\n", argv[0]);
        return 1;
    }
    
    // Read source file
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", argv[1]);
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    
    char* source = (char*)malloc(file_size + 1);
    if (!source) {
        fprintf(stderr, "Error: Could not allocate memory\n");
        fclose(file);
        return 1;
    }
    
    fread(source, 1, file_size, file);
    source[file_size] = '\0';
    fclose(file);
    
    // Initialize lexer
    Lexer lexer;
    lexer_init(&lexer, source);
    
    // Tokenize and print
    printf("Tokenizing: %s\n", argv[1]);
    printf("%-15s %-20s %s\n", "TYPE", "LEXEME", "POSITION");
    printf("-----------------------------------------------------------\n");
    
    Token token;
    do {
        token = lexer_next_token(&lexer);
        
        printf("%-15s ", token_type_to_string(token.type));
        printf("'%.*s'", (int)token.length, token.start);
        printf("%*s", (int)(20 - token.length), "");
        printf(" (line %d, col %d)\n", token.line, token.column);
        
    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);
    
    free(source);
    return 0;
}
