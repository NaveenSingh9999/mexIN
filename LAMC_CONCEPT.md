# LAMC - Low-level Advanced Machine Code

## Overview
LAMC is a high-performance, low-level programming language designed to be:
- **Fast**: Compiles directly to machine code with minimal overhead
- **Low-level**: Provides direct hardware access and memory control
- **Easy to learn**: Simple, intuitive syntax despite being low-level

## Language Philosophy

### Core Principles
1. **Zero-cost abstractions**: High-level features that compile to optimal machine code
2. **Explicit is better than implicit**: Clear syntax that shows what's happening
3. **Memory safety with control**: Safe by default, unsafe when needed
4. **Minimal runtime**: Little to no runtime overhead
5. **Direct hardware access**: Full control over system resources

## Language Features

### 1. Type System
- **Static typing**: Types checked at compile time
- **Type inference**: Compiler infers types when obvious
- **Primitive types**:
  - Integers: `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`
  - Floats: `f32`, `f64`
  - Boolean: `bool`
  - Character: `char`
  - Pointer: `ptr<T>`
  - Array: `[T; size]`

### 2. Memory Management
- **Manual memory control**: Explicit allocation and deallocation
- **Stack allocation**: Default for local variables
- **Heap allocation**: Using `alloc` and `free` keywords
- **No garbage collection**: Predictable performance
- **Ownership tracking**: Optional compile-time checks

### 3. Syntax Design (Simplified - Easier than Python!)

#### Variable Declaration
```lamc
// Super simple - no type annotations needed!
x = 42              // automatic type detection
name = "LAMC"       // strings are easy
pi = 3.14          // floats too

// Want to change it later? Just reassign!
x = 100            // variables are mutable by default

// Optional: specify type only if you want
age: int = 25
```

#### Functions
```lamc
// Simple function syntax - no return type needed
func add(a, b)
    return a + b

// Even simpler - last expression returns automatically
func multiply(a, b)
    a * b

// Entry point - cleaner than Python!
func main()
    result = add(5, 3)
    print("Result: " + result)

// No need to return 0, it's automatic!
```

#### Control Flow
```lamc
// If statements - clean and simple
if x > 10
    print("Big number")
else if x > 5
    print("Medium number")
else
    print("Small number")

// One-liners work too!
if x > 10: print("Big!")

// Loops - cleaner than Python
loop                    // infinite loop
    if done: break

while condition
    print("Running...")

// Range loops - super easy
for i in 0..10         // 0 to 9
    print(i)

for i in 1..=100       // 1 to 100 (inclusive)
    print(i)

// Iterate over arrays
for item in my_array
    print(item)
```

#### Arrays and Collections
```lamc
// Arrays - super simple!
numbers = [1, 2, 3, 4, 5]
names = ["Alice", "Bob", "Charlie"]

// Access elements
first = numbers[0]
last = numbers[-1]        // negative indexing like Python!

// Array methods
numbers.append(6)         // add element
length = numbers.length()
numbers.sort()

// Slicing made easy
subset = numbers[1:4]     // elements 1 to 3
```

#### Memory Operations (Optional - For Advanced Users)
```lamc
// Beginners don't need to worry about memory!
// It's automatic by default

// But if you want low-level control:
func advanced_memory()
    // Manual allocation (only when you need it)
    ptr = alloc(1024)     // allocate 1024 bytes
    ptr[0] = 42
    free(ptr)             // manual cleanup for max performance
    
    // Or just let LAMC handle it automatically!
```

### 4. Compilation Model

#### Compilation Pipeline
1. **Lexical Analysis**: Source code → Tokens
2. **Parsing**: Tokens → Abstract Syntax Tree (AST)
3. **Semantic Analysis**: Type checking, symbol resolution
4. **Optimization**: AST → Optimized Intermediate Representation (IR)
5. **Code Generation**: IR → Machine Code (x86-64, ARM, etc.)
6. **Linking**: Machine code → Executable

#### Optimization Levels
- `-O0`: No optimization (fast compilation)
- `-O1`: Basic optimization
- `-O2`: Aggressive optimization (default)
- `-O3`: Maximum optimization
- `-Os`: Size optimization

### 5. Standard Library (Minimal)

#### Core Modules
- `io`: Input/output operations
- `mem`: Memory operations
- `math`: Mathematical functions
- `sys`: System calls and OS interface
- `str`: String operations

#### Example Usage
```lamc
import io
import file

func main()
    // Super simple I/O
    print("Hello, LAMC!")
    name = input("What's your name? ")
    print("Hi, " + name + "!")
    
    // Easy file operations
    content = file.read("data.txt")
    file.write("output.txt", content)
    
    // No need to return - it's automatic!
```

## Technical Specifications

### Compiler Architecture
- **Frontend**: Lexer, Parser, Semantic Analyzer
- **Middle-end**: IR Generator, Optimizer
- **Backend**: Code Generator (LLVM or custom)

### Target Platforms
- Linux (x86-64, ARM64)
- Windows (x86-64)
- MacOS (x86-64, ARM64)
- Embedded systems (ARM Cortex-M)

### Performance Goals
- Compilation speed: < 1s for 10k lines of code
- Binary size: Minimal (< 50KB for "Hello World")
- Runtime performance: Within 5% of hand-written assembly

## Development Roadmap

### Phase 1: Foundation (Current)
- [x] Language concept and design
- [ ] Formal grammar specification
- [ ] Lexer implementation
- [ ] Parser implementation
- [ ] AST design

### Phase 2: Core Compiler
- [ ] Type system implementation
- [ ] Semantic analyzer
- [ ] Symbol table management
- [ ] Error reporting system

### Phase 3: Code Generation
- [ ] IR design and implementation
- [ ] Basic code generator (x86-64)
- [ ] Linker integration
- [ ] Basic optimization passes

### Phase 4: Standard Library
- [ ] Core I/O functions
- [ ] Memory management utilities
- [ ] Basic data structures
- [ ] System interface

### Phase 5: Advanced Features
- [ ] Inline assembly support
- [ ] SIMD intrinsics
- [ ] Cross-compilation
- [ ] Debugger integration

## Example Programs

### Hello World (The Simplest!)
```lamc
func main()
    print("Hello, World!")
```

### Fibonacci (Clean and Simple)
```lamc
func fibonacci(n)
    if n <= 1
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)

func main()
    result = fibonacci(10)
    print("Fibonacci(10) = " + result)
```

### File Processing (Super Easy!)
```lamc
func main()
    // Read file
    lines = file.read_lines("data.txt")
    
    // Process each line
    total = 0
    for line in lines
        number = int(line)
        total = total + number
    
    // Write result
    print("Total: " + total)
    file.write("result.txt", total)
```

### Web Request (Built-in!)
```lamc
import http

func main()
    // Make HTTP request - it's this easy!
    response = http.get("https://api.example.com/data")
    data = response.json()
    
    print("Status: " + response.status)
    print("Data: " + data)
```

## Why LAMC?

### Compared to Python
- **Easier**: Even simpler syntax, no colons after function definitions
- **Faster**: 100-1000x faster execution (compiled to machine code)
- **No indentation errors**: Flexible whitespace (indentation recommended but optional)
- **Batteries included**: Built-in performance without external libraries

### Compared to C
- **Way easier**: No pointers to learn (unless you want them)
- **Automatic memory**: No manual malloc/free (unless you want it)
- **Modern**: Clean syntax, no header files
- **Same speed**: Compiles to identical machine code

### Compared to Assembly
- **Portable**: Cross-platform code generation
- **Readable**: High-level constructs
- **Maintainable**: Structured programming

## Getting Started (Future)

### Installation
```bash
curl -sSf https://lamc-lang.org/install.sh | sh
```

### Your First Program
```bash
# Create a new file
echo 'fn main() -> i32 { return 0; }' > hello.lamc

# Compile
lamc compile hello.lamc -o hello

# Run
./hello
```

## Community and Contributing

LAMC is open-source and welcomes contributions!

- GitHub: github.com/[your-org]/lamc
- Documentation: lamc-lang.org/docs
- Discord: discord.gg/lamc
- License: MIT

---

**Version**: 0.1.0-alpha  
**Last Updated**: October 8, 2025  
**Status**: In Development
