# LAMC Learning Resources

This folder contains educational scripts and examples to help you understand how the LAMC compiler works internally.

## Contents

### Phase 1: Lexer (Tokenization)
- **`01_lexer_demo.c`** - Interactive lexer demonstration
- **`01_lexer_examples.lamc`** - Example LAMC code for lexer testing

### Phase 2: Parser (AST)
- **`02_ast_demo.c`** - Interactive AST building demonstration
- **`02_ast_examples.lamc`** - Example LAMC code showing AST structure

## How to Use

### Build the demos:
```bash
cd /workspaces/mexIN/learn
make
```

### Run the lexer demo:
```bash
./bin/lexer_demo examples/01_lexer_examples.lamc
```

### Run the AST demo:
```bash
./bin/ast_demo
```

## What You'll Learn

### Phase 1 - Lexer
- How source code is broken into tokens
- Understanding keywords, operators, and literals
- How comments are handled
- Line and column tracking

### Phase 2 - AST
- How tokens become a tree structure
- Understanding expressions vs statements
- How function declarations work
- Memory management in the compiler

## Educational Value

Each demo includes:
- ✅ Detailed comments explaining each step
- ✅ Visual output showing the process
- ✅ Real examples you can modify
- ✅ Production-quality code to learn from

---

**Start with Phase 1, then move to Phase 2!**
