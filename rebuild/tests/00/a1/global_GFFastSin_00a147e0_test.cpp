// Behaviour test for 0x00A147E0 (source body copied from src/00a147e0.cpp; the retail inline-asm idioms are
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
// GFFastSin(float angle) -- sine of a fraction of a full turn, linearly interpolated in the 1024-step cosine table
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00A147F7 `fstp [esp+4]` / 0x00A147FB `fld dword ptr [esp+4]` + 0x00A147FF `fistp dword ptr [esp]` -- the engine's inline-assembly float-to-long helper (the angle*1024 - 0.5 argument stored to its by-value copy in the frame and reloaded, a bare dword fistp into the result word), the same helper as the landed retail_asm 0x00AF9000/0x00B10220; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GFFastSin@@YIMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A147E0, 86 bytes, `ret 4` (__fastcall; the float travels on the stack). No direct
//   callers (the body is inlined everywhere: GetRandomUnitVector 0x00A13EB0 and ~45 other table readers). Name from the
//   FableWin PDB (`?GFFastSin@@YIMM@Z`); the catalog's "Math_InterpolateSineLookup" is descriptive.
// Globals: cosine table 0x013CD550 (1025 floats, cos(k*pi/1024 * 2), filled by the start-up loop at 0x00A0DBB0 with fcos);
//   1024.0f 0x01230010, 0.5f 0x0122F59C, 1.0f 0x0122DED8.
// Shape: angle scaled in place in its argument slot (`fst [esp+0Ch]`), index rounded by fistp, fraction from fild, index
//   shifted a quarter turn back (-256: cos(a - 1/4 turn) = sin a) and masked to 10 bits; lerp of two table entries.
extern float g_CosTable[1025];          // 0x013CD550: cos(2*pi*k/1024), k = 0..1024

// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long FloatToLong(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}

float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

// ---- harness ----

#include <math.h>
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
// independent reference: round half to even (the x87 default mode a bare fistp uses)
static long ref_rne(double v) { double f = floor(v); double d = v - f; long r = (long)f; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long ref_trunc(double v) { long r = ref_rne(v - 0.5); if (r < 0) ++r; return r; }
static int near_(double a, double b, double eps) { return fabs(a - b) <= eps; }

float g_CosTable[1025];
static void fill_cos() { for (int k = 0; k <= 1024; ++k) g_CosTable[k] = (float)cos(6.283185307179586 * k / 1024.0); }

int main()
{
    fill_cos();
    check(near_(GFFastSin(0.25f), 1.0, 1e-6), "sin quarter turn");
    check(near_(GFFastSin(0.0f), 0.0, 1e-6), "sin 0");
    check(near_(GFFastSin(0.1f), sin(0.2 * 3.141592653589793), 1e-4), "sin 0.1 turn");
    check(near_(GFFastSin(-0.3f), sin(-0.6 * 3.141592653589793), 1e-4), "sin negative");
    if (!g_fail) printf("00a147e0_TEST PASS\n");
    return g_fail;
}