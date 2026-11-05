// C3DAnimFileRootChunk::HasHelpers() const -- whether the chunk has a "HLPR" sub chunk
// Leaf: ?HasHelpers@C3DAnimFileRootChunk@@QBE_NXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a99280, 118 bytes, thiscall, plain `ret`.
// Identity (proven): bound ?HasHelpers@C3DAnimFileRootChunk@@QBE_NXZ; callers C3DAnimationStats::InitialiseStats 0x00A4EBB0 (landed) and 0x00A4FE60; the tag "HLPR" is the helpers chunk the landed C3DAnimFileLoader registers under the animation root.
// Callees: CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char*) const 0x00AA7F40 (returns a const CCountedPointer by
//   value; the tag string at 0x0129DFCC), operator delete 0x00BFE9BC (the temporary's and the local's releases).
// Shape (family L113): a default-constructed local assigned from the returned temporary (inline operator= over a known-null
//   block: the pointer is taken only when the block is non-null), the temporary released (block re-read after the
//   decrement), the test, then the local released from registers.
#include "engine/CCountedPointer.h"

class CChunkedFileChunk { public:
    virtual ~CChunkedFileChunk();
    char ChunkType[5];                      // +0x04
};
class CChunkedFileSuperChunk : public CChunkedFileChunk { public:
    const CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* chunkType) const;   // 0x00AA7F40
};
class C3DAnimFileRootChunk : public CChunkedFileSuperChunk { public:
    bool HasHelpers() const;
};

bool C3DAnimFileRootChunk::HasHelpers() const
{
    CCountedPointer<CChunkedFileChunk> chunk;
    chunk = FindFirstSubChunkOfType("HLPR");
    return chunk.Data != 0;
}