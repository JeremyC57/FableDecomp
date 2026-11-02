// CReverbDelay::~CReverbDelay() -- destructor of the libpk reverb's late delay: its vtable (0x012B957C), then the CVtable_012bf050 base destructor (0x00C74450, a tail jump)
// Leaf: ??1CReverbDelay@@UAE@XZ
// Naming wave 31 (family libpk): the reverb DSP base (vtable 0x012BF050; ctor 0x00C74420, virtual dtor 0x00C74450) is CVtable_012bf050 (DECISIONS_W30 D7:
//   libpk has no PDB / RTTI, vtable-address convention; was the descriptive CDspObject, ??1CDspObject@@UAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69740, 11 bytes (+ int3 padding), thiscall.
// Retail bytes:  mov dword ptr [ecx],012B957Ch ; jmp CVtable_012bf050::~CVtable_012bf050
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444 "the libpk reverb (vtable 0x12B9594)" parts):
//   - 0x012B957C is the vtable of the landed CReverbDelay rows (agent444's CReverbDelay::Process 0x00C76B60): its only slot,
//     the scalar deleting destructor 0x00C69780, calls this; the reverb constructor's unwind funclets 0x01216627 /
//     0x012166B7 destroy the +0x248 late delay with it; the reverb destructor 0x00C6A780 inlines the same two steps.
//   The landed CChunkedFileChunk::~CChunkedFileChunk name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI), agent444's classes.
// Shape: a user-defined empty destructor (virtual through CVtable_012bf050): the vtable store, then the out-of-line base
//   destructor as a tail jump.
class CVtable_012bf050 {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CVtable_012bf050();                                       // 0x00C74420 (shared kernel table: create or add a reference)
    virtual ~CVtable_012bf050();                              // 0x00C74450 (drop the reference)
};

class CReverbDelay : public CVtable_012bf050 { // vtable 0x012B957C, 0x14 bytes
public:
    ~CReverbDelay();
    unsigned char _body[0x10];                          // +0x04 delay ring and read offset (agent444 layout)
};

CReverbDelay::~CReverbDelay()
{
}
