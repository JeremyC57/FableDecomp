// CChunkedFileSuperChunk::FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous) const -- returns (by value) the next known sub-chunk after `previous` with the same four-character code, remembering its index
// Leaf: ?FindNextSubChunkOfType@CChunkedFileSuperChunk@@QBE?BV?$CCountedPointer@VCChunkedFileChunk@@@@ABV?$CCountedPointer@$$CBVCChunkedFileChunk@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA7FF0, 240 bytes (`ret 8`: hidden result pointer + previous). A byte-identical copy sits at
//   0x00AA8190 (the non-const overload).
// Identity: FableWin.pdb ?FindNextSubChunkOfType@CChunkedFileSuperChunk@@QBE?BV?$CCountedPointer@VCChunkedFileChunk@@@@ABV?$CCountedPointer@$$CBVCChunkedFileChunk@@@@@Z
//   (engine/CChunkedFileSuperChunk.h); 19 landed callers bind it.
// Callees: each chunk's virtual IsUnknownType (slot 4); strcmp is the VC7.1 intrinsic; GetChunkType (FableWin, inline).
// Shape: the size is cached once (ebx). `previous` is located fast through CurrentChunkIndex, else by a linear pointer scan; the type is
//   previous->GetChunkType() (kept in a frame slot across the virtual call); the scan resumes one past it. Match -> index stored at +0x28
//   and the element copied out (count bumped); a miss returns a null counted pointer.
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILECHUNK_GETCHUNKTYPE
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_SUBCHUNKS_TYPED
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_FINDNEXT_CONST
#include <string.h>
#pragma intrinsic(strcmp)
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CChunkedFileChunk.h"
#include "engine/CChunkedFileSuperChunk.h"

const CCountedPointer<CChunkedFileChunk> CChunkedFileSuperChunk::FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous) const
{
    unsigned long count = SubChunks.size();
    unsigned long i;
    if (CurrentChunkIndex < count && SubChunks[CurrentChunkIndex].Get() == previous.Get()) {
        i = CurrentChunkIndex;
    } else {
        for (i = 0; i < count; ++i)
            if (SubChunks[i].Get() == previous.Get())
                break;
    }
    const char* type = previous->GetChunkType();
    for (++i; i < count; ++i) {
        const CCountedPointer<CChunkedFileChunk>& chunk = SubChunks[i];
        if (!chunk->IsUnknownType() && strcmp(type, chunk->GetChunkType()) == 0) {
            CurrentChunkIndex = i;
            return chunk;
        }
    }
    return CCountedPointer<CChunkedFileChunk>();
}
