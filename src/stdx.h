#ifndef STDX_H
#define STDX_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define STDX_VERSION_MAJOR  0
#define STDX_VERSION_MINOR  0
#define STDX_VERSION_PATCH  1
#define STDX_VERSION "0.0.1"


/**
 * Includes
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

// stdx
#include "mem.h"


/**
 * Macros and Constants
 */

#if (defined(__LP64__) || defined(_LP64))
    #define STDX_64BIT  1
#endif


#define STDX_ASSERT(expr)   assert((expr))
#define STDX_ARRAY_LEN(arr) (sizeof((arr)) / sizeof((arr)[0]))

#ifndef true
    #define true    (1)
#endif // true

#ifndef false
    #define false   (0)
#endif // true


// Daynamic Array
#include "darray.h"


// Logging
#define STDX_LOG_FD stderr

#define STDX_LOG_ERR(format, ...)   \
    (void)fprintf(STDX_LOG_FD, "[ERROR] %s: " format, __FUNCTION__, __VA_ARGS__)

#define STDX_LOG_INF(format, ...)   \
    (void)fprintf(STDX_LOG_FD, "[INFO] %s: " format, __FUNCTION__, __VA_ARGS__)


// General
#define stdx_is_digit(ch) ((ch) >= '0' && (ch) <= '9')



/**
 * Types
 */

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
#ifdef STDX_64BIT
typedef uint64_t u64;
#endif // STDX_64BIT

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
#ifdef STDX_64BIT
typedef int64_t i64;
#endif // STDX_64BIT

typedef long    ssize;
typedef unsigned long   usize;



/**
 * Public API
 */

char *stdx_strdup(const char *s) {
    if (!s)
        return NULL;

    size_t i = 0;
    char *tmp = (char*) STDX_MALLOC((strlen(s) + 1) * sizeof(*tmp));
    if (!tmp)
        goto ret;

    while (s[i]) {
        tmp[i] = s[i];
        i++;
    }
    tmp[i] = '\0';

ret:
    return tmp;
}


char *stdx_strndup(const char *s, const long n) {
    if (!s || n <= 0)
        return NULL;

    long i = 0;
    char *tmp = (char*) STDX_MALLOC((n + 1) * sizeof(*tmp));
    if (!tmp)
        goto ret;

    while (s[i] && i < n) {
        tmp[i] = s[i];
        i++;
    }
    tmp[i] = '\0';

ret:
    return tmp;
}


char *stdx_substr(const char *start, const char *end) {
    //   "a string example"
    //      ^           ^
    //      *start      *end
    //      |-----------|
    //          delta
    return stdx_strndup(start, (long)(end - start));
}


char *stdx_find_substr(const char *source, const char *query) {
    char *s = (char*)source;
    char *q;

again:
    q = (char*)query;
    while (*s && *s != *q) s++;
    if (*s == '\0')
        return NULL;
    while (*s && *q) {
        if (*s == *q) {
            s++;
            q++;
        } else
            goto again;
    }

    if (*q == '\0') {
        while (q != query) {
            s--;
            q--;
        }
        return s;
    }

    return NULL;
}


// Parse first integer value in string
long stdx_parse_long(const char *s) {
    char *tmp = (char*)s;
    long num = 0;
    short neg = false;

    while (*tmp && !(stdx_is_digit(*tmp))) tmp++;
    if (*tmp == '\0')
        goto ret;

    if (tmp != s && *(tmp-1) == '-')
        neg = true;

    while (*tmp && stdx_is_digit(*tmp)) {
        num *= 10;
        num += (*tmp - 48);
        tmp += 1;
    }

ret:
    return (neg) ? (-(num)) : num;
}

typedef DA_DEFINE(intlist, int) intlist_t;
typedef DA_DEFINE(charlist, char) charlist_t;

// Parse all integers in string
intlist_t stdx_parse_long_all(const char *s) {
    char *tmp = (char*)s;
    long num;
    short neg;
    intlist_t nums = {0};

    while (true) {
        neg = false;
        while (*tmp && !(stdx_is_digit(*tmp))) tmp++;
        if (*tmp == '\0')
            break;

        if (tmp != s && *(tmp-1) == '-')
            neg = true;

        num = 0;
        while (*tmp && stdx_is_digit(*tmp)) {
            num *= 10;
            num += ((*tmp) - 48);
            tmp += 1;
        }

        num = (neg) ? (-(num)) : num;
        DA_APPEND(&nums, &num);
    }

    return nums;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // STDX_H
