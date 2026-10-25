// CDspGain::~CDspGain() -- destructor of the libpk reverb gain stage: its vtable (0x012B9578), then the CDspObject base destructor (0x00C74450, a tail jump)
// Leaf: ??1CDspGain@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69680, 11 bytes (+ int3 padding), thiscall.
// Retail bytes:  mov dword ptr [ecx],012B9578h ; jmp CDspObject::~CDspObject
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444 "the libpk reverb (vtable 0x12B9594)" parts):
//   - 0x012B9578 is the CDspGain vtable (the CDspGain::CDspGain rename 0x00C69660; agent444's landed Apply 0x00C74CE0):
//     its scalar deleting destructor 0x00C696A0 calls this, and the CReverbDiffuser constructor 0x00C75980 passes this
//     with the CDspGain constructor to `__ehvec_ctor` for its three gains (0x00C759A2); the reverb part constructors'
//     unwind funclets (0x012165C9..) call it too.
//   The landed CChunkedFileChunk::~CChunkedFileChunk name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI), agent444's classes.
// Shape: a user-defined empty destructor (virtual through CDspObject): the vtable store, then the out-of-line base
//   destructor as a tail jump.
class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420 (shared kernel table: create or add a reference)
    virtual ~CDspObject();                              // 0x00C74450 (drop the reference)
};

class CDspGain : public CDspObject { // vtable 0x012B9578, 0x14 bytes
public:
    ~CDspGain();
    float m_gain[4];                                    // +0x04 running gains
};

CDspGain::~CDspGain()
{
}
