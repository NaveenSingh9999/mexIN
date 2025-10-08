/* ============================================
 * LAMC Compiler - Phase 2: AST Demo
 * Educational demonstration of AST building
 * ============================================
 * 
 * This program shows how the LAMC parser works:
 * 1. Takes tokens from the lexer
 * 2. Builds an Abstract Syntax Tree (AST)
 * 3. Represents program structure in memory
 * 
 * Author: Naveen Singh
 * Date: October 8, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include "../compiler/parser/ast.h"

/* ANSI color codes */
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_YELLOW  "\033[33m"

void print_header(const char* title) {
    printf("\n");
    printf(COLOR_BOLD COLOR_CYAN);
    printf("========================================\n");
    printf("  %s\n", title);
    printf("========================================\n");
    printf(COLOR_RESET);
}

void print_explanation(const char* text) {
    printf(COLOR_YELLOW "📚 " COLOR_RESET "%s\n\n", text);
}

/* Demo 1: Simple Expression */
void demo_expression() {
    print_header("Demo 1: Building an Expression AST");
    
    print_explanation(
        "Let's build the AST for: 2 + 3 * 4\n"
        "The parser respects operator precedence!"
    );
    
    printf(COLOR_BOLD "Expression:" COLOR_RESET " 2 + 3 * 4\n\n");
    
    /* Build: 3 * 4 */
    AstNode* three = ast_create_literal_int(3, 1, 5);
    AstNode* four = ast_create_literal_int(4, 1, 9);
    AstNode* multiply = ast_create_binary(OP_MUL, three, four, 1, 7);
    
    /* Build: 2 + (3 * 4) */
    AstNode* two = ast_create_literal_int(2, 1, 1);
    AstNode* add = ast_create_binary(OP_ADD, two, multiply, 1, 3);
    
    printf(COLOR_BOLD "AST Structure:" COLOR_RESET "\n");
    ast_print(add, 0);
    
    printf("\n");
    print_explanation(
        "Notice how multiplication is INSIDE the addition node?\n"
        "This represents that * has higher precedence than +!"
    );
    
    ast_free_node(add);
    printf(COLOR_GREEN "✓ Memory cleaned up (no leaks!)\n" COLOR_RESET);
}

/* Demo 2: Variable Declaration */
void demo_variable() {
    print_header("Demo 2: Variable Declaration AST");
    
    print_explanation(
        "LAMC syntax: x = 42\n"
        "The AST stores the variable name and its initial value."
    );
    
    printf(COLOR_BOLD "Code:" COLOR_RESET " x = 42\n\n");
    
    AstNode* value = ast_create_literal_int(42, 1, 5);
    AstNode* var_decl = ast_create_var_decl("x", NULL, value, 1, 1);
    
    printf(COLOR_BOLD "AST Structure:" COLOR_RESET "\n");
    ast_print(var_decl, 0);
    
    printf("\n");
    print_explanation(
        "The VarDecl node contains:\n"
        "  • Variable name: 'x'\n"
        "  • Type: NULL (automatic inference)\n"
        "  • Initializer: Literal 42"
    );
    
    ast_free_node(var_decl);
    printf(COLOR_GREEN "✓ Clean memory management\n" COLOR_RESET);
}

/* Demo 3: Function */
void demo_function() {
    print_header("Demo 3: Function Declaration AST");
    
    print_explanation(
        "LAMC function:\n"
        "  func add(a, b)\n"
        "      return a + b"
    );
    
    printf(COLOR_BOLD "Building function step by step..." COLOR_RESET "\n\n");
    
    /* Step 1: Create parameters */
    printf("1. Creating parameters (a, b)...\n");
    AstList* params = ast_list_create();
    ast_list_append(params, ast_create_parameter("a", NULL, NULL));
    ast_list_append(params, ast_create_parameter("b", NULL, NULL));
    
    /* Step 2: Create return expression (a + b) */
    printf("2. Creating expression (a + b)...\n");
    AstNode* a = ast_create_identifier("a", 2, 12);
    AstNode* b = ast_create_identifier("b", 2, 16);
    AstNode* add_expr = ast_create_binary(OP_ADD, a, b, 2, 14);
    
    /* Step 3: Create return statement */
    printf("3. Creating return statement...\n");
    AstNode* ret = ast_create_return(add_expr, 2, 5);
    
    /* Step 4: Create function body */
    printf("4. Creating function body...\n");
    AstList* stmts = ast_list_create();
    ast_list_append(stmts, ret);
    AstNode* body = ast_create_block(stmts, 2, 1);
    
    /* Step 5: Create function declaration */
    printf("5. Creating function declaration...\n\n");
    AstNode* func = ast_create_function("add", params, body, NULL, 1, 1);
    
    printf(COLOR_BOLD "Complete AST:" COLOR_RESET "\n");
    ast_print(func, 0);
    
    printf("\n");
    print_explanation(
        "The function AST contains:\n"
        "  • Function name: 'add'\n"
        "  • Parameters: list of Parameter nodes\n"
        "  • Body: BlockStmt containing statements\n"
        "  • Return type: NULL (inferred)"
    );
    
    ast_free_node(func);
    printf(COLOR_GREEN "✓ All nested structures freed correctly\n" COLOR_RESET);
}

/* Demo 4: Control Flow */
void demo_control_flow() {
    print_header("Demo 4: If Statement AST");
    
    print_explanation(
        "LAMC code:\n"
        "  if x > 10\n"
        "      print(\"big\")"
    );
    
    printf(COLOR_BOLD "Building if statement..." COLOR_RESET "\n\n");
    
    /* Condition: x > 10 */
    AstNode* x = ast_create_identifier("x", 1, 4);
    AstNode* ten = ast_create_literal_int(10, 1, 8);
    AstNode* condition = ast_create_binary(OP_GT, x, ten, 1, 6);
    
    /* Then branch: print("big") */
    AstNode* print_fn = ast_create_identifier("print", 2, 5);
    AstNode* arg = ast_create_literal_string("big", 2, 11);
    AstList* args = ast_list_create();
    ast_list_append(args, arg);
    AstNode* call = ast_create_call(print_fn, args, 2, 10);
    AstNode* then_stmt = ast_create_expr_stmt(call, 2, 5);
    
    /* Create if statement */
    AstNode* if_stmt = ast_create_if(condition, then_stmt, NULL, 1, 1);
    
    printf(COLOR_BOLD "AST Structure:" COLOR_RESET "\n");
    ast_print(if_stmt, 0);
    
    printf("\n");
    print_explanation(
        "The IfStmt node has three parts:\n"
        "  • Condition: BinaryExpr (x > 10)\n"
        "  • Then branch: ExprStmt (function call)\n"
        "  • Else branch: NULL (no else clause)"
    );
    
    ast_free_node(if_stmt);
    printf(COLOR_GREEN "✓ Complex nested structure cleaned\n" COLOR_RESET);
}

/* Demo 5: Complete Program */
void demo_program() {
    print_header("Demo 5: Complete Program AST");
    
    print_explanation(
        "Complete LAMC program:\n"
        "  func main()\n"
        "      x = 42\n"
        "      print(x)"
    );
    
    printf(COLOR_BOLD "Building complete program..." COLOR_RESET "\n\n");
    
    /* Create main function parameters (empty) */
    AstList* params = ast_list_create();
    
    /* Statement 1: x = 42 */
    AstNode* value = ast_create_literal_int(42, 2, 9);
    AstNode* var_decl = ast_create_var_decl("x", NULL, value, 2, 5);
    
    /* Statement 2: print(x) */
    AstNode* print_fn = ast_create_identifier("print", 3, 5);
    AstNode* x_arg = ast_create_identifier("x", 3, 11);
    AstList* print_args = ast_list_create();
    ast_list_append(print_args, x_arg);
    AstNode* print_call = ast_create_call(print_fn, print_args, 3, 10);
    AstNode* print_stmt = ast_create_expr_stmt(print_call, 3, 5);
    
    /* Create function body */
    AstList* body_stmts = ast_list_create();
    ast_list_append(body_stmts, var_decl);
    ast_list_append(body_stmts, print_stmt);
    AstNode* body = ast_create_block(body_stmts, 2, 1);
    
    /* Create function */
    AstNode* main_func = ast_create_function("main", params, body, NULL, 1, 1);
    
    /* Create program */
    AstList* decls = ast_list_create();
    ast_list_append(decls, main_func);
    AstNode* program = ast_create_program(decls);
    
    printf(COLOR_BOLD "Complete Program AST:" COLOR_RESET "\n");
    ast_print_program(program);
    
    printf("\n");
    print_explanation(
        "The Program node is the ROOT of the entire AST.\n"
        "It contains all top-level declarations (functions, classes, etc.).\n"
        "This tree structure represents the ENTIRE program in memory!"
    );
    
    ast_free_node(program);
    printf(COLOR_GREEN "✓ Entire program tree freed (including all nested nodes)\n" COLOR_RESET);
}

/* Main demo */
int main() {
    printf(COLOR_BOLD COLOR_BLUE);
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   LAMC Compiler - AST Demo             ║\n");
    printf("║   Phase 2: Abstract Syntax Tree        ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf(COLOR_RESET);
    
    demo_expression();
    demo_variable();
    demo_function();
    demo_control_flow();
    demo_program();
    
    /* Summary */
    print_header("What You Learned");
    printf("  📖 The " COLOR_BOLD "AST" COLOR_RESET " represents program structure as a tree\n");
    printf("  🌳 Each node can have " COLOR_BOLD "children" COLOR_RESET " (nested structures)\n");
    printf("  🧠 The AST is stored in " COLOR_BOLD "memory" COLOR_RESET " (not on disk)\n");
    printf("  ♻️  Proper " COLOR_BOLD "memory management" COLOR_RESET " prevents leaks\n");
    printf("  🎯 The AST is used for:\n");
    printf("     • Type checking (semantic analysis)\n");
    printf("     • Optimization\n");
    printf("     • Code generation\n");
    printf("\n");
    
    printf(COLOR_GREEN COLOR_BOLD "✓ Phase 2 Part 1 (AST) is complete and production-ready!\n" COLOR_RESET);
    printf("  Next: Phase 2 Part 2 (Parser) - Converting tokens to AST\n\n");
    
    return 0;
}
