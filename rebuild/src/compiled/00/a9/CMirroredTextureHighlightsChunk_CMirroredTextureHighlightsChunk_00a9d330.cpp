// CMirroredTextureHighlightsChunk::~CMirroredTextureHighlightsChunk() -- the user destructor (empty body): reset the class vtable, free the TriangleVertexIndexList block, then ~CChunkedFileChunk
// Leaf: ??1CMirroredTextureHighlightsChunk@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9D330, 33 bytes, thiscall, tail `jmp ~CChunkedFileChunk`.
// Identity (proven): vtable 0x0129E0E0 stored first (RTTI .?AVCMirroredTextureHighlightsChunk@@ : CChunkedFileChunk :
//   CBaseClassNonCopyable : CBaseClass); its slot-0 `??_G` 0x00A9D360 (landed, the same out-of-line `{}` model) inlines this
//   body. FableWin.pdb ??1CMirroredTextureHighlightsChunk@@UAE@XZ (0x033D3F70) sits between the class's ctor and
//   GetChunkDescription, as retail's copy sits after GetChunkDescription 0x00A9D310 and before the ??_G. Ego_r.pdb member
//   TriangleVertexIndexList (CArray<unsigned long>) at +0x10. No callers (the ??_G has its own copy). The catalog label
//   "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14 (~_Vector_base's deallocate), CChunkedFileChunk::~CChunkedFileChunk 0x00AA75C0.
// Shape: the landed ??_G 0x00A9D360 model: an empty destructor defined out of line in this file (vtable store), the member
//   CArray's STLport destructor, then the base destructor as the tail jump.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
#include "engine/CArray.h"

#include "engine/CBaseClassNonCopyable.h"           // ~CBaseClassNonCopyable 0x0099A300 (protected virtual)

class CChunkedFileSuperChunk;
class CChunkedFileChunk : public CBaseClassNonCopyable {   // vtable 0x0129E474, 0x10 bytes
public:
    virtual ~CChunkedFileChunk();                           // 0x00AA75C0
    char ChunkType[5];                                      // +0x04
    CChunkedFileSuperChunk* Parent;                         // +0x0c
};
class CMirroredTextureHighlightsChunk : public CChunkedFileChunk {   // vtable 0x0129E0E0, 0x1c bytes
public:
    CMirroredTextureHighlightsChunk();                      // 0x00A9D2E0 (CChunkedFileChunk ctor 0x00AA7590 + the zeroed array)
    virtual ~CMirroredTextureHighlightsChunk();
    CArray<unsigned long> TriangleVertexIndexList;          // +0x10
};

CMirroredTextureHighlightsChunk::~CMirroredTextureHighlightsChunk()
{
}