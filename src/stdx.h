#ifndef STDX_H
#define STDX_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus



/**
 * Includes
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



/**
 * Macros and Constants
 */

#define STDX_MALLOC     malloc
#define STDX_CALLOC     calloc
#define STDX_REALLOC    realloc
#define STDX_ASSERT     assert
#define STDX_FREE       free
#define STDX_XFREE(p)   do { STDX_FREE((p)); (p) = NULL; } while (0)


#define STDX_ARRAY_LEN(arr) (sizeof(arr)/sizeof(arr[0]))


// Daynamic Array
#define STDX_DA_DEF_CAP 20

#define stdx_da_new(capacity)    \
    (Stdx_DA) { NULL, 0, (capacity > 0) ? capacity : STDX_DA_DEF_CAP }

#define stdx_da_append(da_ptr, item_ptr, item_type)                                           \
do {                                                                                          \
    if (((da_ptr)->len % (da_ptr)->cap) == 0) {                                               \
        size_t __stdx_da_new_size__ = sizeof(item_type) * ((da_ptr)->cap + (da_ptr)->len);    \
        (da_ptr)->items = STDX_REALLOC((da_ptr)->items, __stdx_da_new_size__);                \
        STDX_ASSERT((da_ptr)->items != NULL);                                                 \
    }                                                                                         \
    item_type *__stdx_da_tmp__ = &((item_type*)(da_ptr)->items)[(da_ptr)->len];               \
    *__stdx_da_tmp__ = *(item_ptr);                                                           \
    (da_ptr)->len += 1;                                                                       \
} while (0)

#define stdx_da_append_many(da_ptr, items, item_type, items_count)         \
do {                                                                       \
    for (size_t __stdx_i__ = 0; __stdx_i__ < (items_count); __stdx_i__++)  \
        stdx_da_append((da_ptr), &(items)[__stdx_i__], item_type);         \
} while (0)

#define stdx_da_get(da_ptr, item_type, idx) \
    (STDX_ASSERT((idx) >= 0),               \
    STDX_ASSERT((idx) < ((da_ptr)->len)),   \
    &((item_type*)(da_ptr)->items)[(idx)])

#define stdx_da_free(da_ptr) STDX_XFREE((da_ptr)->items)


// Logging
#define STDX_LOG_ERR(format, ...)   \
    (void)fprintf(stderr, "[ERROR] %s: " format, __FUNCTION__, __VA_ARGS__)

#define STDX_LOG_INF(format, ...)   \
    (void)fprintf(stderr, "[INFO] %s: " format, __FUNCTION__, __VA_ARGS__)



/**
 * Types
 */

typedef struct __stdx_darr {
    void *items;
    size_t len;
    size_t cap;
} Stdx_DA;



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // STDX_H
