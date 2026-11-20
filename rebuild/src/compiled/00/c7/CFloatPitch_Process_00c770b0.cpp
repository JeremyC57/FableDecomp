// CFloatPitch::Process(CFloatPitchShiftParams* pParams, const CFloatPitchShiftParams* pNewParams, const float* pfInput, float* pfOutput, unsigned int uiSamples, unsigned int uiSamplesToFade) -- the fading twin of CFloatPitch::Process while a parameter change is pending: write the input block into the cyclic buffer, then for every sample read the buffer at the LFO-swung delay of both parameter sets, interpolating each linearly, and mix the two reads with the fade fraction (uiSamplesToFade / 512, down by 1/512 a sample until it reaches 0; the LFO rate is the faded period); adopt the new parameters once the fade ends in this block
// Leaf: ?Process@CFloatPitch@@QAEXPAUCFloatPitchShiftParams@@PBU2@PBMPAMII@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C770B0, 857 bytes (`ret 18h` at 0x00C77406, then int3 padding). The oracle row is
//   910 bytes: it runs past the int3 pad into two unmanifested 0x13 / 0x0E-byte functions 0x00C77410 / 0x00C77430
//   (graded against the true extent, CUT 857).
// Name (NF109, q11606; was the descriptive CReverbPreDelay::Blend over a local model): GameCODA 2.10 CodeView public
//   ?Process@CFloatPitch@@QAEXPAUCFloatPitchShiftParams@@PBU2@PBMPAMII@Z (the fading overload). Two sources: (1) this body is
//   byte-identical (857 bytes, relocations masked) to that CODAApi.lib FloatPitch.obj release member (work/tools/sdkcmpx.py);
//   (2) the byte-identical SDK CFloatReverbDspEngine::ProcessShortReverbInputBuffer (retail 0x00C6A080) relocates it at +0x4d, the
//   retail call site. Layout: the CodeView CFloatPitch (m_Buffer +4, m_fControlPoint +0x10) and CFloatPitchShiftParams
//   (m_fModulationPeriodSamples +0, m_fModulationAmplitudeSamples +4).
// Callees in the CODAApiDbg.lib body: CFloatCyclicBuffer::GetWriteCursor / GetLength / SetWriteCursor (expanded here), FloatCopySamples
//   (memcpy here as in the landed row) and the header inline ds_ftoi (the landed file-local RoundHalfDownToLong below).
// Retail-asm: 0x00C77219 `fld dword ptr [esp+28h]` + 0x00C7721D `fistp dword ptr [esp+2Ch]` (six such pairs): libpk's bare
//   dword fld / fistp float-to-long through the shared libpk/FtoL.h, in the round-half-down helper of Process 0x00C76E90.
// Constants: 1/512 0x012A3AF4, 1.0f 0x0122DED8, 0.25f 0x0123078C, 8.0f 0x0125916C, 0.0f 0x0122DEDC, 0.5f 0x0122F59C,
//   0.75f 0x01238174, 2^32 0x0122DCB4.
// Shape: Process's model and round-half-down helper; each read's wrapped delay is the conditional expression
//   `position >= offset ? position - offset : position - offset + size` (the difference hoisted, kept on the FPU stack,
//   `test ah,1 ; je`): the `if (position < offset) delay += size;` statement homes the delay in memory (+8 bytes) and the
//   `<` conditional expression tests `ah,5 ; jp`. The blend fraction lives in the dead `input` slot.
#include <string.h>
#include "libpk/FtoL.h"

inline long RoundHalfDownToLong(float value)
{
    long result = FtoL(value - 0.5f);
    return result < 0 ? result + 1 : result;
}

#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATPITCH_PROCESS
#include "engine/CFloatPitchShiftParams.h"
#include "engine/CFloatPitch.h"

void CFloatPitch::Process(CFloatPitchShiftParams* pParams, const CFloatPitchShiftParams* pNewParams, const float* pfInput,
                            float* pfOutput, unsigned int uiSamples, unsigned int uiSamplesToFade)
{
    float* data = m_Buffer.m_pBase;
    unsigned int pos = m_Buffer.GetWriteCursor();
    unsigned int first = uiSamples;
    if (pos + uiSamples > m_Buffer.GetLength())
        first = m_Buffer.GetLength() - pos;
    memcpy(data + pos, pfInput, first * sizeof(float));
    if (first < uiSamples)
        memcpy(data, pfInput + first, (uiSamples - first) * sizeof(float));
    float blend = (float)uiSamplesToFade * (1.0f / 512.0f);
    float inverse = 1.0f - blend;
    float step = 1.0f / (blend * pParams->m_fModulationPeriodSamples + inverse * pNewParams->m_fModulationPeriodSamples);
    for (unsigned int i = 0; i < uiSamples; ++i) {
        float lfo;
        if (m_fControlPoint < 0.25f)
            lfo = m_fControlPoint * m_fControlPoint * 8.0f;
        else
            lfo = (8.0f - m_fControlPoint * 8.0f) * m_fControlPoint - 1.0f;
        float offsetCurrent = lfo * pParams->m_fModulationAmplitudeSamples;
        float offsetTarget = lfo * pNewParams->m_fModulationAmplitudeSamples;
        float position = (float)pos;
        float delay = position >= offsetCurrent ? position - offsetCurrent : position - offsetCurrent + (float)m_Buffer.GetLength();
        long previous = RoundHalfDownToLong(delay) - 1;
        long next = RoundHalfDownToLong(delay);
        if (previous == -1)
            previous = m_Buffer.GetLength() - 1;
        float fraction = delay - (float)RoundHalfDownToLong(delay);
        float sampleCurrent = (1.0f - fraction) * data[previous] + fraction * data[next];
        delay = position >= offsetTarget ? position - offsetTarget : position - offsetTarget + (float)m_Buffer.GetLength();
        previous = RoundHalfDownToLong(delay) - 1;
        next = RoundHalfDownToLong(delay);
        if (previous == -1)
            previous = m_Buffer.GetLength() - 1;
        fraction = delay - (float)RoundHalfDownToLong(delay);
        float sampleTarget = (1.0f - fraction) * data[previous] + fraction * data[next];
        pfOutput[i] = sampleCurrent * blend + sampleTarget * inverse;
        m_fControlPoint += step;
        if (m_fControlPoint >= 0.75f)
            m_fControlPoint -= 1.0f;
        if (pos + 1 < m_Buffer.GetLength())
            pos = pos + 1;
        else
            pos += 1 - m_Buffer.GetLength();
        if (blend > 0.0f) {
            blend -= 1.0f / 512.0f;
            inverse = 1.0f - blend;
            step = 1.0f / (blend * pParams->m_fModulationPeriodSamples + inverse * pNewParams->m_fModulationPeriodSamples);
        }
    }
    m_Buffer.SetWriteCursor(pos);
    if (uiSamples >= uiSamplesToFade)
        *pParams = *pNewParams;
}