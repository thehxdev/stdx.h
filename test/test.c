#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/stdx.h"


void test_stdx_darr(void);
void test_stdx_parse_long(void);
void test_stdx_parse_long_all(void);


int main(void) {
    test_stdx_darr();
    test_stdx_parse_long();
    test_stdx_parse_long_all();

    STDX_LOG_INF("All tests are PASSED!\n", NULL);
    return 0;
}


void test_stdx_darr(void) {
    Stdx_DArr tmp = stdx_da_new(20);
    STDX_ASSERT(tmp.len == 0);
    STDX_ASSERT(tmp.cap == 20);
    STDX_ASSERT(tmp.items == NULL);

    long num = 10;

    stdx_da_append(&tmp, long, &num);
    STDX_ASSERT((*stdx_da_get(&tmp, long, 0)) == 10);

    num = 873;
    stdx_da_append(&tmp, long, &num);
    STDX_ASSERT((*stdx_da_get(&tmp, long, 1)) == 873);

    stdx_da_free(tmp);
    STDX_ASSERT(tmp.len == 0);
    STDX_ASSERT(tmp.items == NULL);

    stdx_da_append_many(&tmp, char, "Hello World!", strlen("Hello World!"));
    STDX_ASSERT(strcmp(stdx_da_ptr(&tmp, char), "Hello World!") == 0);
    stdx_da_free(tmp);

    STDX_LOG_INF("PASSED...\n", NULL);
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

    STDX_LOG_INF("PASSED...\n", NULL);
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

    STDX_LOG_INF("PASSED...\n", NULL);
}
