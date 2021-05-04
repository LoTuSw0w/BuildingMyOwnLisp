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
            total_nodes += number_of_nodes(t->children[i]);
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
            total_leaves += number_of_leaves(t->children[i]);
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
            total_branches += number_of_branches(t->children[i]);
        }
    }

    return total_branches;
}

//get the most number of children spanning from a branch
int most_num_of_children_from_branch(mpc_ast_t *t)
{
    if (t->children_num == 0)
        return 1;
    if (t->children_num >= 1)
    {
        int most_number_node_from_branch = 0;
        for (int i = 0; i < t->children_num; i++)
        {
            int total_nodes = 1;
            total_nodes += number_of_nodes(t->children[i]);
            if (total_nodes > most_number_node_from_branch)
            {
                most_number_node_from_branch = total_nodes;
            }
        }
        return most_number_node_from_branch;
    }
    return 0;
}