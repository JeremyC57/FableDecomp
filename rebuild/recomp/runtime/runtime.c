/* Host side of the recompiled program: guest memory, dispatch, traps. */
#include "recomp.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/mman.h>
#endif

uint8_t* g_mem;

typedef struct RecompEntry { uint32_t addr; GuestFn fn; } RecompEntry;
extern const RecompEntry recomp_table[];
extern const uint32_t recomp_table_size;

/* Host hooks (set by the platform layer / tests). */
void (*recomp_on_fatal)(Ctx* c, uint32_t eip, const char* what);
int (*recomp_on_unknown_target)(Ctx* c, uint32_t target); /* imports, traps; return 1 if handled */

int recomp_init_memory(void) {
#if defined(_WIN32)
    g_mem = (uint8_t*)VirtualAlloc(NULL, 0x100000000ull, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
    void* p = mmap(NULL, 0x100000000ull, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    g_mem = p == MAP_FAILED ? NULL : (uint8_t*)p;
#endif
    return g_mem != NULL;
}

GuestFn recomp_lookup(uint32_t target) {
    uint32_t lo = 0, hi = recomp_table_size;
    while (lo < hi) {
        const uint32_t mid = (lo + hi) / 2;
        if (recomp_table[mid].addr < target) lo = mid + 1;
        else hi = mid;
    }
    return (lo < recomp_table_size && recomp_table[lo].addr == target) ? recomp_table[lo].fn : NULL;
}

void recomp_dispatch(Ctx* c, uint32_t target) {
    GuestFn fn = recomp_lookup(target);
    if (fn) {
        fn(c);
        return;
    }
    if (recomp_on_unknown_target && recomp_on_unknown_target(c, target)) return;
    recomp_fatal(c, target, "indirect branch to unknown target");
}

void recomp_fatal(Ctx* c, uint32_t eip, const char* what) {
    if (recomp_on_fatal) recomp_on_fatal(c, eip, what);
    fprintf(stderr, "recomp fatal at 0x%08X: %s\n", eip, what);
    abort();
}

void recomp_cpuid(Ctx* c) {
    /* Report a Pentium 4-class CPU with MMX/SSE/SSE2 (what the engine probes for). */
    switch (c->eax) {
    case 0: c->eax = 1; c->ebx = 0x756E6547; c->edx = 0x49656E69; c->ecx = 0x6C65746E; break; /* GenuineIntel */
    case 1: c->eax = 0x00000F29; c->ebx = 0; c->ecx = 0; c->edx = 0x0383FBFF; break;
    default: c->eax = c->ebx = c->ecx = c->edx = 0; break;
    }
}

uint64_t recomp_rdtsc(void) {
    static uint64_t t;
    return t += 1000;
}
