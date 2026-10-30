// CDspFilter::SetHighPass(long attenuation, float reference, unsigned long sampleRate) -- set the six filter coefficients (+4..+0x18) for a first-order high-pass: no attenuation gives the pass-through set {1,0,0, 1,0,0}; otherwise the corner reference * 2^(-(attenuation+300)/600) (clamped to 0.495 * the sample rate) gives c = cos(2 pi f / rate), a = (1 - c/2) / (c - 1/2), the pole t = a - sqrt(a*a - 1) (a + sqrt(...) when that leaves [-1, 1]) and the gain g = 2 / (t + 1) capped at 10: {1, t, 0, g, -g, 0}
// Grade: DIFFER(293v293) -- scheduling only: retail loads the sample rate for the unsigned conversion (`mov eax,[esp+14h] ; test eax,eax`) before `fmul [reference]`, ours after it (`fmul ; mov eax ; fild ; test`); every other byte matches (RTM 3077 and QFE 4035).
// Leaf: ?SetHighPass@CDspFilter@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74BB0, 293 bytes (+ int3 padding), thiscall `ret 0Ch`.
// Callers: the reverb dispatcher 0x00C78462 (CReverbProcessor's update for its dirty bit: `state+0x1C = 1.0f`, then this on
//   state+0x1C with params+8 (millibels), params+0x1C and the mixer format's sample rate) and the unreferenced twin wrapper
//   0x00C777F2. Callee: _CIpow 0x00BFEB40 (pow(10.0, ...)); cos and sqrt are the inline x87 intrinsics (fcos / fsqrt).
// Identity (descriptive; the libpk DSP module has no PDB/RTTI): the coefficient block sits between CDspGain (0x00C744E0..)
//   and CDelayLine (0x00C74F60); 0x00C74AB0, the routine before it, fills the same six floats with a fptan (bilinear)
//   second-order set. Read as {a0, a1, a2, b0, b1, b2} this one is g (1 - z^-1) / (1 + t z^-1). The 10^(x * -log10(2)/600)
//   exponent constant is 0x012BF06C = -0.000501716684f; 0.495f 0x012BF064; 2 pi 0x0128F608; 10.0 (double) 0x0129A7B0.
//   The manifest's "CAudioChannel_ComputePeakFilterCoefficients" is propagated.
// Shape: the sample rate converts unsigned (fild + 2^32 fix-up) and is homed in the dead `reference` slot, the limit in
//   the dead `attenuation` slot; the frequency stays on the x87 stack through the clamp.
extern "C" double __cdecl pow(double x, double y);
extern "C" double __cdecl cos(double x);
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(pow, cos, sqrt)

class CDspFilter {
public:
    float m_gain;                               // +0x00 (the dispatcher sets 1.0f before the update)
    float m_a0;                                 // +0x04
    float m_a1;                                 // +0x08
    float m_a2;                                 // +0x0c
    float m_b0;                                 // +0x10
    float m_b1;                                 // +0x14
    float m_b2;                                 // +0x18
    void SetHighPass(long attenuation, float reference, unsigned long sampleRate);
};

void CDspFilter::SetHighPass(long attenuation, float reference, unsigned long sampleRate)
{
    if (attenuation == 0) {
        m_a0 = 1.0f;
        m_a1 = 0.0f;
        m_a2 = 0.0f;
        m_b0 = 1.0f;
        m_b1 = 0.0f;
        m_b2 = 0.0f;
        return;
    }
    float frequency = (float)pow(10.0, (attenuation + 300) * -0.000501716684f) * reference;
    float rate = (float)sampleRate;
    float limit = rate * 0.495f;
    if (frequency > limit)
        frequency = limit;
    float c = (float)cos(frequency * 6.28318548f / rate);
    float a = (1.0f - 0.5f * c) / (c - 0.5f);
    float s = (float)sqrt(a * a - 1.0f);
    float t = a - s;
    if (t > 1.0f || t < -1.0f)
        t = a + s;
    float g = 2.0f / (t + 1.0f);
    if (g > 10.0f)
        g = 10.0f;
    m_a0 = 1.0f;
    m_a1 = t;
    m_a2 = 0.0f;
    m_b0 = g;
    m_b1 = -g;
    m_b2 = 0.0f;
}
