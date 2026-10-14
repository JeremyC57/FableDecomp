// CChunkedFileSuperChunk::RemoveSubChunk(CCountedPointer<CChunkedFileChunk>&) -- find the sub-chunk whose
// object matches, detach it (Parent = 0) and erase its slot from the SubChunks array (sibling jump into the
// array's Erase with the iterator written over the incoming argument slot).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA8280, length 56 bytes.
// Callees: CArray<CCountedPointer<CChunkedFileChunk>>::Erase(iterator) @ 0x00AA91F0 (manifest label
// CScriptBase::RemovePredicate is propagated garbage).
// Retail CArray is 12 bytes {Begin, End, Capacity}: the PDB header's 16-byte SubChunks at +0x10 agrees on
// the offset but not the size (everything after +0x1c sits 4 bytes high in the header), hence the local model.
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/CChunkedFileChunk.h"
struct CCountedChunkPointer {                   // CCountedPointer<CChunkedFileChunk>
    CChunkedFileChunk* Object;                  // +0
    void*              Info;                    // +4
};
struct CRetailChunkArray {                      // CArray<CCountedPointer<CChunkedFileChunk>> (retail, 12 bytes)
    CCountedChunkPointer* Begin;                // +0
    CCountedChunkPointer* End;                  // +4
    CCountedChunkPointer* Capacity;             // +8
    void Erase(CCountedChunkPointer* where);    // 0x00AA91F0
};
struct CChunkedFileSuperChunk_Retail {
    unsigned char     _base_0x00[0x10];         // CChunkedFileChunk
    CRetailChunkArray SubChunks;                // +0x10
    CRetailChunkArray RegisteredSubChunkTypes;  // +0x1c
    unsigned long     CurrentChunkIndex;        // +0x28
    void RemoveSubChunk(CCountedChunkPointer& chunk);
};
FABLE_STATIC_ASSERT(offsetof(CChunkedFileSuperChunk_Retail, SubChunks) == offsetof(CChunkedFileSuperChunk, SubChunks));

void CChunkedFileSuperChunk_Retail::RemoveSubChunk(CCountedChunkPointer& chunk)
{
    CCountedChunkPointer* it = SubChunks.Begin;
    CCountedChunkPointer* end = SubChunks.End;
    if (it == end)
        return;
    CChunkedFileChunk* object = chunk.Object;
    do {
        if (it->Object == object) {
            it->Object->Parent = 0;
            SubChunks.Erase(it);
            return;
        }
        ++it;
    } while (it != end);
}