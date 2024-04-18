#include <stdio.h>
#include <stdlib.h>
#include "../src/stdx.h"

int main(void) {
    size_t i;
    int nums[5] = { 1, 2, 3, 4, 5, };
    Stdx_DA darr = stdx_da_new(10);

    stdx_da_append_many(&darr, nums, int, STDX_ARRAY_LEN(nums));

    for (i = 0; i < darr.len; i++)
        printf("%d\n", *stdx_da_get(&darr, int, i));

    stdx_da_free(&darr);
    return 0;
}
