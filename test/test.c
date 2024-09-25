#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/stdx.h"


void test_stdx_darr(void);
void test_stdx_strdup(void);
void test_stdx_strndup(void);
void test_stdx_numeric_types(void);
void test_stdx_parse_long(void);
void test_stdx_parse_long_all(void);


int main(void) {
#ifdef STDX_64BIT
    STDX_LOG_INF("%s\n", "64-bit machine detected...");
#endif

    test_stdx_darr();
    test_stdx_strdup();
    test_stdx_strndup();
    test_stdx_numeric_types();
    test_stdx_parse_long();
    test_stdx_parse_long_all();

    STDX_LOG_INF("%s\n", "All tests are PASSED!");
    return 0;
}

void test_stdx_darr(void) {
    intlist_t tmp = {0};
    STDX_ASSERT(tmp.len == 0);
    STDX_ASSERT(tmp.cap == 0);
    STDX_ASSERT(tmp.items == NULL);

    int stat;
    long num = 10;

    DA_APPEND(&tmp, &num);
    STDX_ASSERT((*DA_GET(&tmp, 0)) == 10);

    num = 873;
    DA_APPEND(&tmp, &num);
    STDX_ASSERT((*DA_GET(&tmp, 1)) == 873);

    STDX_XFREE(*(&tmp.items));


    charlist_t tmp2 = {0};
    STDX_ASSERT(tmp2.len == 0);
    STDX_ASSERT(tmp2.cap == 0);
    STDX_ASSERT(tmp2.items == NULL);

    char *sample = "Hello World!";
    for (size_t i = 0; i < strlen(sample); i++)
        DA_APPEND(&tmp2, &sample[i]);

    stat = strcmp(tmp2.items, sample);
    STDX_ASSERT(stat == 0);
    STDX_XFREE(*(&tmp2.items));

    STDX_LOG_INF("%s\n", "PASSED...");
}


void test_stdx_strdup(void) {
    int stat;
    char *tmp;
    char *sample = "Hello World!";
    size_t sample_len = strlen(sample);

    tmp = stdx_strdup(sample);
    stat = strcmp(tmp, sample);
    STDX_ASSERT(stat == 0);
    STDX_ASSERT(tmp[sample_len] == '\0');
    STDX_XFREE(tmp);

    tmp = stdx_strdup(NULL);
    STDX_ASSERT(tmp == NULL);

    STDX_LOG_INF("%s\n", "PASSED...");
}


void test_stdx_strndup(void) {
    char *tmp;
    char *sample = "Hello World!";

    tmp = stdx_strndup(sample, -1);
    STDX_ASSERT(tmp == NULL);

    tmp = stdx_strndup(NULL, 1);
    STDX_ASSERT(tmp == NULL);

    tmp = stdx_strndup(sample, 5);
    STDX_ASSERT(strcmp(tmp, "Hello") == 0);
    STDX_ASSERT(tmp[6] == '\0');
    STDX_XFREE(tmp);


    STDX_LOG_INF("%s\n", "PASSED...");
}


void test_stdx_numeric_types(void) {
    STDX_ASSERT(sizeof(u8) == 1);
    STDX_ASSERT(sizeof(i8) == 1);

    STDX_ASSERT(sizeof(u16) == 2);
    STDX_ASSERT(sizeof(i16) == 2);

    STDX_ASSERT(sizeof(u32) == 4);
    STDX_ASSERT(sizeof(i32) == 4);

#ifdef STDX_64BIT
    STDX_ASSERT(sizeof(u64) == 8);
    STDX_ASSERT(sizeof(i64) == 8);

    STDX_ASSERT(sizeof(usize) == 8);
    STDX_ASSERT(sizeof(ssize) == 8);
#else // else STDX_64BIT
    STDX_ASSERT(sizeof(usize) == 4);
    STDX_ASSERT(sizeof(ssize) == 4);
#endif // STDX_64BIT

    STDX_LOG_INF("%s\n", "PASSED...");
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

    STDX_LOG_INF("%s\n", "PASSED...");
}


void test_stdx_parse_long_all(void) {
    intlist_t tmp;

    tmp = stdx_parse_long_all("");
    STDX_ASSERT(tmp.len == 0);

    tmp = stdx_parse_long_all("foobar");
    STDX_ASSERT(tmp.len == 0);

    tmp = stdx_parse_long_all("foo123");
    STDX_ASSERT(tmp.len == 1 && (*DA_GET(&tmp, 0)) == 123);
    STDX_XFREE(*(&tmp.items));

    tmp = stdx_parse_long_all("foo123bar-456abc-1");
    STDX_ASSERT(tmp.len == 3
                && (*DA_GET(&tmp, 0)) == 123
                && (*DA_GET(&tmp, 1)) == -456
                && (*DA_GET(&tmp, 2)) == -1);
    STDX_XFREE(*(&tmp.items));

    STDX_LOG_INF("%s\n", "PASSED...");
}
