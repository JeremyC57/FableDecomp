// CReverbDiffuserStage::~CReverbDiffuserStage() -- the compiler-generated destructor of a reverb diffuser stage: its CDspGain member at +0x10 (inline: the gain vtable 0x012B9578, then CDspObject::~CDspObject 0x00C74450; EH state 0), then the CDspObject base (0x00C74450)
// Leaf: ??1CReverbDiffuserStage@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69990, 82 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC6A650/0xC6A780/0xC69930/0xC69990 (CTCKraken) ... are the libpk
//   reverb (vtable 0x12B9594)"):
//   - the scalar deleting destructor 0x00C75590 (slot 0 of the CReverbDiffuserStage vtable 0x012BF07C, stored by the
//     constructor 0x00C75530) calls this; the diffuser constructor 0x00C75980 passes it to `__ehvec_ctor` as the element
//     destructor of its two stages at +0x40 (with the stage constructor 0x00C75530), its destructor 0x00C6A340 likewise;
//   - the body destroys exactly the member the constructor builds: the CDspGain at +0x10.
//   The landed unattributed CTCKraken (a `novtable` model) is a propagated label; descriptive names (libpk: no PDB/RTTI).
// Shape: no vtable store of 0x012BF07C because the destructor is IMPLICIT (a compiler-generated destructor does not reset
//   the vptr); the factory below emits the vtable and with it the `??_G` that references this COMDAT.
class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420 (shared kernel table: create or add a reference)
    virtual ~CDspObject();                              // 0x00C74450 (drop the reference)
};

class CDspGain : public CDspObject {                    // vtable 0x012B9578, 0x14 bytes
public:
    CDspGain() { }                                      // out of line at 0x00C69660
    ~CDspGain() { }                                     // out of line at 0x00C69680
    float m_gain[4];                                    // +0x04 running gains
};

class CReverbDiffuserStage : public CDspObject {        // vtable 0x012BF07C, 0x28 bytes
public:
    CReverbDiffuserStage() { }                          // out of line at 0x00C75530
    float*       m_data;                                // +0x04 delay memory (from the reverb's pool)
    unsigned int m_size;                                // +0x08
    unsigned int m_pos;                                 // +0x0c
    CDspGain     m_gain;                                // +0x10
    unsigned int m_index;                               // +0x24 line index
};

CReverbDiffuserStage* Create_CReverbDiffuserStage()
{
    return new CReverbDiffuserStage;
}
