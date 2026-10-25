// Behaviour test for 0x00C6CEF0 (source body copied from src/00c6cef0.cpp; the retail inline-asm idioms are
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
// CVtable_012bed7c::ComputeTapGain(SDelayTap* tap, float scale, long delay) -- fill a delay-tap record: the delay, the Q15 gain FloatToLong(scale * volume * 32767) capped (unsigned) at 0x7fff, and its 128-sample ramp step gain >> 7
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?ComputeTapGain@CVtable_012bed7c
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CEF0, 66 bytes (+ int3 padding), `ret 0Ch`.
// Module: libpk's software mixer, the multi-tap delay effect of vtable 0x012BED7C (ctor 0x00C6CA20 landed; tap record as
//   in the matched MixDelayTap 0x00C6CC10). Callers 0x00C6D018 / 0x00C6D037 / 0x00C6D0E7 / 0x00C6D106 fill the taps of the
//   +0x1e0 set from the effect's per-tap scales and delays.
// Retail-asm: 0x00C6CEFE `fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword [esp]` -- libpk's inline-assembly rounding helper (the product homed in the dead `scale` slot, the result in the `push ecx` slot). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
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
struct SDelayTap {
    long           m_delay;                        // +0x00
    unsigned short m_gain;                         // +0x04 Q15
    short          m_step;                         // +0x06 Q15 per sample
};
struct CVtable_012bed7c : CVtable_012beda4 {
    void ComputeTapGain(SDelayTap* tap, float scale, long delay);
};
void CVtable_012bed7c::ComputeTapGain(SDelayTap* tap, float scale, long delay)
{
    unsigned long gain = FloatToLong(scale * m_volume * 32767.0f);
    if (gain > 0x7fff)
        gain = 0x7fff;
    tap->m_gain = (unsigned short)gain;
    tap->m_delay = delay;
    tap->m_step = (short)((unsigned short)gain >> 7);
}

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }
#define OBJ CVtable_012bed7c

static long g_buf[0x40];
int main()
{
    OBJ* o = (OBJ*)g_buf;

    SDelayTap t;
    o->m_volume = 0.5f; o->ComputeTapGain(&t, 1.0f, 77);  check(t.m_gain == 16384 && t.m_step == 128 && t.m_delay == 77, "tap");
    o->m_volume = 3.0f; o->ComputeTapGain(&t, 1.0f, -5);  check(t.m_gain == 0x7fff && t.m_step == 255 && t.m_delay == -5, "clamp");
    if (!g_fail) printf("00c6cef0_TEST PASS\n");
    return g_fail;
}