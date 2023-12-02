//////////////////////////////////////////////////////////////////////////////
/// @file
///
/// @brief Copyright (c) Victor Baldin, 2023.
///
//////////////////////////////////////////////////////////////////////////////

#include "expr_parser.h"

#include <assert.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////
/// @defgroup Parsers Parsing functions working together in recursive descent.
/// @{

static struct TreeNode *GetAddN(char **strp);
static struct TreeNode *GetMulN(char **strp);
static struct TreeNode *GetPowN(char **strp);
static struct TreeNode *GetBrac(char **strp);
static inline struct TreeNode *GetNumN(char **strp);

/// @}
//////////////////////////////////////////////////////////////////////////////

struct TreeNode *ParseExpression(char str[])
{
    assert(str);

    return GetAddN(&str);
}

///

static inline TreeOperators GetOp(char op);

static struct TreeNode *GetAddN(char **strp)
{
    assert(strp && *strp);

    struct TreeNode *left   = GetMulN(strp);
    struct TreeNode *opnode = left;

    while (**strp == '-' || **strp == '+') {
        char op = **strp;
        ++*strp;
        struct TreeNode *right = GetMulN(strp);
        opnode                 = TreeNodeCtor(TYPE_OPERATOR, GetOp(op), left,
                                              right);
        if (!opnode || !left || !right) {
            free(opnode);
            free(left);
            free(right);
            return NULL;
        }
    }

    return opnode;
}

static struct TreeNode *GetMulN(char **strp)
{
    assert(strp && *strp);

    struct TreeNode *left   = GetPowN(strp);
    struct TreeNode *opnode = left;

    while (**strp == '/' || **strp == '*') {
        char op = **strp;
        ++*strp;
        struct TreeNode *right = GetPowN(strp);
        opnode                 = TreeNodeCtor(TYPE_OPERATOR, GetOp(op), left,
                                              right);
        if (!opnode || !left || !right) {
            free(opnode);
            free(left);
            free(right);
            return NULL;
        }
    }

    return opnode;
}

static struct TreeNode *GetPowN(char **strp)
{
    assert(strp && *strp);

    struct TreeNode *left   = GetBrac(strp);
    struct TreeNode *opnode = left;

    while (**strp == '^') {
        char op = **strp;
        ++*strp;
        struct TreeNode *right = GetBrac(strp);
        opnode                 = TreeNodeCtor(TYPE_OPERATOR, GetOp(op), left,
                                              right);
        if (!opnode || !left || !right) {
            free(opnode);
            free(left);
            free(right);
            return NULL;
        }
    }

    return opnode;
}

static inline TreeOperators GetOp(char op)
{
    switch (op) {
        case '+': return OP_ADD;
        case '-': return OP_SUB;
        case '*': return OP_MUL;
        case '/': return OP_DIV;
        case '^': return OP_POW;
        default:  assert(0 && "Unknown operator");
    }
}

static TreeNode *GetBrac(char **strp)
{
    assert(strp && *strp);

    if (**strp == '(') {
        ++*strp;
        struct TreeNode *opnode = GetAddN(strp);
        ++*strp;
        return opnode;
    }

    // TODO: syntax errors handling
    return GetNumN(strp);
}

static inline struct TreeNode *GetNumN(char **strp)
{
    assert(strp && *strp);

    TreeNodeNumType val = strtod(*strp, strp);
    return TreeNodeCtor(TYPE_NUMBER, val, NULL, NULL);
}
