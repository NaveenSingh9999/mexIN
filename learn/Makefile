# LAMC Learning Demos - Makefile
# Builds educational demonstration programs

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -O2
COMPILER_DIR = ../compiler
LEXER_DIR = $(COMPILER_DIR)/lexer
PARSER_DIR = $(COMPILER_DIR)/parser
OUTDIR = bin

# Source files from compiler
LEXER_SRCS = $(LEXER_DIR)/token.c $(LEXER_DIR)/lexer.c
PARSER_SRCS = $(PARSER_DIR)/ast.c $(PARSER_DIR)/ast_print.c

# Demo source files
LEXER_DEMO_SRC = 01_lexer_demo.c
AST_DEMO_SRC = 02_ast_demo.c

# Object files
LEXER_OBJS = $(LEXER_SRCS:.c=.o)
PARSER_OBJS = $(PARSER_SRCS:.c=.o)

# Targets
all: lexer_demo ast_demo

lexer_demo: $(LEXER_OBJS) $(LEXER_DEMO_SRC)
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$@ $(LEXER_DEMO_SRC) $(LEXER_OBJS)
	@echo "✓ Built lexer_demo -> $(OUTDIR)/lexer_demo"

ast_demo: $(LEXER_OBJS) $(PARSER_OBJS) $(AST_DEMO_SRC)
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$@ $(AST_DEMO_SRC) $(LEXER_OBJS) $(PARSER_OBJS)
	@echo "✓ Built ast_demo -> $(OUTDIR)/ast_demo"

clean:
	rm -rf $(OUTDIR)
	@echo "✓ Cleaned demo build files"

run-lexer: lexer_demo
	@echo ""
	@echo "Running Lexer Demo..."
	@echo ""
	./$(OUTDIR)/lexer_demo examples/01_lexer_examples.lamc

run-ast: ast_demo
	@echo ""
	@echo "Running AST Demo..."
	@echo ""
	./$(OUTDIR)/ast_demo

help:
	@echo "LAMC Learning Demos - Available targets:"
	@echo "  make all         - Build all demos"
	@echo "  make lexer_demo  - Build Phase 1 (Lexer) demo"
	@echo "  make ast_demo    - Build Phase 2 (AST) demo"
	@echo "  make run-lexer   - Run the lexer demo"
	@echo "  make run-ast     - Run the AST demo"
	@echo "  make clean       - Clean build files"

.PHONY: all clean run-lexer run-ast help
