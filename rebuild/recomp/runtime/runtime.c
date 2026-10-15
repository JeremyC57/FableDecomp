/* Host side of the recompiled program: guest memory, dispatch, traps. */
#include "recomp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <intrin.h>
#else
#include <sys/mman.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>
#include "recomp_clock.h"
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
GuestFn* recomp_fns;           /* [1, RECOMP_HOST_SLOTS]: host trap handlers (below); then every lifted function, in table order; [0] unused */
static uint32_t g_numFns;

/* Host targets in the inline caches. Index 1 + (target - lo) / 16 of recomp_fns holds the
   handler of the trap at `target` (16-byte spaced in the host range) once a call site has
   reached it, so the site's next call goes straight to the handler: the import and COM method
   calls (every Direct3D, CRT and kernel32 call, tens of thousands per frame) otherwise all
   missed the cache and went through recomp_dispatch_ic and the host's unknown-target hook,
   whose shared indirect call mispredicts from one trap to the next. The host's hook names the
   plain handler of a trap (NULL: not cacheable, e.g. data handlers or per-call tracing). A
   rebuilt array carries the slots over; a slot is written into every array ever published
   (guest threads may still index an old one), under a lock shared with the rebuild. */
#define RECOMP_HOST_SLOTS 65536u
GuestFn (*recomp_host_handler)(uint32_t target);
static GuestFn* g_fnsArrays[RECOMP_MAX_TABLES + 2];
static int g_numFnsArrays;
#if defined(__GNUC__) || defined(__clang__)
static int g_fnsLock;
static void fnsLock(void) { while (__atomic_exchange_n(&g_fnsLock, 1, __ATOMIC_ACQUIRE)) {} }
static void fnsUnlock(void) { __atomic_store_n(&g_fnsLock, 0, __ATOMIC_RELEASE); }
#else
static void fnsLock(void) {}
static void fnsUnlock(void) {}
#endif

static void hashInsert(Hash* h, uint32_t addr, uint32_t idx) {
    uint32_t i = (addr * 2654435761u) >> 7;
    for (;; ++i) {
        i &= h->mask;
        if (!h->e[i].idx) { h->e[i].addr = addr; h->e[i].idx = idx; return; }
        if (h->e[i].addr == addr) return;
    }
}

static void buildHash(void) {
    uint32_t total = recomp_table_size, cap = 1024, i, n = 1 + RECOMP_HOST_SLOTS;
    int t;
    Hash* h;
    GuestFn* fns;
    for (t = 0; t < g_numTables; ++t) total += g_tables[t].size;
    while (cap < total * 2) cap <<= 1;
    h = calloc(1, sizeof(Hash) + (size_t)cap * sizeof(HashEntry));
    fns = calloc(total + 1 + RECOMP_HOST_SLOTS, sizeof *fns);
    h->mask = cap - 1;
    for (i = 0; i < recomp_table_size; ++i) { fns[n] = recomp_table[i].fn; hashInsert(h, recomp_table[i].addr, n++); }
    for (t = 0; t < g_numTables; ++t)
        for (i = 0; i < g_tables[t].size; ++i) { fns[n] = g_tables[t].table[i].fn; hashInsert(h, g_tables[t].table[i].addr, n++); }
    fnsLock();
    if (recomp_fns) memcpy(fns + 1, recomp_fns + 1, RECOMP_HOST_SLOTS * sizeof *fns);  /* the host slots filled so far */
    if (g_numFnsArrays < (int)(sizeof g_fnsArrays / sizeof g_fnsArrays[0])) g_fnsArrays[g_numFnsArrays++] = fns;
    g_numFns = n;
    recomp_fns = fns;  /* published before the hash that indexes it (the old array is kept) */
    g_hash = h;
    fnsUnlock();
}

#if defined(__GNUC__) || defined(__clang__)
static void hostSlotFill(uint32_t slot, GuestFn fn) {
    int i;
    fnsLock();
    for (i = 0; i < g_numFnsArrays; ++i) __atomic_store_n(&g_fnsArrays[i][slot], fn, __ATOMIC_RELAXED);
    __atomic_store_n(&recomp_fns[slot], fn, __ATOMIC_RELAXED);  /* also an array past g_fnsArrays' capacity */
    fnsUnlock();
}
#endif

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

/* Host-implemented targets (the import and COM method traps) occupy one guest address range
   that holds no lifted code. The inline caches only remember lifted functions, so every call
   into a trap used to miss the cache, miss the hash, binary-search every table and only then
   reach recomp_on_unknown_target: hundreds of cycles per D3D, CRT or kernel32 call. A target
   in the range now goes to the host hook first (the lookups could only have missed). */
static uint32_t g_hostLo, g_hostSpan;

void recomp_set_host_range(uint32_t lo, uint32_t end) {
    g_hostLo = lo;
    g_hostSpan = end > lo ? end - lo : 0;
}

static int hostTarget(Ctx* c, uint32_t target) {
    return target - g_hostLo < g_hostSpan && recomp_on_unknown_target && recomp_on_unknown_target(c, target);
}

void recomp_dispatch(Ctx* c, uint32_t target) {
    GuestFn fn;
    if (hostTarget(c, target)) return;
    fn = recomp_lookup(target);
    if (fn) {
        fn(c);
        return;
    }
    if (recomp_on_unknown_target && recomp_on_unknown_target(c, target)) return;
    recomp_fatal(c, target, "indirect branch to unknown target");
}

void recomp_dispatch_ic(Ctx* c, uint32_t target, uint64_t* ic) {
    uint32_t idx;
#if defined(__GNUC__) || defined(__clang__)
    const uint32_t off = target - g_hostLo;
    if (off < g_hostSpan && !(off & 15) && (off >> 4) < RECOMP_HOST_SLOTS && recomp_host_handler) {
        const GuestFn hf = recomp_host_handler(target);
        if (hf) {
            const uint32_t slot = 1 + (off >> 4);
            if (!g_hash) buildHash();
            if (__atomic_load_n(&recomp_fns[slot], __ATOMIC_RELAXED) != hf) hostSlotFill(slot, hf);  /* sites that alternate between traps */
            __atomic_store_n(ic, ((uint64_t)slot << 32) | target, __ATOMIC_RELEASE);  /* after the slot */
            hf(c);
            return;
        }
    }
#endif
    if (hostTarget(c, target)) return;
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

/* ---- the host's monotonic clock (recomp_clock.h) ------------------------------ */
#if !defined(_WIN32)
#if defined(__GNUC__) || defined(__clang__)
__attribute__((aligned(64)))
#endif
RecompClock recomp_clock_state;

static uint64_t monoNs(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000u + (uint64_t)ts.tv_nsec;
}

#if RECOMP_CLOCK_COUNTER
#define CLOCK_SPAN_NS 64000000u /* the counter time one anchor covers */

static inline uint64_t cntvct(void) {
    uint64_t t;
    __asm__ __volatile__("isb\n\tmrs %0, cntvct_el0" : "=r"(t) : : "memory");
    return t;
}
static inline uint64_t scaleNs(uint64_t ticks, uint64_t mul) { return (uint64_t)(((unsigned __int128)ticks * mul) >> 32); }
/* CLOCK_MONOTONIC between two counter reads (*a before, *b after: b - a bounds the pairing error) */
static uint64_t sample(uint64_t* a, uint64_t* b) {
    *a = cntvct();
    const uint64_t m = monoNs();
    *b = cntvct();
    return m;
}
/* the sample with the closest counter reads of n */
static uint64_t closestSample(uint64_t* a, uint64_t* gap, int n) {
    uint64_t m = 0, x, y;
    *gap = ~(uint64_t)0;
    while (n--) {
        const uint64_t v = sample(&x, &y);
        if (y - x < *gap) *gap = y - x, *a = x, m = v;
    }
    return m;
}

/* Whether the counter can stand in for CLOCK_MONOTONIC; the first anchor. */
static void clockSetup(void) {
    RecompClock* k = &recomp_clock_state;
    uint64_t freq, a, b, c, sum = 0, a0, g0, a1, g1, m0, m1, mid0, mid1;
    int i;
    const char* e = getenv("FABLE_FAST_CLOCK");
    __asm__ __volatile__("mrs %0, cntfrq_el0" : "=r"(freq));
    if ((e && atoi(e) == 0) || freq < 1000000u || freq > 1000000000u) return;
    /* A kernel that works around a counter erratum traps user-space reads (and drops its
     * vDSO): 64 counter reads must not take longer than 64 clock_gettime calls (the best of
     * three rounds: the first runs cold). A trapped read the kernel emulates is still correct. */
    {
        uint64_t bestCnt = ~(uint64_t)0, bestMono = ~(uint64_t)0;
        int round;
        for (round = 0; round < 3; ++round) {
            a = monoNs();
            for (i = 0; i < 64; ++i) sum += cntvct();
            b = monoNs();
            for (i = 0; i < 64; ++i) sum += monoNs();
            c = monoNs();
            if (b - a < bestCnt) bestCnt = b - a;
            if (c - b < bestMono) bestMono = c - b;
        }
        if (bestCnt > bestMono || !sum) return;
    }
    /* The counter's rate against CLOCK_MONOTONIC over 8 ms, each end the closest pair of 16;
     * CNTFRQ_EL0 (what the firmware set) must be within 0.05 % of it, measured to 0.02 %. */
    m0 = closestSample(&a0, &g0, 16);
    do {
        const struct timespec ts = {0, 2000000};
        nanosleep(&ts, NULL);
    } while (monoNs() - m0 < 8000000u);
    m1 = closestSample(&a1, &g1, 16);
    mid0 = a0 + g0 / 2, mid1 = a1 + g1 / 2;
    if (mid1 <= mid0 || m1 <= m0) return;
    {
        const double ticks = (double)(mid1 - mid0), measured = ticks * 1e9 / (double)(m1 - m0);
        if ((double)(g0 + g1) > ticks * 0.0004 || measured < (double)freq * 0.9995 || measured > (double)freq * 1.0005) return;
    }
    /* published like an anchor: odd while the fields are written */
    __atomic_store_n(&k->seq, 1u, __ATOMIC_RELAXED);
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
    k->freq = freq;
    k->nominal = k->rate = (uint64_t)(((unsigned __int128)1000000000u << 32) / freq);
    m0 = closestSample(&a0, &g0, 4);
    k->rateOk = g0 <= freq / 200000u + 1;
    __atomic_store_n(&k->c0, a0, __ATOMIC_RELAXED);
    __atomic_store_n(&k->n0, m0, __ATOMIC_RELAXED);
    __atomic_store_n(&k->mul, k->rate - (k->rate >> 10), __ATOMIC_RELAXED);
    __atomic_store_n(&k->lim, freq * (CLOCK_SPAN_NS / 1000000u) / 1000u, __ATOMIC_RELAXED);
    k->m0 = m0;
    k->anchors = 1;
    __atomic_store_n(&k->seq, 2u, __ATOMIC_RELEASE);
}

/* A new anchor; the caller has made seq odd (s + 1). */
static uint64_t newAnchor(RecompClock* k, uint32_t s) {
    /* the odd seq is visible everywhere before the counter is read: a reader that still saw
     * the old seq read the counter before t1 */
    __asm__ __volatile__("dsb ish" : : : "memory");
    const uint64_t c0 = k->c0, n0 = k->n0, m0 = k->m0, lim = k->lim, mul = k->mul;
    const uint64_t maxGap = k->freq / 200000u + 1; /* 5 us */
    uint64_t t1 = 0, t2 = 0, m1 = 0, n1, rate = k->rate, slow;
    int i, close = 0;
    for (i = 0; i < 3 && !close; ++i) {
        m1 = sample(&t1, &t2);  /* CLOCK_MONOTONIC read after t1: m1 >= CLOCK_MONOTONIC at t1 */
        close = t2 - t1 <= maxGap;
    }
    const uint64_t d = t1 - c0;
    if (t1 < c0 || scaleNs(d, k->nominal) > m1 - m0 + 2u * CLOCK_SPAN_NS) {
        /* The counter went back, or ran ahead of CLOCK_MONOTONIC by more than an interval (a
         * suspend). Reads past the anchor's interval returned nothing, so every value it gave
         * was read before the jump: at most CLOCK_MONOTONIC then plus the anchor's lead (and a
         * rate off by 1/1024). Re-based on CLOCK_MONOTONIC. */
        n1 = m1 + (n0 > m0 ? n0 - m0 : 0) + (CLOCK_SPAN_NS >> 10);
        {   /* ...unless CLOCK_MONOTONIC was slewed slower than the interval's rate by more than
             * ~2000 ppm (chrony's tick slews, up to 8.3 %): readers may then have returned more
             * than that. Not below the most the old anchor could have returned in the time that
             * passed (the kernel caps tick slews at 10 %, hence the 1/8). */
            const uint64_t span = m1 - m0, adv = scaleNs(lim, mul), cap = span + (span >> 3), bound = n0 + (adv < cap ? adv : cap);
            if (n1 < bound) n1 = bound;
        }
        __atomic_store_n(&k->rebases, k->rebases + 1, __ATOMIC_RELAXED);  /* (read by the profiler) */
    } else {
        /* the old anchor returned at most its value at min(t1, its end) */
        n1 = n0 + scaleNs(d < lim ? d : lim, mul);
        if (n1 < m1) n1 = m1;
        /* CLOCK_MONOTONIC's rate against the counter over the interval (NTP slewing), within
         * 0.2 % of CNTFRQ_EL0's */
        if (close && k->rateOk && d && m1 - m0 >= CLOCK_SPAN_NS / 2) {
            const uint64_t r = (uint64_t)(((unsigned __int128)(m1 - m0) << 32) / d), tol = k->nominal >> 9;
            if (r > k->nominal - tol && r < k->nominal + tol) rate = r;
        }
    }
    /* The next interval runs 1/1024 slower than CLOCK_MONOTONIC (it ends at or behind it), and
     * slower still while the clock is ahead of it (the lead taken back over one interval, at
     * most 1/16 slower). */
    slow = rate >> 10;
    if (n1 > m1) {
        const uint64_t take = (uint64_t)(((unsigned __int128)(n1 - m1) << 32) / lim);
        slow += take < (rate >> 4) ? take : (rate >> 4);
    }
    __atomic_store_n(&k->c0, t1, __ATOMIC_RELAXED);
    __atomic_store_n(&k->n0, n1, __ATOMIC_RELAXED);
    __atomic_store_n(&k->mul, rate - slow, __ATOMIC_RELAXED);
    k->m0 = m1;
    __atomic_store_n(&k->rate, rate, __ATOMIC_RELAXED);
    k->rateOk = (uint32_t)close;
    __atomic_store_n(&k->anchors, k->anchors + 1, __ATOMIC_RELAXED);
    __atomic_store_n(&k->seq, s + 2u ? s + 2u : 2u, __ATOMIC_RELEASE);
    return n1;
}
static pthread_once_t g_clockOnce = PTHREAD_ONCE_INIT;
#endif

uint64_t recomp_clock_slow(void) {
#if RECOMP_CLOCK_COUNTER
    RecompClock* k = &recomp_clock_state;
    unsigned spin;
    pthread_once(&g_clockOnce, clockSetup);
    if (!k->freq) return monoNs();
    for (spin = 0;; ++spin) {
        const uint32_t s = __atomic_load_n(&k->seq, __ATOMIC_ACQUIRE);
        if (!(s & 1u)) {  /* the fast path's read again: a new anchor may have just been published */
            const uint64_t c0 = __atomic_load_n(&k->c0, __ATOMIC_RELAXED), n0 = __atomic_load_n(&k->n0, __ATOMIC_RELAXED);
            const uint64_t mul = __atomic_load_n(&k->mul, __ATOMIC_RELAXED), lim = __atomic_load_n(&k->lim, __ATOMIC_RELAXED);
            uint64_t t, z;
            __asm__ __volatile__("isb\n\tmrs %0, cntvct_el0\n\teor %1, %0, %0" : "=r"(t), "=r"(z) : : "memory");
            __atomic_thread_fence(__ATOMIC_ACQUIRE);
            if (__atomic_load_n((const uint32_t*)((const char*)&k->seq + z), __ATOMIC_RELAXED) == s) {
                uint32_t expect = s;
                if (t - c0 < lim) return n0 + scaleNs(t - c0, mul);
                /* past the anchor's interval (or a counter behind it): this thread takes the next */
                if (__atomic_compare_exchange_n(&k->seq, &expect, s + 1u, 0, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED))
                    return newAnchor(k, s);
            }
        }
        if (spin < 64) __asm__ __volatile__("yield" : : : "memory");  /* another thread is taking it */
        else sched_yield();
    }
#else
    return monoNs();
#endif
}
uint64_t recomp_clock_now_ns(void) { return recomp_clock_ns(); }
#endif

uint64_t recomp_rdtsc(void) {
#if defined(RECOMP_IDENTITY_MEMORY) && (defined(__x86_64__) || defined(_M_X64))
    /* The game host: a real cycle counter (ConfigDetect times it against QPC for the CPU speed). */
    return __rdtsc();
#elif defined(RECOMP_IDENTITY_MEMORY) && defined(__aarch64__) && !defined(_WIN32)
    /* The game host on ARM64: a 3 GHz counter from the monotonic clock (the same clock as
     * QueryPerformanceCounter, so ConfigDetect's ratio holds). */
    return recomp_clock_ns() * 3u;
#elif defined(RECOMP_IDENTITY_MEMORY) && defined(__aarch64__)
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

