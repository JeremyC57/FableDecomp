// CBiquadCoefficients::ComputeLowPass_00c74ab0(long cutoffCents, float baseFrequency, unsigned long sampleRate) -- libpk mixer DSP: a zero cutoff gives the pass-through set (1, 0, 0, 1, 0, 0); otherwise the cutoff is baseFrequency * 2^((cents + 300) / 1200) (pow(10, (cents + 300) * log10(2) / 1200)), clamped to 0.495 * sampleRate, and the second-order Butterworth low-pass terms are derived from k = tan(pi * f / rate)
// Grade: DIFFER(256v256) -- scheduling only: retail loads and tests the unsigned sample rate (mov eax,[esp+10h] ; test eax,eax) before fmul [baseFrequency], VC7.1 here after it. Tried: angle inline (fdiv before fmul pi), pi first, angle split into its own statement (this; fixes the pi/rate order). Next: convert the rate before the pow line
// Leaf: ?ComputeLowPass_00c74ab0@CBiquadCoefficients
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C74AB0, 256 bytes, thiscall `ret 0Ch`. Intrinsics: pow -> _CIpow 0x00BFEB40, tan -> fptan.
//   Constants: 10.0 0x0129A7B0 (double), log10(2)/1200 0x012BF068, 0.495 0x012BF064, pi 0x0128F604, sqrt(2) 0x012BF060,
//   1.0 0x0122DED8, 0.25 0x0123078C, 2^32 0x0122DCB4 (the unsigned sample-rate fix-up).
// Identity: callers 0x00C777C7 (a slot thunk passing params +0x0C cents, +0x20 frequency and the device rate) and 0x00C7847D (the
//   channel set-up, which computes the sibling 0x00C74BB0 into the next 0x1C-byte block at +0x1C). libpk has no PDB/RTTI: class and
//   method names are descriptive (the catalog's "CAudioChannel_ComputeShelveFilterCoefficients" is a placeholder).
// Layout (local model, descriptive): +0x00 untouched, then six float terms +0x04..+0x18.
#include <math.h>
#pragma intrinsic(pow, tan)
class CBiquadCoefficients {
public:
    unsigned long m_kind;               // +0x00 (not touched here)
    float         m_a0;                 // +0x04
    float         m_a1;                 // +0x08
    float         m_a2;                 // +0x0c
    float         m_b0;                 // +0x10
    float         m_b1;                 // +0x14
    float         m_b2;                 // +0x18
    void ComputeLowPass_00c74ab0(long cutoffCents, float baseFrequency, unsigned long sampleRate);
};

void CBiquadCoefficients::ComputeLowPass_00c74ab0(long cutoffCents, float baseFrequency, unsigned long sampleRate)
{
    if (cutoffCents == 0) {
        m_a0 = 1.0f;
        m_a1 = 0.0f;
        m_a2 = 0.0f;
        m_b0 = 1.0f;
        m_b1 = 0.0f;
        m_b2 = 0.0f;
        return;
    }
    float frequency = (float)pow(10.0, (double)((cutoffCents + 300) * 0.00025085834f)) * baseFrequency;
    float rate = (float)sampleRate;
    float limit = rate * 0.495f;
    if (frequency > limit)
        frequency = limit;
    float angle = frequency * 3.14159265f;
    m_a0 = 1.0f;
    float k = (float)tan(angle / rate);
    float kk = k * k;
    float d = k * 1.41421356f + kk + 1.0f;
    float a = (1.0f - kk) / d;
    float x = (kk + kk) / (d * d);
    float y = ((1.0f - a) * (1.0f - a) + x) * 0.25f;
    m_a1 = a + a;
    m_a2 = -(a * a + x);
    m_b0 = y;
    m_b1 = y + y;
    m_b2 = y;
}
