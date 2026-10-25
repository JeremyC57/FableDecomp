// Behaviour test for 0x00B05C20 (source body copied from src/00b05c20.cpp; the retail inline-asm idioms are
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
// NParticleEngine::GFInterpolate(long a, long b, float t) -- integer linear interpolation a * (1 - t) + b * t, rounded to the nearest long
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00B05C41 `fstp [esp+0Ch]` / 0x00B05C45 `fld dword ptr [esp+0Ch]` + 0x00B05C49 `fistp dword ptr [esp+4]` -- the engine's inline-assembly GFFloatToLong helper (by-value copy stored and reloaded, bare dword fistp into a frame word). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GFInterpolate@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B05C20, length 55 bytes, __fastcall (a in ecx, b in edx, both spilled
//   for `fimul` / `fild`), `ret 4`.
// Signature from FableWin.pdb: ?GFInterpolate@NParticleEngine@@YIJJJM@Z (the long overload of the family). No direct
//   caller in retail (the CPSC InterpolateTo bodies inline it).
// Constants: 1.0f @ 0x0122DED8.
// Label: catalog "GFInterpolate" (_global) -- the namespace is NParticleEngine.
// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long GFFloatToLong(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}

namespace NParticleEngine {

long __fastcall GFInterpolate(long a, long b, float t)
{
    return GFFloatToLong((1.0f - t) * a + b * t);
}

} // namespace NParticleEngine

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
    using NParticleEngine::GFInterpolate;
    check(GFInterpolate(0, 10, 0.25f) == 2, "2.5 -> 2");
    check(GFInterpolate(0, 10, 0.75f) == 8, "7.5 -> 8");
    check(GFInterpolate(10, 20, 0.5f) == 15, "midpoint");
    check(GFInterpolate(-100, 100, 0.125f) == ref_rne(0.875 * -100 + 100 * 0.125), "negative");
    if (!g_fail) printf("00b05c20_TEST PASS\n");
    return g_fail;
}