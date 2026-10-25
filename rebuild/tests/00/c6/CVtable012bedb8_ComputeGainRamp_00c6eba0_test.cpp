// Behaviour test for 0x00C6EBA0 (source body copied from src/00c6eba0.cpp; the retail inline-asm idioms are
// spelled through RA_* macros: the same __asm under VC7.1, a round-half-to-even C fallback under g++).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
// the retail inline-asm idioms, exactly as in the source (one instruction per __asm, no trailing semicolon)
#define RA_FISTP(v, r) __asm fld v __asm fistp r
#define RA_FISTP_PTR(v, p) __asm fld v __asm mov edx, p __asm fistp dword ptr [edx]
#define RA_RDTSC0(c) __asm xor eax, eax __asm xor edx, edx __asm rdtsc __asm mov dword ptr c, eax __asm mov dword ptr c+4, edx
#define RA_RDTSC(c) __asm rdtsc __asm mov dword ptr c, eax __asm mov dword ptr c+4, edx
#else
// g++ C fallback (test only): fistp in the default FPU mode rounds to nearest, ties to even
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
static long ra_round(double v) { return (long)__builtin_nearbyint(v); }
static unsigned long long ra_tsc = 1000000;
#define RA_FISTP(v, r) (r) = ra_round(v);
#define RA_FISTP_PTR(v, p) *(p) = ra_round(v);
#define RA_RDTSC0(c) (c) = (ra_tsc += 123457);
#define RA_RDTSC(c) (c) = (ra_tsc += 123457);
#endif
// ---- source body ----
// CVtable_012bedb8::ComputeGainRamp(SGainRamp* ramp) -- fill a gain-ramp record from the effect volume alone: gain = min(FloatToLong(volume * 32767), 0x7fff) >> 1 (unsigned) and its 128-sample ramp step gain >> 7
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?ComputeGainRamp@CVtable_012bedb8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6EBA0, 62 bytes (+ int3 padding), `ret 4`.
// Module: libpk's software mixer, the gain-ramp effect of vtable 0x012BEDB8 (ctor 0x00C6E760 matched in this chunk). No
//   direct callers (address-taken).
// Retail-asm: 0x00C6EBAC `fstp [esp] ; fld [esp] ; fistp dword [esp+4]` -- libpk's inline-assembly rounding helper. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// libpk's inline-assembly float-to-long helper. Retail homes the value in a dead stack slot and rounds it with a bare
// `fld [slot] ; fistp dword [tmp]` (round to nearest, the FPU default); VC7.1 expands the __forceinline helper in place.
__forceinline long FloatToLong(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}
struct SCriticalSection { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CVtable_012beda4 {                          // vtable 0x012BEDA4, 0x28 bytes (ctor 0x00C6E6A0, dtor 0x00C6E630 landed)
    void*            _vtable;                      // +0x00
    void*            _lockVtable;                  // +0x04 critical-section owner (vtable 0x012B8FC8)
    SCriticalSection m_lock;                       // +0x08
    float            m_volume;                     // +0x20 linear (the landed dtor model calls it `long m_count`)
    bool             m_alternate;                  // +0x24
};
struct SGainRamp {
    unsigned short m_gain;                         // +0x00 Q15
    short          m_step;                         // +0x02 Q15 per sample
};
struct CVtable_012bedb8 : CVtable_012beda4 {
    void ComputeGainRamp(SGainRamp* ramp);
};
void CVtable_012bedb8::ComputeGainRamp(SGainRamp* ramp)
{
    unsigned long level = FloatToLong(m_volume * 32767.0f);
    if (level > 0x7fff)
        level = 0x7fff;
    unsigned short gain = (unsigned short)((unsigned short)level >> 1);
    ramp->m_gain = gain;
    ramp->m_step = (short)(gain >> 7);
}

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }
#define OBJ CVtable_012bedb8

static long g_buf[0x40];
int main()
{
    OBJ* o = (OBJ*)g_buf;

    SGainRamp r;
    o->m_volume = 0.5f; o->ComputeGainRamp(&r);  check(r.m_gain == 8192 && r.m_step == 64, "16384 >> 1");
    o->m_volume = 2.0f; o->ComputeGainRamp(&r);  check(r.m_gain == 0x3fff && r.m_step == 127, "clamp");
    o->m_volume = 0.0f; o->ComputeGainRamp(&r);  check(r.m_gain == 0 && r.m_step == 0, "zero");
    if (!g_fail) printf("00c6eba0_TEST PASS\n");
    return g_fail;
}