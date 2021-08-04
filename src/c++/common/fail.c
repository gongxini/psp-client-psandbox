// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include <psp/fail.h>

#include <psp/annot.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>

static void default_onfail(int error_arg,
      const char *expr_arg, const char *fnn_arg, const char *filen_arg,
      int lineno_arg);

static psp_onfail_t current_onfail = &default_onfail;

void psp_panic(const char *why_arg, const char *filen_arg, int lineno_arg) {
    if (NULL == why_arg) {
        why_arg = "*unspecified*";
    }

    if (NULL == filen_arg) {
        filen_arg = "*unspecified*";
    }

    /* there's really no point in checking the return code of fprintf().
     * if it fails, i don't have a backup plan for informing the
     * operator. */
    fprintf(stderr, "*** panic in line %d of `%s`: %s\n", lineno_arg, filen_arg, why_arg);
    abort();
}

void psp_onfail(psp_onfail_t onfail_arg) {
    if (NULL == onfail_arg) {
        current_onfail = &default_onfail;
    } else {
        current_onfail = onfail_arg;
    }
}

void psp_fail(int error_arg, const char *expr_arg,
      const char *fnn_arg, const char *filen_arg, int lineno_arg) {
   current_onfail(error_arg, expr_arg, fnn_arg, filen_arg,
         lineno_arg);
}

void default_onfail(int error_arg, const char *expr_arg,
    const char *fnn_arg, const char *filen_arg, int lineno_arg) {
    int n = -1;
    pid_t tid;
#ifdef SYS_gettid
    tid = syscall(SYS_gettid);
#else
    tid = 0;
#endif

    if (0 == error_arg) {
        PSP_PANIC("attempt to fail with a success code.");
    }

    /* to my knowledge, Windows doesn't support providing the function name,
     * so i need to tolerate a NULL value for fnn_arg. */
    const char *err_msg = NULL;
    if (error_arg > 0) {
        err_msg = strerror(error_arg);
    } else {
        err_msg = "error message is undefined";
    }

    if (NULL == fnn_arg) {
        n = fprintf(stderr, "[%d] FAIL (%d => %s) at %s, line %d: %s\n", tid, error_arg, err_msg,
                filen_arg, lineno_arg, expr_arg);
        if (n < 1) {
            PSP_PANIC("fprintf() failed.");
        }
        psp_panic("Aborting the program.", filen_arg, lineno_arg);
    } else {
        n = fprintf(stderr, "[%d] FAIL (%d => %s) in %s, at %s, line %d: %s\n", tid, error_arg, err_msg,
                fnn_arg, filen_arg, lineno_arg, expr_arg);
        if (n < 1) {
            PSP_PANIC("fprintf() failed.");
        }
   }
}
