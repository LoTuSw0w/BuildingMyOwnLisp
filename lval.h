// enumeration values to represent possible lval types
enum
{
    LVAL_NUM,
    LVAL_ERR
};

// enumeration values to represent possible error types
enum
{
    LERR_DIV_ZERO,
    LERR_MOD_ZERO,
    LERR_BAD_OP,
    LERR_BAD_NUM
};

// structure for lval (lisp value)
typedef struct
{
    int type;
    long num;
    int err;
} lval;


