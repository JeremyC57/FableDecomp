// CReverbPreDelay::~CReverbPreDelay() -- destructor of the libpk reverb's pre-delay: its vtable (0x012B9580), then the CVtable_012bf050 base destructor (0x00C74450, a tail jump)
// Leaf: ??1CReverbPreDelay@@UAE@XZ
// Naming wave 31 (family libpk): the reverb DSP base (vtable 0x012BF050; ctor 0x00C74420, virtual dtor 0x00C74450) is CVtable_012bf050 (DECISIONS_W30 D7:
//   libpk has no PDB / RTTI, vtable-address convention; was the descriptive CDspObject, ??1CDspObject@@UAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C697C0, 11 bytes (+ int3 padding), thiscall.
// Retail bytes:  mov dword ptr [ecx],012B9580h ; jmp CVtable_012bf050::~CVtable_012bf050
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444 "the libpk reverb (vtable 0x12B9594)" parts):
//   - 0x012B9580 is agent444's CReverbPreDelay vtable: its only slot, the scalar deleting destructor 0x00C69800, calls this;
//     the reverb constructor's unwind funclets 0x01216601 / 0x01216691 destroy the +0x1e0 pre-delay with it; the reverb
//     destructor 0x00C6A780 inlines the same two steps.
//   The landed CChunkedFileChunk::~CChunkedFileChunk name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI), agent444's classes.
// Shape: a user-defined empty destructor (virtual through CVtable_012bf050): the vtable store, then the out-of-line base
//   destructor as a tail jump.
class CVtable_012bf050 {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CVtable_012bf050();                                       // 0x00C74420 (shared kernel table: create or add a reference)
    virtual ~CVtable_012bf050();                              // 0x00C74450 (drop the reference)
};

class CReverbPreDelay : public CVtable_012bf050 { // vtable 0x012B9580, 0x18 bytes
public:
    ~CReverbPreDelay();
    unsigned char _body[0x14];                          // +0x04
};

CReverbPreDelay::~CReverbPreDelay()
{
}
