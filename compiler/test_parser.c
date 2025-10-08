/* LAMC Compiler - Parser Test Program
 * Tests parser with real LAMC code
 * Copyright (c) 2025 Naveen Singh
 */

#include <stdio.h>
#include <stdlib.h>
#include "parser/parser.h"
#include "lexer/lexer.h"

int main(int argc, char* argv[]) {
    const char* source;
    bool from_file = false;
    
    if (argc > 1) {
        /* Read from file */
        FILE* file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Error: Could not open file '%s'\n", argv[1]);
            return 1;
        }
        
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);
        
        char* file_source = (char*)malloc(file_size + 1);
        fread(file_source, 1, file_size, file);
        file_source[file_size] = '\0';
        fclose(file);
        
        source = file_source;
        from_file = true;
        
        printf("Parsing: %s\n\n", argv[1]);
    } else {
        /* Use inline test */
        source = 
            "x = 42\n"
            "y = (2 + 3) * 4\n"
            "print(\"Hello\")\n"
            "result = add(10, 20)\n";
        
        printf("Parsing inline test code:\n");
        printf("---\n%s---\n\n", source);
    }
    
    /* Initialize lexer */
    Lexer lexer;
    lexer_init(&lexer, source);
    
    /* Initialize parser */
    Parser parser;
    parser_init(&parser, &lexer);
    
    /* Parse */
    printf("Parsing...\n\n");
    AstNode* program = parser_parse(&parser);
    
    if (program && !parser.had_error) {
        printf("✓ Parsing successful!\n\n");
        ast_print_program(program);
        ast_free_node(program);
        
        if (from_file) free((void*)source);
        return 0;
    } else {
        printf("✗ Parsing failed with errors.\n");
        if (from_file) free((void*)source);
        return 1;
    }
}
