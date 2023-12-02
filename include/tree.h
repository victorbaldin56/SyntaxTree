//////////////////////////////////////////////////////////////////////////////
/// @file
///
/// @brief Copyright (c) Victor Baldin, 2023.
///
//////////////////////////////////////////////////////////////////////////////

#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>

/// A enum for math operators.

enum TreeOperators {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
};

const char TREE_OPERATORS[] = "+-*/^"; ///< a kind of name table

/// Type of numeric values containing in TreeNode.

typedef double TreeNodeNumType;
#define TREE_NODE_NUM_FORMAT "%lf"

/// @brief Contains a double or operator data depending on node type.

union TreeNodeData {
    TreeNodeNumType num;    ///<
    TreeOperators op;       ///< operator in case of operator type
};

enum TreeNodeType {
    TYPE_NUMBER,
    TYPE_OPERATOR,
};

//////////////////////////////////////////////////////////////////////////////
/// @brief A node structure.
///
///
//////////////////////////////////////////////////////////////////////////////

struct TreeNode {
    TreeNodeType type;              ///<
    union TreeNodeData data;        ///< data of different types
    struct TreeNode *left;          ///< pointer to the left child
    struct TreeNode *right;         ///< pointer to the right child
    struct TreeNode *parent;        ///<
};

//////////////////////////////////////////////////////////////////////////////
/// @brief A constructor.
///
/// @param data A data to set in the node.
/// @param type  A type to set in the node.
/// @param left  A pointer to the left child.
/// @param right A ponter to the right child.
///
//////////////////////////////////////////////////////////////////////////////

struct TreeNode *TreeNodeCtor(TreeNodeType type, TreeNodeNumType value,
                              struct TreeNode *left, struct TreeNode *right);

//////////////////////////////////////////////////////////////////////////////
/// @brief A destructor.
///
//////////////////////////////////////////////////////////////////////////////

void TreeNodeDtor(struct TreeNode *node);

/// Possible errors in tree configuration.

enum TreeErrors {
    TREE_OK,
    TREE_CYCLED,
};

/// @brief A verificator.

TreeErrors CheckTree(const struct TreeNode *node);

//////////////////////////////////////////////////////////////////////////////
/// @brief Evaluates an arithmetic expression represented as tree.
///
/// @param node Node to evaluate.
/// @return A node value.
//////////////////////////////////////////////////////////////////////////////

TreeNodeNumType EvalTree(const struct TreeNode *node);

#endif // TREE_H_
