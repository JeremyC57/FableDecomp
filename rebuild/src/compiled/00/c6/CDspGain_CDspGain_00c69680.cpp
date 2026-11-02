// CDspGain::~CDspGain() -- destructor of the libpk reverb gain stage: its vtable (0x012B9578), then the CVtable_012bf050 base destructor (0x00C74450, a tail jump)
// Leaf: ??1CDspGain@@UAE@XZ
// Naming wave 31 (family libpk): the reverb DSP base (vtable 0x012BF050; ctor 0x00C74420, virtual dtor 0x00C74450) is CVtable_012bf050 (DECISIONS_W30 D7:
//   libpk has no PDB / RTTI, vtable-address convention; was the descriptive CDspObject, ??1CDspObject@@UAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69680, 11 bytes (+ int3 padding), thiscall.
// Retail bytes:  mov dword ptr [ecx],012B9578h ; jmp CVtable_012bf050::~CVtable_012bf050
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444 "the libpk reverb (vtable 0x12B9594)" parts):
//   - 0x012B9578 is the CDspGain vtable (the CDspGain::CDspGain rename 0x00C69660; agent444's landed Apply 0x00C74CE0):
//     its scalar deleting destructor 0x00C696A0 calls this, and the CReverbDiffuser constructor 0x00C75980 passes this
//     with the CDspGain constructor to `__ehvec_ctor` for its three gains (0x00C759A2); the reverb part constructors'
//     unwind funclets (0x012165C9..) call it too.
//   The landed CChunkedFileChunk::~CChunkedFileChunk name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI), agent444's classes.
// Shape: a user-defined empty destructor (virtual through CVtable_012bf050): the vtable store, then the out-of-line base
//   destructor as a tail jump.
class CVtable_012bf050 {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CVtable_012bf050();                                       // 0x00C74420 (shared kernel table: create or add a reference)
    virtual ~CVtable_012bf050();                              // 0x00C74450 (drop the reference)
};

class CDspGain : public CVtable_012bf050 { // vtable 0x012B9578, 0x14 bytes
public:
    ~CDspGain();
    float m_gain[4];                                    // +0x04 running gains
};

CDspGain::~CDspGain()
{
}
