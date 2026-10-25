// Behaviour test for 0x00C42F50 (source body copied from src/00c42f50.cpp; the retail inline-asm idioms are
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
// CAudioObject::SetGain(float gain) -- set the audio object's linear gain (+0x18) and, when it changed, its DirectSound-style volume in hundredths of a decibel (+0x1C: 2000 * log10(gain), floored at -10000, -10000 for silence) and dirty bit 2 of +0x14
// Grade: RELOCATION_MATCH (1 grade + 1 re-check) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?SetGain@CAudioObject@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42F50, 96 bytes, __fastcall `ret 4`, one `push ecx` frame word for the result.
// Library: libpk audio (no PDB/RTTI): descriptive names. Only caller: the `__stdcall` API wrapper 0x00C35AF0
//   (landed; reached from the landed CCodaPlayback SetSourceDecibels 0x00C49420). Same object as the slot-6/7
//   setters 0x00C43780 / 0x00C437A0 (dirty flags +0x14). The manifest's "Audio_SetDecibels" describes the effect.
// Constants: 0.0f @ 0x0122DEDC, 2000.0f @ 0x012A2AD4; -10000 = DSBVOLUME_MIN.
// Retail-asm: 0x00C42F8F `fld dword ptr [esp+8]` + 0x00C42F93 `fistp dword ptr [esp]` -- libpk's inline-assembly float-to-long block of AmplitudeToMillibels (the out-of-line copy is 0x00C40F50), expanded here as a __forceinline: the product is stored to the by-value parameter copy in the dead `gain` slot and reloaded, the result lands in the `push ecx` word; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Shape otherwise: `fld gain ; fld [ecx+18h] ; fucompp ; test ah,44h ; jnp` = `m_gain != gain`; the float copied as a
//   dword; `0.0f` loaded from the pool before the reload of the argument; the clamp `cmp eax,-10000 ; jge`.
#ifdef _MSC_VER
extern "C" double __cdecl log10(double);
#else
#include <math.h>   // test only: glibc declares log10 noexcept
#endif
#pragma intrinsic(log10)

class CAudioObject {
public:
    unsigned char m_pad00[0x14];
    unsigned long m_dirtyFlags;                 // +0x14
    float         m_gain;                       // +0x18 linear gain
    long          m_volume;                     // +0x1c hundredths of a decibel
    void SetGain(float gain);
};

// libpk's linear-to-millibel conversion (out-of-line copy 0x00C40F50); the asm makes VC expand it only as __forceinline
__forceinline long AmplitudeToMillibels(float amplitude)
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

void CAudioObject::SetGain(float gain)
{
    if (m_gain != gain) {
        m_gain = gain;
        m_volume = AmplitudeToMillibels(gain);
        m_dirtyFlags |= 2;
    }
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
    CAudioObject o; memset(&o, 0, sizeof(o));
    o.m_gain = 0.5f;
    o.SetGain(1.0f);  check(o.m_volume == 0 && o.m_dirtyFlags == 2 && o.m_gain == 1.0f, "unity");
    o.m_dirtyFlags = 0;
    o.SetGain(1.0f);  check(o.m_dirtyFlags == 0, "unchanged gain leaves the flags");
    o.SetGain(0.1f);  check(o.m_volume == -2000 && o.m_dirtyFlags == 2, "-20 dB");
    o.SetGain(0.0f);  check(o.m_volume == -10000, "silence");
    if (!g_fail) printf("00c42f50_TEST PASS\n");
    return g_fail;
}