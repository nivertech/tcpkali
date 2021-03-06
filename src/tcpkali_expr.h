/*
 * Copyright (c) 2015  Machine Zone, Inc.
 * 
 * Original author: Lev Walkin <lwalkin@machinezone.com>
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.

 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef TCPKALI_EXPR_H
#define TCPKALI_EXPR_H

typedef struct tk_expr {
    enum {
        EXPR_DATA,
        EXPR_CONCAT,
        EXPR_MODULO,            /* '%' */
        EXPR_CONNECTION_PTR,    /* 'connection.ptr' */
        EXPR_CONNECTION_UID,    /* 'connection.uid' */
    } type;
    union {
        struct {
            const char *data;
            size_t      size;
        } data;
        struct {
            struct tk_expr *expr;           /* Expression */
            long            modulo_value;   /* '... % 42' => 42 */
        } modulo;
        struct {
            struct tk_expr *expr[2];
        } concat;
    } u;
    size_t estimate_size;
} tk_expr_t;

/*
 * Trivial expression is expression which does not contain any
 * interesting (dynamically computable) values and is basically
 * a constant string.
 */
#define EXPR_IS_TRIVIAL(e)  ((e)->type == EXPR_DATA)

/*
 * Return values:
 *  0: No expression was found, returns a simple EXPR_DATA node.
 *  1: An expression was found, returned.
 * -1: Expression parsing failed.
 */
int parse_expression(tk_expr_t **, const char *expr_buf, size_t size, int debug);

void free_expression(tk_expr_t *expr);


typedef ssize_t (expr_callback_f)(char *buf, size_t size, tk_expr_t *, void *key, long *output_value);
ssize_t eval_expression(char **buf_p, size_t size, tk_expr_t *, expr_callback_f, void *key, long *output_value);

#endif  /* TCPKALI_EXPR_H */
