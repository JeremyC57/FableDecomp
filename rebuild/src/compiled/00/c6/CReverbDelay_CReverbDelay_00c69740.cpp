// CReverbDelay::~CReverbDelay() -- destructor of the libpk reverb's late delay: its vtable (0x012B957C), then the CDspObject base destructor (0x00C74450, a tail jump)
// Leaf: ??1CReverbDelay@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69740, 11 bytes (+ int3 padding), thiscall.
// Retail bytes:  mov dword ptr [ecx],012B957Ch ; jmp CDspObject::~CDspObject
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444 "the libpk reverb (vtable 0x12B9594)" parts):
//   - 0x012B957C is the vtable of the landed CReverbDelay rows (agent444's CReverbDelay::Process 0x00C76B60): its only slot,
//     the scalar deleting destructor 0x00C69780, calls this; the reverb constructor's unwind funclets 0x01216627 /
//     0x012166B7 destroy the +0x248 late delay with it; the reverb destructor 0x00C6A780 inlines the same two steps.
//   The landed CChunkedFileChunk::~CChunkedFileChunk name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI), agent444's classes.
// Shape: a user-defined empty destructor (virtual through CDspObject): the vtable store, then the out-of-line base
//   destructor as a tail jump.
class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420 (shared kernel table: create or add a reference)
    virtual ~CDspObject();                              // 0x00C74450 (drop the reference)
};

class CReverbDelay : public CDspObject { // vtable 0x012B957C, 0x14 bytes
public:
    ~CReverbDelay();
    unsigned char _body[0x10];                          // +0x04 delay ring and read offset (agent444 layout)
};

CReverbDelay::~CReverbDelay()
{
}
