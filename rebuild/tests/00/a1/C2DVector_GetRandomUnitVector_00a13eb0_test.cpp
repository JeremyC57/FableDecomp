// Behaviour test for 0x00A13EB0 (source body copied from src/00a13eb0.cpp; the retail inline-asm idioms are
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
// C2DVector::GetRandomUnitVector(unsigned long* seed) -- a unit vector at a random angle: (cos, sin) of a random turn fraction from the cosine table
// Grade: RELOCATION_MATCH (3 grades: cos evaluated first via a C2DVector result local with X/Y assigned in order; the dead local keeps retail's extra frame word) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: two inlined copies of the engine's inline-assembly float-to-long helper (GFFastCos 0x00A14840 and GFFastSin 0x00A147E0 expanded in place, each storing angle*1024 - 0.5 to its own by-value copy, reloading it and rounding with a bare `fistp dword`); VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GetRandomUnitVector@C2DVector@@SI?AV1@PAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A13EB0, 191 bytes, plain `ret` (static __fastcall: hidden result pointer in ecx,
//   seed in edx). Caller 0x00B110BD. PDB name (Ego_r and FableWin) ?GetRandomUnitVector@C2DVector@@SI?AV1@PAK@Z.
// Callees: GFFloatRandom 0x004978A0 (`?GFFloatRandom@@YIMMPAK@Z`, seed in ecx, max 1.0f pushed); GFFastCos 0x00A14840
//   and GFFastSin 0x00A147E0 inlined (cosine table 0x013CD550).
// Shape: the random turn fraction is scaled by 1024 once (shared by both inlines), X = cos lookup (no shift), Y = sin
//   lookup (-256), X kept in ecx until the end, Y stored straight from st(0).
extern float g_CosTable[1025];          // 0x013CD550: cos(2*pi*k/1024), k = 0..1024

float __fastcall GFFloatRandom(float max, unsigned long* seed);    // 0x004978A0

// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long FloatToLong(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}

__forceinline float __fastcall GFFastCos(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i &= 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

__forceinline float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

class C2DVector {
public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
    static C2DVector __fastcall GetRandomUnitVector(unsigned long* seed);
};

C2DVector __fastcall C2DVector::GetRandomUnitVector(unsigned long* seed)
{
    float angle = GFFloatRandom(1.0f, seed);
    C2DVector result;
    result.X = GFFastCos(angle);
    result.Y = GFFastSin(angle);
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

float g_CosTable[1025];
static void fill_cos() { for (int k = 0; k <= 1024; ++k) g_CosTable[k] = (float)cos(6.283185307179586 * k / 1024.0); }

float __fastcall GFFloatRandom(float max, unsigned long* seed) { return max * (float)*seed / 1000.0f; }
int main()
{
    fill_cos();
    unsigned long seed = 250;
    C2DVector v = C2DVector::GetRandomUnitVector(&seed);
    check(near_(v.X, 0.0, 1e-5) && near_(v.Y, 1.0, 1e-5), "quarter turn");
    seed = 125;
    v = C2DVector::GetRandomUnitVector(&seed);
    check(near_(v.X, 0.70710678, 1e-4) && near_(v.Y, 0.70710678, 1e-4), "eighth turn");
    seed = 0;
    v = C2DVector::GetRandomUnitVector(&seed);
    check(near_(v.X, 1.0, 1e-6) && near_(v.Y, 0.0, 1e-6), "zero");
    if (!g_fail) printf("00a13eb0_TEST PASS\n");
    return g_fail;
}