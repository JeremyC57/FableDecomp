// CReverbTapBank::Reset() -- clear the delay buffers of the bank's four filtered taps and reset each tap's gain ramp
// Leaf: ?Reset@CReverbTapBank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75CA0, length 58 bytes. Callee: CDspGain::Reset 0x00C744E0.
//   Called by the reverb's reset 0x00C69C34 for its +0x94 member.
// Layout (descriptive; libpk reverb module, no PDB/RTTI): CReverbTapBank (vtable 0x012B9590, ctor 0x00C6A280):
//   CDspObject base, four 0x28-byte CReverbFilterTap at +4 (vtable 0x012B958C, ctor 0x00C698D0: CDspObject base,
//   CDspGain +4, ring {data +0x18, size +0x1C, pos +0x20}, line index +0x24), the channel count +0xA4 and the first/end
//   line index +0xA8/+0xAC.
// Shape: one counted loop over an inline tap Reset (memset of the whole buffer, no null test, then the gain's
//   out-of-line Reset): VC7.1 walks &m_taps[0].m_data and reaches the gain with `lea ecx,[esi-14h]`.
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

struct CDspObject {
    virtual ~CDspObject();                 // 0x00C74450
    CDspObject();                          // 0x00C74420
};

struct CDspGain : CDspObject {             // vtable 0x012B9578
    float m_gain[4];                       // +0x04
    void Reset();                          // 0x00C744E0
};

struct CReverbFilterTap : CDspObject {     // vtable 0x012B958C
    CDspGain m_gain;                       // +0x04
    float* m_data;                         // +0x18
    unsigned int m_size;                   // +0x1C
    unsigned int m_pos;                    // +0x20
    unsigned int m_index;                  // +0x24
    void Reset()
    {
        memset(m_data, 0, m_size * sizeof(float));
        m_gain.Reset();
    }
};

struct CReverbTapBank : CDspObject {       // vtable 0x012B9590
    CReverbFilterTap m_taps[4];            // +0x04
    unsigned int m_channels;               // +0xA4
    unsigned int m_firstIndex;             // +0xA8
    unsigned int m_endIndex;               // +0xAC
    void Reset();
};

void CReverbTapBank::Reset()
{
    for (int i = 0; i < 4; ++i)
        m_taps[i].Reset();
}