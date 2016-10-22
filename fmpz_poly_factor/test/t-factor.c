/*
    Copyright (C) 2010 Andy Novocin
    Copyright (C) 2016 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include "flint.h"
#include "fmpz_poly.h"

#define LONG_FAC_TEST 0 /* run an extra long test */

#if LONG_FAC_TEST
#define FAC_MULT 5
#else
#define FAC_MULT 1
#endif

int
main(void)
{
    int i, result;
    FLINT_TEST_INIT(state);

    flint_printf("factor....");
    fflush(stdout);

    for (i = 0; i < 100 * flint_test_multiplier(); i++)
    {
        fmpz_t c;
        fmpz_poly_t f, g, h, t;
        fmpz_poly_factor_t fac;
        slong j, k, n = n_randint(state, 10*FAC_MULT);
        slong facs1 = 0, facs2 = 0;

        fmpz_init(c);
        fmpz_poly_init(f);
        fmpz_poly_init(g);
        fmpz_poly_init(h);
        fmpz_poly_init(t);
        fmpz_poly_factor_init(fac);

        fmpz_randtest_not_zero(c, state, n_randint(state, 10) + 1);
        fmpz_poly_set_fmpz(f, c);

        for (j = 0; j < n; j++)
        {
            do {
               fmpz_poly_randtest(g, state, n_randint(state, 10) + 2, n_randint(state, 100));
            } while (g->length == 0);
            k = 0;
            while (k < g->length && fmpz_is_zero(g->coeffs + k))
               k++;
            facs1 += k; /* count powers of x separately */
            if (k < g->length - 1)
               facs1++; /* rough lower bound of factors of f */
            fmpz_poly_mul(f, f, g);
        }

        fmpz_poly_factor(fac, f);

        fmpz_poly_set_fmpz(h, &fac->c);
        for (j = 0; j < fac->num; j++)
        {
            if (fac->exp[j] == 1)
                fmpz_poly_mul(h, h, fac->p + j);
            else
            {
                fmpz_poly_pow(t, fac->p + j, fac->exp[j]);
                fmpz_poly_mul(h, h, t);
            }
            facs2 += fac->exp[j];
        }

        result = fmpz_poly_equal(f, h) && facs1 <= facs2;
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("facs1 = %wd, facs2 = %wd\n", facs1, facs2);
            flint_printf("f = "), fmpz_poly_print(f), flint_printf("\n\n");
            flint_printf("h = "), fmpz_poly_print(h), flint_printf("\n\n");
            flint_printf("fac = "), fmpz_poly_factor_print(fac), flint_printf("\n\n");
            abort();
        }

        fmpz_clear(c);
        fmpz_poly_clear(f);
        fmpz_poly_clear(g);
        fmpz_poly_clear(h);
        fmpz_poly_clear(t);
        fmpz_poly_factor_clear(fac);
    }

    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}
