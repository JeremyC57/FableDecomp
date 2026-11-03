// CXMVPlayerBuffers::~CXMVPlayerBuffers() -- the XMV movie player's buffer holder: empty virtual destructor, own vtable then the base CBaseClassNonCopyable destructor (tail jump)
// Leaf: ??1CXMVPlayerBuffers@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BC8F0, 11 bytes: `mov dword ptr [ecx],offset ??_7CXMVPlayerBuffers@@6B@ ; jmp ??1CBaseClassNonCopyable@@MAE@XZ`.
// Identity: the stored vtable 0x01230C34 is RTTI .?AVCXMVPlayerBuffers@@ (rtti-vtables.tsv); its slot-0 deleting destructor
//   0x0042DB60 calls this row. FableWin.pdb / Ego_r.pdb: ??1CXMVPlayerBuffers@@UAE@XZ (the class's only destructor).
//   Callee: CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300 (??1CBaseClassNonCopyable@@MAE@XZ).
//   The catalog label "CChunkedFileChunk::~CChunkedFileChunk" is propagated across the byte-twin shape.
// Shape: retail's PC body holds no buffer release (the PDBs' private FreeBuffers leaves no code here): an empty user
//   destructor stores the class vtable and tail-jumps to the base destructor. Layout as the landed constructor 0x0042DB40
//   (Ego_r.pdb): vptr +0, InputBuffer +4, PlaybackBuffer +8, CurrentSize +0xC; size 0x10.
#include "engine/CBaseClassNonCopyable.h"   // ??1CBaseClassNonCopyable@@MAE@XZ 0x0099A300

class CXMVPlayerBuffers : public CBaseClassNonCopyable {
public:
    char* InputBuffer;                      // +0x04
    char* PlaybackBuffer;                   // +0x08
    unsigned long CurrentSize;              // +0x0C
    CXMVPlayerBuffers();                    // 0x0042DB40
    virtual ~CXMVPlayerBuffers();
};

CXMVPlayerBuffers::~CXMVPlayerBuffers()
{
}