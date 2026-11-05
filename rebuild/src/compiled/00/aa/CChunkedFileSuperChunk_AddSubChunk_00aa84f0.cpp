// CChunkedFileSuperChunk::AddSubChunk(const CCountedPointer<CChunkedFileChunk>& chunk) -- parent a chunk to this super chunk and append a counted reference to it to SubChunks
// Leaf: ?AddSubChunk@CChunkedFileSuperChunk@@QAEXABV?$CCountedPointer@VCChunkedFileChunk@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA84F0, 69 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb `?AddSubChunk@CChunkedFileSuperChunk@@QAEXABV?$CCountedPointer@VCChunkedFileChunk@@@@@Z`; the landed
//   COptimisedMesh::CopyTo 0x00ACD5A0 already declares 0x00AA84F0 as AddSubChunk, and the other callers
//   (CEngineWorldMap::CreateNewBankMap 0x00AC0C00, COptimisedPrimitive::CopyTo 0x00ACC490) add mesh-file chunks. The
//   catalog's "vector_push_back" is descriptive.
// Callees: vector<CCountedPointer<CChunkedFileChunk> >::_M_insert_overflow(pos, x, tag, 1, true) 0x00AA94E0.
// Layout: retail CChunkedFileSuperChunk = CChunkedFileChunk {vptr, ChunkType[5] +4, Parent +0xC} + SubChunks +0x10 and
//   RegisteredSubChunkTypes +0x1C as 12-byte STLport vectors (engine/CChunkedFileSuperChunk.h has the 16-byte Ego_r
//   arrays at +0x10/+0x20, so `this` is modelled locally as in the landed siblings 0x00AA7EA0/0x00AA78C0).
//   CCountedPointer = {Data +0, Info +4}, the copy bumps Info->RefCount.
// Shape: the inline private `chunk->SetParent(this)` store first, then STLport `SubChunks.push_back(chunk)` inline: the
//   placement copy under its null test and `add [ecx+4],8`, or the out-of-line overflow with the empty tag homed in the
//   dead `chunk` argument slot.

#include "engine/CCountedPointer.h"


#include "engine/CArray.h"


class CDataInputStream;
class CDataOutputStream;
class CChunkedFileLoader;
class CChunkedFileSaver;
class CChunkedFileSuperChunk;

class CChunkedFileChunk {                               // retail layout = engine/CChunkedFileChunk.h + its vtable
public:
    virtual ~CChunkedFileChunk();
    char                    ChunkType[5];               // +0x04 four-character code + terminator
    CChunkedFileSuperChunk* Parent;                     // +0x0c
    void SetParent(CChunkedFileSuperChunk* parent) { Parent = parent; }
};

class CChunkedFileSuperChunk : public CChunkedFileChunk {
public:
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;                 // +0x10 (Ego_r +0x10, 16-byte debug vector)
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes;   // +0x1c (Ego_r +0x20)
    unsigned long CurrentChunkIndex;                                        // +0x28 (Ego_r +0x30)
    void AddSubChunk(const CCountedPointer<CChunkedFileChunk>& chunk);
    void RegisterSubChunkType(const CCountedPointer<CChunkedFileChunk>& chunk);
};

void CChunkedFileSuperChunk::AddSubChunk(const CCountedPointer<CChunkedFileChunk>& chunk)
{
    chunk->SetParent(this);
    SubChunks.push_back(chunk);
}