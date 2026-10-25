// CReverbDiffuser::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor (0x00C6A340), then the CRT's operator delete (0x00BFE9BC) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCReverbDiffuser@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75A00, 28 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0x12BF080 = CReverbDiffuser"): slot 0 of agent444's CReverbDiffuser
//   vtable 0x012BF080 (stored by CReverbDiffuser::CReverbDiffuser 0x00C75980); it calls the compiler-generated
//   CReverbDiffuser::~CReverbDiffuser 0x00C6A340. The reverb parts have no class allocator (the CRT's 0x00BFE9BC, as
//   CDspGain's 0x00C696A0). The landed CLandscapeBackgroundPatch vector deleting destructor is a propagated label over a
//   placeholder; descriptive names (libpk: no PDB/RTTI).
// Shape: the compiler-generated `??_G`, emitted with the vtable by the inline constructor and the factory below; the
//   implicit destructor (two `eh vector` loops) stays an out-of-line call.
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
