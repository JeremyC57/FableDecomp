// Behaviour test for 0x00A22530 (source body copied from src/00a22530.cpp; the retail inline-asm idioms are
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
// GFFastArcCos(float cosine) -- arc cosine as a fraction of a full turn, linearly interpolated in the 1025-step acos table
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00A2254D `fstp [esp+4]` / 0x00A22551 `fld dword ptr [esp+4]` + 0x00A22555 `fistp dword ptr [esp]` -- the engine's inline-assembly float-to-long helper (the (c+1)*512.5 - 0.5 argument stored to its by-value copy in the frame and reloaded, a bare dword fistp into the result word; same helper as GFFastSin 0x00A147E0). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GFFastArcCos@@YIMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A22530, 88 bytes, `ret 4` (__fastcall). No direct callers (inlined at its users).
//   Name from the FableWin PDB (`?GFFastArcCos@@YIMM@Z`); the catalog's "Math_InterpolateTableLookup" is descriptive.
// Globals: arc-cosine table 0x013CB530 (1025 floats, acos(k*2/1025 - 1), filled at 0x00A0DBF8 through the _CIacos
//   import), 512.5f 0x0129CD1C, 1.0f 0x0122DED8, 0.5f 0x0122F59C, 1/(2pi) 0x01237F08 (the result is in turns).
// Shape: no index mask (the input is assumed within [-1, 1]); the lerp result is scaled by 1/(2pi).
extern float g_ArcCosTable[1026];       // 0x013CB530: acos(k*2/1025 - 1), k = 0..1024

// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long FloatToLong(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}

float __fastcall GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = FloatToLong(cosine - 0.5f);
    float frac = cosine - (float)i;
    return ((1.0f - frac) * g_ArcCosTable[i] + frac * g_ArcCosTable[i + 1]) * 0.15915494f;
}

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }

float g_ArcCosTable[1026];
int main()
{
    for (int k = 0; k <= 1024; ++k) g_ArcCosTable[k] = (float)acos(k * 2.0 / 1025.0 - 1.0);
    g_ArcCosTable[1025] = 0.0f;
    check(near_(GFFastArcCos(1.0f), 0.0, 1e-3), "acos 1");
    check(near_(GFFastArcCos(-1.0f), 0.5, 1e-3), "acos -1 = half turn");
    check(near_(GFFastArcCos(0.0f), 0.25, 1e-3), "acos 0 = quarter turn");
    check(near_(GFFastArcCos(0.5f), 1.0 / 6.0, 2e-3), "acos 0.5");
    if (!g_fail) printf("00a22530_TEST PASS\n");
    return g_fail;
}