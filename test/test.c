#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../src/stdx.h"


void test_stdx_parse_long(void);
void test_stdx_parse_long_all(void);


int main(void) {
    test_stdx_parse_long();
    test_stdx_parse_long_all();

    STDX_LOG_INF("All tests are PASSED!\n", NULL);
    return 0;
}


void test_stdx_parse_long() {
    long tmp;

    tmp = stdx_parse_long("");
    STDX_ASSERT(tmp == 0);

    tmp = stdx_parse_long("abcd");
    STDX_ASSERT(tmp == 0);

    tmp = stdx_parse_long("1");
    STDX_ASSERT(tmp == 1);

    tmp = stdx_parse_long("-1");
    STDX_ASSERT(tmp == (-1));

    tmp = stdx_parse_long("abcd-198");
    STDX_ASSERT(tmp == (-198));

    tmp = stdx_parse_long("abcd198");
    STDX_ASSERT(tmp == 198);

    tmp = stdx_parse_long("ab875kl");
    STDX_ASSERT(tmp == 875);

    tmp = stdx_parse_long("foo453bar");
    STDX_ASSERT(tmp == 453);

    STDX_LOG_INF("stdx_parse_long PASSED...\n", NULL);
}


void test_stdx_parse_long_all(void) {
    Stdx_DArr tmp;

    tmp = stdx_parse_long_all("");
    STDX_ASSERT(tmp.len == 0);

    tmp = stdx_parse_long_all("foobar");
    STDX_ASSERT(tmp.len == 0);

    tmp = stdx_parse_long_all("foo123");
    STDX_ASSERT(tmp.len == 1 && (*stdx_da_get(&tmp, long, 0)) == 123);
    stdx_da_free(tmp);

    tmp = stdx_parse_long_all("foo123bar-456abc-1");
    STDX_ASSERT(tmp.len == 3
                && (*stdx_da_get(&tmp, long, 0)) == 123
                && (*stdx_da_get(&tmp, long, 1)) == -456
                && (*stdx_da_get(&tmp, long, 2)) == -1);
    stdx_da_free(tmp);

    STDX_LOG_INF("stdx_parse_long_all PASSED...\n", NULL);
}
