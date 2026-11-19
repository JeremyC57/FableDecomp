// CFloatFilterParams::Set_HighPass(int iAttenuation, float fReference, unsigned int uiSamplesPerSecond) -- set the six filter coefficients (m_fA[0..2], m_fB[0..2] at +4..+0x18) for a first-order high-pass: no attenuation gives the pass-through set {1,0,0, 1,0,0}; otherwise the corner reference * 2^(-(attenuation+300)/600) (clamped to 0.495 * the sample rate) gives c = cos(2 pi f / rate), a = (1 - c/2) / (c - 1/2), the pole t = a - sqrt(a*a - 1) (a + sqrt(...) when that leaves [-1, 1]) and the gain g = 2 / (t + 1) capped at 10: {1, t, 0, g, -g, 0}
// Leaf: ?Set_HighPass@CFloatFilterParams@@QAEXHMI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74BB0, 293 bytes (+ int3 padding), thiscall `ret 0Ch`.
// Name (NF99P2, q11508; was the descriptive CEffectFilter::SetHighPass over a local model): GameCODA 2.10 CodeView public
//   ?Set_HighPass@CFloatFilterParams@@QAEXHMI@Z. Two sources: (1) this body is byte-identical (293 bytes, relocations masked) to the
//   CODAApi.lib FloatFilter.obj member of that name, its constants resolving to the same retail data (10.0 0x0129A7B0,
//   -0.000501716684f 0x012BF06C, 0.495f 0x012BF064, 2 pi 0x0128F608 (`_gc_f2PI`), 2^32 0x0122DCB4); (2) the byte-identical SDK
//   CFloatReverbParameterTranslator::Translate 0x00C783F0 calls it at REL32 +0x73 (retail 0x00C78462) on
//   CFloatReverbParameters::m_inputFilterHighPass. Member names: the CodeView layout on engine/CFloatFilterParams.h.
// Callers: CFloatReverbParameterTranslator::Translate 0x00C783F0 (its inlined UpdateInputHpFilters). Callee: _CIpow 0x00BFEB40
//   (pow(10.0, ...)); cos and sqrt are the inline x87 intrinsics (fcos / fsqrt).
// Shape: `frequency = pow(...); frequency *= reference;` schedules the rate's `mov eax ; test` before `fmul [reference]`
//   (the one-expression product puts the fmul first); the 2 pi product in a named local keeps `fmul 2pi ; fdiv rate` (inline,
//   VC7.1 reorders `f * 2pi / rate` into `fdiv ; fmul`). The sample rate converts unsigned (fild + 2^32 fix-up) and is homed in the dead `reference` slot, the limit in
//   the dead `attenuation` slot; the frequency stays on the x87 stack through the clamp. The constants stay literals (the landed
//   form; the SDK's `_gc_f2PI` global resolves to the same retail word).
extern "C" double __cdecl pow(double x, double y);
extern "C" double __cdecl cos(double x);
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(pow, cos, sqrt)

#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTERPARAMS_SET_HIGHPASS
#include "engine/CFloatFilterParams.h"

void CFloatFilterParams::Set_HighPass(int attenuation, float reference, unsigned int sampleRate)
{
    if (attenuation == 0) {
        m_fA[0] = 1.0f;
        m_fA[1] = 0.0f;
        m_fA[2] = 0.0f;
        m_fB[0] = 1.0f;
        m_fB[1] = 0.0f;
        m_fB[2] = 0.0f;
        return;
    }
    float frequency = (float)pow(10.0, (attenuation + 300) * -0.000501716684f);
    frequency *= reference;
    float rate = (float)sampleRate;
    float limit = rate * 0.495f;
    if (frequency > limit)
        frequency = limit;
    float omega = frequency * 6.28318548f;
    float c = (float)cos(omega / rate);
    float a = (1.0f - 0.5f * c) / (c - 0.5f);
    float s = (float)sqrt(a * a - 1.0f);
    float t = a - s;
    if (t > 1.0f || t < -1.0f)
        t = a + s;
    float g = 2.0f / (t + 1.0f);
    if (g > 10.0f)
        g = 10.0f;
    m_fA[0] = 1.0f;
    m_fA[1] = t;
    m_fA[2] = 0.0f;
    m_fB[0] = g;
    m_fB[1] = -g;
    m_fB[2] = 0.0f;
}
