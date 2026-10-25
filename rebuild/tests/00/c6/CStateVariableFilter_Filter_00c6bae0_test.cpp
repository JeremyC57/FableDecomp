// Behaviour test for 0x00C6BAE0 (source body copied from src/00c6bae0.cpp; the retail inline-asm idioms are
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
// CStateVariableFilter::Filter(short* samples, unsigned long count) -- low-pass a block of 16-bit samples in place: while the current coefficients differ from the targets the first samples run through FilterRamped until the ramp reaches the block length (then the targets are committed and the ramp restarts); the rest use the current f / damping
// Grade: RELOCATION_MATCH (3 grades: 12-byte coefficient struct copy, m_current.damping * m_band order, i = ramp before the commit test) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Leaf: ?Filter@CStateVariableFilter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6BAE0, 235 bytes (+ int3 padding), `ret 8`.
// Module: libpk's software mixer DSP (local model shared with 0x00C6B8C0 / 0x00C6BBD0). Callers: the voice mixer
//   0x00C5B003, 0x00C5BACD, 0x00C5BAE7. Callee: FilterRamped 0x00C6B8C0 (thiscall). Global: block length 0x0139C9F0.
// Retail-asm: 0x00C6BB87 `fsub [0.5f] ; fstp [esp+14h] ; fld [esp+14h] ; fistp dword [esp+10h] ; test ; jge ; inc` -- libpk's inline-assembly truncation helper. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Shape: `m_current.f != m_target.f` loads the target first (fucompp); the commit copies the three target words
//   through eax = this (`mov eax,esi ; mov [eax],edx ...`), the signature of an inlined struct assignment returning *this.
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
struct SFilterCoefficients {
    float         f;                     // frequency coefficient
    float         damping;               // damping (1/Q)
    float         gain;                  // gain coefficient
};
struct CStateVariableFilter {
    SFilterCoefficients m_current;       // +0x00 (retail copies the target over it as one 12-byte struct)
    SFilterCoefficients m_target;        // +0x0c
    float         m_band;                // +0x18 filter state
    float         m_low;                 // +0x1c filter state
    unsigned long m_rampPosition;        // +0x20 samples into the coefficient ramp
    void FilterRamped(short* samples, unsigned long count);
    void Filter(short* samples, unsigned long count);
};
void CStateVariableFilter::Filter(short* samples, unsigned long count)
{
    unsigned long i = 0;
    if (m_current.f != m_target.f || m_current.damping != m_target.damping) {
        unsigned long ramp = g_mixBlockLength - m_rampPosition;
        if (ramp > count)
            ramp = count;
        FilterRamped(samples, ramp);
        m_rampPosition += ramp;
        i = ramp;
        if (m_rampPosition == g_mixBlockLength) {
            m_current = m_target;
            m_rampPosition = 0;
        }
    }
    for (; i < count; ++i) {
        long in = samples[i];
        m_band = m_current.f * (((float)in - m_low) - m_current.damping * m_band) + m_band;
        m_low = m_current.f * m_band + m_low;
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

static int g_ramped = 0;
void CStateVariableFilter::FilterRamped(short* samples, unsigned long count) { for (unsigned long i = 0; i < count; ++i) samples[i] = 7; g_ramped += (int)count; }
int main()
{
    CStateVariableFilter s; memset(&s, 0, sizeof(s));
    short in[10] = { 100, 100, -50, 0, 300, 7, 9, 11, 1000, -1000 }, a[10], b[10];
    memcpy(a, in, sizeof(a));
    s.m_current.f = s.m_target.f = 0.5f; s.m_current.damping = s.m_target.damping = 0.25f;
    s.Filter(a, 6);
    float band = 0, low = 0; ref_svf(0.5f, 0.25f, &band, &low, in, b, 6);
    check(memcmp(a, b, 6 * sizeof(short)) == 0 && g_ramped == 0, "steady coefficients");
    memcpy(a, in, sizeof(a));
    s.m_band = 0; s.m_low = 0; s.m_target.f = 0.25f; s.m_rampPosition = 120;
    s.Filter(a, 10);
    check(g_ramped == 8 && a[0] == 7 && a[7] == 7, "ramp to the block end");
    check(s.m_rampPosition == 0 && s.m_current.f == 0.25f, "target committed");
    band = 0; low = 0; ref_svf(0.25f, 0.25f, &band, &low, in + 8, b, 2);
    check(a[8] == b[0] && a[9] == b[1], "tail with the new coefficients");
    if (!g_fail) printf("00c6bae0_TEST PASS\n");
    return g_fail;
}