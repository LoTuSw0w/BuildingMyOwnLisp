#include <stdio.h> // define the header file
#include <stdlib.h>
#include "mpc.h"

/* Declare a buffer for user input of size 2048 */
static char input[2048];

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
      operator : '+' | '-' | '*' | '/' ;                  \
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
            /* On success print and delete the AST */
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        }
        else
        {
            /* Otherwise print and delete the Error */
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
    }

    free(input);

    return 0;
}