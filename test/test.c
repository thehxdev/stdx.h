#include <stdio.h>
#include <stdlib.h>
#include "../src/stdx.h"

int main(void) {
    size_t i;
    Stdx_DA darr = stdx_da_new(10);

    for (i = 0; i < 10; i++)
        stdx_da_append(&darr, &i, int);

    for (i = 0; i < darr.len; i++)
        printf("%d\n", *stdx_da_get(&darr, int, i));

    stdx_da_free(&darr);
    return 0;
}
