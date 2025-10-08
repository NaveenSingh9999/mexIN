# LAMC Development Schedule - Detailed Work Breakdown

## 🗓️ Week-by-Week Schedule

---

## WEEK 1: Parser Foundation (Days 1-7)

### **Day 1: AST Node Structures**
**Time**: 4-6 hours  
**Goal**: Design and implement AST node types  
**Status**: ✅ COMPLETE

**Files to Create:**
- [x] `compiler/parser/ast.h`
- [x] `compiler/parser/ast.c`

**Step-by-Step Tasks:**
1. ✅ Define `AstNodeType` enum (15 min)
   - Expression types
   - Statement types
   - Declaration types

2. ✅ Create base `AstNode` structure (30 min)
   - Node type field
   - Location info (line, column)
   - Union for node-specific data

3. ✅ Define expression node structures (1 hour)
   - Binary expressions
   - Unary expressions
   - Literal expressions
   - Identifier expressions
   - Call expressions

4. ✅ Define statement node structures (1 hour)
   - Variable declarations
   - Assignment statements
   - If statements
   - While/for/loop statements
   - Return statements
   - Block statements

5. ✅ Implement node constructors (1.5 hours)
   - `ast_create_binary_expr()`
   - `ast_create_literal()`
   - `ast_create_if_stmt()`
   - etc.

6. ✅ Implement node destructors (1 hour)
   - `ast_free_node()` with recursive cleanup

7. ✅ Test node creation/destruction (30 min)

---

### **Day 2: Basic Parser Setup**
**Time**: 4-6 hours  
**Goal**: Set up parser infrastructure and parse literals  
**Status**: ✅ COMPLETE

**Files to Create:**
- [x] `compiler/parser/parser.h`
- [x] `compiler/parser/parser.c` (initial)

**Step-by-Step Tasks:**
1. ✅ Define `Parser` structure (20 min)
   - Token array/pointer
   - Current position
   - Error handling

2. ✅ Implement parser initialization (20 min)
   - `parser_init()`
   - Token consumption functions

3. ✅ Helper functions (1 hour)
   - `peek()` - look at current token
   - `advance()` - consume token
   - `match()` - check and consume if matches
   - `check()` - check without consuming
   - `expect()` - require specific token

4. ✅ Parse primary expressions (2 hours)
   - Integer literals
   - Float literals
   - String literals
   - Boolean literals
   - Identifiers
   - Grouped expressions `(expr)`

5. ✅ Error handling basics (1 hour)
   - `parser_error()` function
   - Error token creation
   - Synchronization points

6. ✅ Write simple tests (30 min)
   - Test literal parsing
   - Test error cases

**Additional Work Completed:**
- ✅ Implemented postfix expression parsing (function calls, array indexing, member access)
- ✅ Implemented basic statement parsing (variable declarations)
- ✅ Fixed identifier vs expression statement disambiguation
- ✅ Created `parse_postfix_continue()` for handling already-parsed identifiers
- ✅ All tests passing (test_lexer, test_ast, test_parser)

---

### **Day 3: Expression Parser**
**Time**: 5-7 hours  
**Goal**: Implement full expression parsing with precedence  
**Status**: ✅ COMPLETE (completed during Day 2)

**Files to Update:**
- [x] `compiler/parser/parser.c`

**Step-by-Step Tasks:**
1. ✅ Study operator precedence (30 min)
   - Review LAMC_GRAMMAR.md
   - Plan Pratt parser approach

2. ✅ Implement Pratt parser foundation (1 hour)
   - `parse_precedence()` function
   - Precedence level enum

3. ✅ Implement prefix parsers (1.5 hours)
   - Unary operators (-, !, ~)
   - Grouping
   - Literals
   - Identifiers

4. ✅ Implement infix parsers (2 hours)
   - Binary operators (+, -, *, /, %)
   - Comparison operators (==, !=, <, >, etc.)
   - Logical operators (&&, ||)
   - Bitwise operators (&, |, ^)

5. ✅ Implement postfix parsers (1.5 hours)
   - Array indexing `[expr]`
   - Function calls `(args)`
   - Member access `.field`

6. ✅ Test expression parsing (1 hour)
   - Simple expressions: `2 + 3`
   - Complex: `(a + b) * c - d`
   - Nested: `func(x + 1, y * 2)`

---

### **Day 4: Statement Parser - Part 1**
**Time**: 4-6 hours  
**Goal**: Parse basic statements  
**Status**: ✅ COMPLETE

**Files to Update:**
- [x] `compiler/parser/parser.c`

**Step-by-Step Tasks:**
1. ✅ Implement statement dispatcher (30 min)
   - `parse_statement()` function
   - Route to specific parsers

2. ✅ Parse variable declarations (1.5 hours)
   - `x = value`
   - `x: type = value`
   - Handle type annotations

3. ✅ Parse assignment statements (1 hour)
   - Simple: `x = expr`
   - Compound: `x += expr`
   - Array/field assignments

4. ✅ Parse expression statements (30 min)
   - Function calls
   - Standalone expressions

5. ✅ Parse block statements (1 hour)
   - Handle indentation (optional)
   - Parse statement lists
   - Scope handling

6. ✅ Test statement parsing (1 hour)
   - Test each statement type
   - Test nested blocks

---

### **Day 5: Statement Parser - Part 2**
**Time**: 4-6 hours  
**Goal**: Parse control flow statements  
**Status**: ✅ COMPLETE

**Files to Update:**
- [x] `compiler/parser/parser.c`

**Step-by-Step Tasks:**
1. ✅ Parse if statements (1.5 hours)
   - `if condition`
   - `else if condition`
   - `else`
   - Nested if statements

2. ✅ Parse while loops (1 hour)
   - `while condition`
   - Loop body

3. ✅ Parse for loops (1.5 hours)
   - Range: `for i in 0..10` (not yet - need range operator)
   - Iterator: `for item in array`
   - With index: `for i, item in array`

4. ✅ Parse loop statements (30 min)
   - Infinite loops
   - Break/continue

5. ✅ Parse return statements (30 min)
   - With value
   - Without value

6. ✅ Test control flow (1 hour)
   - Test all loop types
   - Test nested control flow

---

### **Day 6: Function Parser**
**Time**: 4-6 hours  
**Goal**: Parse function declarations and calls  
**Status**: ✅ COMPLETE

**Files to Update:**
- [x] `compiler/parser/parser.c`

**Step-by-Step Tasks:**
1. ✅ Parse function declarations (2 hours)
   - Function name
   - Parameter list
   - Default parameters (not implemented - future feature)
   - Function body

2. ✅ Parse parameter lists (1 hour)
   - Simple parameters
   - Type annotations
   - Default values (not implemented - future feature)

3. ✅ Parse function calls (1 hour)
   - Positional arguments
   - Argument expressions

4. ⏳ Parse import statements (45 min) - Not yet needed
   - Module names
   - Import lists

5. ✅ Test function parsing (1.5 hours)
   - Simple functions
   - Functions with parameters
   - Nested function calls
   - Recursive functions

**Additional Work Completed:**
- ✅ Created `parse_function_declaration()` with proper `Parameter*` objects
- ✅ Fixed AST printer compatibility with function parameters
- ✅ Tested complex programs with all control flow constructs

---

### **Day 7: Testing & Integration**
**Time**: 4-6 hours  
**Goal**: Comprehensive testing and bug fixes  
**Status**: ⏳ IN PROGRESS

**Files to Create:**
- [ ] `compiler/test_parser.c`
- [ ] Update `Makefile`

**Step-by-Step Tasks:**
1. Create parser test program (1.5 hours)
   - Load source file
   - Initialize lexer and parser
   - Parse and print AST

2. Create AST printer (1.5 hours)
   - `ast_print()` function
   - Pretty-print tree structure
   - Show node details

3. Test with example programs (1.5 hours)
   - `hello.lamc`
   - `fibonacci.lamc`
   - Create more examples

4. Fix bugs and issues (1 hour)
   - Debug parsing errors
   - Handle edge cases

5. Update documentation (30 min)
   - Update PROGRESS_REPORT.md
   - Document parser usage

---

## WEEK 2: Parser Refinement (Days 8-14)

### **Day 8: Advanced Features - Arrays**
**Time**: 3-4 hours

**Tasks:**
1. Parse array literals `[1, 2, 3]`
2. Parse array indexing `arr[0]`
3. Parse array slicing `arr[1:4]`
4. Test array operations

---

### **Day 9: Advanced Features - Dictionaries**
**Time**: 3-4 hours

**Tasks:**
1. Parse dictionary literals `{"key": value}`
2. Parse dictionary access `dict["key"]`
3. Parse dictionary methods
4. Test dictionary operations

---

### **Day 10: Advanced Features - Classes**
**Time**: 4-5 hours

**Tasks:**
1. Parse class declarations
2. Parse method definitions
3. Parse field access
4. Test class parsing

---

### **Day 11: Error Recovery**
**Time**: 4-5 hours

**Tasks:**
1. Implement synchronization points
2. Improve error messages
3. Multiple error reporting
4. Test error scenarios

---

### **Day 12: Parser Optimization**
**Time**: 3-4 hours

**Tasks:**
1. Optimize token consumption
2. Reduce allocations
3. Profile parser performance
4. Benchmark against examples

---

### **Day 13: Documentation**
**Time**: 3-4 hours

**Tasks:**
1. Update ARCHITECTURE.md
2. Create parser usage guide
3. Document AST structure
4. Add code comments

---

### **Day 14: Final Testing**
**Time**: 4-5 hours

**Tasks:**
1. Create comprehensive test suite
2. Test all language features
3. Stress test with large files
4. Final bug fixes

---

## Quick Reference: Files to Create/Modify

### Week 1:
- `compiler/parser/ast.h` - NEW
- `compiler/parser/ast.c` - NEW
- `compiler/parser/parser.h` - NEW
- `compiler/parser/parser.c` - NEW
- `compiler/test_parser.c` - NEW
- `compiler/Makefile` - UPDATE

### Week 2:
- `compiler/parser/parser.c` - UPDATE
- `examples/*.lamc` - NEW (more examples)
- `compiler/README.md` - UPDATE
- `docs/ARCHITECTURE.md` - UPDATE

---

## Daily Time Commitment

**Minimum**: 3-4 hours/day  
**Recommended**: 4-6 hours/day  
**Total for Phase 2**: 60-80 hours over 2-3 weeks

---

## Success Metrics

After Week 1:
- [ ] Can parse all basic expressions
- [ ] Can parse all statements
- [ ] Can parse function declarations
- [ ] Test program shows AST structure

After Week 2:
- [ ] Can parse advanced features
- [ ] Error messages are helpful
- [ ] Parser is well-documented
- [ ] Ready for semantic analysis phase

---

**Next Update**: After Day 7 (end of Week 1)
