/* LAMC Compiler - AST Pretty Printer
 * Production-ready AST visualization
 * Copyright (c) 2025 Naveen Singh
 */

#include "ast.h"
#include <stdio.h>

static void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

void ast_print(AstNode* node, int indent) {
    if (!node) {
        print_indent(indent);
        printf("(null)\n");
        return;
    }
    
    print_indent(indent);
    
    switch (node->type) {
        case AST_BINARY_EXPR:
            printf("BinaryExpr (%s)\n", binary_op_name(node->as.binary.op));
            ast_print(node->as.binary.left, indent + 1);
            ast_print(node->as.binary.right, indent + 1);
            break;
            
        case AST_UNARY_EXPR:
            printf("UnaryExpr (%s)\n", unary_op_name(node->as.unary.op));
            ast_print(node->as.unary.operand, indent + 1);
            break;
            
        case AST_LITERAL_EXPR:
            switch (node->as.literal.type) {
                case LIT_INT:
                    printf("Literal (int: %ld)\n", node->as.literal.as.int_value);
                    break;
                case LIT_FLOAT:
                    printf("Literal (float: %g)\n", node->as.literal.as.float_value);
                    break;
                case LIT_STRING:
                    printf("Literal (string: \"%s\")\n", node->as.literal.as.string_value);
                    break;
                case LIT_BOOL:
                    printf("Literal (bool: %s)\n", node->as.literal.as.bool_value ? "true" : "false");
                    break;
                case LIT_NULL:
                    printf("Literal (null)\n");
                    break;
            }
            break;
            
        case AST_IDENTIFIER_EXPR:
            printf("Identifier (%s)\n", node->as.identifier);
            break;
            
        case AST_CALL_EXPR:
            printf("CallExpr\n");
            print_indent(indent + 1);
            printf("callee:\n");
            ast_print(node->as.call.callee, indent + 2);
            print_indent(indent + 1);
            printf("arguments:\n");
            if (node->as.call.arguments) {
                for (size_t i = 0; i < node->as.call.arguments->count; i++) {
                    ast_print((AstNode*)node->as.call.arguments->items[i], indent + 2);
                }
            }
            break;
            
        case AST_INDEX_EXPR:
            printf("IndexExpr\n");
            print_indent(indent + 1);
            printf("object:\n");
            ast_print(node->as.index.object, indent + 2);
            print_indent(indent + 1);
            printf("index:\n");
            ast_print(node->as.index.index, indent + 2);
            break;
            
        case AST_MEMBER_EXPR:
            printf("MemberExpr (field: %s)\n", node->as.member.member);
            ast_print(node->as.member.object, indent + 1);
            break;
            
        case AST_ARRAY_EXPR:
            printf("ArrayExpr\n");
            if (node->as.array.elements) {
                for (size_t i = 0; i < node->as.array.elements->count; i++) {
                    ast_print((AstNode*)node->as.array.elements->items[i], indent + 1);
                }
            }
            break;
            
        case AST_DICT_EXPR:
            printf("DictExpr\n");
            if (node->as.dict.entries) {
                for (size_t i = 0; i < node->as.dict.entries->count; i++) {
                    DictEntry* entry = (DictEntry*)node->as.dict.entries->items[i];
                    print_indent(indent + 1);
                    printf("entry:\n");
                    print_indent(indent + 2);
                    printf("key:\n");
                    ast_print(entry->key, indent + 3);
                    print_indent(indent + 2);
                    printf("value:\n");
                    ast_print(entry->value, indent + 3);
                }
            }
            break;
            
        case AST_VAR_DECL:
            printf("VarDecl (name: %s", node->as.var_decl.name);
            if (node->as.var_decl.type_name) {
                printf(", type: %s", node->as.var_decl.type_name);
            }
            printf(")\n");
            if (node->as.var_decl.initializer) {
                print_indent(indent + 1);
                printf("initializer:\n");
                ast_print(node->as.var_decl.initializer, indent + 2);
            }
            break;
            
        case AST_ASSIGN_STMT:
            printf("AssignStmt\n");
            print_indent(indent + 1);
            printf("target:\n");
            ast_print(node->as.assign.target, indent + 2);
            print_indent(indent + 1);
            printf("value:\n");
            ast_print(node->as.assign.value, indent + 2);
            break;
            
        case AST_EXPR_STMT:
            printf("ExprStmt\n");
            ast_print(node->as.expr_stmt, indent + 1);
            break;
            
        case AST_IF_STMT:
            printf("IfStmt\n");
            print_indent(indent + 1);
            printf("condition:\n");
            ast_print(node->as.if_stmt.condition, indent + 2);
            print_indent(indent + 1);
            printf("then:\n");
            ast_print(node->as.if_stmt.then_branch, indent + 2);
            if (node->as.if_stmt.else_branch) {
                print_indent(indent + 1);
                printf("else:\n");
                ast_print(node->as.if_stmt.else_branch, indent + 2);
            }
            break;
            
        case AST_WHILE_STMT:
            printf("WhileStmt\n");
            print_indent(indent + 1);
            printf("condition:\n");
            ast_print(node->as.while_stmt.condition, indent + 2);
            print_indent(indent + 1);
            printf("body:\n");
            ast_print(node->as.while_stmt.body, indent + 2);
            break;
            
        case AST_FOR_STMT:
            printf("ForStmt (var: %s", node->as.for_stmt.variable);
            if (node->as.for_stmt.index_var) {
                printf(", index: %s", node->as.for_stmt.index_var);
            }
            printf(")\n");
            print_indent(indent + 1);
            printf("iterable:\n");
            ast_print(node->as.for_stmt.iterable, indent + 2);
            print_indent(indent + 1);
            printf("body:\n");
            ast_print(node->as.for_stmt.body, indent + 2);
            break;
            
        case AST_LOOP_STMT:
            printf("LoopStmt\n");
            ast_print(node->as.loop_stmt.body, indent + 1);
            break;
            
        case AST_RETURN_STMT:
            printf("ReturnStmt\n");
            if (node->as.return_stmt.value) {
                ast_print(node->as.return_stmt.value, indent + 1);
            }
            break;
            
        case AST_BREAK_STMT:
            printf("BreakStmt\n");
            break;
            
        case AST_CONTINUE_STMT:
            printf("ContinueStmt\n");
            break;
            
        case AST_BLOCK_STMT:
            printf("BlockStmt\n");
            if (node->as.block.statements) {
                for (size_t i = 0; i < node->as.block.statements->count; i++) {
                    ast_print((AstNode*)node->as.block.statements->items[i], indent + 1);
                }
            }
            break;
            
        case AST_FUNCTION_DECL:
            printf("FunctionDecl (name: %s", node->as.function.name);
            if (node->as.function.return_type) {
                printf(", return: %s", node->as.function.return_type);
            }
            printf(")\n");
            print_indent(indent + 1);
            printf("parameters:\n");
            if (node->as.function.parameters) {
                for (size_t i = 0; i < node->as.function.parameters->count; i++) {
                    Parameter* param = (Parameter*)node->as.function.parameters->items[i];
                    print_indent(indent + 2);
                    printf("param: %s", param->name);
                    if (param->type_name) {
                        printf(": %s", param->type_name);
                    }
                    if (param->default_value) {
                        printf(" = ...");
                    }
                    printf("\n");
                }
            }
            print_indent(indent + 1);
            printf("body:\n");
            ast_print(node->as.function.body, indent + 2);
            break;
            
        case AST_CLASS_DECL:
            printf("ClassDecl (name: %s)\n", node->as.class_decl.name);
            if (node->as.class_decl.fields && node->as.class_decl.fields->count > 0) {
                print_indent(indent + 1);
                printf("fields:\n");
                for (size_t i = 0; i < node->as.class_decl.fields->count; i++) {
                    ast_print((AstNode*)node->as.class_decl.fields->items[i], indent + 2);
                }
            }
            if (node->as.class_decl.methods && node->as.class_decl.methods->count > 0) {
                print_indent(indent + 1);
                printf("methods:\n");
                for (size_t i = 0; i < node->as.class_decl.methods->count; i++) {
                    ast_print((AstNode*)node->as.class_decl.methods->items[i], indent + 2);
                }
            }
            break;
            
        case AST_IMPORT_STMT:
            printf("ImportStmt (module: %s)\n", node->as.import.module_name);
            break;
            
        case AST_PROGRAM:
            printf("Program\n");
            if (node->as.program.declarations) {
                for (size_t i = 0; i < node->as.program.declarations->count; i++) {
                    ast_print((AstNode*)node->as.program.declarations->items[i], indent + 1);
                }
            }
            break;
    }
}

void ast_print_program(AstNode* program) {
    if (!program || program->type != AST_PROGRAM) {
        printf("Error: Not a program node\n");
        return;
    }
    
    printf("===== LAMC Abstract Syntax Tree =====\n\n");
    ast_print(program, 0);
    printf("\n===== End of AST =====\n");
}
