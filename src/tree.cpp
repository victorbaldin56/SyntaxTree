//////////////////////////////////////////////////////////////////////////////
/// @file
///
/// @brief Copyright (c) Victor Baldin, 2023.
///
//////////////////////////////////////////////////////////////////////////////

#include <assert.h>

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

    if (left)  left->parent  = node;
    if (right) right->parent = node;

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
        default: {
            assert(0 && "Unhandled enum value");
        }
    }
}

void TreeNodeDtor(struct TreeNode *node)
{
    TREE_ASSERT(node);

    if (!node) return;

    TreeNodeDtor(node->left);
    TreeNodeDtor(node->right);

    free(node);
}

TreeErrors CheckTree(const struct TreeNode *node)
{
    if (!node) return TREE_OK;

    if (node->left && node->left == node->right) return TREE_CYCLED;

    TreeErrors lefterr  = CheckTree(node->left);
    TreeErrors righterr = CheckTree(node->right);

    if (lefterr != TREE_OK)  return lefterr;
    if (righterr != TREE_OK) return righterr;

    return TREE_OK;
}
