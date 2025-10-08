# LAMC Formal Grammar Specification

## Lexical Structure

### Keywords
```
let, mut, fn, return, if, else, while, loop, for, in, break, continue,
struct, enum, import, export, unsafe, const, static, type, match,
true, false, null, as, sizeof, alignof
```

### Operators
```
Arithmetic: + - * / %
Comparison: == != < > <= >=
Logical: && || !
Bitwise: & | ^ ~ << >>
Assignment: = += -= *= /= %= &= |= ^= <<= >>=
Pointer: * &
Range: ..
```

### Delimiters
```
( ) { } [ ] , ; : :: . ->
```

### Literals
```
Integer: 0-9+ (decimal), 0x[0-9a-fA-F]+ (hex), 0b[01]+ (binary), 0o[0-7]+ (octal)
Float: [0-9]+\.[0-9]+ | [0-9]+\.[0-9]+[eE][+-]?[0-9]+
String: "..." | '...'
Character: 'x'
Boolean: true | false
```

### Identifiers
```
[a-zA-Z_][a-zA-Z0-9_]*
```

## Syntax Grammar (EBNF)

### Program Structure
```ebnf
program = { import_stmt | function_decl | struct_decl | enum_decl | const_decl }* ;

import_stmt = "import" identifier { "::" identifier }* ";" ;

const_decl = "const" identifier ":" type "=" expression ";" ;
```

### Type System
```ebnf
type = primitive_type
     | pointer_type
     | array_type
     | struct_type
     | function_type
     ;

primitive_type = "i8" | "i16" | "i32" | "i64"
               | "u8" | "u16" | "u32" | "u64"
               | "f32" | "f64"
               | "bool" | "char" | "void"
               ;

pointer_type = "ptr" "<" type ">" ;

array_type = "[" type ";" integer_literal "]" ;

struct_type = identifier ;

function_type = "fn" "(" [ type { "," type }* ] ")" [ "->" type ] ;
```

### Declarations
```ebnf
function_decl = "fn" identifier "(" [ param_list ] ")" [ "->" type ] block ;

param_list = param { "," param }* ;

param = identifier ":" type ;

struct_decl = "struct" identifier "{" { field_decl }* "}" ;

field_decl = identifier ":" type ";" ;

enum_decl = "enum" identifier "{" enum_variant { "," enum_variant }* "}" ;

enum_variant = identifier [ "(" type { "," type }* ")" ] ;
```

### Statements
```ebnf
statement = var_decl
          | assignment
          | expression_stmt
          | if_stmt
          | while_stmt
          | loop_stmt
          | for_stmt
          | return_stmt
          | break_stmt
          | continue_stmt
          | block
          | unsafe_block
          ;

var_decl = "let" [ "mut" ] identifier [ ":" type ] "=" expression ";" ;

assignment = lvalue assign_op expression ";" ;

assign_op = "=" | "+=" | "-=" | "*=" | "/=" | "%=" | "&=" | "|=" | "^=" | "<<=" | ">>=" ;

expression_stmt = expression ";" ;

if_stmt = "if" expression block [ "else" ( if_stmt | block ) ] ;

while_stmt = "while" expression block ;

loop_stmt = "loop" block ;

for_stmt = "for" identifier "in" expression block ;

return_stmt = "return" [ expression ] ";" ;

break_stmt = "break" ";" ;

continue_stmt = "continue" ";" ;

block = "{" { statement }* "}" ;

unsafe_block = "unsafe" block ;
```

### Expressions
```ebnf
expression = assignment_expr ;

assignment_expr = logical_or_expr ;

logical_or_expr = logical_and_expr { "||" logical_and_expr }* ;

logical_and_expr = bitwise_or_expr { "&&" bitwise_or_expr }* ;

bitwise_or_expr = bitwise_xor_expr { "|" bitwise_xor_expr }* ;

bitwise_xor_expr = bitwise_and_expr { "^" bitwise_and_expr }* ;

bitwise_and_expr = equality_expr { "&" equality_expr }* ;

equality_expr = relational_expr { ("==" | "!=") relational_expr }* ;

relational_expr = shift_expr { ("<" | ">" | "<=" | ">=") shift_expr }* ;

shift_expr = additive_expr { ("<<" | ">>") additive_expr }* ;

additive_expr = multiplicative_expr { ("+" | "-") multiplicative_expr }* ;

multiplicative_expr = unary_expr { ("*" | "/" | "%") unary_expr }* ;

unary_expr = postfix_expr
           | ("+" | "-" | "!" | "~" | "*" | "&") unary_expr
           | "sizeof" "(" type ")"
           | "alignof" "(" type ")"
           | cast_expr
           ;

cast_expr = postfix_expr [ "as" type ] ;

postfix_expr = primary_expr
             | postfix_expr "[" expression "]"
             | postfix_expr "(" [ arg_list ] ")"
             | postfix_expr "." identifier
             ;

primary_expr = identifier
             | literal
             | "(" expression ")"
             | array_literal
             | struct_literal
             | alloc_expr
             | free_expr
             ;

literal = integer_literal
        | float_literal
        | string_literal
        | char_literal
        | bool_literal
        | "null"
        ;

array_literal = "[" [ expression { "," expression }* ] "]" ;

struct_literal = identifier "{" [ field_init { "," field_init }* ] "}" ;

field_init = identifier ":" expression ;

alloc_expr = "alloc" "(" type ")" ;

free_expr = "free" "(" expression ")" ;

arg_list = expression { "," expression }* ;

lvalue = identifier
       | "*" lvalue
       | lvalue "[" expression "]"
       | lvalue "." identifier
       ;
```

## Operator Precedence (Highest to Lowest)

1. Postfix: `()` `[]` `.`
2. Unary: `+` `-` `!` `~` `*` `&` `sizeof` `alignof`
3. Cast: `as`
4. Multiplicative: `*` `/` `%`
5. Additive: `+` `-`
6. Shift: `<<` `>>`
7. Relational: `<` `>` `<=` `>=`
8. Equality: `==` `!=`
9. Bitwise AND: `&`
10. Bitwise XOR: `^`
11. Bitwise OR: `|`
12. Logical AND: `&&`
13. Logical OR: `||`
14. Assignment: `=` `+=` `-=` etc.

## Associativity

- Left-associative: Most operators
- Right-associative: Unary operators, Assignment operators

## Comments

```
// Single-line comment

/* 
   Multi-line comment
   Can span multiple lines
*/
```

## White Space

White space (spaces, tabs, newlines) is generally ignored except:
- To separate tokens
- Inside string literals
- For indentation (recommended for readability, not required)

## Example with Grammar Applied

```lamc
// Import statement
import io;

// Function declaration
fn factorial(n: i32) -> i32 {
    // If statement
    if n <= 1 {
        return 1;
    } else {
        // Expression with binary operators
        return n * factorial(n - 1);
    }
}

// Main function
fn main() -> i32 {
    // Variable declaration with type inference
    let mut result = 1;
    
    // For loop
    for i in 1..10 {
        result = result * i;
    }
    
    // Function call
    io.print("Result: ");
    
    // Return statement
    return 0;
}
```
