// CFloatFilterParams::Set_LpButterworth2(int iCutoff, float fReference, unsigned int uiSamplesPerSecond) -- set the six filter coefficients (m_fA[0..2], m_fB[0..2] at +4..+0x18) for a second-order Butterworth low-pass: a zero cutoff gives the pass-through set {1,0,0, 1,0,0}; otherwise the corner reference * 2^((cutoff+300)/1200) (pow(10, (cutoff+300) * log10(2)/1200)), clamped to 0.495 * the sample rate, gives k = tan(pi f / rate) and the bilinear terms {1, 2a, -(a*a + x), y, 2y, y}
// Leaf: ?Set_LpButterworth2@CFloatFilterParams@@QAEXHMI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74AB0, 256 bytes (+ int3 padding), thiscall `ret 0Ch`.
// Name (NF99P2, q11509; was the descriptive CEffectFilter::SetLowPass over a local model): GameCODA 2.10 CodeView public
//   ?Set_LpButterworth2@CFloatFilterParams@@QAEXHMI@Z. Two sources: (1) this body is byte-identical (256 bytes, relocations masked)
//   to the CODAApi.lib FloatFilter.obj member of that name, its constants resolving to the same retail data (10.0 0x0129A7B0,
//   log10(2)/1200 0x012BF068, 0.495f 0x012BF064, pi 0x0128F604 (`_gc_fPI`), the function-local static fSqrt2 0x012BF060, 1.0f,
//   0.25f, 2^32); (2) the byte-identical SDK CFloatReverbParameterTranslator::Translate 0x00C783F0 calls it at REL32 +0x8e (retail
//   0x00C7847D) on CFloatReverbParameters::m_inputFilterLowPass. Member names: the CodeView layout on engine/CFloatFilterParams.h.
// Callers: CFloatReverbParameterTranslator::Translate 0x00C783F0 (its inlined UpdateInputLpFilters). Callee: _CIpow 0x00BFEB40
//   (pow(10.0, ...)); tan is the inline x87 intrinsic (fptan).
// Shape: `frequency = pow(...); frequency *= reference;` as two statements schedules the sample rate's
//   `mov eax,[esp+10h] ; test eax,eax` before `fmul [reference]` (the one-expression product put the fmul first); the pi product
//   in a named local keeps `fmul pi ; fdiv rate`; m_fA[0] = 1 is stored before the tan. The constants stay literals (the landed
//   form; the SDK's `_gc_fPI` and static fSqrt2 resolve to the same retail words).
#include <math.h>
#pragma intrinsic(pow, tan)
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTERPARAMS_SET_LPBUTTERWORTH2
#include "engine/CFloatFilterParams.h"

void CFloatFilterParams::Set_LpButterworth2(int cutoff, float reference, unsigned int sampleRate)
{
    if (cutoff == 0) {
        m_fA[0] = 1.0f;
        m_fA[1] = 0.0f;
        m_fA[2] = 0.0f;
        m_fB[0] = 1.0f;
        m_fB[1] = 0.0f;
        m_fB[2] = 0.0f;
        return;
    }
    float frequency = (float)pow(10.0, (double)((cutoff + 300) * 0.00025085834f));
    frequency *= reference;
    float rate = (float)sampleRate;
    float limit = rate * 0.495f;
    if (frequency > limit)
        frequency = limit;
    float angle = frequency * 3.14159265f;
    m_fA[0] = 1.0f;
    float k = (float)tan(angle / rate);
    float kk = k * k;
    float d = k * 1.41421356f + kk + 1.0f;
    float a = (1.0f - kk) / d;
    float x = (kk + kk) / (d * d);
    float y = ((1.0f - a) * (1.0f - a) + x) * 0.25f;
    m_fA[1] = a + a;
    m_fA[2] = -(a * a + x);
    m_fB[0] = y;
    m_fB[1] = y + y;
    m_fB[2] = y;
}
