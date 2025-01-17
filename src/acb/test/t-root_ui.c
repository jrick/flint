/*
    Copyright (C) 2012 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "acb.h"

TEST_FUNCTION_START(acb_root_ui, state)
{
    slong iter;

    for (iter = 0; iter < 1000 * 0.1 * flint_test_multiplier(); iter++)
    {
        acb_t a, b, c;
        ulong k;
        slong prec;

        prec = 2 + n_randint(state, 1000);
        k = n_randtest_not_zero(state);

        acb_init(a);
        acb_init(b);
        acb_init(c);

        acb_randtest(a, state, 1 + n_randint(state, 1000), 1 + n_randint(state, 100));
        acb_randtest(b, state, 1 + n_randint(state, 1000), 1 + n_randint(state, 100));

        acb_root_ui(b, a, k, prec);
        acb_pow_ui(c, b, k, prec);

        if (!acb_contains(c, a))
        {
            flint_printf("FAIL: containment\n\n");
            flint_printf("k = %wu\n", k);
            flint_printf("a = "); acb_print(a); flint_printf("\n\n");
            flint_printf("b = "); acb_print(b); flint_printf("\n\n");
            flint_printf("c = "); acb_print(c); flint_printf("\n\n");
            flint_abort();
        }

        acb_root_ui(a, a, k, prec);

        if (!acb_equal(a, b))
        {
            flint_printf("FAIL: aliasing\n\n");
            flint_abort();
        }

        acb_clear(a);
        acb_clear(b);
        acb_clear(c);
    }

    TEST_FUNCTION_END(state);
}
