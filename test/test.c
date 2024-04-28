#include <stdio.h>
#include <stdlib.h>

#include "../src/stdx.h"

int main(void) {
    usize i;
    int ns[5] = { 12, 1, 51, 0, -1 };
    Stdx_DArr nums = stdx_da_new(10);
    stdx_da_append_many(&nums, int, ns, 5);

    for (i = 0; i < nums.len; i++)
        printf("%d ", *stdx_da_get(&nums, int, i));
    printf("\n");

    stdx_da_free(nums);
    return 0;
}
