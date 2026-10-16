// CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char*) -- returns (by value) the first known sub-chunk whose four-character code equals the type, remembering its index
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA7F40, 176 bytes (`ret 8`: hidden result pointer + type). A byte-identical copy sits at 0x00AA80E0.
// Callees: each chunk's virtual IsUnknownType (slot 4); strcmp is the VC7.1 intrinsic.
// Shape: indexed loop over the STLport SubChunks vector (+0x10/+0x14, 8-byte counted pointers); the size
//          `(end-begin)>>3` is re-read every iteration because the body calls out. The element is referenced
//          (no counted copy) while testing; the match is copied out into the result (count bumped) and its index
//          stored in CurrentChunkIndex (+0x28); a miss returns a null counted pointer.
// Retail layout: SubChunks +0x10 (12-byte vector), RegisteredSubChunkTypes +0x1c, CurrentChunkIndex +0x28 -- the
//          generated engine header (0x10-byte CArrays, index at +0x30) disagrees, so `this` is modelled locally
//          (same model as the landed CountSubChunksOfType 0x00AA7EA0).
#include <string.h>
#pragma intrinsic(strcmp)

struct CChunkedFileSuperChunk;

struct CChunkedFileChunk {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual long IsUnknownType() const;             // slot 4
    char ChunkType[5];                              // +0x04 four-character code + terminator
    CChunkedFileSuperChunk* Parent;                 // +0x0c
};

struct SCountBlock {
    long m_references;                              // +0
};

struct CChunkPointer {                              // CCountedPointer<CChunkedFileChunk>
    CChunkedFileChunk* Object;                      // +0
    SCountBlock* Count;                             // +4
    CChunkPointer() : Object(0), Count(0) {}
    CChunkPointer(const CChunkPointer& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    CChunkedFileChunk* operator->() const { return Object; }
};

struct CChunkArray {                                // STLport vector, 12 bytes
    CChunkPointer* m_pFirst;
    CChunkPointer* m_pLast;
    CChunkPointer* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    CChunkPointer& operator[](unsigned long i) const { return m_pFirst[i]; }
};

struct CChunkedFileSuperChunk : CChunkedFileChunk {
    CChunkArray SubChunks;                          // +0x10
    CChunkArray RegisteredSubChunkTypes;            // +0x1c
    unsigned long CurrentChunkIndex;                // +0x28
    CChunkPointer FindFirstSubChunkOfType(const char* type);
};

CChunkPointer CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char* type)
{
    for (unsigned long i = 0; i < SubChunks.size(); ++i) {
        CChunkPointer& chunk = SubChunks[i];
        if (!chunk->IsUnknownType() && strcmp(type, chunk->ChunkType) == 0) {
            CurrentChunkIndex = i;
            return chunk;
        }
    }
    return CChunkPointer();
}