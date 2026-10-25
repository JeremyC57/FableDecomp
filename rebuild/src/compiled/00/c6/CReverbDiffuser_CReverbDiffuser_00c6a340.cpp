// CReverbDiffuser::~CReverbDiffuser() -- the compiler-generated destructor of the libpk reverb's diffuser: its two CReverbDiffuserStages (`__ehvec_dtor`, +0x40, element 0x28 bytes, ~CReverbDiffuserStage 0x00C69990; EH state 1), its three CDspGains (+4, element 0x14 bytes, ~CDspGain 0x00C69680; state 0), then the CDspObject base (0x00C74450)
// Leaf: ??1CReverbDiffuser@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6A340, 110 bytes, thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0x12BF080 = CReverbDiffuser"):
//   - agent444's CReverbDiffuser scalar deleting destructor 0x00C75A00 (slot 0 of 0x012BF080, stored by the constructor
//     0x00C75980) calls this, and the reverb destructor 0x00C6A780 calls it for its +0x144 member;
//   - the body destroys exactly the arrays the constructor 0x00C75980 builds, in reverse order.
//   The landed unattributed CKrakenData is a propagated label; descriptive names (libpk: no PDB/RTTI).
// Shape: no store of 0x012BF080 because the destructor is IMPLICIT (a compiler-generated destructor does not reset the
//   vptr); the class is not `novtable`. The inline constructor and the factory below emit the vtable and `??_G`.
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
    float*       m_data;                                // +0x04
    unsigned int m_size;                                // +0x08
    unsigned int m_pos;                                 // +0x0c
    CDspGain     m_gain;                                // +0x10
    unsigned int m_index;                               // +0x24
};

struct CReverbConfig;

class CReverbDiffuser : public CDspObject {             // vtable 0x012BF080, 0x9c bytes (destructor compiler-generated)
public:
    CReverbDiffuser() : m_pConfig(0) { }                // out of line at 0x00C75980
    CDspGain             m_gains[3];                    // +0x04
    CReverbDiffuserStage m_stages[2];                   // +0x40
    CReverbConfig*       m_pConfig;                     // +0x90
    unsigned int         m_firstIndex;                  // +0x94
    unsigned int         m_endIndex;                    // +0x98
};

CReverbDiffuser* Create_CReverbDiffuser()
{
    return new CReverbDiffuser;
}
