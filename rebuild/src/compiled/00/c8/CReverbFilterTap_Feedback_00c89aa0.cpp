// CReverbFilterTap::Feedback(float (*blocks)[128], float* feedback, const float* mixRow, unsigned int count) -- write `count` samples into the tap's delay ring: each is the feedback input plus the four taps' blocks weighted by one row of the mix matrix, advancing (and wrapping) the ring position
// Leaf: ?Feedback@CReverbFilterTap@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C89AA0, length 147 bytes (+ int3 padding), ret 10h. No callees.
//   Callers: the tap bank's process paths 0x00C75E71 / 0x00C75F56 (CReverbTapBank::Process 0x00C75E90, agent444),
//   once per tap with the 4x4 table 0x012BF088 row (0.5 on the diagonal, -0.5 elsewhere).
// Layout (descriptive; libpk reverb module, no PDB/RTTI; agent444's CReverbFilterTap, vtable 0x012B958C):
//   CDspObject base, CDspGain +4, ring {data +0x18, size +0x1C, pos +0x20}, line index +0x24.
// Shape: the four-block sum is an inner `for (j < 4)` accumulating onto `sum = feedback[i]`: VC7.1 fully unrolls the
//   inner loop in source order (input first, then block 0..3) and leaves the outer loop rolled. Writing the sum as
//   one expression makes VC reassociate it and unroll the outer loop four times. The wrap is
//   `pos + 1 < size ? pos + 1 : pos + 1 - size` (VC emits `1 - size` added to pos); m_size is re-read every sample
//   (the ring store may alias it) and the data pointer lives in the push-ecx slot.
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

struct CReverbFilterTap : CDspObject {     // vtable 0x012B958C
    CDspGain m_gain;                       // +0x04
    CRingBuffer m_ring;                    // +0x18
    unsigned int m_index;                  // +0x24
    void Feedback(float (*blocks)[128], float* feedback, const float* mixRow, unsigned int count);
};

void CReverbFilterTap::Feedback(float (*blocks)[128], float* feedback, const float* mixRow, unsigned int count)
{
    float* data = m_ring.m_data;
    unsigned int pos = m_ring.m_pos;
    for (unsigned int i = 0; i < count; i++) {
        float sum = feedback[i];
        for (unsigned int j = 0; j < 4; j++)
            sum += blocks[j][i] * mixRow[j];
        data[pos] = sum;
        if (pos + 1 < m_ring.m_size)
            pos = pos + 1;
        else
            pos = pos + 1 - m_ring.m_size;
    }
    m_ring.m_pos = pos;
}