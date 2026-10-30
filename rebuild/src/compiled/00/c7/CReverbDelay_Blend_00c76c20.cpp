// CReverbDelay::Blend(SReverbDelayParams* current, const SReverbDelayParams* target, const float* input, float* output, unsigned int count, unsigned int blendPosition) -- the ramping twin of CReverbDelay::Process while a parameter change is pending: read the ring at the current delay with the feedback ramping out (blendPosition / 512 down by 1/512 a sample), mix in the target delay's read with its feedback ramping in, write the input and add the feedback back into the ring, mix the delayed block into the output with the wet gain ramping from current to target, advance the ring, and adopt the target parameters once the blend completes in this block
// Leaf: ?Blend@CReverbDelay@@QAEXPAUSReverbDelayParams@@PBU2@PBMPAMII@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C76C20, 381 bytes, thiscall, six stack arguments, `ret 18h`, 16-byte aligned ebp
//   frame (`and esp,-16 ; sub esp,214h`) for the 128-float scratch block.
// Identity: the landed CReverbDelay::Process 0x00C76B60 (the row right before this one) names this address "the ramping
//   twin ... runs while a parameter change is pending"; same object layout (m_ring at +4, its pos +0xC / size +8), same
//   param block (delay +0, feedback +4, wet +8) and the same shared-ops callees on CDspObject::s_pOps 0x0143CCDC:
//   ReadRingScaledRamp 0x00C76560, MixRampFromRing 0x00C767E0, WriteRing 0x00C76460, AddToRing 0x00C766F0, virtual slot 6
//   MixRamp (+0x18). Its only caller, the reverb block process 0x00C6A080, calls it as the "Blend" of the +0x248 stage with
//   (current, target, input, output, count, BlendPosition), the pattern of the landed CCombFilter::Blend 0x00C76120 and
//   the stage Blends 0x00C75AB0 / 0x00C75E90. Name descriptive (libpk reverb module: no PDB / RTTI); the landed caller's
//   `StageTwo::Blend(void*,void*,float*,float*,unsigned long,unsigned long)` spelling is a placeholder (NAME_FIXES.tsv).
//   `current` is written (the final 12-byte copy), so it is non-const. The manifest label "Array_PartitionAndInsert" is wrong.
// Constants: 1/512 0x012A3AF4, -1/512 0x012BF070, 1.0f 0x0122DED8, 2^32 0x0122DCB4 (unsigned-to-float fix-ups).
// Shape: the landed Process model (CRingBuffer / CDspOps / CDspObject / SReverbDelayParams, inline AdvanceRing); the two
//   read positions are if/else on `pos >= delay`; the blend fraction is a float local reused for the target's (1 - blend).
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
    void Blend(SReverbDelayParams* current, const SReverbDelayParams* target, const float* input, float* output,
               unsigned int count, unsigned int blendPosition);
};

inline void AdvanceRing(CRingBuffer* ring, unsigned int count)
{
    ring->m_pos += count;
    if (ring->m_pos >= ring->m_size)
        ring->m_pos -= ring->m_size;
}

void CReverbDelay::Blend(SReverbDelayParams* current, const SReverbDelayParams* target, const float* input, float* output,
                         unsigned int count, unsigned int blendPosition)
{
    __declspec(align(16)) float delayed[128];
    unsigned int pos = m_ring.m_pos;
    unsigned int readCurrent;
    if (pos >= current->m_delay)
        readCurrent = pos - current->m_delay;
    else
        readCurrent = m_ring.m_size - current->m_delay + pos;
    unsigned int readTarget;
    if (pos >= target->m_delay)
        readTarget = pos - target->m_delay;
    else
        readTarget = m_ring.m_size - target->m_delay + pos;
    float blend = (float)blendPosition * (1.0f / 512.0f);
    s_pOps->ReadRingScaledRamp(delayed, &m_ring, readCurrent, blend * current->m_feedback,
                               current->m_feedback * (-1.0f / 512.0f), count);
    s_pOps->MixRampFromRing(delayed, &m_ring, readTarget, (1.0f - blend) * target->m_feedback,
                            target->m_feedback * (1.0f / 512.0f), count);
    s_pOps->WriteRing(&m_ring, pos, input, count);
    s_pOps->AddToRing(&m_ring, pos, delayed, count);
    float wetStep = (target->m_wet - current->m_wet) * (1.0f / 512.0f);
    s_pOps->MixRamp(output, delayed, current->m_wet + (float)(512 - blendPosition) * wetStep, wetStep, count);
    AdvanceRing(&m_ring, count);
    if (count >= blendPosition)
        *current = *target;
}