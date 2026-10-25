// Behaviour test for 0x00AF8D50 (source body copied from src/00af8d50.cpp; the retail inline-asm idioms are
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
// GFPackFloat(float value, unsigned long numBits, float minValue, float maxValue, unsigned long numDecimalPlaces) -- quantises a float: rounds it to numDecimalPlaces decimals, normalises it into [minValue, maxValue] and scales it to a numBits-bit integer
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00AF8D70 `fstp [esp+0Ch]` / 0x00AF8D74 `fld dword ptr [esp+0Ch]` + 0x00AF8D78 `fistp dword ptr [esp+4]` and 0x00AF8DA8 / 0x00AF8DAC + 0x00AF8DB0 `fistp dword ptr [esp+0Ch]` -- two expansions of the engine's inline-assembly GFFloatToLong helper (by-value copy stored and reloaded, bare dword fistp). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GFPackFloat@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8D50, length 110 bytes, __fastcall (numBits in ecx -- `shl eax,cl` --,
//   numDecimalPlaces in edx -- the `dec edx ; fmul 10.0` loop --, the three floats on the stack), `ret 0Ch`.
// Signature from FableWin.pdb: ?GFPackFloat@@YIKMKMMK@Z (the paired ?GFUnpackFloat@@YIMKKMMK@Z is its inverse). No
//   direct caller in retail.
// Constants: 1.0f @ 0x0122DED8, 10.0f @ 0x01237F0C.
// Retail-asm evidence: `fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword ptr [esp+4]` and `fstp [esp+10h] ; fld [esp+10h] ;
//   fistp dword ptr [esp+0Ch]` -- a float spilled and reloaded straight into a bare fistp, the TLC GFFloatToLong inline
//   asm (C++ conversions call _ftol2 / set the rounding mode).
// Label: catalog "Audio_CalculateFilterParameter" is a guess.
// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long GFFloatToLong(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}

unsigned long __fastcall GFPackFloat(float value, unsigned long numBits, float minValue, float maxValue, unsigned long numDecimalPlaces)
{
    float scale = 1.0f;
    for (unsigned long i = numDecimalPlaces; i != 0; --i)
        scale *= 10.0f;
    float rounded = (float)GFFloatToLong(value * scale) / scale;
    return (unsigned long)GFFloatToLong((rounded - minValue) / (maxValue - minValue) * ((float)(long)(1 << numBits) - 1.0f));
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
    check(GFPackFloat(0.5f, 8, 0.0f, 1.0f, 2) == 128, "127.5 -> 128");
    check(GFPackFloat(0.25f, 4, 0.0f, 1.0f, 1) == 3, "0.25 -> 0.2 (tie to even) -> 3");
    check(GFPackFloat(1.0f, 10, -1.0f, 1.0f, 0) == 1023, "max");
    check(GFPackFloat(-1.0f, 10, -1.0f, 1.0f, 0) == 0, "min");
    if (!g_fail) printf("00af8d50_TEST PASS\n");
    return g_fail;
}