// Behaviour test for 0x00B0A420 (source body copied from src/00b0a420.cpp; the retail inline-asm idioms are
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
// NParticleEngine::CParticle::SetAngleChange(float angleChange) -- stores the per-frame spin as a signed byte (angleChange * 63.5, rounded)
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 3-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00B0A438 `fld dword ptr [esp+4]` + 0x00B0A43C `mov edx,[esp]` + 0x00B0A43F `fistp dword ptr [edx]` -- the engine's pointer-out inline-assembly GFFloatToLong(long* result, float value) helper (the landed retail_asm 0x00B1B5E0 body, 3 instr): the product is stored to the by-value copy at [esp+4] and reloaded, the result pointer copy at [esp] reloaded into edx; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?SetAngleChange@CParticle@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0A420, length 46 bytes, thiscall `ret 4`.
// Signature from FableWin.pdb: ?SetAngleChange@CParticle@NParticleEngine@@QAEXM@Z. Neighbours: SetAngle 0x00B0A410
//   (+0x24), UpdateAngle 0x00B0A450 (`Angle += AngleChange * k`).
// Layout (Ego_r.pdb): CParticle::AngleChange signed char at +0x28. Constant 63.5f @ 0x0129FC10.
// Label: catalog "PackUVForVertexBuffer" is a guess.
// the engine's pointer-out float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline,
// its two by-value parameters get frame copies
__forceinline void GFFloatToLong(long* result, float value)
{
    RA_FISTP_PTR(value, result)
}

namespace NParticleEngine {

class CParticle {
public:
    unsigned char _pad_0x00[0x28];
    signed char   AngleChange;                              // +0x28
    void SetAngleChange(float angleChange);
};

void CParticle::SetAngleChange(float angleChange)
{
    long change;
    GFFloatToLong(&change, angleChange * 63.5f);
    AngleChange = (signed char)change;
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
    NParticleEngine::CParticle p; memset(&p, 0, sizeof(p));
    p.SetAngleChange(1.0f);   check(p.AngleChange == 64, "63.5 -> 64");
    p.SetAngleChange(-1.0f);  check(p.AngleChange == -64, "-63.5 -> -64");
    p.SetAngleChange(0.5f);   check(p.AngleChange == 32, "31.75 -> 32");
    p.SetAngleChange(0.0f);   check(p.AngleChange == 0, "zero");
    if (!g_fail) printf("00b0a420_TEST PASS\n");
    return g_fail;
}