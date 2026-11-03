// CChunkedFileChunk::~CChunkedFileChunk() -- the chunked-file chunk's empty virtual destructor: own vtable, then the base CBaseClassNonCopyable destructor (tail jump)
// Leaf: ??1CChunkedFileChunk@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA75C0, 11 bytes: `mov dword ptr [ecx],offset ??_7CChunkedFileChunk@@6B@ ; jmp ??1CBaseClassNonCopyable@@MAE@XZ`.
// Identity: the stored vtable 0x0129E474 is RTTI .?AVCChunkedFileChunk@@ (rtti-vtables.tsv); its slot-0 deleting
//   destructor 0x00AA8D50 inlines the same two steps. Ego_r.pdb / FableWin.pdb: ??1CChunkedFileChunk@@UAE@XZ (public
//   virtual, the class's only destructor). Callee: CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300
//   (??1CBaseClassNonCopyable@@MAE@XZ, protected virtual, both PDBs).
// Shape: an empty user destructor of a derived polymorphic class stores its own vtable and tail-jumps to the base
//   destructor (no members with destructors: ChunkType char[5], Parent pointer).
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#include "engine/CChunkedFileChunk.h"

CChunkedFileChunk::~CChunkedFileChunk()
{
}