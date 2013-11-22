/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 Mike Hansen

******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "fq.h"
#include "fq_poly.h"

void
fq_ctx_init_modulus(fq_ctx_t ctx, const fmpz_t p, slong d,
                    fmpz_mod_poly_t modulus, const char *var)
{
    slong nz;
    int i, j;

    /* Count number of nonzero coefficients */
    nz = 0;
    for (i = 0; i < modulus->length; i++)
    {
        if (!fmpz_is_zero(modulus->coeffs + i))
        {
            nz += 1;
        }
    }

    ctx->len = nz;
    ctx->a = _fmpz_vec_init(ctx->len);
    ctx->j = flint_malloc(ctx->len * sizeof(slong));

    /* Copy the polynomial */
    j = 0;
    for (i = 0; i < modulus->length; i++)
    {
        if (!fmpz_is_zero(modulus->coeffs + i))
        {
            fmpz_set(ctx->a + j, modulus->coeffs + i);
            ctx->j[j] = i;
            j++;
        }
    }

    if (ctx->len < 6)
        ctx->sparse_modulus = 1;
    else
        ctx->sparse_modulus = 0;

    fmpz_init_set(fq_ctx_prime(ctx), p);

    ctx->var = flint_malloc(strlen(var) + 1);
    strcpy(ctx->var, var);

    /* Precompute the inverse of the modulus */
    fmpz_mod_poly_init(ctx->inv, fq_ctx_prime(ctx));
    fmpz_mod_poly_make_monic(ctx->inv, modulus);
    fmpz_mod_poly_reverse(ctx->inv, ctx->inv, ctx->inv->length);
    fmpz_mod_poly_inv_series_newton(ctx->inv, ctx->inv, ctx->inv->length);

    fmpz_mod_poly_init(ctx->modulus, fq_ctx_prime(ctx));
    fmpz_mod_poly_set(ctx->modulus, modulus);
}
