// Behaviour test for 0x00C40F50 (source body copied from src/00c40f50.cpp; the retail inline-asm idioms are
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
// _global::AmplitudeToMillibels(float amplitude) -- -10000 mB (silence) for a zero amplitude; otherwise 2000 * log10(amplitude) rounded to an integer with the FPU's current rounding mode and clamped at -10000
// Grade: RELOCATION_MATCH (2 grades) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?AmplitudeToMillibels@@
// Retail-asm: 0x00C40F78 `fld dword ptr [esp+8]` + 0x00C40F7C `fistp dword ptr [esp]` -- libpk's inline-assembly float-to-long block (the product is stored back to the `amplitude` argument slot because the asm reads it, the result lands in the `push ecx` word); VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`. The log10 (`fldlg2 ; fxch ; fyl2x`) is the compiler's intrinsic expansion, not asm.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C40F50, 64 bytes, __cdecl, long in eax.
// Library: libpk audio (no PDB/RTTI); descriptive name. Called once, from 0x00C424FD. Constants: 0x0122DEDC = 0.0f,
//   0x012A2AD4 = 2000.0f. The manifest's "Math_ComputeLogValue" is propagated.
#pragma intrinsic(log10)
#ifdef _MSC_VER
extern "C" double __cdecl log10(double x);
#else
#include <math.h>   // test only: glibc declares log10 noexcept
#endif

long __cdecl AmplitudeToMillibels(float amplitude)
{
    if (amplitude != 0.0f) {
        long result;
        amplitude = (float)log10(amplitude) * 2000.0f;
        RA_FISTP(amplitude, result)
        if (result >= -10000)
            return result;
    }
    return -10000;
}

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }

int main()
{
    check(AmplitudeToMillibels(0.0f) == -10000, "silence");
    check(AmplitudeToMillibels(1.0f) == 0, "unity");
    check(AmplitudeToMillibels(0.1f) == -2000, "-20 dB");
    check(AmplitudeToMillibels(10.0f) == 2000, "+20 dB");
    check(AmplitudeToMillibels(0.000001f) == -10000, "clamped");
    check(AmplitudeToMillibels(0.5f) == ref_rne(2000.0 * log10(0.5)), "-6 dB");
    if (!g_fail) printf("00c40f50_TEST PASS\n");
    return g_fail;
}