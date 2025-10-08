# LAMC Compiler

This is the LAMC compiler implementation, written in **C** for independence and maximum performance.

## Structure

```
compiler/
├── lexer/          # Lexical analyzer (tokenizer)
│   ├── token.h     # Token definitions
│   ├── token.c     # Token implementation
│   ├── lexer.h     # Lexer interface
│   └── lexer.c     # Lexer implementation
├── parser/         # (Coming soon) Syntax analyzer
├── semantic/       # (Coming soon) Semantic analyzer
├── codegen/        # (Coming soon) Code generator
├── test_lexer.c    # Lexer test program
├── Makefile        # Build system
└── bin/            # Compiled binaries
```

## Building

```bash
cd compiler
make
```

This creates `bin/test_lexer` which can tokenize LAMC source files.

## Testing

Test the lexer on example files:

```bash
./bin/test_lexer ../examples/hello.lamc
./bin/test_lexer ../examples/fibonacci.lamc
```

## Current Status

✅ **Phase 1: Lexer** - Complete!
- Tokenizes LAMC source code
- Recognizes keywords, operators, literals
- Handles comments (// and /* */)
- Tracks line and column numbers

⏳ **Phase 2: Parser** - Coming next
- Build Abstract Syntax Tree (AST)
- Syntax validation

⏳ **Phase 3: Semantic Analysis** - Planned
- Type checking
- Symbol resolution

⏳ **Phase 4: Code Generation** - Planned
- Generate machine code
- Optimization passes

## Language Features Supported

The lexer currently recognizes:

### Keywords
`func`, `return`, `if`, `else`, `while`, `for`, `in`, `loop`, `break`, `continue`, `import`, `export`, `class`, `this`, `try`, `catch`, `finally`, `true`, `false`

### Operators
`+`, `-`, `*`, `/`, `%`, `=`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `&&`, `||`, `!`, `&`, `|`, `^`, `~`, `..`, `..=`

### Delimiters
`(`, `)`, `{`, `}`, `[`, `]`, `,`, `:`, `.`, `->`

### Literals
- Integers: `42`, `0`, `1000`
- Floats: `3.14`, `0.5`, `2.0`
- Strings: `"hello"`, `'world'`
- Booleans: `true`, `false`

## Clean Build

```bash
make clean
```

## Next Steps

1. Implement parser to build AST
2. Add semantic analysis
3. Implement code generation for x86-64
4. Add optimization passes
5. Create standard library

---

**Written in C** - No dependencies, completely independent!
