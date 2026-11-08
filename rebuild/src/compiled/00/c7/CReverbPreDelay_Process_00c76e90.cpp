// CReverbPreDelay::Process(const SReverbPreDelayParams* params, const float* input, float* output, unsigned int count) -- one block of the reverb's modulated pre-delay: write the input block into the ring (wrapping once), then for every sample read the ring behind the write position by a delay that a parabolic LFO swings by up to the delay length, interpolating linearly between the two neighbouring samples, and advance the LFO phase (one cycle per `modulation rate` samples, wrapped into [-0.25, 0.75)) and the write position
// Leaf: ?Process@CReverbPreDelay@@QAEXPBUSReverbPreDelayParams@@PBMPAMI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C76E90, 544 bytes, thiscall `ret 10h`.
// Identity (descriptive; libpk reverb module, no PDB / RTTI): the reverb block process 0x00C6A080 calls it (0x00C6A0E6) on its
//   +0x1E0 member -- the CReverbPreDelay of the landed CReverbProcessor::Allocate 0x00C6A460 / ctor 0x00C697A0 (vtable
//   0x012B9580) -- with the processor's parameter pair at +0x220 (m_modulationRate, m_delayLength of the landed
//   UpdateDelayLength 0x00C78040), and calls the ramping twin 0x00C770B0 instead while a change is pending, as it does for the
//   landed CReverbDelay::Process / Blend 0x00C76B60 / 0x00C76C20. The landed caller's `StageOne::Process` spelling is a
//   placeholder (NAME_FIXES.tsv); the catalog's "FFT_ComputeRealTransform" is propagated.
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
    void Process(const SReverbPreDelayParams* params, const float* input, float* output, unsigned int count);
};

void CReverbPreDelay::Process(const SReverbPreDelayParams* params, const float* input, float* output, unsigned int count)
{
    float* data = m_ring.m_data;
    unsigned int pos = m_ring.m_pos;
    unsigned int first = count;
    if (pos + count > m_ring.m_size)
        first = m_ring.m_size - pos;
    memcpy(data + pos, input, first * sizeof(float));
    if (first < count)
        memcpy(data, input + first, (count - first) * sizeof(float));
    float step = params->m_modulationRate > 1.0f ? 1.0f / params->m_modulationRate : 1.0f;
    for (unsigned int i = 0; i < count; ++i) {
        float lfo;
        if (m_phase < 0.25f)
            lfo = m_phase * m_phase * 8.0f;
        else
            lfo = (8.0f - m_phase * 8.0f) * m_phase - 1.0f;
        float delay = (float)pos - lfo * params->m_delayLength;
        if (delay < 0.0f)
            delay += (float)m_ring.m_size;
        long previous = RoundHalfDownToLong(delay) - 1;
        long next = RoundHalfDownToLong(delay);
        if (previous == -1)
            previous = m_ring.m_size - 1;
        float fraction = delay - (float)RoundHalfDownToLong(delay);
        output[i] = (1.0f - fraction) * data[previous] + fraction * data[next];
        m_phase += step;
        if (m_phase >= 0.75f)
            m_phase -= 1.0f;
        if (pos + 1 < m_ring.m_size)
            pos = pos + 1;
        else
            pos += 1 - m_ring.m_size;
    }
    m_ring.m_pos = pos;
}