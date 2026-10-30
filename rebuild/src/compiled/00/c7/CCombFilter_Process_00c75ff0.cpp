// CCombFilter::Process(const SCombSettings* current, CRingBuffer* line, float* output, const float* input, unsigned long count) -- one reverb comb block: add the first history tap of the shared line into the output, read the comb's own ring back by its delay scaled by the feedback gain into a 16-aligned block, add both history taps into it, write the block back into the comb ring, mix it into the input channel at the comb gain and advance the ring position
// Leaf: ?Process@CCombFilter@@QAEXPBUSCombSettings@@PAUCRingBuffer@@PAMPBMK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75FF0, 303 bytes, thiscall `ret 14h`, ebp frame with `and esp,-16` for the
//   128-float block (`__declspec(align(16))`).
// Identity: the landed CReverbProcessor::ProcessCombs 0x00C69FB0 calls it on each of the four 0x14-byte combs (+0x1F8,
//   vtable 0x012B9588) as Process(current settings, &m_line, output, inputs[i], count) when the comb is not blending (the
//   blending twin is Blend 0x00C76120); names are the landed descriptive ones (libpk: no PDB/RTTI). The manifest's
//   "Array_InsertWrappedWithCallbacks" is descriptive.
// Callees (all on the shared ops table CDspObject::s_pOps 0x0143CCDC): CDspOps::AddFromRing 0x00C76680 (x3),
//   CDspOps::ReadRingScaled 0x00C764E0, CDspOps::WriteRing 0x00C76460, virtual slot 7 Mix (+0x1C).
// Layout: CCombFilter = CDspObject vptr + its own ring (data +4, size +8, position +0xC) + blend mode +0x10 (landed Reset
//   0x00C69B60 / ProcessCombs models); SCombSettings 0x14 bytes: two history-tap delays +0, comb delay +8, feedback gain
//   +0xC, output gain +0x10. The shared line is passed straight to the CDspOps ring helpers, so it is a CRingBuffer (the
//   landed caller's local `CDelayLine` has the same three members; NAME_FIXES.tsv).
// Shape: the ring wrap is CRingBuffer::Back (`pos >= n ? pos - n : size - n + pos`) as in the landed
//   CReverbFilterTap::Process 0x00C89750; the start position is read into a named local before the first call and the
//   advance is the mirror `pos + n < size ? pos + n : pos - size + n` written on the members (an inline CRingBuffer
//   method there makes VC7.1 keep &m_ring in ebx across the Mix call; retail reloads `this`); `this` and the old
//   position live in frame slots.
struct CDspOps;

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
    static CDspOps* s_pOps;                // 0x0143CCDC
};

struct CRingBuffer {
    float* m_data;                         // +0x00
    unsigned int m_size;                   // +0x04 (floats)
    unsigned int m_pos;                    // +0x08 (write position)
    unsigned int Back(unsigned int pos, unsigned int n) const { return pos >= n ? pos - n : m_size - n + pos; }
};

struct SCombSettings {                     // 0x14 bytes
    unsigned int m_historyDelay[2];        // +0x00 samples back from the block start in the shared line
    unsigned int m_delay;                  // +0x08 comb delay
    float m_feedback;                      // +0x0C
    float m_gain;                          // +0x10
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
    void WriteRing(CRingBuffer* ring, unsigned int pos, const float* src, unsigned int count);                 // 0x00C76460
    void ReadRingScaled(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, unsigned int count);  // 0x00C764E0
    void AddFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, unsigned int count);                // 0x00C76680
};

class CCombFilter : public CDspObject {    // vtable 0x012B9588, 0x14 bytes
public:
    CRingBuffer m_ring;                    // +0x04
    long        m_mode;                    // +0x10 blend mode (landed ProcessCombs)
    void Process(const SCombSettings* current, CRingBuffer* line, float* output, const float* input, unsigned long count);
};

void CCombFilter::Process(const SCombSettings* current, CRingBuffer* line, float* output, const float* input, unsigned long count)
{
    __declspec(align(16)) float block[128];
    unsigned int pos = m_ring.m_pos;
    unsigned int read = m_ring.Back(pos, current->m_delay);
    unsigned int start = line->Back(line->m_pos, count);
    s_pOps->AddFromRing(output, line, line->Back(start, current->m_historyDelay[0]), count);
    s_pOps->ReadRingScaled(block, &m_ring, read, current->m_feedback, count);
    for (unsigned int i = 0; i < 2; ++i)
        s_pOps->AddFromRing(block, line, line->Back(start, current->m_historyDelay[i]), count);
    s_pOps->WriteRing(&m_ring, pos, block, count);
    s_pOps->Mix((float*)input, block, current->m_gain, count);
    m_ring.m_pos = pos + count < m_ring.m_size ? pos + count : pos - m_ring.m_size + count;
}