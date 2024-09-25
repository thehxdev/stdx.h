#ifndef APTGIT_DARRAY_H
#define APTGIT_DARRAY_H

#include "mem.h"

#define DARRAY_DEFINE(struct_name, item_type)   \
    struct struct_name {                        \
        item_type *items;                       \
        size_t len;                             \
        size_t cap;                             \
    }

#define __DARRAY_CAP_UNIT   (25)

#define __DARRAY_ADD_CAP(da_ptr) ((da_ptr)->cap += __DARRAY_CAP_UNIT)

/**
 * Default set-last-value macro. this macro sets last item the dynamic array to
 * a value pointed by `item_ptr`
 */
#define __DARRAY_SET_LAST_VALUE(da_ptr, item_ptr) \
    memmove(&((da_ptr)->items[(da_ptr)->len]), (item_ptr), sizeof(*(da_ptr)->items))

/**
 * Grow dynamic array's items pointer
 */
#define DARRAY_GROW(da_ptr)                                             \
    if ((da_ptr)->len == (da_ptr)->cap) {                               \
        __DARRAY_ADD_CAP(da_ptr);                                       \
        size_t __new_size__ = (da_ptr)->cap * sizeof(*(da_ptr)->items); \
        (da_ptr)->items = STDX_REALLOC((da_ptr)->items, __new_size__);  \
    }

/**
 * Customizable append macro
 *
 * `fn` is a macro or function that takes a pointer to a dynamic array and
 * a pointer to a value that you want to append, and sets the last item in the
 * list to that value. like `__DARRAY_SET_LAST_VALUE` macro defined above.
 */
#define DARRAY_APPEND_C(da_ptr, item_ptr, fn)   \
    do {                                        \
        DARRAY_GROW((da_ptr));                  \
        if (!(da_ptr)->items)                   \
            break;                              \
        fn((da_ptr), (item_ptr));               \
        (da_ptr)->len += 1;                     \
    } while(0)

#define DARRAY_APPEND(da_ptr, item_ptr) \
    DARRAY_APPEND_C((da_ptr), (item_ptr), __DARRAY_SET_LAST_VALUE)

/**
 * foreach :D
 */
#define DARRAY_FOREACH(da_ptr, var, proc)                   \
    do {                                                    \
        for (size_t __i = 0; __i < (da_ptr)->len; __i++) {  \
            var = &(da_ptr)->items[__i];                    \
            proc                                            \
        }                                                   \
    } while (0)

#define DARRAY_GET(da_ptr, idx) \
    (assert((idx) >= 0), assert((idx) < (da_ptr)->len), &((da_ptr)->items[(idx)]))


#endif /* APTGIT_DARRAY_H */
