/* LAMC Compiler - Abstract Syntax Tree
 * Production-ready AST node definitions
 * Copyright (c) 2025 Naveen Singh
 */

#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <stdbool.h>
#include "../lexer/token.h"

/* AST Node Types */
typedef enum {
    /* Expressions */
    AST_BINARY_EXPR,
    AST_UNARY_EXPR,
    AST_LITERAL_EXPR,
    AST_IDENTIFIER_EXPR,
    AST_CALL_EXPR,
    AST_INDEX_EXPR,
    AST_MEMBER_EXPR,
    AST_ARRAY_EXPR,
    AST_DICT_EXPR,
    
    /* Statements */
    AST_VAR_DECL,
    AST_ASSIGN_STMT,
    AST_EXPR_STMT,
    AST_IF_STMT,
    AST_WHILE_STMT,
    AST_FOR_STMT,
    AST_LOOP_STMT,
    AST_RETURN_STMT,
    AST_BREAK_STMT,
    AST_CONTINUE_STMT,
    AST_BLOCK_STMT,
    
    /* Declarations */
    AST_FUNCTION_DECL,
    AST_CLASS_DECL,
    AST_IMPORT_STMT,
    
    /* Program */
    AST_PROGRAM
} AstNodeType;

/* Forward declarations */
typedef struct AstNode AstNode;
typedef struct AstList AstList;

/* Binary operators */
typedef enum {
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE,
    OP_AND, OP_OR,
    OP_BIT_AND, OP_BIT_OR, OP_BIT_XOR,
    OP_SHL, OP_SHR
} BinaryOp;

/* Unary operators */
typedef enum {
    OP_NEG, OP_NOT, OP_BIT_NOT
} UnaryOp;

/* Literal types */
typedef enum {
    LIT_INT,
    LIT_FLOAT,
    LIT_STRING,
    LIT_BOOL,
    LIT_NULL
} LiteralType;

/* Literal value */
typedef struct {
    LiteralType type;
    union {
        long int_value;
        double float_value;
        char* string_value;
        bool bool_value;
    } as;
} Literal;

/* Binary expression */
typedef struct {
    BinaryOp op;
    AstNode* left;
    AstNode* right;
} BinaryExpr;

/* Unary expression */
typedef struct {
    UnaryOp op;
    AstNode* operand;
} UnaryExpr;

/* Call expression */
typedef struct {
    AstNode* callee;
    AstList* arguments;
} CallExpr;

/* Index expression */
typedef struct {
    AstNode* object;
    AstNode* index;
} IndexExpr;

/* Member expression */
typedef struct {
    AstNode* object;
    char* member;
} MemberExpr;

/* Array expression */
typedef struct {
    AstList* elements;
} ArrayExpr;

/* Dictionary entry */
typedef struct {
    AstNode* key;
    AstNode* value;
} DictEntry;

/* Dictionary expression */
typedef struct {
    AstList* entries;  /* List of DictEntry* */
} DictExpr;

/* Variable declaration */
typedef struct {
    char* name;
    char* type_name;  /* Optional type annotation */
    AstNode* initializer;
} VarDecl;

/* Assignment statement */
typedef struct {
    AstNode* target;  /* Can be identifier, index, or member */
    AstNode* value;
} AssignStmt;

/* If statement */
typedef struct {
    AstNode* condition;
    AstNode* then_branch;
    AstNode* else_branch;  /* NULL if no else */
} IfStmt;

/* While statement */
typedef struct {
    AstNode* condition;
    AstNode* body;
} WhileStmt;

/* For statement */
typedef struct {
    char* variable;
    AstNode* iterable;
    AstNode* body;
    char* index_var;  /* For "for i, item in array" - NULL if not used */
} ForStmt;

/* Loop statement (infinite loop) */
typedef struct {
    AstNode* body;
} LoopStmt;

/* Return statement */
typedef struct {
    AstNode* value;  /* NULL for empty return */
} ReturnStmt;

/* Block statement */
typedef struct {
    AstList* statements;
} BlockStmt;

/* Function parameter */
typedef struct {
    char* name;
    char* type_name;  /* Optional */
    AstNode* default_value;  /* Optional */
} Parameter;

/* Function declaration */
typedef struct {
    char* name;
    AstList* parameters;  /* List of Parameter* */
    AstNode* body;
    char* return_type;  /* Optional */
} FunctionDecl;

/* Class declaration */
typedef struct {
    char* name;
    AstList* methods;
    AstList* fields;
} ClassDecl;

/* Import statement */
typedef struct {
    char* module_name;
} ImportStmt;

/* Program (root node) */
typedef struct {
    AstList* declarations;
} Program;

/* Main AST Node structure */
struct AstNode {
    AstNodeType type;
    int line;
    int column;
    
    union {
        BinaryExpr binary;
        UnaryExpr unary;
        Literal literal;
        char* identifier;
        CallExpr call;
        IndexExpr index;
        MemberExpr member;
        ArrayExpr array;
        DictExpr dict;
        VarDecl var_decl;
        AssignStmt assign;
        AstNode* expr_stmt;
        IfStmt if_stmt;
        WhileStmt while_stmt;
        ForStmt for_stmt;
        LoopStmt loop_stmt;
        ReturnStmt return_stmt;
        BlockStmt block;
        FunctionDecl function;
        ClassDecl class_decl;
        ImportStmt import;
        Program program;
    } as;
};

/* Dynamic array for AST nodes */
struct AstList {
    void** items;
    size_t count;
    size_t capacity;
};

/* AST List functions */
AstList* ast_list_create(void);
void ast_list_append(AstList* list, void* item);
void ast_list_free(AstList* list);

/* AST Node constructors */
AstNode* ast_create_binary(BinaryOp op, AstNode* left, AstNode* right, int line, int col);
AstNode* ast_create_unary(UnaryOp op, AstNode* operand, int line, int col);
AstNode* ast_create_literal_int(long value, int line, int col);
AstNode* ast_create_literal_float(double value, int line, int col);
AstNode* ast_create_literal_string(const char* value, int line, int col);
AstNode* ast_create_literal_bool(bool value, int line, int col);
AstNode* ast_create_literal_null(int line, int col);
AstNode* ast_create_identifier(const char* name, int line, int col);
AstNode* ast_create_call(AstNode* callee, AstList* args, int line, int col);
AstNode* ast_create_index(AstNode* object, AstNode* index, int line, int col);
AstNode* ast_create_member(AstNode* object, const char* member, int line, int col);
AstNode* ast_create_array(AstList* elements, int line, int col);
AstNode* ast_create_dict(AstList* entries, int line, int col);

AstNode* ast_create_var_decl(const char* name, const char* type, AstNode* init, int line, int col);
AstNode* ast_create_assign(AstNode* target, AstNode* value, int line, int col);
AstNode* ast_create_expr_stmt(AstNode* expr, int line, int col);
AstNode* ast_create_if(AstNode* cond, AstNode* then_br, AstNode* else_br, int line, int col);
AstNode* ast_create_while(AstNode* cond, AstNode* body, int line, int col);
AstNode* ast_create_for(const char* var, AstNode* iter, AstNode* body, const char* idx, int line, int col);
AstNode* ast_create_loop(AstNode* body, int line, int col);
AstNode* ast_create_return(AstNode* value, int line, int col);
AstNode* ast_create_break(int line, int col);
AstNode* ast_create_continue(int line, int col);
AstNode* ast_create_block(AstList* stmts, int line, int col);

AstNode* ast_create_function(const char* name, AstList* params, AstNode* body, const char* ret_type, int line, int col);
AstNode* ast_create_class(const char* name, AstList* methods, AstList* fields, int line, int col);
AstNode* ast_create_import(const char* module, int line, int col);
AstNode* ast_create_program(AstList* decls);

Parameter* ast_create_parameter(const char* name, const char* type, AstNode* default_val);
DictEntry* ast_create_dict_entry(AstNode* key, AstNode* value);

/* AST Node destructor */
void ast_free_node(AstNode* node);
void ast_free_parameter(Parameter* param);
void ast_free_dict_entry(DictEntry* entry);

/* AST utilities */
const char* ast_node_type_name(AstNodeType type);
const char* binary_op_name(BinaryOp op);
const char* unary_op_name(UnaryOp op);

/* AST pretty printer */
void ast_print(AstNode* node, int indent);
void ast_print_program(AstNode* program);

#endif /* AST_H */
