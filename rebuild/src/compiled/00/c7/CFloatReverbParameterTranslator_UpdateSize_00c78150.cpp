// CFloatReverbParameterTranslator::UpdateSize(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams) -- map the clamped environment size (0.9..2.5) to 0..1, set both long-reverb modules' feedback delay, then lay out each module's four input delays from the default long-reverb input delay table, one long-reverb input segment per delay, recording the first three column maxima as the input filter delays
// Leaf: ?UpdateSize@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C78150, 561 bytes, thiscall `ret 8`. Constants 0.9f (0x012BF1B4), 0.8f (0x01254DA8),
//   1/1.7 = 0.5882353f (0x012BF1B0), 2.5f (0x01241D68), 0.5f (0x0122F59C), 1.0f (0x0122DED8), 0.0f (0x0122DEDC), 2^32 (0x0122DCB4,
//   the unsigned fix-up); the early-reflection time table sc_fDefaultLongReverbInputDelay[2][4] at 0x012BF138 (rows at 0x012BF138 / 0x012BF148).
// Retail-asm: 0x00C781CB `fld dword ptr [esp+28h]` + 0x00C781CF `fistp dword ptr [esp+8]`, 0x00C7827D `fld dword ptr [esp+20h]` +
//   0x00C78281 `fistp dword ptr [esp+24h]`, 0x00C782E1 `fld dword ptr [esp+20h]` + 0x00C782E5 `fistp dword ptr [esp+2Ch]` -- libpk's
//   round-half-down (landed RoundHalfDownToLong 0x00C3FF70) expanded in place: `value - 0.5f` homed and reloaded, a bare dword
//   fistp, then the C++ `+1 if negative`. VC7.1 C++ `(long)x` / `(int)x` / `(short)x` / temporaries / `(double)` / a same-TU C++
//   callee call __ftol2, /QIfist emits `fistp qword`, `_mm_cvtss_si32` / `_mm_cvttss_si32` emit cvtss2si / cvttss2si (NOTES.md
//   "C++ attempts before helper"), so the dword fistp is the original's inline asm, taken from the shared libpk/FtoL.h helper.
// Helper: libpk/FtoL.h (two-path: the byte-matching asm, and the FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Shape: the clamp is `if (room < 0.9f) room = 0.9f;` then `room < 2.5f ? (room - 0.8f) * k : 1.0f` (retail keeps the
//   clamped value on the x87 stack); the column offset `i * segment` is a local of the inner loop body (VC7.1 hoists the
//   multiply but converts it per tap); the first-column factor is the inline `?:` (kept on the x87 stack and popped).
// Name (NF99P2, q11499; was the descriptive CReverbProcessor::UpdateTapDelays over local models): GameCODA 2.10 CodeView private
//   ?UpdateSize@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z. Two sources: (1) this body is
//   byte-identical (561 bytes, relocations masked) to the CODAApi.lib FloatReverbParameterTranslator.obj member of that name, whose
//   five data relocations name the table at 0x012BF138 / 0x012BF148 `_sc_fDefaultLongReverbInputDelay` (was the local static
//   CReverbProcessor::s_tapTimes; this row is its only landed binder); (2) the byte-identical SDK Translate 0x00C783F0 calls it at
//   REL32 +0x1e (retail 0x00C7840D) for update flag 1. Its only landed binder (0x00C783F0) binds this spelling in this hand-back.
//   Members: m_pReverbConstants (CFloatReverbConstants: m_uiSamplesPerSecond +8, m_uiMaxShortReverbInputSamples +0x10,
//   m_uiMaxLongReverbInputSamples +0x14), CHLRvbParams::m_fSize +0x64, CFloatReverbParameters::m_LongReverb (+0x148:
//   m_uiInputFilterDelay[3], m_aLongRvb[2] at +0x60 with m_auiInputDelay[4] and m_uiFeedbackDelay +0x34).
#include "fable_compiler.h"
#include "libpk/FtoL.h"
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
#define FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE
#include "engine/CFloatReverbParameterTranslator.h"
#include "engine/CFloatReverbParameters.h"
#include "engine/CFloatReverbConstants.h"
#include "engine/CHLRvbParams.h"
extern "C" const float sc_fDefaultLongReverbInputDelay[2][4];   // 0x012BF138 (SDK _sc_fDefaultLongReverbInputDelay)

FABLE_FORCEINLINE long RoundHalfDownToLong(float value)
{
    long result = FtoL(value - 0.5f);
    if (result < 0)
        ++result;
    return result;
}

void CFloatReverbParameterTranslator::UpdateSize(CFloatReverbParameters* pReverbParams, CHLRvbParams* pSrcParams)
{
    float room = pSrcParams->m_fSize;
    float scale;
    if (room < 0.9f)
        room = 0.9f;
    if (room < 2.5f)
        scale = (room - 0.8f) * 0.5882353f;
    else
        scale = 1.0f;
    long base = (RoundHalfDownToLong((float)m_pReverbConstants->m_uiSamplesPerSecond * scale) + 3) & ~3;
    unsigned long segment = m_pReverbConstants->m_uiMaxLongReverbInputSamples >> 2;
    unsigned long lineBase = m_pReverbConstants->m_uiMaxShortReverbInputSamples;
    unsigned long extent = 0;
    pReverbParams->m_LongReverb.m_aLongRvb[0].m_uiFeedbackDelay = base;
    pReverbParams->m_LongReverb.m_aLongRvb[1].m_uiFeedbackDelay = base;
    float previous = 0.0f;
    for (unsigned int i = 0; i < 4; ++i) {
        for (unsigned int j = 0; j < 2; ++j) {
            unsigned long offset = i * segment;
            unsigned long delay = RoundHalfDownToLong((sc_fDefaultLongReverbInputDelay[j][i] - previous) * (float)m_pReverbConstants->m_uiSamplesPerSecond * scale);
            if (delay > segment - 128)
                delay = segment - 128;
            unsigned long tap = (RoundHalfDownToLong((float)lineBase * (i == 0 ? scale : 1.0f) + (float)delay + (float)offset) + 3) & ~3;
            pReverbParams->m_LongReverb.m_aLongRvb[j].m_auiInputDelay[i] = tap;
            if (i < 3 && extent < tap) {
                extent = tap;
                pReverbParams->m_LongReverb.m_uiInputFilterDelay[i] = tap;
            }
        }
        previous = 0.0f;
        if (previous < sc_fDefaultLongReverbInputDelay[0][i])
            previous = sc_fDefaultLongReverbInputDelay[0][i];
        if (previous < sc_fDefaultLongReverbInputDelay[1][i])
            previous = sc_fDefaultLongReverbInputDelay[1][i];
    }
}
