// CReverbFilterTap::MixHistory(const SReverbTapParams* params, const CRingBuffer* history, float* block, float* channel, unsigned int count) -- SIMD-path second half of a filtered tap: mix the two history taps (delay and gain from the parameters) out of the shared history ring into the tap's block, then mix the block into its channel buffer at the tap's output gain
// Leaf: ?MixHistory@CReverbFilterTap@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C89B60, length 125 bytes (+ int3 padding), ret 14h. `this` is never
//   read. Callees on the shared ops table CDspObject::s_pOps 0x0143CCDC: CDspOps::MixFromRing 0x00C76760 (agent444)
//   twice, then virtual slot 7 Mix (+0x1C). Caller: the tap bank's SIMD process path 0x00C75DC4 (after the per-tap
//   ring read 0x00C89B40 and the four-wide filter 0x00C88E60); the x87 path calls Process 0x00C89750 instead.
// Layout (descriptive; libpk reverb module, no PDB/RTTI): the 0x30-byte tap parameter record: two history delays +0,
//   two history gains +8, the filter's SGainParams +0x10 (used by Process 0x00C89750), output gain +0x2C.
// Shape: both ring offsets come from one inline CRingBuffer::Back (pos >= n ? pos - n : size - n + pos); the 2-trip
//   tap loop stays rolled (esi counter, `cmp esi,2`) and the count is re-read from its slot after each call.
struct CDspOps;

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
    static CDspOps* s_pOps;                // 0x0143CCDC
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
    void Reset();                          // 0x00C744E0
};

struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 (floats)
    unsigned int m_pos;                    // +0x08 (write position)
    unsigned int Back(unsigned int pos, unsigned int n) const { return pos >= n ? pos - n : m_size - n + pos; }
};

struct SGainParams {
    float m_master;                        // +0x00
    float m_current[3];                    // +0x04
    float m_target[3];                     // +0x10
};

struct SReverbTapParams {                  // 0x30 bytes
    unsigned int m_historyDelay[2];        // +0x00 samples back from the block start
    float m_historyGain[2];                // +0x08
    SGainParams m_filter;                  // +0x10
    float m_outputGain;                    // +0x2C
};

struct CDspOps {                           // x87 vtable 0x012BF02C (SIMD build: ctor 0x00C88A30)
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
    void MixHistory(const SReverbTapParams* params, const CRingBuffer* history, float* block, float* channel, unsigned int count);
};

void CReverbFilterTap::MixHistory(const SReverbTapParams* params, const CRingBuffer* history, float* block, float* channel, unsigned int count)
{
    unsigned int start = history->Back(history->m_pos, count);
    for (unsigned int i = 0; i < 2; ++i)
        s_pOps->MixFromRing(block, history, history->Back(start, params->m_historyDelay[i]), params->m_historyGain[i], count);
    s_pOps->Mix(channel, block, params->m_outputGain, count);
}