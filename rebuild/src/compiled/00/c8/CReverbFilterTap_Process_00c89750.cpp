// CReverbFilterTap::Process(const SReverbTapParams* params, const CRingBuffer* history, float* block, const float* input, unsigned int count) -- x87-path block of one filtered reverb tap: clear the tap's block, run the tap's gain filter over its own ring from the current position into the block (in two pieces when the ring wraps), mix the two history taps (delay and gain from the parameters) out of the shared history ring into the block, then mix the block into the channel buffer at the tap's output gain
// Leaf: ?Process@CReverbFilterTap@@QAEXPBUSReverbTapParams@@PBUCRingBuffer@@PAMPBMI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C89750, 255 bytes, thiscall `ret 14h`. The block clear is the memset intrinsic (`rep stosd`
//   of count dwords); the history start lives in the dead `count` argument slot, &m_gain in a dead argument slot.
// Identity: the landed CReverbTapBank::Process 0x00C75E90 calls it as CReverbFilterTap::Process (5 args) on its x87 path and binds
//   exactly this symbol; the landed CReverbFilterTap::MixHistory 0x00C89B60 is its SIMD-path second half (same history taps and
//   output mix). Names are the landed descriptive ones (libpk: no PDB/RTTI); the catalog's "Graphics_RenderWaveform" is a placeholder.
// Callees: CDspGain::Apply @ 0x00C74CE0 (twice when the ring wraps), CDspOps::MixFromRing @ 0x00C76760 (twice) and virtual slot 7
//   Mix (+0x1C) on the shared ops table CDspObject::s_pOps 0x0143CCDC.
// Layout: the landed MixHistory model: CReverbFilterTap gain +0x04, ring +0x18 (data, size, pos), line index +0x24; the 0x30-byte
//   SReverbTapParams (history delays +0, history gains +8, filter SGainParams +0x10, output gain +0x2C).
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
struct CDspOps;

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
    void Reset();                          // 0x00C744E0
    void Apply(const SGainParams* params, const float* source, float* block, bool ramp, unsigned long count);   // 0x00C74CE0
};

struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 (floats)
    unsigned int m_pos;                    // +0x08 (write position)
    unsigned int Back(unsigned int pos, unsigned int n) const { return pos >= n ? pos - n : m_size - n + pos; }
};

struct SReverbTapParams {                  // 0x30 bytes
    unsigned int m_historyDelay[2];        // +0x00 samples back from the block start
    float m_historyGain[2];                // +0x08
    SGainParams m_filter;                  // +0x10
    float m_outputGain;                    // +0x2C
};

struct CDspOps {                           // x87 vtable 0x012BF02C
    virtual long GetKind();                                                                         // 0x00C73FB0
    virtual void ScaleRampCopy(float* dst, const float* src, float gain, float step, unsigned int count);
    virtual void ScaleRamp(float* buffer, float gain, float step, unsigned int count);
    virtual void ScaleCopy(float* dst, const float* src, float gain, unsigned int count);
    virtual void Scale(float* buffer, float gain, unsigned int count);
    virtual void Add(float* dst, const float* src, unsigned int count);
    virtual void MixRamp(float* dst, const float* src, float gain, float step, unsigned int count);
    virtual void Mix(float* dst, const float* src, float gain, unsigned int count);                // slot 7
    void MixFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, unsigned int count);   // 0x00C76760
};

struct CReverbFilterTap : CDspObject {     // vtable 0x012B958C
    CDspGain m_gain;                       // +0x04
    CRingBuffer m_ring;                    // +0x18
    unsigned int m_index;                  // +0x24
    void Process(const SReverbTapParams* params, const CRingBuffer* history, float* block, const float* input,
        unsigned int count);
};

void CReverbFilterTap::Process(const SReverbTapParams* params, const CRingBuffer* history, float* block, const float* input,
    unsigned int count)
{
    float* data = m_ring.m_data;
    unsigned int pos = m_ring.m_pos;
    unsigned int start = history->Back(history->m_pos, count);
    memset(block, 0, count * sizeof(float));
    unsigned int first = count;
    if (pos + count > m_ring.m_size)
        first = m_ring.m_size - pos;
    m_gain.Apply(&params->m_filter, data + pos, block, true, first);
    if (first < count)
        m_gain.Apply(&params->m_filter, data, block + first, true, count - first);
    for (unsigned int i = 0; i < 2; ++i)
        s_pOps->MixFromRing(block, history, history->Back(start, params->m_historyDelay[i]), params->m_historyGain[i], count);
    s_pOps->Mix((float*)input, block, params->m_outputGain, count);
}