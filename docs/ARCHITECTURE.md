# LAMC Compiler Architecture

## Overview

The LAMC compiler is designed as a multi-pass compiler with clear separation of concerns. Each phase transforms the input into a more refined representation until machine code is generated.

## Compilation Pipeline

```
Source Code (.lamc)
    ↓
[Lexical Analysis]
    ↓
Token Stream
    ↓
[Syntax Analysis]
    ↓
Abstract Syntax Tree (AST)
    ↓
[Semantic Analysis]
    ↓
Annotated AST
    ↓
[IR Generation]
    ↓
Intermediate Representation (IR)
    ↓
[Optimization]
    ↓
Optimized IR
    ↓
[Code Generation]
    ↓
Assembly/Machine Code
    ↓
[Linking]
    ↓
Executable Binary
```

## Phase 1: Lexical Analysis (Lexer)

### Purpose
Convert source code text into a stream of tokens.

### Responsibilities
- Read source file character by character
- Identify keywords, identifiers, literals, operators
- Skip whitespace and comments
- Report lexical errors (invalid characters, malformed literals)
- Track line and column numbers for error reporting

### Token Types
```rust
enum TokenType {
    // Keywords
    Let, Mut, Fn, Return, If, Else, While, Loop, For,
    Struct, Enum, Import, Unsafe, Const,
    
    // Literals
    IntLiteral, FloatLiteral, StringLiteral, CharLiteral, BoolLiteral,
    
    // Identifiers
    Identifier,
    
    // Operators
    Plus, Minus, Star, Slash, Percent,
    Equal, EqualEqual, NotEqual, Less, Greater, LessEqual, GreaterEqual,
    And, Or, Not, BitwiseAnd, BitwiseOr, BitwiseXor,
    
    // Delimiters
    LeftParen, RightParen, LeftBrace, RightBrace, LeftBracket, RightBracket,
    Comma, Semicolon, Colon, Arrow,
    
    // Special
    Eof, Error
}
```

### Data Structure
```rust
struct Token {
    type: TokenType,
    lexeme: String,
    line: u32,
    column: u32,
}
```

## Phase 2: Syntax Analysis (Parser)

### Purpose
Build an Abstract Syntax Tree (AST) from the token stream.

### Parsing Strategy
- **Recursive Descent Parser**: Hand-written, predictable, easy to debug
- **Operator Precedence**: Pratt parsing for expressions
- **Error Recovery**: Synchronization points for multiple error reporting

### AST Node Types
```rust
enum AstNode {
    Program(Vec<Declaration>),
    
    // Declarations
    FunctionDecl { name: String, params: Vec<Param>, return_type: Type, body: Block },
    StructDecl { name: String, fields: Vec<Field> },
    VarDecl { name: String, mutable: bool, type_: Option<Type>, init: Expr },
    
    // Statements
    ExprStmt(Expr),
    IfStmt { condition: Expr, then_block: Block, else_block: Option<Block> },
    WhileStmt { condition: Expr, body: Block },
    LoopStmt { body: Block },
    ReturnStmt { value: Option<Expr> },
    Block { statements: Vec<Statement> },
    
    // Expressions
    BinaryExpr { left: Box<Expr>, op: Operator, right: Box<Expr> },
    UnaryExpr { op: Operator, operand: Box<Expr> },
    CallExpr { callee: Box<Expr>, args: Vec<Expr> },
    IndexExpr { array: Box<Expr>, index: Box<Expr> },
    FieldExpr { object: Box<Expr>, field: String },
    LiteralExpr { value: Literal },
    IdentifierExpr { name: String },
}
```

### Parser Structure
```rust
struct Parser {
    tokens: Vec<Token>,
    current: usize,
}

impl Parser {
    fn parse_program() -> Program;
    fn parse_declaration() -> Declaration;
    fn parse_statement() -> Statement;
    fn parse_expression() -> Expression;
    fn parse_primary() -> Expression;
}
```

## Phase 3: Semantic Analysis

### Purpose
Validate the AST for semantic correctness.

### Responsibilities
1. **Symbol Table Management**
   - Track variable, function, and type declarations
   - Handle scoping rules
   - Detect redeclarations

2. **Type Checking**
   - Verify type compatibility in assignments
   - Check function call arguments
   - Resolve type inference
   - Validate operator usage

3. **Name Resolution**
   - Resolve variable and function references
   - Check for undefined symbols
   - Validate import statements

4. **Control Flow Analysis**
   - Verify all paths return values
   - Detect unreachable code
   - Validate break/continue usage

### Symbol Table
```rust
struct SymbolTable {
    scopes: Vec<HashMap<String, Symbol>>,
}

struct Symbol {
    name: String,
    type_: Type,
    mutable: bool,
    scope_level: u32,
}
```

### Type System
```rust
enum Type {
    Primitive(PrimitiveType),
    Pointer(Box<Type>),
    Array(Box<Type>, usize),
    Struct(String),
    Function(Vec<Type>, Box<Type>),
    Void,
    Unknown,
}

enum PrimitiveType {
    I8, I16, I32, I64,
    U8, U16, U32, U64,
    F32, F64,
    Bool, Char,
}
```

## Phase 4: Intermediate Representation (IR)

### Purpose
Transform the AST into a lower-level, platform-independent representation.

### IR Design
- **Three-Address Code (TAC)**: Simple instruction format
- **Static Single Assignment (SSA)**: Each variable assigned once
- **Control Flow Graph (CFG)**: Explicit control flow

### IR Instructions
```rust
enum IrInstruction {
    // Arithmetic
    Add { dest: Register, left: Operand, right: Operand },
    Sub { dest: Register, left: Operand, right: Operand },
    Mul { dest: Register, left: Operand, right: Operand },
    Div { dest: Register, left: Operand, right: Operand },
    
    // Memory
    Load { dest: Register, addr: Operand },
    Store { addr: Operand, src: Register },
    Alloc { dest: Register, size: usize },
    Free { addr: Operand },
    
    // Control Flow
    Jump { target: Label },
    JumpIf { condition: Register, target: Label },
    Call { function: String, args: Vec<Operand>, dest: Option<Register> },
    Return { value: Option<Operand> },
    
    // Comparison
    Compare { dest: Register, left: Operand, right: Operand, op: CompareOp },
}
```

### Basic Block
```rust
struct BasicBlock {
    label: Label,
    instructions: Vec<IrInstruction>,
    successors: Vec<Label>,
}
```

## Phase 5: Optimization

### Optimization Levels
- **-O0**: No optimization (fast compilation, debugging)
- **-O1**: Basic optimizations (dead code elimination, constant folding)
- **-O2**: Aggressive optimizations (inlining, loop unrolling)
- **-O3**: Maximum optimizations (may increase code size)

### Optimization Passes

#### Local Optimizations (Within Basic Blocks)
1. **Constant Folding**: Evaluate constant expressions at compile time
2. **Constant Propagation**: Replace variables with known constant values
3. **Dead Code Elimination**: Remove unreachable code
4. **Algebraic Simplification**: Simplify expressions (x * 1 = x)

#### Global Optimizations (Across Basic Blocks)
1. **Common Subexpression Elimination (CSE)**: Reuse computed values
2. **Loop Invariant Code Motion**: Move loop-invariant code outside loops
3. **Function Inlining**: Replace function calls with function body
4. **Register Allocation**: Efficiently map variables to registers

#### Advanced Optimizations
1. **Loop Unrolling**: Reduce loop overhead
2. **Vectorization**: Use SIMD instructions
3. **Tail Call Optimization**: Convert tail recursion to iteration

### Optimization Pipeline
```rust
struct Optimizer {
    ir: Vec<BasicBlock>,
}

impl Optimizer {
    fn optimize(&mut self, level: OptLevel) {
        match level {
            OptLevel::O0 => {},
            OptLevel::O1 => {
                self.constant_folding();
                self.dead_code_elimination();
            },
            OptLevel::O2 => {
                self.constant_folding();
                self.constant_propagation();
                self.cse();
                self.dead_code_elimination();
                self.inline_functions();
            },
            OptLevel::O3 => {
                // All O2 optimizations plus:
                self.loop_unrolling();
                self.vectorization();
            }
        }
    }
}
```

## Phase 6: Code Generation

### Purpose
Convert optimized IR to target machine code.

### Target Architecture Support
- **Primary**: x86-64
- **Future**: ARM64, RISC-V

### Code Generation Strategy
1. **Instruction Selection**: Map IR to machine instructions
2. **Register Allocation**: Assign variables to physical registers
3. **Instruction Scheduling**: Reorder instructions for performance
4. **Peephole Optimization**: Local assembly-level optimizations

### Register Allocation
- **Algorithm**: Linear Scan or Graph Coloring
- **Spilling**: Handle cases with insufficient registers
- **Calling Convention**: Follow platform ABI (System V, Windows x64)

### Assembly Generation
```rust
struct CodeGenerator {
    target: Target,
    register_allocator: RegisterAllocator,
}

impl CodeGenerator {
    fn generate(&self, ir: &[BasicBlock]) -> String {
        // Generate assembly code
        let mut asm = String::new();
        
        for block in ir {
            asm.push_str(&format!("{}:\n", block.label));
            for instr in &block.instructions {
                asm.push_str(&self.emit_instruction(instr));
            }
        }
        
        asm
    }
    
    fn emit_instruction(&self, instr: &IrInstruction) -> String {
        match instr {
            IrInstruction::Add { dest, left, right } => {
                format!("  add {}, {}\n", self.reg(*dest), self.operand(right))
            },
            // ... other instructions
        }
    }
}
```

## Phase 7: Linking

### Purpose
Combine compiled object files into an executable.

### Linker Integration
- **Static Linking**: Include all dependencies in binary
- **Dynamic Linking**: Link against shared libraries at runtime

### Linker Invocation
```bash
# For Linux
gcc -nostdlib -o program program.o -lc

# For Windows
link /OUT:program.exe program.obj /SUBSYSTEM:CONSOLE
```

## Error Handling

### Error Types
1. **Lexical Errors**: Invalid tokens
2. **Syntax Errors**: Malformed syntax
3. **Semantic Errors**: Type mismatches, undefined symbols
4. **Link Errors**: Unresolved symbols

### Error Reporting
```rust
struct CompilerError {
    message: String,
    file: String,
    line: u32,
    column: u32,
    severity: Severity,
}

enum Severity {
    Error,
    Warning,
    Info,
}
```

### User-Friendly Messages
```
Error: Type mismatch in assignment
  --> example.lamc:10:5
   |
10 |     let x: i32 = "hello";
   |                  ^^^^^^^ expected 'i32', found 'string'
```

## Testing Strategy

### Unit Tests
- Test each compiler phase independently
- Mock dependencies for isolation

### Integration Tests
- End-to-end compilation tests
- Verify generated code correctness

### Benchmark Tests
- Measure compilation speed
- Compare generated code performance

## Future Enhancements

1. **JIT Compilation**: Runtime code generation
2. **LLVM Backend**: Leverage LLVM infrastructure
3. **Incremental Compilation**: Only recompile changed modules
4. **Parallel Compilation**: Multi-threaded compilation
5. **LSP Server**: IDE integration with Language Server Protocol

---

**Document Version**: 1.0  
**Last Updated**: October 8, 2025
