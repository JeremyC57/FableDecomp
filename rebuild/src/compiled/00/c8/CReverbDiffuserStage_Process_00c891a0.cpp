// CReverbDiffuserStage::Process(const SDiffuserStageParams* params, const CRingBuffer* history, float* output, float* mix, unsigned int count) -- one block of a libpk reverb diffuser stage: sum four delayed taps of the shared history ring into a 16-byte-aligned scratch block, copy it scaled to `output`, scale it, add the stage's own line at its delay, write it back into the own line through the ramped gain (split at the ring's end), mix it scaled into `mix`, and advance the own line's write position
// Leaf: ?Process@CReverbDiffuserStage@@QAEXPBUSDiffuserStageParams@@PBUCRingBuffer@@PAM2I@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C891A0, 428 bytes, thiscall `ret 14h`, ebp frame with `and esp,-16` (the aligned
//   float[128] scratch block). Callers: CReverbDiffuser::Process 0x00C75A8C and ProcessRamped 0x00C75B9C (both landed).
// Names: DESCRIPTIVE (libpk reverb, no PDB/RTTI). Class and method are the landed callers' (0x00C75A20 binds this address as
//   CReverbDiffuserStage::Process). Its parameter types are respelled from the bytes (NAME_FIXES.tsv): the second argument is
//   a delay ring, not a parameter block (read as {data +0, size +4, write position +8}, passed to CDspOps::ReadRing /
//   AddFromRing as `const CRingBuffer*`, the same correction the landed UpdateGain 0x00C756C0 made), and the fourth is
//   written (the destination of the Mix kernel), so it is `float*`, not `const float*`.
// Callees: CDspOps::ReadRing 0x00C763E0, CDspOps::AddFromRing 0x00C76680 (x4) through CDspObject::s_pOps 0x0143CCDC; ops
//   virtual slots 3 ScaleCopy, 4 Scale, 7 Mix; CDspGain::Apply 0x00C74CE0 (the landed definition's spelling; mode 1).
// Shape: the own line's write-position update is an inline CRingBuffer member: VC7.1 then reuses the `&m_ring` it formed for
//   AddFromRing and keeps `this` in a frame slot, so the tap loop index gets ebx as in retail (written out on `m_ring`
//   directly, `this` takes ebx and the index spills: 419v428). Grades: 2.
// Layout: CReverbDiffuserStage as the landed CReverbDiffuser rows (CDspObject base, ring +4, CDspGain +0x10, index +0x24) with
//   the ring as the landed CRingBuffer sub-object; SDiffuserStageParams (0x3C, two per SDiffuserParams at +0x60) field names
//   are descriptive.
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
    void ReadRing(float* dst, const CRingBuffer* ring, unsigned int pos, unsigned int count);      // 0x00C763E0
    void AddFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, unsigned int count);   // 0x00C76680
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
    void Apply(const SGainParams* params, const float* source, float* block, int mode, unsigned int count);   // 0x00C74CE0
};

struct SDiffuserStageParams {              // 0x3C
    unsigned int m_taps[4];                // +0x00 read delays into the shared history (samples)
    float m_tapGain;                       // +0x10 scale of the summed taps before the stage's own line is added
    float m_directGain;                    // +0x14 scale of the summed taps copied to `output`
    SGainParams m_gain;                    // +0x18 ramped gain written into the stage's own line
    unsigned int m_delay;                  // +0x34 read delay into the stage's own line
    float m_mixGain;                       // +0x38 scale of the result mixed into `mix`
};

struct CReverbDiffuserStage : CDspObject { // vtable 0x012BF07C
    CRingBuffer m_ring;                    // +0x04
    CDspGain m_gain;                       // +0x10
    unsigned int m_index;                  // +0x24
    void Process(const SDiffuserStageParams* params, const CRingBuffer* history, float* output, float* mix, unsigned int count);
};

void CReverbDiffuserStage::Process(const SDiffuserStageParams* params, const CRingBuffer* history, float* output, float* mix, unsigned int count)
{
    __declspec(align(16)) float block[128];
    unsigned int start = history->m_pos >= count ? history->m_pos - count : history->m_size - count + history->m_pos;
    float* data = m_ring.m_data;
    unsigned int pos = m_ring.m_pos;
    unsigned int tap = params->m_taps[0];
    s_pOps->ReadRing(block, history, start >= tap ? start - tap : history->m_size - tap + start, count);
    for (unsigned int i = 1; i < 4; ++i)
    {
        tap = params->m_taps[i];
        s_pOps->AddFromRing(block, history, start >= tap ? start - tap : history->m_size - tap + start, count);
    }
    s_pOps->ScaleCopy(output, block, params->m_directGain, count);
    s_pOps->Scale(block, params->m_tapGain, count);
    unsigned int delay = params->m_delay;
    s_pOps->AddFromRing(block, &m_ring, pos >= delay ? pos - delay : m_ring.m_size - delay + pos, count);
    unsigned int first = count;
    if (pos + count > m_ring.m_size)
        first = m_ring.m_size - pos;
    m_gain.Apply(&params->m_gain, block, data + pos, 1, first);
    if (first < count)
        m_gain.Apply(&params->m_gain, block + first, data, 1, count - first);
    s_pOps->Mix(mix, block, params->m_mixGain, count);
    m_ring.Advance(count);
}