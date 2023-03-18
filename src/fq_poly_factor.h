/*
    Copyright (C) 2013 Mike Hansen

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#ifndef FQ_POLY_FACTOR_H
#define FQ_POLY_FACTOR_H

#ifdef FQ_POLY_FACTOR_INLINES_C
#define FQ_POLY_FACTOR_TEMPLATES_INLINE
#define FQ_POLY_FACTOR_INLINE
#else
#define FQ_POLY_FACTOR_TEMPLATES_INLINE static __inline__
#define FQ_POLY_FACTOR_INLINE static __inline__
#endif

#include "thread_pool.h"
#include "ulong_extras.h"
#include "fq_vec.h"
#include "fq_poly.h"

FQ_POLY_FACTOR_INLINE
int FQ_POLY_ITERATED_FROBENIUS_CUTOFF(const fq_ctx_t ctx, slong length)
{
    int result;
    fmpz_t q;
    fmpz_init(q);
    fq_ctx_order(q, ctx);
    if ( fmpz_sizeinbase(q, 2) < 3 * (n_sqrt(length) + 1))
        result = 1;
    else
        result = 0;
    fmpz_clear(q);
    return result;
}

#ifdef T
#undef T
#endif

#define T fq
#define CAP_T FQ
#include "fq_poly_factor_templates.h"
#undef CAP_T
#undef T

/* Inlines *******************************************************************/

void fq_poly_factor_get_poly(fq_poly_t z,
		      const fq_poly_factor_t fac, slong i, const fq_ctx_t ctx);

#endif
