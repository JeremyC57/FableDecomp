// CChunkedFileSuperChunk::FindNextSubChunkOfType(const CCountedPointer<CChunkedFileChunk>&) -- returns (by value) the next known sub-chunk after `previous` with the same four-character code, remembering its index
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA8190, 240 bytes (`ret 8`: hidden result pointer + previous). Byte-identical to 0x00AA7FF0 (a second emitted copy).
// Callees: each chunk's virtual IsUnknownType (slot 4); strcmp is the VC7.1 intrinsic.
// Shape: the size is cached once (ebx). `previous` is located fast through CurrentChunkIndex, else by a linear
//          pointer scan; the type is previous->ChunkType (esi, kept in a frame slot across the virtual call);
//          the scan resumes one past it. Match -> index stored at +0x28 and the element copied out (count bumped);
//          a miss returns a null counted pointer.
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
    CChunkPointer FindNextSubChunkOfType(const CChunkPointer& previous);
};

CChunkPointer CChunkedFileSuperChunk::FindNextSubChunkOfType(const CChunkPointer& previous)
{
    unsigned long count = SubChunks.size();
    unsigned long i;
    if (CurrentChunkIndex < count && SubChunks[CurrentChunkIndex].Object == previous.Object) {
        i = CurrentChunkIndex;
    } else {
        for (i = 0; i < count; ++i)
            if (SubChunks[i].Object == previous.Object)
                break;
    }
    const char* type = previous->ChunkType;
    for (++i; i < count; ++i) {
        CChunkPointer& chunk = SubChunks[i];
        if (!chunk->IsUnknownType() && strcmp(type, chunk->ChunkType) == 0) {
            CurrentChunkIndex = i;
            return chunk;
        }
    }
    return CChunkPointer();
}