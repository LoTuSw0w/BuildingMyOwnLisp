#include "mpc.h"

//count the number of nodes in the abstract syntax tree
int number_of_nodes(mpc_ast_t *t)
{
    if (t->children_num == 0)
        return 1;
    if (t->children_num >= 1)
    {
        int total_nodes = 1;
        for (int i = 0; i < t->children_num; i++)
        {
            total_nodes = total_nodes + number_of_nodes(t->children[i]);
        }
        return total_nodes;
    }
    return 0;
}

//count the number of leaves
int number_of_leaves(mpc_ast_t *t)
{
    int total_leaves = 0;
    if (t->children_num == 0)
    {
        return 1;
    }
    if (t->children_num >= 1)
    {
        for (int i = 0; i < t->children_num; i++)
        {
            total_leaves = total_leaves + number_of_leaves(t->children[i]);
        }
    }
    return total_leaves;
}

//count the number of branches
int number_of_branches(mpc_ast_t *t)
{
    int total_branches = 0;

    if (t->children_num > 0)
    {
        total_branches = total_branches + t->children_num;
        for (int i = 0; i < t->children_num; i++)
        {
            total_branches = total_branches + number_of_branches(t->children[i]);
        }
    }

    return total_branches;
}