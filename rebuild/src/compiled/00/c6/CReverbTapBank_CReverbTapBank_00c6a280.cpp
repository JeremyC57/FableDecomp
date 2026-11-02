// CReverbTapBank::CReverbTapBank() -- EH-framed constructor of the libpk reverb's filtered-tap bank: the CVtable_012bf050 base (0x00C74420), the bank vtable 0x012B9590 (EH state 0), then its four CReverbFilterTaps at +4 through `__ehvec_ctor` (element 0x28 bytes, CReverbFilterTap::CReverbFilterTap 0x00C698D0 / ~CReverbFilterTap 0x00C69930)
// Leaf: ??0CReverbTapBank@@QAE@XZ
// Naming wave 31 (family libpk): the reverb DSP base (vtable 0x012BF050; ctor 0x00C74420, virtual dtor 0x00C74450) is CVtable_012bf050 (DECISIONS_W30 D7:
//   libpk has no PDB / RTTI, vtable-address convention; was the descriptive CDspObject, ??1CDspObject@@UAE@XZ)
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6A280, 88 bytes, thiscall, returns `this`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444, the libpk reverb family):
//   - 0x012B9590's only slot is the scalar deleting destructor 0x00C6A3B0 around the destructor 0x00C6A2E0, which destroys
//     the same four 0x28-byte elements with the same element destructor 0x00C69930;
//   - the element constructor and destructor are CReverbFilterTap's (vtable 0x012B958C: CDspGain +4, delay memory +0x18,
//     size +0x1c, position +0x20, line index +0x24);
//   - the only caller is the reverb constructor 0x00C6A650 (agent443's CReverbProcessor, vtable 0x012B9594), for its
//     +0x94 member.
//   The landed unattributed CArenaRoundDef is a propagated manifest label; descriptive names (libpk: no PDB/RTTI).
// Shape: the base runs first, then the vtable store with EH state 0 (the base's destructor), then the array; the tail
//   words (+0xa4..+0xac) are set later by the reverb's setup, not here.
class CVtable_012bf050 {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CVtable_012bf050();                                       // 0x00C74420 (shared kernel table: create or add a reference)
    virtual ~CVtable_012bf050();                              // 0x00C74450 (drop the reference)
};

class CDspGain : public CVtable_012bf050 {                    // vtable 0x012B9578, 0x14 bytes
public:
    CDspGain() { }                                      // out of line at 0x00C69660
    ~CDspGain() { }                                     // out of line at 0x00C69680
    float m_gain[4];                                    // +0x04 running gains
};

class CReverbFilterTap : public CVtable_012bf050 {            // vtable 0x012B958C, 0x28 bytes
public:
    CReverbFilterTap();                                 // 0x00C698D0
    CDspGain     m_gain;                                // +0x04
    float*       m_data;                                // +0x18
    unsigned int m_size;                                // +0x1c
    unsigned int m_pos;                                 // +0x20
    unsigned int m_index;                               // +0x24
};

class CReverbTapBank : public CVtable_012bf050 {              // vtable 0x012B9590, 0xb0 bytes (destructor compiler-generated)
public:
    CReverbTapBank();
    CReverbFilterTap m_taps[4];                         // +0x04
    unsigned int     m_channels;                        // +0xa4
    unsigned int     m_firstIndex;                      // +0xa8
    unsigned int     m_endIndex;                        // +0xac
};

CReverbTapBank::CReverbTapBank()
{
}
