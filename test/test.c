#include <stdio.h>
#include <stdlib.h>

#include "../src/stdx.h"

int main(void) {
    size_t i;
    Stdx_DArr nums = stdx_da_new(20);

    for (i = 0; i < 20; i++)
        stdx_da_append(&nums, size_t, &i);

    for (i = 0; i < nums.len; i++)
        printf("%zu ", *stdx_da_get(&nums, size_t, i));
    printf("\n");

    stdx_da_free(nums);
    return 0;
}
