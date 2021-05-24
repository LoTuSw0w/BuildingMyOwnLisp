#include <stdio.h> // define the header file
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "exercise.h"
#include "mpc.h"
#include "lval.h"


/* Declare a buffer for user input of size 2048 */
static char input[2048];

////////////////////////////////////
/*ERROR HANDLING*/
////////////////////////////////////

// create a new lval number
lval lval_num(long x)
{
    lval v;
    v.type = LVAL_NUM;
    v.num = x;
    return v;
}

// create a new lval error
lval lval_err(int x)
{
    lval v;
    v.type = LVAL_ERR;
    v.err = x;
    return v;
}

void lval_print(lval v)
{
    switch (v.type)
    {
    case LVAL_NUM:
        printf("%li", v.num);
        break;
    case LVAL_ERR:
        if (v.err == LERR_DIV_ZERO)
        {
            puts("Error: Division by zero");
        }
        if (v.err == LERR_BAD_NUM)
        {
            puts("Error: Invalid number");
        }
        if (v.err == LERR_BAD_OP)
        {
            puts("Error: Invalid operator");
        }
        if(v.err == LERR_MOD_ZERO)
        {
            puts("Error: Modulo by zero");
        }
        break;

    default:
        break;
    }
}

void lval_println(lval v)
{
    printf("\nResult: ");
    lval_print(v);
    puts("\n");
}

//function to recognize the operation
lval eval_op(lval x, char *op, lval y)
{
    //if either x or y is an error then just return it
    if (x.type == LVAL_ERR)
        return x;
    if (y.type == LVAL_ERR)
        return y;

    //do these operations
    if (strcmp(op, "+") == 0)
        return lval_num(x.num + y.num);
    if (strcmp(op, "-") == 0)
        return lval_num(x.num - y.num);
    if (strcmp(op, "*") == 0)
        return lval_num(x.num * y.num);
    if (strcmp(op, "/") == 0)
    {
        return y.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x.num / y.num);
    }
    if (strcmp(op, "%") == 0)
    {
        return y.num == 0 ? lval_err(LERR_MOD_ZERO) : lval_num(x.num % y.num);
    }
        
    if (strcmp(op, "^") == 0)
        return lval_num(pow(x.num, y.num));
    if (strcmp(op, "max") == 0)
        return x.num > y.num ? x : y;
    if (strcmp(op, "min") == 0)
        return x.num < y.num ? x : y;

    //if user enter a invalid operator then this is the default case
    return lval_err(LERR_BAD_OP);
}

//evaluation function
lval eval(mpc_ast_t *t)
{
    //return immediately if it's a number
    if (strstr(t->tag, "number"))
    {
        long x = atoi(t->contents);
        return lval_num(x);
    }

    //get the operator since the second child must be an operator
    char *op = t->children[1]->contents;

    //store the 3rd child in this variable "x"
    lval x = eval(t->children[2]);

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
              "                                                                         \
      number   : /-?[0-9]+/ ;                                                           \
      operator : '+' | '-' | '*' | '/' | '%' | '^' | \"min\" | \"max\";                 \
      expr     : <number> | '(' <operator> <expr>+ ')' ;                                \
      lispy    : /^/ <operator> <expr>+ /$/ ;                                           \
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
            lval result = eval(r.output);

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
            

            lval_println(result);

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