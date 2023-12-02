//////////////////////////////////////////////////////////////////////////////
/// @file
///
/// @brief Copyright (C) Victor Baldin, 2023.
///
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "dump_tree.h"
#include "math_dsl.h"
#include "tree.h"

int main()
{
    struct TreeNode *plus = CreateOpNode(OP_ADD, 10.8, 10);
    struct TreeNode *mul  = CreateOpNode(OP_MUL, 1, 2);
    struct TreeNode *node = TreeNodeCtor(TYPE_OPERATOR, OP_DIV, plus, mul);

    TREE_DUMP(node);

    TreeNodeDtor(node);
    return 0;
}
