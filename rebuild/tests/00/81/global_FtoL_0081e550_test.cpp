// Behaviour test for 0x0081E550 (source body copied from src/0081e550.cpp; the retail inline-asm idioms are
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
// FtoL(float value) -- round a float to a long with the FPU's current rounding mode (the engine's out-of-line float-to-long helper)
// Grade: MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 3-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: FtoL
// Retail-asm: 0x0081E55A `fld dword ptr [esp+0Ch]` + 0x0081E55E `mov edx,[esp+4]` + 0x0081E562 `fistp dword ptr [edx]` -- the engine's inline-assembly FloatToLong(long* result, float value) helper (the same three instructions as the landed retail_asm 0x00B1B5E0), the result pointer homed at [esp+4] and reloaded into edx; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op /Og /G5..G7) emits `fistp qword`, /arch:SSE2 a cvttss2si, so no C/C++ form gives a dword fistp.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081E550, 29 bytes, `ret 4` (one float on the stack; ecx unused). No callees, no code xrefs.
// the engine's pointer-out float-to-long helper: expanded in place, the pointer parameter gets a frame copy
__forceinline void FloatToLong(long* result, float value)
{
    RA_FISTP_PTR(value, result)
}
long __stdcall FtoL(float value)
{
    long result;
    FloatToLong(&result, value);
    return result;
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
    float v[] = { 2.5f, 3.5f, -2.5f, 1.4f, -1.6f, 0.0f, 1000000.5f, -7.5f };
    for (int i = 0; i < 8; ++i) check(FtoL(v[i]) == ref_rne(v[i]), "FtoL");
    check(FtoL(2.5f) == 2 && FtoL(3.5f) == 4 && FtoL(-2.5f) == -2, "ties to even");
    if (!g_fail) printf("0081e550_TEST PASS\n");
    return g_fail;
}