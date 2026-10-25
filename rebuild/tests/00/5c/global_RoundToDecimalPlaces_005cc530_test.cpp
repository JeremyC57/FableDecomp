// Behaviour test for 0x005CC530 (source body copied from src/005cc530.cpp; the retail inline-asm idioms are
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
// RoundToDecimalPlaces(long decimalPlaces, float value) -- round value to the given number of decimal places: scale = 10^places (1.0f times 10.0f per place), value * scale rounded to the nearest integer, divided back by scale
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x005CC552 `fstp [esp+0Ch]` / 0x005CC556 `fld dword ptr [esp+0Ch]` + 0x005CC55A `fistp dword ptr [esp+4]` -- the engine's inline-assembly float-to-long helper (the product stored to its by-value copy in the dead `value` slot and reloaded, a bare dword fistp into a frame word; the asm barrier also spills `scale` to [esp]). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: RoundToDecimalPlaces
// Retail: Fable.exe @ 0x005CC530, 59 bytes, `ret 4`: __fastcall-shaped (places in ecx, the float on the stack), result in
// st(0). No direct callers (xref finds none) and no Ego_r.pdb symbol: name from the catalog's descriptive label.
// Constants: 0x0122DED8 = 1.0f (the scale's start), 0x01237F0C = 10.0f (one decimal place).
// Body: float scale = 1.0f; for (long n = places; n; --n) scale *= 10.0f; return (float)Round(value * scale) / scale;
//   the scale lives in st(0) through the loop with `fst [esp]` copies on both paths; the division reads the frame copy.
// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long FloatToLong(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}

float __fastcall RoundToDecimalPlaces(long decimalPlaces, float value)
{
    float scale = 1.0f;
    for (long n = decimalPlaces; n != 0; --n)
        scale *= 10.0f;
    long rounded = FloatToLong(value * scale);
    return (float)rounded / scale;
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
    check(near_(RoundToDecimalPlaces(2, 3.14159f), 3.14, 1e-6), "2 places");
    check(near_(RoundToDecimalPlaces(0, 2.5f), 2.0, 0), "0 places, tie to even");
    check(near_(RoundToDecimalPlaces(1, -1.25f), -1.2, 1e-6), "1 place, tie to even");
    check(near_(RoundToDecimalPlaces(3, 0.0005f), 0.0, 1e-6) || near_(RoundToDecimalPlaces(3, 0.0005f), 0.001, 1e-6), "3 places");
    if (!g_fail) printf("005cc530_TEST PASS\n");
    return g_fail;
}