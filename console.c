#include <stdio.h> // define the header file
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "exercise.h"
#include "mpc.h"

/* Declare a buffer for user input of size 2048 */
static char input[2048];

//function to recognize the operation
long eval_op(long x, char *op, long y)
{
    if (strcmp(op, "+") == 0) return x + y;
    if (strcmp(op, "-") == 0) return x - y;
    if (strcmp(op, "*") == 0) return x * y;
    if (strcmp(op, "/") == 0) return x / y;
    if (strcmp(op, "%") == 0) return x % y;
    if (strcmp(op, "^") == 0) return pow(x,y);
    return 0;
}

//evaluation function
long eval(mpc_ast_t *t)
{
    //return immediately if it's a number
    if (strstr(t->tag, "number"))
    {
        return atoi(t->contents);
    }

    //get the operator since the second child must be an operator
    char *op = t->children[1]->contents;

    //store the 3rd child in this variable "x"
    long x = eval(t->children[2]);

    //use recursion to iterate through the remaining children
    int i = 3;
    while (strstr(t->children[i]->tag, "expr"))
    {
        x = eval_op(x, op, eval(t->children[i]));
        i++;
    }
    return x;
}


int main()
{
    /* Create Some Parsers */
    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Operator = mpc_new("operator");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Lispy = mpc_new("lispy");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT,
              "                                                     \
      number   : /-?[0-9]+/ ;                             \
      operator : '+' | '-' | '*' | '/' | '%' | '^';                  \
      expr     : <number> | '(' <operator> <expr>+ ')' ;  \
      lispy    : /^/ <operator> <expr>+ /$/ ;             \
    ",
              Number, Operator, Expr, Lispy);

    puts("Homemade console version 0.0.1");
    puts("Press Ctrl + C to exit this console!");

    while (1)
    {
        //print the prompt
        fputs("> ", stdout);

        //record the input
        fgets(input, 2048, stdin);

        /* Attempt to parse the user input */
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r))
        {
            //evaluate the result
            long result = eval(r.output);

            //calculate the number of nodes and leaves respectively
            long numberOfNodes = number_of_nodes(r.output);
            long numberOfLeaves = number_of_leaves(r.output);
            long numberOfBranches = number_of_branches(r.output);
            long mostNumberOfChildren = most_num_of_children_from_branch(r.output);

            //print the result, the numbers of nodes and leaves
            printf("The number of nodes are %li\n", numberOfNodes);
            printf("The number of leaves are %li\n", numberOfLeaves);
            printf("The number of branches are %li\n", numberOfBranches);
            printf("The most number of children from a branch are %li\n", mostNumberOfChildren);
            printf("\n\nThe result is: %li\n", result);

            //clear the tree
            mpc_ast_delete(r.output);
        }
        else
        {
            /* Otherwise print and delete the Error */
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
    }
    
    return 0;
}