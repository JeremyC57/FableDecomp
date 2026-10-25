// CReverbDiffuserStage::CReverbDiffuserStage() -- EH-framed constructor of one all-pass stage of the libpk reverb's diffuser: the CDspObject base (0x00C74420, EH state 0), the stage vtable 0x012BF07C, then its CDspGain member at +0x10 (inline: a second CDspObject construction and the gain vtable 0x012B9578)
// Leaf: ??0CReverbDiffuserStage@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75530, 84 bytes, thiscall, returns `this`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC698D0/0xC75530 = CReverbFilterTap/CReverbDiffuserStage ctors"):
//   - the diffuser constructor 0x00C75980 passes this with the destructor 0x00C69990 to `__ehvec_ctor` for its two 0x28-byte
//     stages at +0x40; agent444's landed CReverbDiffuser::Reset 0x00C75670 / Process 0x00C75A20 work on the same stages
//     (delay memory +4, size +8, position +0xc, CDspGain +0x10, line index +0x24);
//   - 0x012BF07C's only slot is the scalar deleting destructor 0x00C75590 (around 0x00C69990).
//   The landed CAIStateGroupBehaviour name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI).
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
