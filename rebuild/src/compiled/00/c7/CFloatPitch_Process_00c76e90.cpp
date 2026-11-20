// CFloatPitch::Process(const CFloatPitchShiftParams* pParams, const float* pfInput, float* pfOutput, unsigned int uiSamples) -- one block of the reverb's modulated pre-delay (pitch shifter): write the input block into the cyclic buffer (wrapping once), then for every sample read the buffer behind the write cursor by a delay that a parabolic LFO swings by up to the modulation amplitude, interpolating linearly between the two neighbouring samples, and advance the LFO control point (one cycle per modulation period, wrapped into [-0.25, 0.75)) and the write cursor
// Leaf: ?Process@CFloatPitch@@QAEXPBUCFloatPitchShiftParams@@PBMPAMI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C76E90, 544 bytes, thiscall `ret 10h`.
// Name (NF109, q11605; was the descriptive CReverbPreDelay::Process over a local model): GameCODA 2.10 CodeView public
//   ?Process@CFloatPitch@@QAEXPBUCFloatPitchShiftParams@@PBMPAMI@Z. Two sources: (1) this body is byte-identical (544 bytes, relocations
//   masked) to that CODAApi.lib FloatPitch.obj release member (work/tools/sdkcmpx.py; its constants resolve to the retail words below);
//   (2) the byte-identical SDK CFloatReverbDspEngine::ProcessShortReverbInputBuffer (retail 0x00C6A080) relocates it at +0x67, the
//   retail call site. Layout: the CodeView CFloatPitch (m_Buffer +4, m_fControlPoint +0x10, m_iMyFaderToken +0x14) and
//   CFloatPitchShiftParams (m_fModulationPeriodSamples +0, m_fModulationAmplitudeSamples +4).
// Callees in the CODAApiDbg.lib body: CFloatCyclicBuffer::GetWriteCursor / GetLength / SetWriteCursor (expanded here), FloatCopySamples
//   (the two block copies, memcpy here as in the landed row) and the header inline ds_ftoi (the release expands it; the landed
//   file-local RoundHalfDownToLong below, the spelling of the landed 0x00C6A460).
// Retail-asm: 0x00C76FD8 `fld dword ptr [esp+18h]` + 0x00C76FDC `fistp dword ptr [esp+1Ch]` (and 0x00C76FF2 / 0x00C76FF6,
//   0x00C77014 / 0x00C77018): libpk's bare dword fld / fistp float-to-long, reached through the shared libpk/FtoL.h (the
//   round-half-down idiom of the landed 0x00C3FF70: FtoL(x - 0.5f), negative results biased up by one). VC7.1 `(long)x` calls
//   __ftol2 and /QIfist stores a qword: no C++ spelling emits the dword fistp (family FISTP, asm/w3 ATTEMPTS.tsv).
// Constants: 1.0f 0x0122DED8, 0.25f 0x0123078C, 8.0f 0x0125916C, 0.0f 0x0122DEDC, 0.5f 0x0122F59C, 0.75f 0x01238174,
//   2^32 0x0122DCB4 (the unsigned-to-float fix-ups).
// Shape: the round-half-down helper is a plain `inline` (the spelling of the landed 0x00C77FC0's local helper) returning
//   `result < 0 ? result + 1 : result`: VC7.1 then keeps the third result in a register and stores it once for the `fild`
//   (the `if (result < 0) ++result;` body homes it in memory and stores it in both arms, +4 bytes); `delay - 0.5f` is
//   computed once and copied into each FtoL argument; the three calls are separate (the asm is not CSE'd).
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

void CFloatPitch::Process(const CFloatPitchShiftParams* pParams, const float* pfInput, float* pfOutput, unsigned int uiSamples)
{
    float* data = m_Buffer.m_pBase;
    unsigned int pos = m_Buffer.GetWriteCursor();
    unsigned int first = uiSamples;
    if (pos + uiSamples > m_Buffer.GetLength())
        first = m_Buffer.GetLength() - pos;
    memcpy(data + pos, pfInput, first * sizeof(float));
    if (first < uiSamples)
        memcpy(data, pfInput + first, (uiSamples - first) * sizeof(float));
    float step = pParams->m_fModulationPeriodSamples > 1.0f ? 1.0f / pParams->m_fModulationPeriodSamples : 1.0f;
    for (unsigned int i = 0; i < uiSamples; ++i) {
        float lfo;
        if (m_fControlPoint < 0.25f)
            lfo = m_fControlPoint * m_fControlPoint * 8.0f;
        else
            lfo = (8.0f - m_fControlPoint * 8.0f) * m_fControlPoint - 1.0f;
        float delay = (float)pos - lfo * pParams->m_fModulationAmplitudeSamples;
        if (delay < 0.0f)
            delay += (float)m_Buffer.GetLength();
        long previous = RoundHalfDownToLong(delay) - 1;
        long next = RoundHalfDownToLong(delay);
        if (previous == -1)
            previous = m_Buffer.GetLength() - 1;
        float fraction = delay - (float)RoundHalfDownToLong(delay);
        pfOutput[i] = (1.0f - fraction) * data[previous] + fraction * data[next];
        m_fControlPoint += step;
        if (m_fControlPoint >= 0.75f)
            m_fControlPoint -= 1.0f;
        if (pos + 1 < m_Buffer.GetLength())
            pos = pos + 1;
        else
            pos += 1 - m_Buffer.GetLength();
    }
    m_Buffer.SetWriteCursor(pos);
}