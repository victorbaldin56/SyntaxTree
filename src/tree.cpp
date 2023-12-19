//////////////////////////////////////////////////////////////////////////////
/// @file
///
/// @brief Copyright (c) Victor Baldin, 2023.
///
//////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <math.h>

#include "dump_tree.h"

static inline void SetNodeData(TreeNodeType type, TreeNodeNumType value,
                               union TreeNodeData *data);

struct TreeNode *TreeNodeCtor(TreeNodeType type, TreeNodeNumType value,
                              struct TreeNode *left, struct TreeNode *right)
{
    struct TreeNode *node = (struct TreeNode *)calloc(1, sizeof(*node));
    if (!node)
        return NULL;

    SetNodeData(type, value, &node->data);
    node->type  = type;
    node->left  = left;
    node->right = right;

    if (left)
        left->parent  = node;
    if (right)
        right->parent = node;

    return node;
}

static inline void SetNodeData(TreeNodeType type, TreeNodeNumType value,
                               union TreeNodeData *data)
{
    assert(data);

    switch (type) {
        case TYPE_NUMBER: {
            data->num = value;
            break;
        }
        case TYPE_OPERATOR: {
            data->op = (TreeOperators)value;
            break;
        }
        case TYPE_VARIABLE: {
            data->varname = (char)value;
            break;
        }
        default: {
            assert(0 && "Unhandled enum value");
        }
    }
}

void TreeNodeDtor(struct TreeNode *node)
{
    TREE_ASSERT(node);

    if (!node)
        return;
    TreeNodeDtor(node->left);
    TreeNodeDtor(node->right);
    free(node);
}

TreeErrors CheckTree(const struct TreeNode *node)
{
    if (!node)
        return TREE_OK;

    if (node->left && node->left == node->right)
        return TREE_CYCLED;

    TreeErrors lefterr  = CheckTree(node->left);
    TreeErrors righterr = CheckTree(node->right);

    if (lefterr != TREE_OK)
        return lefterr;
    if (righterr != TREE_OK)
        return righterr;
    return TREE_OK;
}

/// Calculates a node with operator.
/// @warning Only node containing operator (NOT A NUMBER) may be passed

static inline TreeNodeNumType CalculateOpNode(const struct TreeNode *node,
                                              TreeNodeNumType var);

TreeNodeNumType EvalTree(const struct TreeNode *node, TreeNodeNumType var)
{
    TREE_ASSERT(node);

    switch (node->type) {
        case TYPE_NUMBER:
            return node->data.num;
        case TYPE_OPERATOR:
            return CalculateOpNode(node, var);
        case TYPE_VARIABLE:
            return var;
        default:
            assert(0 && "Unhandled enum value");
    }
}

static inline TreeNodeNumType CalculateOpNode(const struct TreeNode *node,
                                              TreeNodeNumType var)
{
    TREE_ASSERT(node);
    assert(node);

    switch (node->data.op) {
        case OP_ADD:
            return EvalTree(node->left, var) + EvalTree(node->right, var);
        case OP_SUB:
            return EvalTree(node->left, var) - EvalTree(node->right, var);
        case OP_MUL:
            return EvalTree(node->left, var) * EvalTree(node->right, var);
        case OP_DIV:
            return EvalTree(node->left, var) / EvalTree(node->right, var);
        case OP_POW:
            return pow(EvalTree(node->left, var), EvalTree(node->right, var));
        case OP_LN:
            return log(EvalTree(node->left, var));
        default:
            assert(0 && "Unhandled enum value");
    }
}
