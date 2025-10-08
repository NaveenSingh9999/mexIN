# LAMC Compiler Makefile
# Builds the LAMC compiler from C source

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -O2
SRCDIR = .
LEXERDIR = lexer
PARSERDIR = parser
OUTDIR = bin

# Source files
LEXER_SRCS = $(LEXERDIR)/token.c $(LEXERDIR)/lexer.c
PARSER_SRCS = $(PARSERDIR)/ast.c $(PARSERDIR)/ast_print.c $(PARSERDIR)/parser.c
TEST_LEXER_SRCS = test_lexer.c

# Object files
LEXER_OBJS = $(LEXER_SRCS:.c=.o)
PARSER_OBJS = $(PARSER_SRCS:.c=.o)
TEST_LEXER_OBJS = $(TEST_LEXER_SRCS:.c=.o)

# Targets
all: test_lexer test_ast test_parser

test_lexer: $(LEXER_OBJS) $(TEST_LEXER_OBJS)
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$@ $^
	@echo "✓ Built test_lexer -> $(OUTDIR)/test_lexer"

test_ast: $(LEXER_OBJS) $(PARSER_OBJS) test_ast.o
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$@ $^
	@echo "✓ Built test_ast -> $(OUTDIR)/test_ast"

test_parser: $(LEXER_OBJS) $(PARSER_OBJS) test_parser.o
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$@ $^
	@echo "✓ Built test_parser -> $(OUTDIR)/test_parser"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(LEXER_OBJS) $(PARSER_OBJS) $(TEST_LEXER_OBJS) test_ast.o test_parser.o
	rm -rf $(OUTDIR)
	@echo "✓ Cleaned build files"

.PHONY: all clean
