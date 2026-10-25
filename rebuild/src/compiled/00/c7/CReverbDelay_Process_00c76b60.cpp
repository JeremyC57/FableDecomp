// CReverbDelay::Process(const SReverbDelayParams* params, const float* input, float* output, unsigned int count) -- one block of the reverb's feedback delay: read the delayed signal scaled by the feedback gain, write the input into the ring, add the feedback back in, mix the delayed signal into the output when the wet gain is non-zero, then advance the ring
// Leaf: ?Process@CReverbDelay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C76B60, length 183 bytes, ret 10h. Callees (all on the shared
//   ops table CDspObject::s_pOps 0x0143CCDC): ReadRingScaled 0x00C764E0, WriteRing 0x00C76460, AddToRing 0x00C766F0,
//   virtual slot 7 Mix. Called by the reverb's block process 0x00C6A12E for its +0x248 member with the param block
//   at +0x228 (the ramping twin 0x00C76C20 runs while a parameter change is pending).
// Layout (descriptive; libpk reverb module, no PDB/RTTI): CReverbDelay (vtable 0x012B957C, 0x14 bytes): CDspObject
//   base, CRingBuffer {data +4, size +8, pos +0xC}, line index +0x10 (Allocate 0x00C76AB0). Params: delay in samples
//   +0, feedback gain +4, wet gain +8.
// Shape: the 128-float scratch block is a 16-byte-aligned local (`and esp,-16 ; sub esp,204h`); the read position
//   is an if/else on `pos >= delay`; the ring advance is the inline AdvanceRing helper (m_size loaded before the
//   m_pos store, eax keeps the sum for the wrap).
struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 (floats)
    unsigned int m_pos;                    // +0x08 (write position)
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
    void ReadRing(float* dst, const CRingBuffer* ring, unsigned int pos, unsigned int count);
    void WriteRing(CRingBuffer* ring, unsigned int pos, const float* src, unsigned int count);
    void ReadRingScaled(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, unsigned int count);
    void ReadRingScaledRamp(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step, unsigned int count);
    void AddFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, unsigned int count);
    void AddToRing(CRingBuffer* ring, unsigned int pos, const float* src, unsigned int count);
    void MixFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, unsigned int count);
    void MixRampFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step, unsigned int count);
};

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
    static CDspOps* s_pOps;                // 0x0143CCDC
    static long s_refCount;                // 0x0143CCE0
};

struct SReverbDelayParams {
    unsigned int m_delay;                  // +0x00 samples
    float m_feedback;                      // +0x04
    float m_wet;                           // +0x08
};

struct CReverbDelay : CDspObject {         // vtable 0x012B957C
    CRingBuffer m_ring;                    // +0x04
    unsigned int m_index;                  // +0x10
    void Process(const SReverbDelayParams* params, const float* input, float* output, unsigned int count);
};

inline void AdvanceRing(CRingBuffer* ring, unsigned int count)
{
    ring->m_pos += count;
    if (ring->m_pos >= ring->m_size)
        ring->m_pos -= ring->m_size;
}

void CReverbDelay::Process(const SReverbDelayParams* params, const float* input, float* output, unsigned int count)
{
    __declspec(align(16)) float delayed[128];
    unsigned int pos = m_ring.m_pos;
    unsigned int read;
    if (pos >= params->m_delay)
        read = pos - params->m_delay;
    else
        read = m_ring.m_size - params->m_delay + pos;
    s_pOps->ReadRingScaled(delayed, &m_ring, read, params->m_feedback, count);
    s_pOps->WriteRing(&m_ring, pos, input, count);
    s_pOps->AddToRing(&m_ring, pos, delayed, count);
    if (params->m_wet != 0.0f)
        s_pOps->Mix(output, delayed, params->m_wet, count);
    AdvanceRing(&m_ring, count);
}