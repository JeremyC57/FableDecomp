// CReverbFilterTap::~CReverbFilterTap() -- the compiler-generated destructor of a reverb filtered tap: its CDspGain member at +4 (inline: the gain vtable 0x012B9578, then CVtable_012bf050::~CVtable_012bf050 0x00C74450; EH state 0), then the CVtable_012bf050 base (0x00C74450)
// Leaf: ??1CReverbFilterTap@@UAE@XZ
// Naming wave 31 (family libpk): the reverb DSP base (vtable 0x012BF050; ctor 0x00C74420, virtual dtor 0x00C74450) is CVtable_012bf050 (DECISIONS_W30 D7:
//   libpk has no PDB / RTTI, vtable-address convention; was the descriptive CDspObject, ??1CDspObject@@UAE@XZ)
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69930, 82 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC6A650/0xC6A780/0xC69930/0xC69990 (CTCKraken) ... are the libpk
//   reverb (vtable 0x12B9594)"):
//   - the scalar deleting destructor 0x00C69A10 (slot 0 of the CReverbFilterTap vtable 0x012B958C, stored by the
//     constructor 0x00C698D0) calls this; the tap bank's constructor 0x00C6A280 and destructor 0x00C6A2E0 pass it to the
//     `eh vector` iterators as the element destructor of the four taps;
//   - the body destroys exactly the member the constructor builds: the CDspGain at +4 (agent444's layout: gain +4, delay
//     memory +0x18, size +0x1c, position +0x20, line index +0x24).
//   The landed unattributed CTCKraken (a `novtable` model) is a propagated label; descriptive names (libpk: no PDB/RTTI).
// Shape: no vtable store of 0x012B958C because the destructor is IMPLICIT (a compiler-generated destructor does not reset
//   the vptr) -- the class is not `novtable`, its constructor stores the vtable. CDspGain's destructor is a user-declared
//   inline one (it stores 0x012B9578). The factory below emits the vtable and with it the `??_G` that references this COMDAT.
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
    CReverbFilterTap() { }                              // out of line at 0x00C698D0
    CDspGain     m_gain;                                // +0x04
    float*       m_data;                                // +0x18 delay memory (from the reverb's pool)
    unsigned int m_size;                                // +0x1c
    unsigned int m_pos;                                 // +0x20
    unsigned int m_index;                               // +0x24 line index
};

CReverbFilterTap* Create_CReverbFilterTap()
{
    return new CReverbFilterTap;
}
