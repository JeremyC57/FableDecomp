// Behaviour test for 0x00C6B8C0 (source body copied from src/00c6b8c0.cpp; the retail inline-asm idioms are
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
// CStateVariableFilter::FilterRamped(short* samples, unsigned long count) -- low-pass `count` 16-bit samples in place while the coefficients ramp: per sample t = (ramp position + i) / block length interpolates f and damping between the current and target values, then band += f * (in - low - damping * band), low += f * band, and the output is trunc(low) clamped to 16 bits
// Grade: RELOCATION_MATCH (3 grades: x87 operand order needs (1.0f - t) written inline in both lerps) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?FilterRamped@CStateVariableFilter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6B8C0, 219 bytes (+ int3 padding), `ret 8`.
// Module: libpk's software mixer DSP, the Chamberlin state-variable filter of the matched 0x00C6BBD0 / 0x00C6BC80 /
//   0x00C6BD00 rows (same local model). Caller: Filter 0x00C6BAE0 (the ramp part of a block). Global: the mix block length
//   0x0139C9F0 (unsigned, 128).
// Retail-asm: 0x00C6B951 `fsub [0.5f] ; fstp [esp+14h] ; fld [esp+14h] ; fistp dword [esp+8] ; test edx,edx ; jge ; inc edx` -- libpk's inline-assembly truncation helper (the per-sample f local's slot is reused for the homed value). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Shape: both unsigned-to-float conversions get the `jge ; fadd [2^32]` fix-up; the interpolated f is a
//   float local (`fstp [esp+14h]`, reloaded by both state updates); the clamp is signed (`jle` / `jge`).
// libpk's inline-assembly float-to-long helper. Retail homes the value in a dead stack slot and rounds it with a bare
// `fld [slot] ; fistp dword [tmp]` (round to nearest, the FPU default); VC7.1 expands the __forceinline helper in place.
__forceinline long FloatToLong(float value)
{
    long result;
    RA_FISTP(value, result)
    return result;
}
// truncation toward zero on top of the rounding helper: round(x - 0.5) is floor(x), negatives step back up by one
// (retail: `fsub [0.5f] ; fstp [slot] ; fld [slot] ; fistp dword [tmp] ; test ; jge ; inc`)
__forceinline long FloatToLongTruncate(float value)
{
    long result = FloatToLong(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}
extern unsigned long g_mixBlockLength;    // 0x0139C9F0 (128 samples)
struct CStateVariableFilter {
    float         m_f;                   // +0x00 current frequency coefficient
    float         m_damping;             // +0x04 current damping (1/Q)
    float         m_gain;                // +0x08 current gain coefficient
    float         m_targetF;             // +0x0c
    float         m_targetDamping;       // +0x10
    float         m_targetGain;          // +0x14
    float         m_band;                // +0x18 filter state
    float         m_low;                 // +0x1c filter state
    unsigned long m_rampPosition;        // +0x20 samples into the coefficient ramp
    void FilterRamped(short* samples, unsigned long count);
    void Filter(short* samples, unsigned long count);
};
void CStateVariableFilter::FilterRamped(short* samples, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i) {
        float t = (float)(m_rampPosition + i) / (float)g_mixBlockLength;
        long in = samples[i];
        float f = (1.0f - t) * m_f + t * m_targetF;
        float damping = (1.0f - t) * m_damping + t * m_targetDamping;
        m_band = f * (((float)in - m_low) - damping * m_band) + m_band;
        m_low = f * m_band + m_low;
        long out = FloatToLongTruncate(m_low);
        if (out > 0x7fff)
            out = 0x7fff;
        else if (out < -0x8000)
            out = -0x8000;
        samples[i] = (short)out;
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

static void ref_svf(float f, float damping, float* band, float* low, const short* in, short* out, int n)
{
    for (int i = 0; i < n; ++i) {
        *band = f * (((float)in[i] - *low) - damping * *band) + *band;
        *low = f * *band + *low;
        long o = ref_trunc(*low);
        out[i] = (short)(o > 0x7fff ? 0x7fff : (o < -0x8000 ? -0x8000 : o));
    }
}
unsigned long g_mixBlockLength = 128;

int main()
{
    CStateVariableFilter s; memset(&s, 0, sizeof(s));
    short in[6] = { 100, 100, -50, 0, 300, 7 }, a[6], b[6];
    memcpy(a, in, sizeof(a));
    s.m_f = s.m_targetF = 0.5f; s.m_damping = s.m_targetDamping = 0.25f; s.m_rampPosition = 0;
    s.FilterRamped(a, 6);
    float band = 0, low = 0; ref_svf(0.5f, 0.25f, &band, &low, in, b, 6);
    check(memcmp(a, b, sizeof(a)) == 0 && s.m_band == band && s.m_low == low, "filter");
    check(a[0] == 24 && a[1] == 65, "24.5 -> 24 and 65.125 -> 65 (ties to even)");
    s.m_f = s.m_targetF = 0.0f; s.m_low = 40000.0f; a[0] = 1; s.FilterRamped(a, 1); check(a[0] == 0x7fff, "clamp high");
    s.m_low = -40000.0f; s.FilterRamped(a, 1); check(a[0] == -0x8000, "clamp low");
    if (!g_fail) printf("00c6b8c0_TEST PASS\n");
    return g_fail;
}