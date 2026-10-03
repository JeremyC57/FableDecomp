/* Host side of the recompiled program: guest memory, dispatch, traps. */
#include "recomp.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)
#include <windows.h>
#include <intrin.h>
#else
#include <sys/mman.h>
#include <time.h>
#endif

#if !defined(RECOMP_IDENTITY_MEMORY)
uint8_t* g_mem;
#endif

typedef struct RecompEntry { uint32_t addr; GuestFn fn; } RecompEntry;
extern const RecompEntry recomp_table[];
extern const uint32_t recomp_table_size;

/* Host hooks (set by the platform layer / tests). */
void (*recomp_on_fatal)(Ctx* c, uint32_t eip, const char* what);
int (*recomp_on_unknown_target)(Ctx* c, uint32_t target); /* imports, traps; return 1 if handled */
void (*recomp_on_trace)(Ctx* c, uint32_t fn);

int recomp_init_memory(void) {
#if defined(RECOMP_IDENTITY_MEMORY)
    return 1; /* the host platform layer reserves the guest regions itself */
#elif defined(_WIN32)
    g_mem = (uint8_t*)VirtualAlloc(NULL, 0x100000000ull, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
    void* p = mmap(NULL, 0x100000000ull, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    g_mem = p == MAP_FAILED ? NULL : (uint8_t*)p;
#endif
#if !defined(RECOMP_IDENTITY_MEMORY)
    return g_mem != NULL;
#endif
}

/* Additional images (recompiled DLLs from the game folder), each with its own sorted table. */
#define RECOMP_MAX_TABLES 8
static struct { const RecompEntry* table; uint32_t size; } g_tables[RECOMP_MAX_TABLES];
static int g_numTables;

void recomp_register_table(const RecompEntry* table, uint32_t size) {
    if (g_numTables < RECOMP_MAX_TABLES) {
        g_tables[g_numTables].table = table;
        g_tables[g_numTables].size = size;
        ++g_numTables;
    }
}

static GuestFn lookupIn(const RecompEntry* t, uint32_t n, uint32_t target) {
    uint32_t lo = 0, hi = n;
    while (lo < hi) {
        const uint32_t mid = (lo + hi) / 2;
        if (t[mid].addr < target) lo = mid + 1;
        else hi = mid;
    }
    return (lo < n && t[lo].addr == target) ? t[lo].fn : NULL;
}

GuestFn recomp_lookup(uint32_t target) {
    GuestFn f = lookupIn(recomp_table, recomp_table_size, target);
    for (int i = 0; !f && i < g_numTables; ++i) f = lookupIn(g_tables[i].table, g_tables[i].size, target);
    return f;
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
#if defined(RECOMP_IDENTITY_MEMORY) && (defined(__x86_64__) || defined(_M_X64))
    /* The game host: a real cycle counter (ConfigDetect times it against QPC for the CPU speed). */
    return __rdtsc();
#elif defined(RECOMP_IDENTITY_MEMORY) && defined(__aarch64__)
    /* The game host on ARM64: a 3 GHz counter from the monotonic clock. */
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((uint64_t)ts.tv_sec * 1000000000u + (uint64_t)ts.tv_nsec) * 3u;
#else
    /* Tests: deterministic. */
    static uint64_t t;
    return t += 1000;
#endif
}

/* ---- C++ exception landing pads -------------------------------------------- */
static _Thread_local RecompLanding* t_landings;

void recomp_landing_push(RecompLanding* l, uint32_t entry_esp) {
    l->entry_esp = entry_esp;
    l->target = 0;
    l->prev = t_landings;
    t_landings = l;
}
void recomp_landing_pop(RecompLanding* l) {
    /* Normally the top; frames skipped by a longjmp were already dropped. */
    RecompLanding** p = &t_landings;
    while (*p && *p != l) p = &(*p)->prev;
    if (*p) *p = l->prev;
}
void* recomp_landing_chain_get(void) { return t_landings; }
void recomp_landing_chain_set(void* chain) { t_landings = (RecompLanding*)chain; }

void recomp_resume_at(Ctx* c, uint32_t frame, uint32_t target) {
    (void)c;
    for (RecompLanding* l = t_landings; l; l = l->prev)
        if (l->entry_esp > frame) {
            t_landings = l;
            l->target = target;
            __builtin_longjmp(l->jb, 1);
        }
}

