// CFloatReverbParameterTranslator::PanToGains(const _RVB_VECTOR* pPan, float* pfGains, unsigned int uiNumGains, unsigned int uiNumOutputs) -- GameCODA panner: from a pan vector's x and z fill a gain block (stereo: a linear left/right pan of x; otherwise four quad speaker gains at (+-0.707, +-0.707) weighted by inverse squared distance, a speaker exactly hit taking everything), repeated over `count` gains in strides of min(outputs, count)
// Leaf: ?PanToGains@CFloatReverbParameterTranslator@@CAXPBU_RVB_VECTOR@@PAMII@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C77D90, 557 bytes, `__cdecl` (private static member), plain `ret`.
// Name (NF99P2, q11510; was the descriptive free MixChannels_00c77d90): GameCODA 2.10 CodeView private static
//   ?PanToGains@CFloatReverbParameterTranslator@@CAXPBU_RVB_VECTOR@@PAMII@Z. Two sources: (1) this body is byte-identical (557 bytes,
//   relocations masked) to the CODAApi.lib FloatReverbParameterTranslator.obj member of that name; (2) the byte-identical SDK
//   Translate 0x00C783F0 calls it at REL32 +0x140 (retail 0x00C7852F, its inlined UpdateReflectionsPan on CHLRvbParams::m_ReflectionsPan)
//   and the byte-identical SDK UpdateReverbPan 0x00C783B0 calls it twice on m_ReverbPan (its two REL32s). Its landed binders (those two)
//   move with it in this hand-back. Header: engine/CFloatReverbParameterTranslator.h opt-in FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_PANTOGAINS.
// Callees: none. Constants 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 0.5f 0x0122F59C, 0.707f 0x0125D044.
// Shape: `if (count < channels) channels = count;` then the stereo branch on `channels == 2`; the four squared distances are
//   named locals built from the four signed offsets (x +- 0.707, z +- 0.707), the gains are four locals zeroed up front, the
//   exact-hit chain `if (d0 == 0.0f) g0 = 1.0f; else if ...` ends in the inverse-distance normalisation; both fill loops are
//   `for (i = 0; i < count; i += stride)` (the unsigned compares `jae`/`jb`/`jbe` of retail).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_PANTOGAINS
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/_RVB_VECTOR.h"

void CFloatReverbParameterTranslator::PanToGains(const _RVB_VECTOR* position, float* gains, unsigned int count, unsigned int channels)
{
    if (count < channels)
        channels = count;
    float x = position->x;
    float z = position->z;
    if (channels == 2) {
        float left = (1.0f - x) * 0.5f;
        float right = (x + 1.0f) * 0.5f;
        for (unsigned int i = 0; i < count; i += 2) {
            gains[i] = left;
            gains[i + 1] = right;
        }
        return;
    }
    float xm = 0.707f - x;
    float xp = x + 0.707f;
    float zm = 0.707f - z;
    float zp = z + 0.707f;
    float d0 = zm * zm + xp * xp;
    float d1 = xm * xm + zm * zm;
    float d2 = zp * zp + xp * xp;
    float d3 = zp * zp + xm * xm;
    float g0 = 0.0f;
    float g1 = 0.0f;
    float g2 = 0.0f;
    float g3 = 0.0f;
    if (d0 == 0.0f)
        g0 = 1.0f;
    else if (d1 == 0.0f)
        g1 = 1.0f;
    else if (d2 == 0.0f)
        g2 = 1.0f;
    else if (d3 == 0.0f)
        g3 = 1.0f;
    else {
        float w0 = 1.0f / d0;
        float w1 = 1.0f / d1;
        float w2 = 1.0f / d2;
        float w3 = 1.0f / d3;
        float norm = 1.0f / (w3 + w2 + w1 + w0);
        g0 = norm * w0;
        g1 = norm * w1;
        g2 = norm * w2;
        g3 = norm * w3;
    }
    for (unsigned int i = 0; i < count; i += channels) {
        gains[i] = g0;
        gains[i + 1] = g1;
        gains[i + 2] = g2;
        gains[i + 3] = g3;
    }
}