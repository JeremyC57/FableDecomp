// Behaviour test for 0x00C3FF70 (source body copied from src/00c3ff70.cpp; the retail inline-asm idioms are
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
// _global::RoundHalfDownToLong(float value) -- subtract one half and round to an integer with the FPU's current rounding mode, then bias negative results up by one
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?RoundHalfDownToLong@@
// Retail-asm: 0x00C3FF7F `fld dword ptr [esp+8]` + 0x00C3FF83 `fistp dword ptr [esp]` -- libpk's inline-assembly float-to-long block on the argument (value-0.5f is stored back to the argument slot because the asm reads it, the result lands in the `push ecx` word); VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM and QFE, any /G or /Op) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si, so the bare dword fistp is the original's `__asm`.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3FF70, 32 bytes, __cdecl, long in eax.
// Library: libpk audio (no PDB/RTTI); descriptive name. Called once, from 0x00C3FFF3. Float constant 0x0122F59C =
//   0.5f. The manifest's "GFFloatToLongFloor" is propagated.
long __cdecl RoundHalfDownToLong(float value)
{
    long result;
    value -= 0.5f;
    RA_FISTP(value, result)
    if (result < 0)
        ++result;
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
    float v[] = { 3.0f, 3.2f, -1.0f, 0.5f, 1.0f, 2.0f, -3.7f, 100.25f };
    for (int i = 0; i < 8; ++i) check(RoundHalfDownToLong(v[i]) == ref_trunc(v[i]), "RoundHalfDownToLong");
    check(RoundHalfDownToLong(3.2f) == 3 && RoundHalfDownToLong(-1.0f) == -1, "spot");
    if (!g_fail) printf("00c3ff70_TEST PASS\n");
    return g_fail;
}