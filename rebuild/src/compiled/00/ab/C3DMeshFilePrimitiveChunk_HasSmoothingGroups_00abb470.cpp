// C3DMeshFilePrimitiveChunk::HasSmoothingGroups() const -- whether the chunk has a "SMTH" sub chunk
// Leaf: ?HasSmoothingGroups@C3DMeshFilePrimitiveChunk@@QBE_NXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00abb470, 118 bytes, thiscall, plain `ret`.
// Identity (probable): unbound (manifest label propagated); it tests for the "SMTH" sub chunk; Ego_r.pdb has ?HasSmoothingGroups@C3DMeshFilePrimitiveChunk@@QBE_NXZ (Ego_r 0x00C0FFE0) just before ?HasUniqueVertices@ (0x00C10190), the same order and spacing class as retail 0x00ABB470 / 0x00ABB610; no direct callers found.
// Callees: CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char*) const 0x00AA7F40 (returns a const CCountedPointer by
//   value; the tag string at 0x0129EB04), operator delete 0x00BFE9BC (the temporary's and the local's releases).
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
class C3DMeshFilePrimitiveChunk : public CChunkedFileSuperChunk { public:
    bool HasSmoothingGroups() const;
};

bool C3DMeshFilePrimitiveChunk::HasSmoothingGroups() const
{
    CCountedPointer<CChunkedFileChunk> chunk;
    chunk = FindFirstSubChunkOfType("SMTH");
    return chunk.Data != 0;
}