// CReverbFilterTap::CReverbFilterTap() -- EH-framed constructor of one filtered tap of the libpk reverb's tap bank: the CDspObject base (0x00C74420, EH state 0), the tap vtable 0x012B958C, then its CDspGain member at +4 (inline: a second CDspObject construction and the gain vtable 0x012B9578)
// Leaf: ??0CReverbFilterTap@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C698D0, 84 bytes, thiscall, returns `this`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC698D0/0xC75530 = CReverbFilterTap/CReverbDiffuserStage ctors"):
//   - 0x012B958C is the vtable of agent444's CReverbFilterTap (the four 0x28-byte taps at +4 of CReverbTapBank, vtable
//     0x012B9590: agent444's landed CReverbTapBank::Allocate 0x00C75C30 / Reset 0x00C75CA0 / Process 0x00C75E90): slot 0
//     is the scalar deleting destructor 0x00C69A10 around the destructor 0x00C69930;
//   - the tap bank's constructor 0x00C6A280 passes this to `__ehvec_ctor` with that destructor (0x00C6A2A3 / 0x00C6A2A8);
//   - 0x00C74420 is agent444's landed CDspObject::CDspObject; the +4 member is a CDspGain (vtable 0x012B9578, its
//     out-of-line constructor 0x00C69660 is the same two stores).
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

class CReverbFilterTap : public CDspObject {            // vtable 0x012B958C, 0x28 bytes
public:
    CReverbFilterTap();                                 // 0x00C698D0
    CDspGain     m_gain;                                // +0x04
    float*       m_data;                                // +0x18 delay memory (from the reverb's pool)
    unsigned int m_size;                                // +0x1c
    unsigned int m_pos;                                 // +0x20
    unsigned int m_index;                               // +0x24 line index
};

CReverbFilterTap::CReverbFilterTap()
{
}
