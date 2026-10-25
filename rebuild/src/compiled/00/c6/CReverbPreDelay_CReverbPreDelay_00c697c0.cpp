// CReverbPreDelay::~CReverbPreDelay() -- destructor of the libpk reverb's pre-delay: its vtable (0x012B9580), then the CDspObject base destructor (0x00C74450, a tail jump)
// Leaf: ??1CReverbPreDelay@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C697C0, 11 bytes (+ int3 padding), thiscall.
// Retail bytes:  mov dword ptr [ecx],012B9580h ; jmp CDspObject::~CDspObject
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444 "the libpk reverb (vtable 0x12B9594)" parts):
//   - 0x012B9580 is agent444's CReverbPreDelay vtable: its only slot, the scalar deleting destructor 0x00C69800, calls this;
//     the reverb constructor's unwind funclets 0x01216601 / 0x01216691 destroy the +0x1e0 pre-delay with it; the reverb
//     destructor 0x00C6A780 inlines the same two steps.
//   The landed CChunkedFileChunk::~CChunkedFileChunk name is a propagated manifest label; descriptive names (libpk: no PDB/RTTI), agent444's classes.
// Shape: a user-defined empty destructor (virtual through CDspObject): the vtable store, then the out-of-line base
//   destructor as a tail jump.
class CDspObject {                                      // vtable 0x012BF050: the reverb DSP parts' base
public:
    CDspObject();                                       // 0x00C74420 (shared kernel table: create or add a reference)
    virtual ~CDspObject();                              // 0x00C74450 (drop the reference)
};

class CReverbPreDelay : public CDspObject { // vtable 0x012B9580, 0x18 bytes
public:
    ~CReverbPreDelay();
    unsigned char _body[0x14];                          // +0x04
};

CReverbPreDelay::~CReverbPreDelay()
{
}
