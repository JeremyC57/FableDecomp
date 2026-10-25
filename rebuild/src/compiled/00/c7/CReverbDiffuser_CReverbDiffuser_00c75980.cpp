// CReverbDiffuser::CReverbDiffuser() -- EH-framed constructor of the libpk reverb's diffuser: the CDspObject base (0x00C74420, EH state 0), the diffuser vtable 0x012BF080, three CDspGains at +4 (`__ehvec_ctor`: 0x00C69660 / 0x00C69680, state 1), two CReverbDiffuserStages at +0x40 (`__ehvec_ctor`: 0x00C75530 / 0x00C69990), and no configuration yet (+0x90 = 0)
// Leaf: ??0CReverbDiffuser@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75980, 127 bytes, thiscall, returns `this`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC75980 = CReverbDiffuser ctor"):
//   - 0x012BF080 is agent444's CReverbDiffuser vtable (slot 0: scalar deleting destructor 0x00C75A00 around the
//     destructor 0x00C6A340, freeing with the CRT's 0x00BFE9BC); agent444's landed Reset 0x00C75670 / Process 0x00C75A20
//     use the three gains at +4 and the two stages at +0x40, and the configuration pointer +0x90;
//   - its caller is the reverb constructor 0x00C6A650 (vtable 0x012B9594), for the +0x144 member that Reset 0x00C69C3F
//     resets;
//   - the element constructors and destructors are CDspGain's (0x00C69660 / 0x00C69680) and CReverbDiffuserStage's
//     (0x00C75530 / 0x00C69990).
//   The landed WaypointData name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI).
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

struct CReverbConfig;

class CReverbDiffuser : public CDspObject {             // vtable 0x012BF080, 0x9c bytes
public:
    CReverbDiffuser();
    ~CReverbDiffuser();                                 // 0x00C6A340
    CDspGain             m_gains[3];                    // +0x04
    CReverbDiffuserStage m_stages[2];                   // +0x40
    CReverbConfig*       m_pConfig;                     // +0x90
    unsigned int         m_firstIndex;                  // +0x94
    unsigned int         m_endIndex;                    // +0x98
};

CReverbDiffuser::CReverbDiffuser()
    : m_pConfig(0)
{
}
