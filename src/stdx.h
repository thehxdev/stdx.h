#ifndef STDX_H
#define STDX_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


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



/**
 * Macros and Constants
 */

#if (defined(__LP64__) || defined(_LP64))
    // #define STDX_WSIZE  64
    #define STDX_64BIT  1
#endif


#define STDX_MALLOC     malloc
#define STDX_CALLOC     calloc
#define STDX_REALLOC    realloc
#define STDX_ASSERT     assert
#define STDX_FREE       free
#define STDX_XFREE(p)   do { STDX_FREE((p)); (p) = NULL; } while (0)
#define STDX_ARRAY_LEN(arr) (sizeof((arr)) / sizeof((arr)[0]))

#ifndef true
    #define true    1
#endif // true

#ifndef false
    #define false   0
#endif // true


// Daynamic Array
#define STDX_DA_DEF_CAP 20

#define stdx_da_new(capacity)    \
    (Stdx_DArr) { NULL, 0, (capacity > 0) ? capacity : STDX_DA_DEF_CAP }

#define stdx_da_append(da_ptr, item_type, item_ptr)                                             \
    do {                                                                                        \
        if (((da_ptr)->len % (da_ptr)->cap) == 0) {                                             \
            size_t __stdx_da_new_size__ = sizeof(item_type) * ((da_ptr)->cap + (da_ptr)->len);  \
            (da_ptr)->items = STDX_REALLOC((da_ptr)->items, __stdx_da_new_size__);              \
            STDX_ASSERT((da_ptr)->items != NULL);                                               \
        }                                                                                       \
        item_type *__stdx_da_tmp__ = &((item_type*)(da_ptr)->items)[(da_ptr)->len];             \
        *__stdx_da_tmp__ = *(item_ptr);                                                         \
        (da_ptr)->len += 1;                                                                     \
    } while (0)

#define stdx_da_append_many(da_ptr, item_type, items, items_count)              \
    do {                                                                        \
        for (size_t __stdx_i__ = 0; __stdx_i__ < (items_count); __stdx_i__++)   \
            stdx_da_append((da_ptr), item_type, &(items)[__stdx_i__]);          \
    } while (0)

#define stdx_da_get(da_ptr, item_type, idx) \
    (STDX_ASSERT((idx) >= 0),               \
    STDX_ASSERT((idx) < ((da_ptr)->len)),   \
    &((item_type*)(da_ptr)->items)[(idx)])

#define stdx_da_free(da) STDX_XFREE((da).items)


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

// Dynamic array
typedef struct __stdx_darr {
    void *items;
    size_t len;
    size_t cap;
} Stdx_DArr;

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



/**
 * Public API
 */

char *stdx_strdup(const char *s) {
    if (!s)
        return NULL;

    size_t i = 0;
    char *tmp = STDX_MALLOC((strlen(s) + 1) * sizeof(*tmp));
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


char *stdx_strndup(const char *s, const size_t n) {
    if (!s)
        return NULL;

    size_t i = 0;
    char *tmp = STDX_MALLOC((n + 1) * sizeof(*tmp));
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

    size_t delta = 0;
    if (end < start)
        return NULL;

    delta = end - start;
    return stdx_strndup(start, delta);
}


char *stdx_find_substr(const char *source, const char *query) {
    register char *s = (char*)source;
    register char *q;

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
    long num = -1;
    short neg = false;

    while (*tmp && !(stdx_is_digit(*tmp))) tmp++;
    if (*tmp == '\0')
        goto ret;

    if (tmp != s && *(tmp-1) == '-')
        neg = true;

    num = 0;
    while (*tmp && stdx_is_digit(*tmp)) {
        num *= 10;
        num += (*tmp - 48);
        tmp += 1;
    }

ret:
    return (neg) ? (-(num)) : num;
}


// Parse all integers in string
Stdx_DArr stdx_parse_long_all(const char *s) {
    char *tmp = (char*)s;
    long num;
    short neg;
    Stdx_DArr nums = stdx_da_new(5);

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
        stdx_da_append(&nums, long, &num);
    }

    return nums;
}



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // STDX_H
