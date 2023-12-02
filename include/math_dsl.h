//////////////////////////////////////////////////////////////////////////////
/// @file
///
/// @brief Copyright (c) Victor Baldin, 2023.
///
//////////////////////////////////////////////////////////////////////////////

#ifndef MATH_DSL_H_
#define MATH_DSL_H_

#include "dump_tree.h"
#include "tree.h"

inline struct TreeNode *CreateOpNode(TreeOperators op,
                                     TreeNodeNumType leftval,
                                     TreeNodeNumType rightval)
{
    struct TreeNode *left  = TreeNodeCtor(TYPE_NUMBER, leftval, NULL, NULL);
    struct TreeNode *right = TreeNodeCtor(TYPE_NUMBER, rightval, NULL, NULL);

    if (!left || !right) {
        free(left);
        free(right);
        return NULL;
    }

    struct TreeNode *opnode = TreeNodeCtor(TYPE_OPERATOR, op, left, right);
    return opnode;
}

#endif // MATH_DSL_H_
