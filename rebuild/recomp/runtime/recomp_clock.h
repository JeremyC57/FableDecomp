/* CLOCK_MONOTONIC for the POSIX hosts, a few cycles per read on AArch64 Linux / Android.
 *
 * clock_gettime(CLOCK_MONOTONIC) goes through the PLT, the C library's wrapper and the vDSO
 * (a sequence lock, the counter read, a split into seconds and nanoseconds that the caller
 * multiplies back): about 100-150 cycles on a Cortex-A53, and the guest's
 * QueryPerformanceCounter / GetTickCount / timeGetTime / rdtsc read the clock thousands of times
 * per frame. recomp_clock_ns() reads the generic timer's virtual counter (CNTVCT_EL0, the counter
 * CLOCK_MONOTONIC itself is built on) and extrapolates from an anchor taken with
 * clock_gettime(CLOCK_MONOTONIC):
 *   - an anchor covers 64 ms of counter time; the first read past that takes a new one (one
 *     clock_gettime per anchor, whichever thread gets there first; the others wait for it);
 *   - between anchors the clock runs at the rate CLOCK_MONOTONIC had against the counter over the
 *     previous anchor's interval (NTP slewing included), made 1/1024 slower, so with ordinary
 *     slewing (up to ~1000 ppm) it stays behind CLOCK_MONOTONIC by at most ~64 us (~90 us right
 *     after the slewing changes) and each anchor steps it forward to it; under chrony's large tick
 *     slews (up to 8.3 %) it can run a few ms behind or up to ~45 ms ahead, still monotonic;
 *   - it never goes backwards, also across threads: an anchor never starts below a value the old
 *     one can have returned (readers' counter reads are ordered against the anchor's sequence
 *     lock), and it starts ahead of CLOCK_MONOTONIC only after a counter jump shorter than an
 *     anchor's interval (that lead is then taken back within about a second);
 *   - a counter that moved further than CLOCK_MONOTONIC did (a suspend: the counter keeps counting,
 *     CLOCK_MONOTONIC does not; a counter reset) is never extrapolated: reads past the anchor's
 *     64 ms re-anchor, re-based on CLOCK_MONOTONIC.
 * The first read checks the counter once (and sleeps 8 ms for it): user-space reads no slower than
 * clock_gettime, CNTFRQ_EL0 within 0.05 % of the rate measured against CLOCK_MONOTONIC over 8 ms;
 * from the first anchor on, the rate is the measured one. Otherwise, with
 * FABLE_FAST_CLOCK=0 and on other hosts, every read is clock_gettime(CLOCK_MONOTONIC).
 * The values follow CLOCK_MONOTONIC closely but are not it: for an absolute clock_nanosleep
 * deadline use clock_gettime itself. Never read this clock from a signal handler: a handler that
 * interrupts its own thread while it holds the anchor's odd sequence would spin forever.
 * Not for Windows.
 */
#ifndef RECOMP_CLOCK_H
#define RECOMP_CLOCK_H

#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__aarch64__) && defined(__linux__) && (defined(__GNUC__) || defined(__clang__))
#define RECOMP_CLOCK_COUNTER 1
#else
#define RECOMP_CLOCK_COUNTER 0
#endif

typedef struct RecompClock {
    /* read by every call (one cache line) */
    uint32_t seq;  /* even: the anchor below is stable; odd: a new one is being written */
    uint32_t pad_;
    uint64_t c0;   /* the counter at the anchor */
    uint64_t n0;   /* the clock at c0, ns */
    uint64_t mul;  /* ns per counter tick, 32.32 fixed point */
    uint64_t lim;  /* counter ticks past c0 the anchor covers; 0: not in use (clock_gettime) */
    /* the anchor's own (written under the odd seq) */
    uint64_t m0;   /* CLOCK_MONOTONIC at c0 */
    uint64_t rate; /* CLOCK_MONOTONIC ns per tick over the last interval, 32.32 */
    uint64_t nominal; /* ns per tick from CNTFRQ_EL0, 32.32 */
    uint64_t freq; /* counter Hz, 0: clock_gettime is the clock */
    uint64_t anchors, rebases; /* anchors taken; of them, re-based after a counter jump */
    uint32_t rateOk, pad2_;    /* the last anchor's two reads were close enough to measure the rate */
} RecompClock;
extern RecompClock recomp_clock_state;

uint64_t recomp_clock_slow(void);   /* the check on first use, a new anchor, or clock_gettime */
uint64_t recomp_clock_now_ns(void); /* recomp_clock_ns() out of line */

static inline uint64_t recomp_clock_ns(void) {
#if RECOMP_CLOCK_COUNTER
    const RecompClock* k = &recomp_clock_state;
    const uint32_t s = __atomic_load_n(&k->seq, __ATOMIC_ACQUIRE);
    const uint64_t c0 = __atomic_load_n(&k->c0, __ATOMIC_RELAXED), n0 = __atomic_load_n(&k->n0, __ATOMIC_RELAXED);
    const uint64_t mul = __atomic_load_n(&k->mul, __ATOMIC_RELAXED), lim = __atomic_load_n(&k->lim, __ATOMIC_RELAXED);
    uint64_t t, z;
    /* isb: the counter is read after the loads above; z (always 0) makes the re-read of seq
     * depend on the counter value, so it happens after the counter read (the vDSO's ordering) */
    __asm__ __volatile__("isb\n\tmrs %0, cntvct_el0\n\teor %1, %0, %0" : "=r"(t), "=r"(z) : : "memory");
    __atomic_thread_fence(__ATOMIC_ACQUIRE);
    const uint32_t s2 = __atomic_load_n((const uint32_t*)((const char*)&k->seq + z), __ATOMIC_RELAXED);
    const uint64_t d = t - c0;
    if (__builtin_expect(((s ^ s2) | (s & 1u)) == 0 && d < lim, 1))
        return n0 + (uint64_t)(((unsigned __int128)d * mul) >> 32);
#endif
    return recomp_clock_slow();
}

#ifdef __cplusplus
}
#endif

#endif /* RECOMP_CLOCK_H */
