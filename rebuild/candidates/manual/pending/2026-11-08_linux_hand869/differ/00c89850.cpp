// CReverbFilterTap::ProcessRamp(SReverbTapParams* current, const SReverbTapParams* target, const CRingBuffer* history, float* block, const float* input, unsigned int count, unsigned int rampPosition) -- x87-path block of one filtered reverb tap while its parameters are being changed: clear the tap's block, run the gain filter's cross-fade from the current to the target record over the tap's own ring into the block (in two pieces when the ring wraps), mix the two history taps of both records out of the shared history ring with gains ramping from the old weight (rampPosition/512) to its value at the end of the block, mix the block into the channel buffer with the cross-faded output gain, and adopt the target record once the ramp ends inside this block
// Grade: DIFFER(607v588) after 14 grades (631 -> 615 -> 607; v5/v6/v7/v19/v20/v21 equal, v2/v8/v12 worse). Residue: the two-trip history-tap loop -- retail eliminates `i` (ebp walks target->m_historyDelay[i] with current reached through the `current - target` distance for BOTH the delay [eax+ebp] and the gain [eax+ebp+8], a `mov [esp+20h],2 ; dec ; jne` down-counter), VC7.1 /O2 keeps `i` as an up-counter in a frame slot and indexes current->m_historyDelay[i] through a reload of `current`, basing ebp at target+8 (the gain); the frame-slot numbering of other/blendEnd/data/distance follows from that. Everything before the loop and after it (tail gain mix, the rep movsd record copy) matches.
// Next: c2prio.py on the loop body (which address expression becomes the strength-reduction base and why the delay index is not reduced); try modelling the two taps as a struct array walked by pointer, or an LTCG-style TU with CDspOps::MixRampFromRing's body present so VC7.1 sees its register use.
// Leaf: ?ProcessRamp@CReverbFilterTap@@QAEXPAUSReverbTapParams@@PBU2@PBUCRingBuffer@@PAMPBMII@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C89850, 588 bytes, thiscall `ret 1Ch`. Callees: CDspGain::Crossfade 0x00C74D30 (twice when the
//   ring wraps; mode 1 as the landed CReverbDiffuserStage::ProcessRamped), CDspOps::MixRampFromRing 0x00C767E0 (four times) and
//   virtual slot 6 MixRamp (+0x18) on the shared ops table CDspObject::s_pOps 0x0143CCDC. The block clear is the memset intrinsic
//   (`rep stosd`), the record copy the 0x30-byte `rep movsd`.
// Identity: the landed CReverbTapBank::Process 0x00C75E90 calls it as CReverbFilterTap::ProcessRamp (7 args) for the tap whose line is
//   being changed and binds exactly this symbol; the landed CReverbFilterTap::Process 0x00C89750 is its un-ramped twin (same filter /
//   history-tap / output-mix sequence with fixed gains) and the landed CReverbDiffuserStage::ProcessRamped 0x00C89350 the same ramp
//   scheme for the diffuser. Names are the landed descriptive ones (libpk: no PDB/RTTI); the catalog's "ProcessCrossfade" is the
//   catalog's own guess for the same function. `current` is written (the closing copy from `target`), so it is non-const.
// Constants: 1/512 0x012A3AF4, 1.0f 0x0122DED8, 2^32 0x0122DCB4 (unsigned -> float fix-ups of rampPosition and count).
// Layout: the landed Process / MixHistory model: CReverbFilterTap gain +0x04, ring +0x18 (data, size, pos), line index +0x24; the
//   0x30-byte SReverbTapParams (history delays +0, history gains +8, filter SGainParams +0x10, output gain +0x2C).
// Shape: the old weight `blend` = rampPosition/512 and `other` = 1 - blend, then their values at the block end from (rampPosition -
//   count)/512; each history tap is mixed twice (current record at blend, target record at other) with the per-sample step
//   (end - start) * gain / count; the two-trip tap loop stays rolled and walks both records through one pointer plus their distance.
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
    void Crossfade(SGainParams* oldParams, const SGainParams* newParams, const float* source, float* block, int mode,
                   unsigned int count, unsigned int remaining);   // 0x00C74D30
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
    virtual void MixRamp(float* dst, const float* src, float gain, float step, unsigned int count); // slot 6
    virtual void Mix(float* dst, const float* src, float gain, unsigned int count);                // slot 7
    void MixRampFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, float gain, float step, unsigned int count);   // 0x00C767E0
};

struct CReverbFilterTap : CDspObject {     // vtable 0x012B958C
    CDspGain m_gain;                       // +0x04
    CRingBuffer m_ring;                    // +0x18
    unsigned int m_index;                  // +0x24
    void ProcessRamp(SReverbTapParams* current, const SReverbTapParams* target, const CRingBuffer* history, float* block,
        const float* input, unsigned int count, unsigned int rampPosition);
};

void CReverbFilterTap::ProcessRamp(SReverbTapParams* current, const SReverbTapParams* target, const CRingBuffer* history,
    float* block, const float* input, unsigned int count, unsigned int rampPosition)
{
    float* data = m_ring.m_data;
    unsigned int pos = m_ring.m_pos;
    unsigned int start = history->Back(history->m_pos, count);
    float ramp = (float)rampPosition;
    float blend = ramp * (1.0f / 512.0f);
    float other = 1.0f - blend;
    float blendEnd = (ramp - (float)count) * (1.0f / 512.0f);
    float otherEnd = 1.0f - blendEnd;
    memset(block, 0, count * sizeof(float));
    unsigned int first = count;
    if (pos + count > m_ring.m_size)
        first = m_ring.m_size - pos;
    m_gain.Crossfade(&current->m_filter, &target->m_filter, data + pos, block, 1, first, rampPosition);
    if (first < count)
        m_gain.Crossfade(&current->m_filter, &target->m_filter, data, block + first, 1, count - first, rampPosition);
    float blendStep = blendEnd - blend;
    float otherStep = otherEnd - other;
    for (unsigned int i = 0; i < 2; ++i) {
        float g = current->m_historyGain[i];
        s_pOps->MixRampFromRing(block, history, history->Back(start, current->m_historyDelay[i]),
            g * blend, blendStep * g / (float)count, count);
        g = target->m_historyGain[i];
        s_pOps->MixRampFromRing(block, history, history->Back(start, target->m_historyDelay[i]),
            g * other, otherStep * g / (float)count, count);
    }
    float gain = blend * current->m_outputGain + other * target->m_outputGain;
    float gainEnd = blendEnd * current->m_outputGain + otherEnd * target->m_outputGain;
    s_pOps->MixRamp((float*)input, block, gain, (gainEnd - gain) / (float)count, count);
    if (count >= rampPosition)
        *current = *target;
}
