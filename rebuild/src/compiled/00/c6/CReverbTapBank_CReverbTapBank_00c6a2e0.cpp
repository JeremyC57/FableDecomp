// CReverbTapBank::~CReverbTapBank() -- the compiler-generated destructor of the libpk reverb's filtered-tap bank: its four CReverbFilterTaps (`__ehvec_dtor`, element 0x28 bytes, ~CReverbFilterTap 0x00C69930; EH state 0), then the CVtable_012bf050 base (0x00C74450)
// Leaf: ??1CReverbTapBank@@UAE@XZ
// Naming wave 31 (family libpk): the reverb DSP base (vtable 0x012BF050; ctor 0x00C74420, virtual dtor 0x00C74450) is CVtable_012bf050 (DECISIONS_W30 D7:
//   libpk has no PDB / RTTI, vtable-address convention; was the descriptive CDspObject, ??1CDspObject@@UAE@XZ)
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6A2E0, 87 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444, the libpk reverb family):
//   - the scalar deleting destructor 0x00C6A3B0 (the only slot of the bank vtable 0x012B9590, stored by the constructor
//     0x00C6A280) calls this, and the reverb destructor 0x00C6A780 calls it for its +0x94 member;
//   - the body destroys exactly the array the constructor 0x00C6A280 builds (+4, four 0x28-byte CReverbFilterTaps).
//   The landed unattributed CKrakenAttacks is a propagated label; descriptive names (libpk: no PDB/RTTI).
// Shape: no store of 0x012B9590 because the destructor is IMPLICIT (a compiler-generated destructor does not reset the
//   vptr); the class is not `novtable`, its constructor stores the vtable. The inline constructor and the factory below
//   emit the vtable and with it the `??_G` that references this COMDAT.
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
    CReverbTapBank() { }                                // out of line at 0x00C6A280
    CReverbFilterTap m_taps[4];                         // +0x04
    unsigned int     m_channels;                        // +0xa4
    unsigned int     m_firstIndex;                      // +0xa8
    unsigned int     m_endIndex;                        // +0xac
};

CReverbTapBank* Create_CReverbTapBank()
{
    return new CReverbTapBank;
}
