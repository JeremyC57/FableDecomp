// CDspGain::Apply(const SGainParams* params, const float* source, float* block, int mode, unsigned int count) -- run the gain's per-channel ramps over one block (ApplyRamps), then scale the block by the master gain unless it is exactly 1.0
// Leaf: ?Apply@CDspGain
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74CE0, length 73 bytes, ret 14h. Callees: CDspGain::ApplyRamps
//   0x00C74510 (a thiscall that never reads `this`, ret 1Ch: params+4, params+0x10, the gain's own ramp state +4,
//   source, block, mode, count), then virtual slot 4 Scale of the shared ops table 0x0143CCDC. Ten callers in the
//   reverb (0x00C69E7A.., the +0x6C / +0x80 gains, and the diffuser stages 0x00C892E3..).
// Layout (descriptive; libpk reverb module, no PDB/RTTI): CDspGain (vtable 0x012B9578): CDspObject base, four ramp
//   floats +4..+0x10 (Reset 0x00C744E0 sets them to 1.0f). The params: master gain +0, two three-float sets +4/+0x10.
// Shape: `master != 1.0f` compiles to an integer compare with 0x3F800000 (the master is forwarded as a dword to
//   Scale); the arguments are loaded into callee-saved registers before the pushes so the Scale call reuses them.
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

struct SGainParams {
    float m_master;                        // +0x00
    float m_current[3];                    // +0x04
    float m_target[3];                     // +0x10
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
    void Reset();                          // 0x00C744E0
    void ApplyRamps(const float* current, const float* target, float* state, const float* source, float* block,
        int mode, unsigned int count);     // 0x00C74510
    void Apply(const SGainParams* params, const float* source, float* block, int mode, unsigned int count);
};

void CDspGain::Apply(const SGainParams* params, const float* source, float* block, int mode, unsigned int count)
{
    ApplyRamps(params->m_current, params->m_target, m_gain, source, block, mode, count);
    if (params->m_master != 1.0f)
        s_pOps->Scale(block, params->m_master, count);
}