/* LAMC Compiler - Abstract Syntax Tree Implementation
 * Production-ready AST with proper memory management
 * Copyright (c) 2025 Naveen Singh
 */

#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ===== AST List Implementation ===== */

AstList* ast_list_create(void) {
    AstList* list = (AstList*)malloc(sizeof(AstList));
    if (!list) return NULL;
    
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
    return list;
}

void ast_list_append(AstList* list, void* item) {
    if (!list) return;
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 8 : list->capacity * 2;
        void** new_items = (void**)realloc(list->items, new_capacity * sizeof(void*));
        if (!new_items) return;
        
        list->items = new_items;
        list->capacity = new_capacity;
    }
    
    list->items[list->count++] = item;
}

void ast_list_free(AstList* list) {
    if (!list) return;
    free(list->items);
    free(list);
}

/* ===== Helper Functions ===== */

static char* string_duplicate(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    char* dup = (char*)malloc(len + 1);
    if (!dup) return NULL;
    memcpy(dup, str, len + 1);
    return dup;
}

static AstNode* ast_node_alloc(AstNodeType type, int line, int col) {
    AstNode* node = (AstNode*)calloc(1, sizeof(AstNode));
    if (!node) return NULL;
    
    node->type = type;
    node->line = line;
    node->column = col;
    return node;
}

/* ===== Expression Constructors ===== */

AstNode* ast_create_binary(BinaryOp op, AstNode* left, AstNode* right, int line, int col) {
    AstNode* node = ast_node_alloc(AST_BINARY_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.binary.op = op;
    node->as.binary.left = left;
    node->as.binary.right = right;
    return node;
}

AstNode* ast_create_unary(UnaryOp op, AstNode* operand, int line, int col) {
    AstNode* node = ast_node_alloc(AST_UNARY_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.unary.op = op;
    node->as.unary.operand = operand;
    return node;
}

AstNode* ast_create_literal_int(long value, int line, int col) {
    AstNode* node = ast_node_alloc(AST_LITERAL_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.literal.type = LIT_INT;
    node->as.literal.as.int_value = value;
    return node;
}

AstNode* ast_create_literal_float(double value, int line, int col) {
    AstNode* node = ast_node_alloc(AST_LITERAL_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.literal.type = LIT_FLOAT;
    node->as.literal.as.float_value = value;
    return node;
}

AstNode* ast_create_literal_string(const char* value, int line, int col) {
    AstNode* node = ast_node_alloc(AST_LITERAL_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.literal.type = LIT_STRING;
    node->as.literal.as.string_value = string_duplicate(value);
    return node;
}

AstNode* ast_create_literal_bool(bool value, int line, int col) {
    AstNode* node = ast_node_alloc(AST_LITERAL_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.literal.type = LIT_BOOL;
    node->as.literal.as.bool_value = value;
    return node;
}

AstNode* ast_create_literal_null(int line, int col) {
    AstNode* node = ast_node_alloc(AST_LITERAL_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.literal.type = LIT_NULL;
    return node;
}

AstNode* ast_create_identifier(const char* name, int line, int col) {
    AstNode* node = ast_node_alloc(AST_IDENTIFIER_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.identifier = string_duplicate(name);
    return node;
}

AstNode* ast_create_call(AstNode* callee, AstList* args, int line, int col) {
    AstNode* node = ast_node_alloc(AST_CALL_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.call.callee = callee;
    node->as.call.arguments = args;
    return node;
}

AstNode* ast_create_index(AstNode* object, AstNode* index, int line, int col) {
    AstNode* node = ast_node_alloc(AST_INDEX_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.index.object = object;
    node->as.index.index = index;
    return node;
}

AstNode* ast_create_member(AstNode* object, const char* member, int line, int col) {
    AstNode* node = ast_node_alloc(AST_MEMBER_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.member.object = object;
    node->as.member.member = string_duplicate(member);
    return node;
}

AstNode* ast_create_array(AstList* elements, int line, int col) {
    AstNode* node = ast_node_alloc(AST_ARRAY_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.array.elements = elements;
    return node;
}

AstNode* ast_create_dict(AstList* entries, int line, int col) {
    AstNode* node = ast_node_alloc(AST_DICT_EXPR, line, col);
    if (!node) return NULL;
    
    node->as.dict.entries = entries;
    return node;
}

/* ===== Statement Constructors ===== */

AstNode* ast_create_var_decl(const char* name, const char* type, AstNode* init, int line, int col) {
    AstNode* node = ast_node_alloc(AST_VAR_DECL, line, col);
    if (!node) return NULL;
    
    node->as.var_decl.name = string_duplicate(name);
    node->as.var_decl.type_name = type ? string_duplicate(type) : NULL;
    node->as.var_decl.initializer = init;
    return node;
}

AstNode* ast_create_assign(AstNode* target, AstNode* value, int line, int col) {
    AstNode* node = ast_node_alloc(AST_ASSIGN_STMT, line, col);
    if (!node) return NULL;
    
    node->as.assign.target = target;
    node->as.assign.value = value;
    return node;
}

AstNode* ast_create_expr_stmt(AstNode* expr, int line, int col) {
    AstNode* node = ast_node_alloc(AST_EXPR_STMT, line, col);
    if (!node) return NULL;
    
    node->as.expr_stmt = expr;
    return node;
}

AstNode* ast_create_if(AstNode* cond, AstNode* then_br, AstNode* else_br, int line, int col) {
    AstNode* node = ast_node_alloc(AST_IF_STMT, line, col);
    if (!node) return NULL;
    
    node->as.if_stmt.condition = cond;
    node->as.if_stmt.then_branch = then_br;
    node->as.if_stmt.else_branch = else_br;
    return node;
}

AstNode* ast_create_while(AstNode* cond, AstNode* body, int line, int col) {
    AstNode* node = ast_node_alloc(AST_WHILE_STMT, line, col);
    if (!node) return NULL;
    
    node->as.while_stmt.condition = cond;
    node->as.while_stmt.body = body;
    return node;
}

AstNode* ast_create_for(const char* var, AstNode* iter, AstNode* body, const char* idx, int line, int col) {
    AstNode* node = ast_node_alloc(AST_FOR_STMT, line, col);
    if (!node) return NULL;
    
    node->as.for_stmt.variable = string_duplicate(var);
    node->as.for_stmt.iterable = iter;
    node->as.for_stmt.body = body;
    node->as.for_stmt.index_var = idx ? string_duplicate(idx) : NULL;
    return node;
}

AstNode* ast_create_loop(AstNode* body, int line, int col) {
    AstNode* node = ast_node_alloc(AST_LOOP_STMT, line, col);
    if (!node) return NULL;
    
    node->as.loop_stmt.body = body;
    return node;
}

AstNode* ast_create_return(AstNode* value, int line, int col) {
    AstNode* node = ast_node_alloc(AST_RETURN_STMT, line, col);
    if (!node) return NULL;
    
    node->as.return_stmt.value = value;
    return node;
}

AstNode* ast_create_break(int line, int col) {
    return ast_node_alloc(AST_BREAK_STMT, line, col);
}

AstNode* ast_create_continue(int line, int col) {
    return ast_node_alloc(AST_CONTINUE_STMT, line, col);
}

AstNode* ast_create_block(AstList* stmts, int line, int col) {
    AstNode* node = ast_node_alloc(AST_BLOCK_STMT, line, col);
    if (!node) return NULL;
    
    node->as.block.statements = stmts;
    return node;
}

/* ===== Declaration Constructors ===== */

AstNode* ast_create_function(const char* name, AstList* params, AstNode* body, const char* ret_type, int line, int col) {
    AstNode* node = ast_node_alloc(AST_FUNCTION_DECL, line, col);
    if (!node) return NULL;
    
    node->as.function.name = string_duplicate(name);
    node->as.function.parameters = params;
    node->as.function.body = body;
    node->as.function.return_type = ret_type ? string_duplicate(ret_type) : NULL;
    return node;
}

AstNode* ast_create_class(const char* name, AstList* methods, AstList* fields, int line, int col) {
    AstNode* node = ast_node_alloc(AST_CLASS_DECL, line, col);
    if (!node) return NULL;
    
    node->as.class_decl.name = string_duplicate(name);
    node->as.class_decl.methods = methods;
    node->as.class_decl.fields = fields;
    return node;
}

AstNode* ast_create_import(const char* module, int line, int col) {
    AstNode* node = ast_node_alloc(AST_IMPORT_STMT, line, col);
    if (!node) return NULL;
    
    node->as.import.module_name = string_duplicate(module);
    return node;
}

AstNode* ast_create_program(AstList* decls) {
    AstNode* node = ast_node_alloc(AST_PROGRAM, 1, 1);
    if (!node) return NULL;
    
    node->as.program.declarations = decls;
    return node;
}

/* ===== Helper Structure Constructors ===== */

Parameter* ast_create_parameter(const char* name, const char* type, AstNode* default_val) {
    Parameter* param = (Parameter*)malloc(sizeof(Parameter));
    if (!param) return NULL;
    
    param->name = string_duplicate(name);
    param->type_name = type ? string_duplicate(type) : NULL;
    param->default_value = default_val;
    return param;
}

DictEntry* ast_create_dict_entry(AstNode* key, AstNode* value) {
    DictEntry* entry = (DictEntry*)malloc(sizeof(DictEntry));
    if (!entry) return NULL;
    
    entry->key = key;
    entry->value = value;
    return entry;
}

/* ===== Destructors ===== */

void ast_free_parameter(Parameter* param) {
    if (!param) return;
    free(param->name);
    free(param->type_name);
    if (param->default_value) ast_free_node(param->default_value);
    free(param);
}

void ast_free_dict_entry(DictEntry* entry) {
    if (!entry) return;
    if (entry->key) ast_free_node(entry->key);
    if (entry->value) ast_free_node(entry->value);
    free(entry);
}

void ast_free_node(AstNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_BINARY_EXPR:
            ast_free_node(node->as.binary.left);
            ast_free_node(node->as.binary.right);
            break;
            
        case AST_UNARY_EXPR:
            ast_free_node(node->as.unary.operand);
            break;
            
        case AST_LITERAL_EXPR:
            if (node->as.literal.type == LIT_STRING) {
                free(node->as.literal.as.string_value);
            }
            break;
            
        case AST_IDENTIFIER_EXPR:
            free(node->as.identifier);
            break;
            
        case AST_CALL_EXPR:
            ast_free_node(node->as.call.callee);
            if (node->as.call.arguments) {
                for (size_t i = 0; i < node->as.call.arguments->count; i++) {
                    ast_free_node((AstNode*)node->as.call.arguments->items[i]);
                }
                ast_list_free(node->as.call.arguments);
            }
            break;
            
        case AST_INDEX_EXPR:
            ast_free_node(node->as.index.object);
            ast_free_node(node->as.index.index);
            break;
            
        case AST_MEMBER_EXPR:
            ast_free_node(node->as.member.object);
            free(node->as.member.member);
            break;
            
        case AST_ARRAY_EXPR:
            if (node->as.array.elements) {
                for (size_t i = 0; i < node->as.array.elements->count; i++) {
                    ast_free_node((AstNode*)node->as.array.elements->items[i]);
                }
                ast_list_free(node->as.array.elements);
            }
            break;
            
        case AST_DICT_EXPR:
            if (node->as.dict.entries) {
                for (size_t i = 0; i < node->as.dict.entries->count; i++) {
                    ast_free_dict_entry((DictEntry*)node->as.dict.entries->items[i]);
                }
                ast_list_free(node->as.dict.entries);
            }
            break;
            
        case AST_VAR_DECL:
            free(node->as.var_decl.name);
            free(node->as.var_decl.type_name);
            ast_free_node(node->as.var_decl.initializer);
            break;
            
        case AST_ASSIGN_STMT:
            ast_free_node(node->as.assign.target);
            ast_free_node(node->as.assign.value);
            break;
            
        case AST_EXPR_STMT:
            ast_free_node(node->as.expr_stmt);
            break;
            
        case AST_IF_STMT:
            ast_free_node(node->as.if_stmt.condition);
            ast_free_node(node->as.if_stmt.then_branch);
            ast_free_node(node->as.if_stmt.else_branch);
            break;
            
        case AST_WHILE_STMT:
            ast_free_node(node->as.while_stmt.condition);
            ast_free_node(node->as.while_stmt.body);
            break;
            
        case AST_FOR_STMT:
            free(node->as.for_stmt.variable);
            free(node->as.for_stmt.index_var);
            ast_free_node(node->as.for_stmt.iterable);
            ast_free_node(node->as.for_stmt.body);
            break;
            
        case AST_LOOP_STMT:
            ast_free_node(node->as.loop_stmt.body);
            break;
            
        case AST_RETURN_STMT:
            ast_free_node(node->as.return_stmt.value);
            break;
            
        case AST_BLOCK_STMT:
            if (node->as.block.statements) {
                for (size_t i = 0; i < node->as.block.statements->count; i++) {
                    ast_free_node((AstNode*)node->as.block.statements->items[i]);
                }
                ast_list_free(node->as.block.statements);
            }
            break;
            
        case AST_FUNCTION_DECL:
            free(node->as.function.name);
            free(node->as.function.return_type);
            if (node->as.function.parameters) {
                for (size_t i = 0; i < node->as.function.parameters->count; i++) {
                    ast_free_parameter((Parameter*)node->as.function.parameters->items[i]);
                }
                ast_list_free(node->as.function.parameters);
            }
            ast_free_node(node->as.function.body);
            break;
            
        case AST_CLASS_DECL:
            free(node->as.class_decl.name);
            if (node->as.class_decl.methods) {
                for (size_t i = 0; i < node->as.class_decl.methods->count; i++) {
                    ast_free_node((AstNode*)node->as.class_decl.methods->items[i]);
                }
                ast_list_free(node->as.class_decl.methods);
            }
            if (node->as.class_decl.fields) {
                for (size_t i = 0; i < node->as.class_decl.fields->count; i++) {
                    ast_free_node((AstNode*)node->as.class_decl.fields->items[i]);
                }
                ast_list_free(node->as.class_decl.fields);
            }
            break;
            
        case AST_IMPORT_STMT:
            free(node->as.import.module_name);
            break;
            
        case AST_PROGRAM:
            if (node->as.program.declarations) {
                for (size_t i = 0; i < node->as.program.declarations->count; i++) {
                    ast_free_node((AstNode*)node->as.program.declarations->items[i]);
                }
                ast_list_free(node->as.program.declarations);
            }
            break;
            
        case AST_BREAK_STMT:
        case AST_CONTINUE_STMT:
            /* No additional cleanup needed */
            break;
    }
    
    free(node);
}

/* ===== Utility Functions ===== */

const char* ast_node_type_name(AstNodeType type) {
    switch (type) {
        case AST_BINARY_EXPR: return "BinaryExpr";
        case AST_UNARY_EXPR: return "UnaryExpr";
        case AST_LITERAL_EXPR: return "Literal";
        case AST_IDENTIFIER_EXPR: return "Identifier";
        case AST_CALL_EXPR: return "CallExpr";
        case AST_INDEX_EXPR: return "IndexExpr";
        case AST_MEMBER_EXPR: return "MemberExpr";
        case AST_ARRAY_EXPR: return "ArrayExpr";
        case AST_DICT_EXPR: return "DictExpr";
        case AST_VAR_DECL: return "VarDecl";
        case AST_ASSIGN_STMT: return "AssignStmt";
        case AST_EXPR_STMT: return "ExprStmt";
        case AST_IF_STMT: return "IfStmt";
        case AST_WHILE_STMT: return "WhileStmt";
        case AST_FOR_STMT: return "ForStmt";
        case AST_LOOP_STMT: return "LoopStmt";
        case AST_RETURN_STMT: return "ReturnStmt";
        case AST_BREAK_STMT: return "BreakStmt";
        case AST_CONTINUE_STMT: return "ContinueStmt";
        case AST_BLOCK_STMT: return "BlockStmt";
        case AST_FUNCTION_DECL: return "FunctionDecl";
        case AST_CLASS_DECL: return "ClassDecl";
        case AST_IMPORT_STMT: return "ImportStmt";
        case AST_PROGRAM: return "Program";
        default: return "Unknown";
    }
}

const char* binary_op_name(BinaryOp op) {
    switch (op) {
        case OP_ADD: return "+";
        case OP_SUB: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_MOD: return "%";
        case OP_EQ: return "==";
        case OP_NE: return "!=";
        case OP_LT: return "<";
        case OP_GT: return ">";
        case OP_LE: return "<=";
        case OP_GE: return ">=";
        case OP_AND: return "&&";
        case OP_OR: return "||";
        case OP_BIT_AND: return "&";
        case OP_BIT_OR: return "|";
        case OP_BIT_XOR: return "^";
        case OP_SHL: return "<<";
        case OP_SHR: return ">>";
        default: return "?";
    }
}

const char* unary_op_name(UnaryOp op) {
    switch (op) {
        case OP_NEG: return "-";
        case OP_NOT: return "!";
        case OP_BIT_NOT: return "~";
        default: return "?";
    }
}
