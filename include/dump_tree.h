//////////////////////////////////////////////////////////////////////////////
/// @file
///
/// @brief Copyright (C) Victor Baldin, 2023.
///
//////////////////////////////////////////////////////////////////////////////

#ifndef DUMP_TREE_H_
#define DUMP_TREE_H_

#include <assert.h>
#include <stdio.h>

#include "tree.h"

/// @brief A structure for storing current place of call and a variable name.

struct CallInfo {
    const char *file;           ///< source file
    const char *func;           ///<
    size_t line;                ///< source code line
    const char *node_name;      ///< the name of variable
};

//////////////////////////////////////////////////////////////////////////////
/// @brief Dumps tree as a visual graph with GraphViz
///
/// @param node         A tree node to begin dump from.
/// @param state        Current state of a tree, @see CheckTree.
/// @param file         A source code file from which this function called.
/// @param func         A function from which this function called.
/// @param line         A source code line from which this function called.
/// @param node_name    The name of passed variable.
/// @param src_path     The path to a source file for GrapViz.
///
//////////////////////////////////////////////////////////////////////////////

void GraphTreeDump(const struct TreeNode *node, TreeErrors state,
                   struct CallInfo info, const char src_path[]);

#ifndef NDEBUG
#define TREE_DUMP(node) GraphTreeDump(node, CheckTree(node), {__FILE__, \
                                      __func__, __LINE__, #node}, NULL);
#else
#define TREE_DUMP(node)
#endif

#ifndef NDEBUG
#define TREE_ASSERT(node) \
    do { \
        TreeErrors error = CheckTree(node); \
        if (error != TREE_OK) { \
            GraphTreeDump(node, error, {__FILE__, __func__, __LINE__, \
                          #node}, NULL); \
            abort(); \
        } \
    } while (0)

#else
#define TREE_ASSERT(node)
#endif

#endif // DUMP_TREE_H_
