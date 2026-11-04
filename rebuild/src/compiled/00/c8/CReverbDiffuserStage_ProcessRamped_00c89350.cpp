// CReverbDiffuserStage::ProcessRamped(SDiffuserStageParams* params, const SDiffuserStageParams* next, const CRingBuffer* history, float* output, float* mix, unsigned int count, unsigned int rampLength) -- the ramped twin of Process while a stage parameter change is pending: sum the four history taps of the old record fading out (rampLength/512 down by 1/512 a sample) and of the new record fading in, copy it to `output` and scale it with gains ramping from old to new, add the old and new delays of the stage's own line the same way, mix it into `mix`, cross-fade the own line's write gain (split at the ring's end), advance the own line and adopt the new record once the ramp ends in this block
// Leaf: ?ProcessRamped@CReverbDiffuserStage@@QAEXPAUSDiffuserStageParams@@PBU2@PBUCRingBuffer@@PAM3II@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C89350, 850 bytes (padded to 880), thiscall `ret 1Ch`, ebp frame with `and esp,-16` (the
//   aligned float[128] scratch block). Only caller: the landed CReverbDiffuser::ProcessRamped 0x00C75B9C (dead in TLC).
// Names: DESCRIPTIVE (libpk reverb, no PDB/RTTI): class and method are the landed caller's binding; the parameter types are
//   respelled from the bytes as the landed twin Process 0x00C891A0 was (NAME_FIXES.tsv): the third argument is the history ring
//   ({data, size, pos}, passed to ReadRingScaledRamp / MixRampFromRing), the fifth is written (MixRamp's destination), and
//   `params` is written (the closing 0x3C-byte `rep movsd` from `next`), so it is non-const.
// Callees: CDspOps::ReadRingScaledRamp 0x00C76560, CDspOps::MixRampFromRing 0x00C767E0 (x9) through CDspObject::s_pOps
//   0x0143CCDC; ops virtual slots 1 ScaleRampCopy (x2) and 6 MixRamp; CDspGain::Crossfade 0x00C74D30 (the definer's spelling,
//   mode 1). Constants: 1/512 0x012A3AF4, 1.0f 0x0122DED8, 2^32 0x0122DCB4 (unsigned fix-up); the +-1/512 steps are pushed as
//   immediates (0x3B000000 / 0xBB000000).
// Shape: the landed Process model; the ramp fraction converts through its own float (`float ramp = (float)rampLength;` then
//   `* (1/512)`: VC7.1 then schedules the fmul after the first tap compare as retail; folded into one expression the fmul
//   comes first); the other weight is a stored `1 - blend`; the tap-gain step is a named local (keeps that gain's
//   `other * next` operand first, the direct and mix gains load `blend * params` first); the tap loop reads both records
//   through one pointer plus their distance (VC7.1 strength reduction).
struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 (floats)
    unsigned int m_pos;                    // +0x08 (write position)
    void Advance(unsigned int count)
    {
        m_pos += count;
        if (m_pos >= m_size)
            m_pos -= m_size;
    }
};

struct CDspOps {                           // x87 vtable 0x012BF02C (SIMD build: ctor 0x00C88A30)
    virtual long GetKind();                                                                         // 0x00C73FB0
    virtual void ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned int count);   // 0x00C74130
    virtual void ScaleRamp(float* buffer, float gain, float step, unsigned int count);             // 0x00C740B0
    virtual void ScaleCopy(float* dst, const float* src, float gain, unsigned int count);          // 0x00C74030
    virtual void Scale(float* buffer, float gain, unsigned int count);                             // 0x00C73FC0
    virtual void Add(float* dst, const float* src, unsigned int count);                            // 0x00C741C0
    virtual void MixRamp(float* dst, const float* src, float gain, float step, unsigned int count); // 0x00C742C0
    virtual void Mix(float* dst, const float* src, float gain, unsigned int count);                // 0x00C74230
    void ReadRingScaledRamp(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step, unsigned int count);   // 0x00C76560
    void MixRampFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step, unsigned int count);      // 0x00C767E0
};

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
    static CDspOps* s_pOps;                // 0x0143CCDC
};

struct SGainParams {
    float m_master;                        // +0x00
    float m_current[3];                    // +0x04
    float m_target[3];                     // +0x10
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
    void Crossfade(SGainParams* oldParams, const SGainParams* newParams, const float* source, float* block, int mode,
                   unsigned int count, unsigned int remaining);   // 0x00C74D30
};

struct SDiffuserStageParams {              // 0x3C
    unsigned int m_taps[4];                // +0x00
    float m_tapGain;                       // +0x10
    float m_directGain;                    // +0x14
    SGainParams m_gain;                    // +0x18
    unsigned int m_delay;                  // +0x34
    float m_mixGain;                       // +0x38
};

struct CReverbDiffuserStage : CDspObject { // vtable 0x012BF07C
    CRingBuffer m_ring;                    // +0x04
    CDspGain m_gain;                       // +0x10
    unsigned int m_index;                  // +0x24
    void ProcessRamped(SDiffuserStageParams* params, const SDiffuserStageParams* next, const CRingBuffer* history,
                       float* output, float* mix, unsigned int count, unsigned int rampLength);
};

void CReverbDiffuserStage::ProcessRamped(SDiffuserStageParams* params, const SDiffuserStageParams* next,
                                         const CRingBuffer* history, float* output, float* mix, unsigned int count,
                                         unsigned int rampLength)
{
    __declspec(align(16)) float block[128];
    unsigned int start = history->m_pos >= count ? history->m_pos - count : history->m_size - count + history->m_pos;
    float* data = m_ring.m_data;
    unsigned int pos = m_ring.m_pos;
    float ramp = (float)rampLength;
    float blend = ramp * (1.0f / 512.0f);
    float other = 1.0f - blend;
    unsigned int tap = params->m_taps[0];
    s_pOps->ReadRingScaledRamp(block, history, start >= tap ? start - tap : history->m_size - tap + start, blend,
                               -1.0f / 512.0f, count);
    tap = next->m_taps[0];
    s_pOps->MixRampFromRing(block, history, start >= tap ? start - tap : history->m_size - tap + start, other,
                            1.0f / 512.0f, count);
    for (unsigned int i = 1; i < 4; ++i) {
        tap = params->m_taps[i];
        s_pOps->MixRampFromRing(block, history, start >= tap ? start - tap : history->m_size - tap + start, blend,
                                -1.0f / 512.0f, count);
        tap = next->m_taps[i];
        s_pOps->MixRampFromRing(block, history, start >= tap ? start - tap : history->m_size - tap + start, other,
                                1.0f / 512.0f, count);
    }
    s_pOps->ScaleRampCopy(output, block, blend * params->m_directGain + other * next->m_directGain,
                          (next->m_directGain - params->m_directGain) * (1.0f / 512.0f), count);
    float tapStep = (next->m_tapGain - params->m_tapGain) * (1.0f / 512.0f);
    s_pOps->ScaleRampCopy(block, block, blend * params->m_tapGain + other * next->m_tapGain, tapStep, count);
    unsigned int delay = params->m_delay;
    s_pOps->MixRampFromRing(block, &m_ring, pos >= delay ? pos - delay : m_ring.m_size - delay + pos, blend,
                            -1.0f / 512.0f, count);
    delay = next->m_delay;
    s_pOps->MixRampFromRing(block, &m_ring, pos >= delay ? pos - delay : m_ring.m_size - delay + pos, other,
                            1.0f / 512.0f, count);
    s_pOps->MixRamp(mix, block, blend * params->m_mixGain + other * next->m_mixGain,
                    (next->m_mixGain - params->m_mixGain) * (1.0f / 512.0f), count);
    unsigned int first = count;
    if (pos + count > m_ring.m_size)
        first = m_ring.m_size - pos;
    m_gain.Crossfade(&params->m_gain, &next->m_gain, block, data + pos, 1, first, rampLength);
    if (first < count)
        m_gain.Crossfade(&params->m_gain, &next->m_gain, block + first, data, 1, count - first, rampLength);
    m_ring.Advance(count);
    if (count >= rampLength)
        *params = *next;
}