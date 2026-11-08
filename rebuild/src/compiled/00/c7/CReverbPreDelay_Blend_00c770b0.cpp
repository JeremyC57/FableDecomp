// CReverbPreDelay::Blend(SReverbPreDelayParams* current, const SReverbPreDelayParams* target, const float* input, float* output, unsigned int count, unsigned int blendPosition) -- the ramping twin of CReverbPreDelay::Process while a parameter change is pending: write the input block into the ring, then for every sample read the ring at the LFO-swung delay of both parameter sets, interpolating each linearly, and mix the two reads with the blend fraction (blendPosition / 512, down by 1/512 a sample until it reaches 0; the LFO rate is the blended period); adopt the target parameters once the blend completes in this block
// Leaf: ?Blend@CReverbPreDelay@@QAEXPAUSReverbPreDelayParams@@PBU2@PBMPAMII@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C770B0, 857 bytes (`ret 18h` at 0x00C77406, then int3 padding). The oracle row is
//   910 bytes: it runs past the int3 pad into two unmanifested 0x13 / 0x0E-byte functions 0x00C77410 / 0x00C77430
//   (graded against the true extent, CUT 857).
// Identity (descriptive; libpk reverb module, no PDB / RTTI): the reverb block process 0x00C6A080 calls it (0x00C6A0CC) on its
//   +0x1E0 CReverbPreDelay (landed Allocate 0x00C6A460 / ctor 0x00C697A0) with (current, target, input, output, count,
//   BlendPosition) while the stage's mode matches, and Process 0x00C76E90 otherwise -- the pattern of the landed
//   CReverbDelay::Blend 0x00C76C20 (same 1/512 blend step, same final `*current = *target`). The landed caller's
//   `StageOne::Blend` spelling is a placeholder (NAME_FIXES.tsv); the catalog's "FFT_ComputeComplexTransform" is propagated.
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

struct SReverbPreDelayParams {                          // the processor's pair at +0x220
    float m_modulationRate;                             // +0x00 LFO period in samples
    float m_delayLength;                                // +0x04 LFO depth in samples
};
struct CRingBuffer {
    float*       m_data;                                // +0x00
    unsigned int m_size;                                // +0x04 (floats)
    unsigned int m_pos;                                 // +0x08 (write position)
};
class CVtable_012bf050 {                                // vtable 0x012BF050: the reverb DSP parts' base
public:
    CVtable_012bf050();                                 // 0x00C74420
    virtual ~CVtable_012bf050();                        // 0x00C74450
};
class CReverbPreDelay : public CVtable_012bf050 {       // vtable 0x012B9580, 0x18 bytes
public:
    CRingBuffer m_ring;                                 // +0x04
    float       m_phase;                                // +0x10 LFO phase
    long        m_mode;                                 // +0x14
    void Process(const SReverbPreDelayParams* params, const float* input, float* output, unsigned int count);   // 0x00C76E90
    void Blend(SReverbPreDelayParams* current, const SReverbPreDelayParams* target, const float* input, float* output,
               unsigned int count, unsigned int blendPosition);
};

void CReverbPreDelay::Blend(SReverbPreDelayParams* current, const SReverbPreDelayParams* target, const float* input,
                            float* output, unsigned int count, unsigned int blendPosition)
{
    float* data = m_ring.m_data;
    unsigned int pos = m_ring.m_pos;
    unsigned int first = count;
    if (pos + count > m_ring.m_size)
        first = m_ring.m_size - pos;
    memcpy(data + pos, input, first * sizeof(float));
    if (first < count)
        memcpy(data, input + first, (count - first) * sizeof(float));
    float blend = (float)blendPosition * (1.0f / 512.0f);
    float inverse = 1.0f - blend;
    float step = 1.0f / (blend * current->m_modulationRate + inverse * target->m_modulationRate);
    for (unsigned int i = 0; i < count; ++i) {
        float lfo;
        if (m_phase < 0.25f)
            lfo = m_phase * m_phase * 8.0f;
        else
            lfo = (8.0f - m_phase * 8.0f) * m_phase - 1.0f;
        float offsetCurrent = lfo * current->m_delayLength;
        float offsetTarget = lfo * target->m_delayLength;
        float position = (float)pos;
        float delay = position >= offsetCurrent ? position - offsetCurrent : position - offsetCurrent + (float)m_ring.m_size;
        long previous = RoundHalfDownToLong(delay) - 1;
        long next = RoundHalfDownToLong(delay);
        if (previous == -1)
            previous = m_ring.m_size - 1;
        float fraction = delay - (float)RoundHalfDownToLong(delay);
        float sampleCurrent = (1.0f - fraction) * data[previous] + fraction * data[next];
        delay = position >= offsetTarget ? position - offsetTarget : position - offsetTarget + (float)m_ring.m_size;
        previous = RoundHalfDownToLong(delay) - 1;
        next = RoundHalfDownToLong(delay);
        if (previous == -1)
            previous = m_ring.m_size - 1;
        fraction = delay - (float)RoundHalfDownToLong(delay);
        float sampleTarget = (1.0f - fraction) * data[previous] + fraction * data[next];
        output[i] = sampleCurrent * blend + sampleTarget * inverse;
        m_phase += step;
        if (m_phase >= 0.75f)
            m_phase -= 1.0f;
        if (pos + 1 < m_ring.m_size)
            pos = pos + 1;
        else
            pos += 1 - m_ring.m_size;
        if (blend > 0.0f) {
            blend -= 1.0f / 512.0f;
            inverse = 1.0f - blend;
            step = 1.0f / (blend * current->m_modulationRate + inverse * target->m_modulationRate);
        }
    }
    m_ring.m_pos = pos;
    if (count >= blendPosition)
        *current = *target;
}