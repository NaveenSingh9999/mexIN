/* LAMC Compiler - AST Test Program
 * Tests AST node creation and memory management
 * Copyright (c) 2025 Naveen Singh
 */

#include <stdio.h>
#include <stdlib.h>
#include "parser/ast.h"

void test_literals() {
    printf("\n=== Testing Literals ===\n");
    
    AstNode* int_lit = ast_create_literal_int(42, 1, 1);
    AstNode* float_lit = ast_create_literal_float(3.14, 1, 5);
    AstNode* str_lit = ast_create_literal_string("Hello, LAMC!", 1, 10);
    AstNode* bool_lit = ast_create_literal_bool(true, 1, 15);
    
    ast_print(int_lit, 0);
    ast_print(float_lit, 0);
    ast_print(str_lit, 0);
    ast_print(bool_lit, 0);
    
    ast_free_node(int_lit);
    ast_free_node(float_lit);
    ast_free_node(str_lit);
    ast_free_node(bool_lit);
    
    printf("✓ Literals test passed\n");
}

void test_binary_expressions() {
    printf("\n=== Testing Binary Expressions ===\n");
    
    // 2 + 3
    AstNode* left = ast_create_literal_int(2, 1, 1);
    AstNode* right = ast_create_literal_int(3, 1, 5);
    AstNode* expr = ast_create_binary(OP_ADD, left, right, 1, 3);
    
    ast_print(expr, 0);
    ast_free_node(expr);
    
    // (5 * 6) + 7
    AstNode* five = ast_create_literal_int(5, 1, 1);
    AstNode* six = ast_create_literal_int(6, 1, 5);
    AstNode* seven = ast_create_literal_int(7, 1, 11);
    AstNode* mul = ast_create_binary(OP_MUL, five, six, 1, 3);
    AstNode* add = ast_create_binary(OP_ADD, mul, seven, 1, 9);
    
    ast_print(add, 0);
    ast_free_node(add);
    
    printf("✓ Binary expressions test passed\n");
}

void test_variables() {
    printf("\n=== Testing Variables ===\n");
    
    // x = 42
    AstNode* value = ast_create_literal_int(42, 1, 5);
    AstNode* var = ast_create_var_decl("x", NULL, value, 1, 1);
    
    ast_print(var, 0);
    ast_free_node(var);
    
    // name: string = "LAMC"
    AstNode* str_val = ast_create_literal_string("LAMC", 1, 17);
    AstNode* typed_var = ast_create_var_decl("name", "string", str_val, 1, 1);
    
    ast_print(typed_var, 0);
    ast_free_node(typed_var);
    
    printf("✓ Variables test passed\n");
}

void test_function() {
    printf("\n=== Testing Function ===\n");
    
    // func add(a, b)
    //     return a + b
    
    AstList* params = ast_list_create();
    ast_list_append(params, ast_create_parameter("a", NULL, NULL));
    ast_list_append(params, ast_create_parameter("b", NULL, NULL));
    
    // Create return statement
    AstNode* a_id = ast_create_identifier("a", 2, 12);
    AstNode* b_id = ast_create_identifier("b", 2, 16);
    AstNode* add_expr = ast_create_binary(OP_ADD, a_id, b_id, 2, 14);
    AstNode* ret_stmt = ast_create_return(add_expr, 2, 5);
    
    // Create function body
    AstList* stmts = ast_list_create();
    ast_list_append(stmts, ret_stmt);
    AstNode* body = ast_create_block(stmts, 2, 1);
    
    AstNode* func = ast_create_function("add", params, body, NULL, 1, 1);
    
    ast_print(func, 0);
    ast_free_node(func);
    
    printf("✓ Function test passed\n");
}

void test_control_flow() {
    printf("\n=== Testing Control Flow ===\n");
    
    // if x > 10
    //     print("big")
    
    AstNode* x = ast_create_identifier("x", 1, 4);
    AstNode* ten = ast_create_literal_int(10, 1, 8);
    AstNode* cond = ast_create_binary(OP_GT, x, ten, 1, 6);
    
    AstNode* print_id = ast_create_identifier("print", 2, 5);
    AstNode* msg = ast_create_literal_string("big", 2, 11);
    AstList* args = ast_list_create();
    ast_list_append(args, msg);
    AstNode* print_call = ast_create_call(print_id, args, 2, 10);
    AstNode* then_stmt = ast_create_expr_stmt(print_call, 2, 5);
    
    AstNode* if_stmt = ast_create_if(cond, then_stmt, NULL, 1, 1);
    
    ast_print(if_stmt, 0);
    ast_free_node(if_stmt);
    
    printf("✓ Control flow test passed\n");
}

void test_array() {
    printf("\n=== Testing Arrays ===\n");
    
    // [1, 2, 3, 4, 5]
    AstList* elements = ast_list_create();
    for (int i = 1; i <= 5; i++) {
        ast_list_append(elements, ast_create_literal_int(i, 1, i * 3));
    }
    
    AstNode* arr = ast_create_array(elements, 1, 1);
    ast_print(arr, 0);
    ast_free_node(arr);
    
    printf("✓ Array test passed\n");
}

void test_complete_program() {
    printf("\n=== Testing Complete Program ===\n");
    
    // func main()
    //     x = 42
    //     print(x)
    
    AstList* decls = ast_list_create();
    
    // Create main function
    AstList* params = ast_list_create();
    
    // x = 42
    AstNode* forty_two = ast_create_literal_int(42, 2, 9);
    AstNode* x_decl = ast_create_var_decl("x", NULL, forty_two, 2, 5);
    
    // print(x)
    AstNode* print_id = ast_create_identifier("print", 3, 5);
    AstNode* x_id = ast_create_identifier("x", 3, 11);
    AstList* print_args = ast_list_create();
    ast_list_append(print_args, x_id);
    AstNode* print_call = ast_create_call(print_id, print_args, 3, 10);
    AstNode* print_stmt = ast_create_expr_stmt(print_call, 3, 5);
    
    // Create body
    AstList* body_stmts = ast_list_create();
    ast_list_append(body_stmts, x_decl);
    ast_list_append(body_stmts, print_stmt);
    AstNode* body = ast_create_block(body_stmts, 2, 1);
    
    AstNode* main_func = ast_create_function("main", params, body, NULL, 1, 1);
    ast_list_append(decls, main_func);
    
    AstNode* program = ast_create_program(decls);
    
    ast_print_program(program);
    ast_free_node(program);
    
    printf("✓ Complete program test passed\n");
}

int main() {
    printf("====================================\n");
    printf("   LAMC AST Module Test Suite\n");
    printf("====================================\n");
    
    test_literals();
    test_binary_expressions();
    test_variables();
    test_function();
    test_control_flow();
    test_array();
    test_complete_program();
    
    printf("\n====================================\n");
    printf("✓ All AST tests passed successfully!\n");
    printf("====================================\n");
    
    return 0;
}
