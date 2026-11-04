// CReverbFilterTap::ProcessCrossfade(SReverbTapParams* current, const SReverbTapParams* target, const CRingBuffer* history, float* block, const float* input, unsigned int count, unsigned int remaining) -- x87-path block of one filtered reverb tap while its parameters cross-fade (remaining of 512 samples): clear the block, cross-fade the tap filter over its own ring (two pieces on wrap), mix the two history taps of both parameter sets with ramped gains, ramp-mix the block into the channel buffer; adopt the target parameters when the fade ends
// Grade: DIFFER(607v588) -- loop induction: VC7.1 bases the two history-tap pointers on target->m_historyGain (+8) and keeps the index i for current->m_historyDelay[i], retail bases them on target->m_historyDelay (+0) with an eliminated 2-count; the extra i slot shifts every frame slot (dl 45)
// Leaf: ?ProcessCrossfade@CReverbFilterTap@@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C89850, 588 bytes, thiscall `ret 1Ch`. Constants 1/512 (0x012A3AF4), 1.0f (0x0122DED8),
//   2^32 (0x0122DCB4, unsigned fix-up).
// Identity: the cross-fade twin of the landed CReverbFilterTap::Process 0x00C89750 (same ring, filter and history taps); names
//   descriptive (libpk, no PDB/RTTI); the catalog's "Animation_InterpolateProcess" is propagated.
// Callees: CDspGain::Crossfade 0x00C74D30 (twice when the ring wraps), CDspOps::MixRampFromRing 0x00C767E0 (four times), virtual
//   slot 6 MixRamp (+0x18) on CDspObject::s_pOps 0x0143CCDC.
// Shape found: the step divides by `(float)count` spelled at each use (a named `n` lets VC7.1 hoist a reciprocal out of the loop),
//   the memset after the end gains. 27 attempts (LOG.tsv).
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
    void Crossfade(SGainParams* current, const SGainParams* target, const float* source, float* block, int ramp, unsigned int count, unsigned int remaining);   // 0x00C74D30
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
    void MixRampFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step, unsigned int count);   // 0x00C767E0
};

struct CReverbFilterTap : CDspObject {     // vtable 0x012B958C
    CDspGain m_gain;                       // +0x04
    CRingBuffer m_ring;                    // +0x18
    unsigned int m_index;                  // +0x24
    void ProcessCrossfade(SReverbTapParams* current, const SReverbTapParams* target, const CRingBuffer* history, float* block,
        const float* input, unsigned int count, unsigned int remaining);
};

void CReverbFilterTap::ProcessCrossfade(SReverbTapParams* current, const SReverbTapParams* target, const CRingBuffer* history,
    float* block, const float* input, unsigned int count, unsigned int remaining)
{
    float* data = m_ring.m_data;
    unsigned int pos = m_ring.m_pos;
    unsigned int start = history->Back(history->m_pos, count);
    float fade = (float)remaining;
    float startOld = fade * (1.0f / 512);
    float startNew = 1.0f - startOld;
    float endOld = (fade - (float)count) * (1.0f / 512);
    float endNew = 1.0f - endOld;
    memset(block, 0, count * sizeof(float));
    unsigned int first = count;
    if (pos + count > m_ring.m_size)
        first = m_ring.m_size - pos;
    m_gain.Crossfade(&current->m_filter, &target->m_filter, data + pos, block, 1, first, remaining);
    if (first < count)
        m_gain.Crossfade(&current->m_filter, &target->m_filter, data, block + first, 1, count - first, remaining);
    float deltaOld = endOld - startOld;
    float deltaNew = endNew - startNew;
    for (unsigned int i = 0; i < 2; ++i) {
        float gain = current->m_historyGain[i];
        s_pOps->MixRampFromRing(block, history, history->Back(start, current->m_historyDelay[i]), gain * startOld, deltaOld * gain / (float)count, count);
        gain = target->m_historyGain[i];
        s_pOps->MixRampFromRing(block, history, history->Back(start, target->m_historyDelay[i]), gain * startNew, deltaNew * gain / (float)count, count);
    }
    float levelStart = startNew * target->m_outputGain + startOld * current->m_outputGain;
    float levelEnd = endNew * target->m_outputGain + endOld * current->m_outputGain;
    s_pOps->MixRamp((float*)input, block, levelStart, (levelEnd - levelStart) / (float)count, count);
    if (count >= remaining)
        *current = *target;
}
