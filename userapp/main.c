

#include <inttypes.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <sys/mman.h>
#include <syscall.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "graphics.h"
#include <time.h>

extern void* tlsf_create_with_pool(void* mem, size_t bytes);
extern void* g_heap;
extern void test_allocator();


void __main()
{
    size_t heap_size = 32 * 1024 * 1024;
    void* heap_base = mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    g_heap = tlsf_create_with_pool(heap_base, heap_size);
}

void main(void* pv)
{
    printf("task #%d: I'm the first user task(pv=0x%08x)!\r\n",
        task_getid(), pv);

    test_allocator();

    exit(0);
}
