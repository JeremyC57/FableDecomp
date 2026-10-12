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

/* Indirect branches resolve through an open-addressing hash of every table (the sorted
   tables stay the fallback and the source): a binary search over ~40k entries per virtual
   call was the single hottest host function on the handheld. Built once the tables are
   registered (before the guest starts), rebuilt when another image registers. */
typedef struct HashEntry { uint32_t addr; uint32_t idx; } HashEntry;  /* idx: 1-based into recomp_fns */
typedef struct Hash { uint32_t mask; HashEntry e[1]; } Hash;
static Hash* volatile g_hash;  /* swapped whole; a superseded table is kept (guest threads may still read it) */
GuestFn* recomp_fns;           /* every lifted function, in table order; [0] unused */
static uint32_t g_numFns;

static void hashInsert(Hash* h, uint32_t addr, uint32_t idx) {
    uint32_t i = (addr * 2654435761u) >> 7;
    for (;; ++i) {
        i &= h->mask;
        if (!h->e[i].idx) { h->e[i].addr = addr; h->e[i].idx = idx; return; }
        if (h->e[i].addr == addr) return;
    }
}

static void buildHash(void) {
    uint32_t total = recomp_table_size, cap = 1024, i, n = 1;
    int t;
    Hash* h;
    GuestFn* fns;
    for (t = 0; t < g_numTables; ++t) total += g_tables[t].size;
    while (cap < total * 2) cap <<= 1;
    h = calloc(1, sizeof(Hash) + (size_t)cap * sizeof(HashEntry));
    fns = calloc(total + 1, sizeof *fns);
    h->mask = cap - 1;
    for (i = 0; i < recomp_table_size; ++i) { fns[n] = recomp_table[i].fn; hashInsert(h, recomp_table[i].addr, n++); }
    for (t = 0; t < g_numTables; ++t)
        for (i = 0; i < g_tables[t].size; ++i) { fns[n] = g_tables[t].table[i].fn; hashInsert(h, g_tables[t].table[i].addr, n++); }
    g_numFns = n;
    recomp_fns = fns;  /* published before the hash that indexes it (the old array is kept) */
    g_hash = h;
}

static uint32_t lookupIdx(uint32_t target) {
    const Hash* h = g_hash;
    uint32_t i;
    if (!h) return 0;
    i = (target * 2654435761u) >> 7;
    for (;; ++i) {
        i &= h->mask;
        if (!h->e[i].idx) return 0;
        if (h->e[i].addr == target) return h->e[i].idx;
    }
}

void recomp_register_table(const RecompEntry* table, uint32_t size) {
    if (g_numTables < RECOMP_MAX_TABLES) {
        g_tables[g_numTables].table = table;
        g_tables[g_numTables].size = size;
        ++g_numTables;
    }
    buildHash();
}

/* For the host's sampling profiler: the main table (index 0) and the registered ones. */
uint32_t recomp_num_tables(void) { return (uint32_t)g_numTables + 1; }
const RecompEntry* recomp_table_entries(uint32_t i, uint32_t* n) {
    if (i == 0) { *n = recomp_table_size; return recomp_table; }
    if (i > (uint32_t)g_numTables) { *n = 0; return 0; }
    *n = g_tables[i - 1].size;
    return g_tables[i - 1].table;
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
    GuestFn f;
    uint32_t idx;
    if (!g_hash) buildHash();  /* no image registered itself: build on the first indirect branch */
    idx = lookupIdx(target);
    if (idx) return recomp_fns[idx];
    f = lookupIn(recomp_table, recomp_table_size, target);
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

void recomp_dispatch_ic(Ctx* c, uint32_t target, uint64_t* ic) {
    uint32_t idx;
    if (!g_hash) buildHash();
    idx = lookupIdx(target);
    if (idx) {
        const uint64_t v = ((uint64_t)idx << 32) | target;
#if defined(__GNUC__) || defined(__clang__)
        __atomic_store_n(ic, v, __ATOMIC_RELAXED);
#else
        *(volatile uint64_t*)ic = v;
#endif
        recomp_fns[idx](c);
        return;
    }
    recomp_dispatch(c, target);
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

