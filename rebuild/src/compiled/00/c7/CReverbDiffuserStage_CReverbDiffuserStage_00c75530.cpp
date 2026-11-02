// CReverbDiffuserStage::CReverbDiffuserStage() -- EH-framed constructor of one all-pass stage of the libpk reverb's diffuser: the CVtable_012bf050 base (0x00C74420, EH state 0), the stage vtable 0x012BF07C, then its CDspGain member at +0x10 (inline: a second CVtable_012bf050 construction and the gain vtable 0x012B9578)
// Leaf: ??0CReverbDiffuserStage@@QAE@XZ
// Naming wave 31 (family libpk): the reverb DSP base (vtable 0x012BF050; ctor 0x00C74420, virtual dtor 0x00C74450) is CVtable_012bf050 (DECISIONS_W30 D7:
//   libpk has no PDB / RTTI, vtable-address convention; was the descriptive CDspObject, ??1CDspObject@@UAE@XZ)
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75530, 84 bytes, thiscall, returns `this`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC698D0/0xC75530 = CReverbFilterTap/CReverbDiffuserStage ctors"):
//   - the diffuser constructor 0x00C75980 passes this with the destructor 0x00C69990 to `__ehvec_ctor` for its two 0x28-byte
//     stages at +0x40; agent444's landed CReverbDiffuser::Reset 0x00C75670 / Process 0x00C75A20 work on the same stages
//     (delay memory +4, size +8, position +0xc, CDspGain +0x10, line index +0x24);
//   - 0x012BF07C's only slot is the scalar deleting destructor 0x00C75590 (around 0x00C69990).
//   The landed CAIStateGroupBehaviour name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI).
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

class CReverbDiffuserStage : public CVtable_012bf050 {        // vtable 0x012BF07C, 0x28 bytes
public:
    CReverbDiffuserStage();                             // 0x00C75530
    float*       m_data;                                // +0x04 delay memory (from the reverb's pool)
    unsigned int m_size;                                // +0x08
    unsigned int m_pos;                                 // +0x0c
    CDspGain     m_gain;                                // +0x10
    unsigned int m_index;                               // +0x24 line index
};

CReverbDiffuserStage::CReverbDiffuserStage()
{
}
