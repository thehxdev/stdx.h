#ifndef STDX_MEM_H
#define STDX_MEM_H

#define STDX_MALLOC  malloc
#define STDX_CALLOC  calloc
#define STDX_REALLOC realloc
#define STDX_FREE    free
#define STDX_XFREE(p)    do { STDX_FREE((p)); (p) = NULL; } while (0)

#endif /* STDX_MEM_H */
