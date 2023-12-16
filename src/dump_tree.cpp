//////////////////////////////////////////////////////////////////////////////
/// @file
///
/// @brief Copyright (C) Victor Baldin, 2023.
///
//////////////////////////////////////////////////////////////////////////////

#include "dump_tree.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <time.h>

//============================================================================

//////////////////////////////////////////////////////////////////////////////
/// @defgroup DumpInfo Methods for printing debug info.
/// @{

static inline void PrintGraphHeader(FILE *output, struct CallInfo info,
                                    TreeErrors state);
static inline void PrintCallInfo(FILE *output, struct CallInfo info);

/// @}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// @defgroup Tree Methods for printing tree.
/// @{

static void PrintTree(FILE *output, const struct TreeNode *node);
static inline void PrintTreeNode(FILE *output, const struct TreeNode *node);

/// @}
//////////////////////////////////////////////////////////////////////////////

const size_t MAX_CMD_SIZE = 10000; ///< max command size for calling GraphViz

static inline void RenderGraph(FILE *output, const char src_path[]);

#define CALL_INFO_ASSERT(info) assert(info.file && info.func)

/// @warning Remember that @code src_path @endcode is @code PATH_MAX @endcode
///          size.

static inline void NameLogFile(char src_path[]);

static inline void PrintState(FILE *output, TreeErrors state);

void GraphTreeDump(const struct TreeNode *node, TreeErrors state,
                   struct CallInfo info, const char src_path[])
{
    assert(node);
    CALL_INFO_ASSERT(info);

    char default_path[PATH_MAX] = {};

    if (!src_path) {
        system("mkdir -p logs");
        NameLogFile(default_path);
        src_path = default_path;
    }

    FILE *output = fopen(src_path, "w");
    if (!output) {
        perror("GraphTreeDump");
        PrintCallInfo(stderr, info);
        return;
    }

    PrintGraphHeader(output, info, state);
    PrintTree(output, node);
    RenderGraph(output, src_path);
}

static inline void NameLogFile(char src_path[])
{
    assert(src_path);

    time_t now = time(NULL);
    strftime(src_path, PATH_MAX, "logs/%Y-%m-%d_%H-%M-%S.dot", localtime(&now));
}

static inline void PrintGraphHeader(FILE *output, struct CallInfo info,
                                    TreeErrors state)
{
    assert(output);
    CALL_INFO_ASSERT(info);

    fprintf(output, "digraph G {\n"
                    "    label = \"");
    PrintCallInfo(output, info);
    PrintState(output, state);
    fprintf(output, "\";\n");
}

static inline void PrintState(FILE *output, TreeErrors state)
{
    assert(output);

    if (state != TREE_OK) {
        // TODO: more detailed description
        fprintf(output, "; error in tree detected");
    } else {
        fprintf(output, "; no errors detected");
    }
}

static inline void PrintCallInfo(FILE *output, struct CallInfo info)
{
    assert(output);
    CALL_INFO_ASSERT(info);

    fprintf(output, "%s from file %s, line %zu, function %s",
                    info.node_name, info.file, info.line, info.func);
}

#undef CALL_INFO_ASSERT

static void PrintTree(FILE *output, const struct TreeNode *node)
{
    assert(output);

    if (!node) return;

    PrintTreeNode(output, node);

    PrintTree(output, node->left);
    PrintTree(output, node->right);
}

static inline void PrintTreeNode(FILE *output, const struct TreeNode *node)
{
    assert(output);
    assert(node);

    fprintf(output, "    n%p [shape = Mrecord, style = filled, fillcolor = ",
                    node);

    switch (node->type) {
        case TYPE_NUMBER: {
            fprintf(output, "cyan, label = \"" TREE_NODE_NUM_FORMAT,
                            node->data.num);
            break;
        }
        case TYPE_OPERATOR: {
            fprintf(output, "orange, label = \"%s", OP_SYMBOLS[node->data.op]);
            break;
        }
        case TYPE_VARIABLE: {
            fprintf(output, "green, label = \"%c", node->data.varname);
            break;
        }
        default: {
            assert(0 && "Unhandled enum value");
        }
    }

    fprintf(output, "\"];\n");

    if (node->parent) fprintf(output, "    n%p -> n%p\n", node->parent, node);
}

static inline void RenderGraph(FILE *output, const char src_path[])
{
    assert(output);
    assert(src_path);

    fprintf(output, "}\n");
    fclose(output);

    char *cmd = (char *)calloc(MAX_CMD_SIZE, sizeof(*cmd));
    snprintf(cmd, MAX_CMD_SIZE, "dot -T svg %s -o %s.svg",
                                src_path, src_path);
    system(cmd);
    free(cmd);
}
