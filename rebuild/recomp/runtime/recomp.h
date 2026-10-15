/*
 * Runtime support for statically recompiled Fable.exe code.
 *
 * Every guest function becomes `void F_xxxxxxxx(Ctx* c)`. Guest memory is a
 * flat 32-bit space at `g_mem` (the PE image is mapped at its retail base
 * 0x00400000), so guest pointers stay 32-bit on 64-bit hosts (x64, ARM64).
 *
 * Generated code keeps the eight GPRs, the lazy flag state and (in functions
 * with x87 code) the x87 registers, TOP, control and status words in C locals
 * and writes them back to the context only at calls, returns and indirect
 * branches, so the C compiler can optimise straight-line code freely.
 *
 * Must be compiled without floating-point contraction (-ffp-contract=off):
 * x87 values are modelled as IEEE double, which matches retail bit-for-bit
 * under the MSVC 7.1 control word (53-bit precision, round-to-nearest).
 */
#ifndef FABLE_RECOMP_H
#define FABLE_RECOMP_H

#include <math.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union Xmm {
    uint8_t b[16];
    uint16_t w[8];
    uint32_t d[4];
    uint64_t q[2];
    float f[4];
    double fd[2];
} Xmm;

typedef struct X87 {
    double st[8];
    uint32_t top;   /* physical index of ST(0) */
    uint16_t cw;    /* control word */
    uint16_t sw;    /* status word without TOP */
} X87;

typedef struct Ctx {
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t fs_base; /* guest address of this thread's TEB */
    uint32_t df;      /* direction flag */
    uint32_t extflags; /* EFLAGS AC (bit 18) and ID (bit 21): software-toggled for CPU detection */
    X87 fpu;
    Xmm xmm[8];
    uint64_t mm[8];
    uint32_t mxcsr;
} Ctx;

/* Guest memory. Default: a flat 4 GiB reservation at `g_mem`. With
 * RECOMP_IDENTITY_MEMORY (Windows host) guest address == host address: the
 * guest image, heap and stacks live in the host's low 4 GiB, so guest
 * pointers can be passed straight to host APIs. */
#if defined(RECOMP_IDENTITY_MEMORY)
#define GP(a) ((uint8_t*)(uintptr_t)(uint32_t)(a))
#else
extern uint8_t* g_mem;
#define GP(a) (g_mem + (uint32_t)(a))
#endif

typedef void (*GuestFn)(Ctx*);

/* Indirect call/jump: run the guest code at `target` (function, import thunk
 * or other entry). Implemented by the runtime. */
void recomp_dispatch(Ctx* c, uint32_t target);
/* Indirect calls from lifted code go through a per-site inline cache: (index << 32) | address
 * of the last target, index into recomp_fns (1-based, 0 = empty). A 64-bit store/load is
 * single-copy atomic on the hosts, so guest threads race on it benignly. */
/* Hidden on ELF: defined in the same image as all lifted code, so the hit path loads it
 * PC-relative instead of through the GOT (one dependent load less per indirect call). */
#if defined(__ELF__) && (defined(__GNUC__) || defined(__clang__))
__attribute__((visibility("hidden")))
#endif
extern GuestFn* recomp_fns;
void recomp_dispatch_ic(Ctx* c, uint32_t target, uint64_t* ic);
/* Guest range [lo, end) of host-implemented targets (traps), never lifted code: the dispatch
 * hands those to the host's unknown-target hook before any table lookup. */
void recomp_set_host_range(uint32_t lo, uint32_t end);
#if defined(__GNUC__) || defined(__clang__)
#define RECOMP_IC_LOAD(p) __atomic_load_n((p), __ATOMIC_RELAXED)
#else
#define RECOMP_IC_LOAD(p) (*(volatile uint64_t*)(p))
#endif
#define RECOMP_ICALL(c, t) do { static uint64_t _ic; uint64_t _v = RECOMP_IC_LOAD(&_ic); \
    if ((uint32_t)_v == (uint32_t)(t) && _v) recomp_fns[(uint32_t)(_v >> 32)](c); else recomp_dispatch_ic((c), (t), &_ic); } while (0)
/* Reached code the lifter could not translate, or a guest fault. */
void recomp_fatal(Ctx* c, uint32_t eip, const char* what);
/* C++ exception landing pads. Every lifted function that registers an MSVC EH frame
 * pushes one on entry (popped on return by a cleanup attribute). When the host's
 * _CxxThrowException has run a catch block, recomp_resume_at() longjmps to the
 * innermost such function whose frame holds the catching EH registration node and
 * continues it at the catch continuation `target`. One chain per thread (the host
 * swaps it when switching guest coroutine fibers). */
#if defined(__aarch64__)
/* Clang has no __builtin_setjmp on AArch64: the landing pads use the C library's _setjmp
 * (which does not save the signal mask, so it costs about the same). */
#include <setjmp.h>
#define __builtin_setjmp(b) _setjmp(b)
#define __builtin_longjmp(b, v) _longjmp(b, v)
typedef jmp_buf RecompJmpBuf;
#else
typedef void* RecompJmpBuf[5];   /* __builtin_setjmp buffer */
#endif
typedef struct RecompLanding {
    RecompJmpBuf jb;
    uint32_t entry_esp;           /* guest esp on entry (the frame lies below it) */
    uint32_t target;              /* continuation address after a longjmp */
    struct RecompLanding* prev;
} RecompLanding;
void recomp_landing_push(RecompLanding* l, uint32_t entry_esp);
void recomp_landing_pop(RecompLanding* l);
void* recomp_landing_chain_get(void);
void recomp_landing_chain_set(void* chain);
/* Returns only if no landing covers `frame` (then the caller reports the error). */
void recomp_resume_at(Ctx* c, uint32_t frame, uint32_t target);

/* Debug hook: functions lifted with --trace call this on entry (no-op unless set). */
extern void (*recomp_on_trace)(Ctx* c, uint32_t fn);
static inline void recomp_trace(Ctx* c, uint32_t fn) { if (recomp_on_trace) recomp_on_trace(c, fn); }

/* Optional host services used by a few instructions. */
void recomp_cpuid(Ctx* c);
uint64_t recomp_rdtsc(void);

/* ---- guest memory ---------------------------------------------------------- */

/* Accessors on a host pointer into guest memory (the `p` forms), and on a guest address. */
static inline uint8_t rd8p(const uint8_t* p) { return *p; }
static inline uint16_t rd16p(const uint8_t* p) { uint16_t v; memcpy(&v, p, 2); return v; }
static inline uint32_t rd32p(const uint8_t* p) { uint32_t v; memcpy(&v, p, 4); return v; }
static inline uint64_t rd64p(const uint8_t* p) { uint64_t v; memcpy(&v, p, 8); return v; }
static inline float rdf32p(const uint8_t* p) { float v; memcpy(&v, p, 4); return v; }
static inline double rdf64p(const uint8_t* p) { double v; memcpy(&v, p, 8); return v; }
static inline void wr8p(uint8_t* p, uint8_t v) { *p = v; }
static inline void wr16p(uint8_t* p, uint16_t v) { memcpy(p, &v, 2); }
static inline void wr32p(uint8_t* p, uint32_t v) { memcpy(p, &v, 4); }
static inline void wr64p(uint8_t* p, uint64_t v) { memcpy(p, &v, 8); }
static inline void wrf32p(uint8_t* p, float v) { memcpy(p, &v, 4); }
static inline void wrf64p(uint8_t* p, double v) { memcpy(p, &v, 8); }
static inline void rdxmmp(Xmm* x, const uint8_t* p) { memcpy(x->b, p, 16); }
static inline void wrxmmp(uint8_t* p, const Xmm* x) { memcpy(p, x->b, 16); }

static inline uint8_t rd8(uint32_t a) { return rd8p(GP(a)); }
static inline uint16_t rd16(uint32_t a) { return rd16p(GP(a)); }
static inline uint32_t rd32(uint32_t a) { return rd32p(GP(a)); }
static inline uint64_t rd64(uint32_t a) { return rd64p(GP(a)); }
static inline float rdf32(uint32_t a) { return rdf32p(GP(a)); }
static inline double rdf64(uint32_t a) { return rdf64p(GP(a)); }
static inline void wr8(uint32_t a, uint8_t v) { wr8p(GP(a), v); }
static inline void wr16(uint32_t a, uint16_t v) { wr16p(GP(a), v); }
static inline void wr32(uint32_t a, uint32_t v) { wr32p(GP(a), v); }
static inline void wr64(uint32_t a, uint64_t v) { wr64p(GP(a), v); }
static inline void wrf32(uint32_t a, float v) { wrf32p(GP(a), v); }
static inline void wrf64(uint32_t a, double v) { wrf64p(GP(a), v); }
static inline void rdxmm(Xmm* x, uint32_t a) { rdxmmp(x, GP(a)); }
static inline void wrxmm(uint32_t a, const Xmm* x) { wrxmmp(GP(a), x); }

/* A lifted memory operand `base [+ index * scale] + disp` with a small displacement
 * (|disp| < 64 KiB, emitted by the lifter): the base part wraps to 32 bits like x86, the
 * displacement is added to the host pointer, so the compiler folds it into a [reg, #imm]
 * addressing mode instead of a separate 32-bit add before every access. The two forms
 * differ only when the x86 sum wraps around 4 GiB, which lands in the first or last 64 KiB
 * of the guest space: never mapped (the guest's null page / the kernel half), a fault in
 * retail as well. */
#define MA(base, disp) (GP(base) + (int32_t)(disp))

/* ---- lazy integer flags -----------------------------------------------------
 * A flag-setting instruction records (op, result, a, b); consumers derive the
 * flag they need. `op` encodes kind and operand size; it is usually a
 * compile-time constant at the consumer, so the switch folds away. */

enum {
    FK_ADD = 0, FK_SUB, FK_LOGIC, FK_INC, FK_DEC, FK_SHL, FK_SHR, FK_SAR,
    FK_NEG, FK_MUL, FK_ADC, FK_SBB, FK_EXPLICIT, FK_ROL, FK_ROR
};
#define FOP(kind, bytes) ((kind) * 8 + (bytes))
#define FOP_KIND(op) ((op) >> 3)
#define FOP_BYTES(op) ((op) & 7)

/* FK_EXPLICIT packs EFLAGS-style bits into `r`. */
enum { EF_CF = 0x1, EF_PF = 0x4, EF_ZF = 0x40, EF_SF = 0x80, EF_OF = 0x800 };

static inline uint32_t f_mask(int op) {
    switch (FOP_BYTES(op)) {
    case 1: return 0xFFu;
    case 2: return 0xFFFFu;
    default: return 0xFFFFFFFFu;
    }
}
static inline uint32_t f_sign(int op) {
    switch (FOP_BYTES(op)) {
    case 1: return 0x80u;
    case 2: return 0x8000u;
    default: return 0x80000000u;
    }
}

/* For FK_INC/FK_DEC `b` carries the preserved CF; for FK_ADC/FK_SBB the carry-in;
 * for shifts `b` is the (non-zero) count and `a` the source; FK_MUL: r = overflow flag. */
static inline int f_cf(int op, uint32_t r, uint32_t a, uint32_t b) {
    const uint32_t m = f_mask(op);
    switch (FOP_KIND(op)) {
    case FK_ADD: return (r & m) < (a & m);
    case FK_ADC: return b ? (r & m) <= (a & m) : (r & m) < (a & m);
    case FK_SUB: return (a & m) < (b & m);
    case FK_SBB: return 0; /* resolved at emit time into FK_EXPLICIT */
    case FK_LOGIC: return 0;
    case FK_INC: case FK_DEC: return (int)b;
    case FK_SHL: return (int)((a >> ((FOP_BYTES(op) * 8) - b)) & 1u);
    case FK_SHR: case FK_SAR: return (int)((a >> (b - 1)) & 1u);
    case FK_NEG: return (a & m) != 0;
    case FK_MUL: return (int)r;
    case FK_ROL: return (int)(r & 1u);
    case FK_ROR: return (r & f_sign(op)) != 0;
    case FK_EXPLICIT: return (r & EF_CF) != 0;
    }
    return 0;
}
static inline int f_zf(int op, uint32_t r) {
    if (FOP_KIND(op) == FK_EXPLICIT) return (r & EF_ZF) != 0;
    return (r & f_mask(op)) == 0;
}
static inline int f_sf(int op, uint32_t r) {
    if (FOP_KIND(op) == FK_EXPLICIT) return (r & EF_SF) != 0;
    return (r & f_sign(op)) != 0;
}
static inline int f_pf(int op, uint32_t r) {
    if (FOP_KIND(op) == FK_EXPLICIT) return (r & EF_PF) != 0;
    uint32_t v = r & 0xFFu;
    v ^= v >> 4; v ^= v >> 2; v ^= v >> 1;
    return !(v & 1u);
}
static inline int f_of(int op, uint32_t r, uint32_t a, uint32_t b) {
    const uint32_t s = f_sign(op);
    switch (FOP_KIND(op)) {
    case FK_ADD: case FK_ADC: return ((~(a ^ b) & (a ^ r)) & s) != 0;
    case FK_SUB: case FK_SBB: return (((a ^ b) & (a ^ r)) & s) != 0;
    case FK_INC: return (r & f_mask(op)) == s;
    case FK_DEC: return (r & f_mask(op)) == s - 1u;
    case FK_NEG: return (r & f_mask(op)) == s;
    case FK_SHL: return ((r ^ (a << (b - 1))) & s) != 0; /* defined for count 1 */
    case FK_SHR: return b == 1 ? (a & s) != 0 : 0;
    case FK_MUL: return (int)r;
    case FK_EXPLICIT: return (r & EF_OF) != 0;
    default: return 0;
    }
}

static inline uint32_t f_pack(int op, uint32_t r, uint32_t a, uint32_t b) {
    return (f_cf(op, r, a, b) ? EF_CF : 0) | (f_pf(op, r) ? EF_PF : 0) | (f_zf(op, r) ? EF_ZF : 0) |
           (f_sf(op, r) ? EF_SF : 0) | (f_of(op, r, a, b) ? EF_OF : 0);
}

/* adc/sbb resolve their flags eagerly into an FK_EXPLICIT word. */
static inline uint32_t f_adc_flags(int bytes, uint32_t a, uint32_t b, uint32_t cin, uint32_t r) {
    const int op = FOP(FK_ADD, bytes);
    const uint32_t m = f_mask(op), s = f_sign(op);
    const uint64_t wide = (uint64_t)(a & m) + (uint64_t)(b & m) + (uint64_t)cin;
    uint32_t ef = (wide > m) ? EF_CF : 0;
    if ((~(a ^ b) & (a ^ r)) & s) ef |= EF_OF;
    if ((r & m) == 0) ef |= EF_ZF;
    if (r & s) ef |= EF_SF;
    if (f_pf(op, r)) ef |= EF_PF;
    return ef;
}
static inline uint32_t f_sbb_flags(int bytes, uint32_t a, uint32_t b, uint32_t cin, uint32_t r) {
    const int op = FOP(FK_SUB, bytes);
    const uint32_t m = f_mask(op), s = f_sign(op);
    uint32_t ef = ((uint64_t)(a & m) < (uint64_t)(b & m) + (uint64_t)cin) ? EF_CF : 0;
    if (((a ^ b) & (a ^ r)) & s) ef |= EF_OF;
    if ((r & m) == 0) ef |= EF_ZF;
    if (r & s) ef |= EF_SF;
    if (f_pf(op, r)) ef |= EF_PF;
    return ef;
}

/* Condition codes, numbered like the x86 cc nibble. */
static inline int f_cond(int cc, int op, uint32_t r, uint32_t a, uint32_t b) {
    if (FOP_KIND(op) == FK_SUB) { /* cmp/sub fast paths */
        const uint32_t m = f_mask(op), s = f_sign(op);
        const uint32_t ua = a & m, ub = b & m;
        const int32_t sa = (int32_t)((ua ^ s) - s), sb = (int32_t)((ub ^ s) - s);
        switch (cc) {
        case 0x2: return ua < ub;
        case 0x3: return ua >= ub;
        case 0x4: return ua == ub;
        case 0x5: return ua != ub;
        case 0x6: return ua <= ub;
        case 0x7: return ua > ub;
        case 0xC: return sa < sb;
        case 0xD: return sa >= sb;
        case 0xE: return sa <= sb;
        case 0xF: return sa > sb;
        default: break;
        }
    }
    switch (cc) {
    case 0x0: return f_of(op, r, a, b);
    case 0x1: return !f_of(op, r, a, b);
    case 0x2: return f_cf(op, r, a, b);
    case 0x3: return !f_cf(op, r, a, b);
    case 0x4: return f_zf(op, r);
    case 0x5: return !f_zf(op, r);
    case 0x6: return f_cf(op, r, a, b) || f_zf(op, r);
    case 0x7: return !f_cf(op, r, a, b) && !f_zf(op, r);
    case 0x8: return f_sf(op, r);
    case 0x9: return !f_sf(op, r);
    case 0xA: return f_pf(op, r);
    case 0xB: return !f_pf(op, r);
    case 0xC: return f_sf(op, r) != f_of(op, r, a, b);
    case 0xD: return f_sf(op, r) == f_of(op, r, a, b);
    case 0xE: return f_zf(op, r) || (f_sf(op, r) != f_of(op, r, a, b));
    case 0xF: return !f_zf(op, r) && (f_sf(op, r) == f_of(op, r, a, b));
    }
    return 0;
}

/* ---- x87 ---------------------------------------------------------------------- */

#define X87_C0 0x0100u
#define X87_C1 0x0200u
#define X87_C2 0x0400u
#define X87_C3 0x4000u

static inline double* st_ref(Ctx* c, int i) { return &c->fpu.st[(c->fpu.top + (uint32_t)i) & 7u]; }
#define ST(i) (*st_ref(c, (i)))
static inline void fpush(Ctx* c, double v) { c->fpu.top = (c->fpu.top - 1u) & 7u; c->fpu.st[c->fpu.top] = v; }
static inline double fpop(Ctx* c) { double v = c->fpu.st[c->fpu.top]; c->fpu.top = (c->fpu.top + 1u) & 7u; return v; }

/* The `_cw` / `_sw` forms take the control word or status word as a value (or a pointer to
 * it): lifted functions keep the x87 stack, TOP, CW and SW in C locals (see the lifter's
 * x87 notes) and write them back to `c->fpu` only at calls and returns. The `Ctx` forms are
 * the same operations on `c->fpu` (host code, and functions lifted in the memory model). */

/* Precision control: 24-bit mode rounds every result to float (Direct3D sets
 * it unless D3DCREATE_FPU_PRESERVE); 53/64-bit modes are modelled as double. */
static inline double fprec_cw(uint32_t cw, double v) {
    return ((cw >> 8) & 3u) == 0 ? (double)(float)v : v;
}
static inline double fprec(Ctx* c, double v) { return fprec_cw(c->fpu.cw, v); }

static inline double fround_cw(uint32_t cw, double v) {
    switch ((cw >> 10) & 3u) {
    case 0: return nearbyint(v);
    case 1: return floor(v);
    case 2: return ceil(v);
    default: return trunc(v);
    }
}
static inline double fround_rc(Ctx* c, double v) { return fround_cw(c->fpu.cw, v); }
static inline int32_t fist32_cw(uint32_t cw, double v) {
    const double r = fround_cw(cw, v);
    if (!(r >= -2147483648.0 && r <= 2147483647.0)) return (int32_t)0x80000000u;
    return (int32_t)r;
}
static inline int16_t fist16_cw(uint32_t cw, double v) {
    const double r = fround_cw(cw, v);
    if (!(r >= -32768.0 && r <= 32767.0)) return (int16_t)0x8000;
    return (int16_t)r;
}
static inline int64_t fist64_cw(uint32_t cw, double v) {
    const double r = fround_cw(cw, v);
    if (!(r >= -9223372036854775808.0 && r < 9223372036854775808.0)) return (int64_t)0x8000000000000000ull;
    return (int64_t)r;
}
static inline int32_t fist32(Ctx* c, double v) { return fist32_cw(c->fpu.cw, v); }
static inline int16_t fist16(Ctx* c, double v) { return fist16_cw(c->fpu.cw, v); }
static inline int64_t fist64(Ctx* c, double v) { return fist64_cw(c->fpu.cw, v); }

/* fsin/fcos/fptan/fsincos: hardware range rule. +-inf -> invalid -> NaN;
 * finite |x| >= 2^63 -> operand unchanged and C2 set; otherwise C2 cleared. */
static inline int x87_trig_in_range_sw(uint16_t* sw, double v) {
    if (isinf(v)) { *sw &= (uint16_t)~X87_C2; return 1; } /* libm returns NaN, like hardware */
    if (fabs(v) >= 9223372036854775808.0) { *sw |= X87_C2; return 0; }
    *sw &= (uint16_t)~X87_C2;
    return 1;
}
static inline int x87_trig_in_range(Ctx* c, double v) { return x87_trig_in_range_sw(&c->fpu.sw, v); }

/* fcom/fucom: C3 C2 C0 = 000 (>), 001 (<), 100 (=), 111 (unordered). */
static inline uint16_t fcmp_sw(uint16_t sw0, double a, double b) {
    uint16_t sw = (uint16_t)(sw0 & ~(X87_C0 | X87_C2 | X87_C3));
    if (isnan(a) || isnan(b)) sw |= X87_C0 | X87_C2 | X87_C3;
    else if (a < b) sw |= X87_C0;
    else if (a == b) sw |= X87_C3;
    return sw;
}
static inline void fcmp(Ctx* c, double a, double b) { c->fpu.sw = fcmp_sw(c->fpu.sw, a, b); }
/* fcomi: result in ZF/PF/CF. */
static inline uint32_t fcmpi(double a, double b) {
    if (isnan(a) || isnan(b)) return EF_ZF | EF_PF | EF_CF;
    if (a < b) return EF_CF;
    if (a == b) return EF_ZF;
    return 0;
}
/* MSVC's _ftol2 (hooked as host_ftol2, and inlined by the lifter at direct call sites):
 * ST(0) truncated toward zero to edx:eax; NaN or out of range -> the integer indefinite. */
static inline int64_t ftol2_trunc(double v) {
    if (!(v > -9223372036854775808.0 && v < 9223372036854775808.0)) return (int64_t)0x8000000000000000ull;
    return (int64_t)v;
}
static inline uint16_t fnstsw_top(uint16_t sw, uint32_t top) { return (uint16_t)((sw & ~0x3800u) | ((top & 7u) << 11)); }
static inline uint16_t fnstsw(Ctx* c) { return fnstsw_top(c->fpu.sw, c->fpu.top); }

/* 80-bit extended <-> double. */
static inline double f80_load(uint32_t a) {
    uint64_t m;
    uint16_t se;
    memcpy(&m, GP(a), 8);
    memcpy(&se, GP(a + 8), 2);
    const int e = se & 0x7FFF;
    double v;
    if (e == 0 && m == 0) v = 0.0;
    else if (e == 0x7FFF) v = (m << 1) == 0 ? INFINITY : NAN;
    else v = ldexp((double)m, e - 16383 - 63);
    return (se & 0x8000) ? -v : v;
}
static inline void f80_store(uint32_t a, double v) {
    uint64_t m = 0;
    uint16_t se = signbit(v) ? 0x8000 : 0;
    const double x = fabs(v);
    if (isnan(v)) { m = 0xC000000000000000ull; se |= 0x7FFF; }
    else if (isinf(v)) { m = 0x8000000000000000ull; se |= 0x7FFF; }
    else if (x != 0.0) {
        int e;
        const double f = frexp(x, &e); /* x = f * 2^e, f in [0.5,1) */
        m = (uint64_t)ldexp(f, 64);
        se |= (uint16_t)(e - 1 + 16383);
    }
    memcpy(GP(a), &m, 8);
    memcpy(GP(a + 8), &se, 2);
}

/* ---- misc helpers --------------------------------------------------------------- */

static inline uint32_t rol32(uint32_t v, unsigned n) { n &= 31u; return n ? (v << n) | (v >> (32u - n)) : v; }
static inline uint32_t ror32(uint32_t v, unsigned n) { n &= 31u; return n ? (v >> n) | (v << (32u - n)) : v; }
static inline uint8_t rol8(uint8_t v, unsigned n) { n &= 7u; return n ? (uint8_t)((v << n) | (v >> (8u - n))) : v; }
static inline uint8_t ror8(uint8_t v, unsigned n) { n &= 7u; return n ? (uint8_t)((v >> n) | (v << (8u - n))) : v; }
static inline uint16_t rol16(uint16_t v, unsigned n) { n &= 15u; return n ? (uint16_t)((v << n) | (v >> (16u - n))) : v; }
static inline uint16_t ror16(uint16_t v, unsigned n) { n &= 15u; return n ? (uint16_t)((v >> n) | (v << (16u - n))) : v; }

static inline int16_t sat16(int32_t v) { return (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : v); }
static inline int8_t sat8(int32_t v) { return (int8_t)(v > 127 ? 127 : v < -128 ? -128 : v); }
static inline uint8_t satu8(int32_t v) { return (uint8_t)(v > 255 ? 255 : v < 0 ? 0 : v); }

#ifdef __cplusplus
}
#endif

#endif /* FABLE_RECOMP_H */
