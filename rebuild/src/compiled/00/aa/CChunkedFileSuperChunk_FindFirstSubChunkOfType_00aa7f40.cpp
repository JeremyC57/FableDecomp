// CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char* type) const -- returns (by value) the first known sub-chunk whose four-character code equals the type, remembering its index
// Leaf: ?FindFirstSubChunkOfType@CChunkedFileSuperChunk@@QBE?BV?$CCountedPointer@VCChunkedFileChunk@@@@PBD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA7F40, 176 bytes (`ret 8`: hidden result pointer + type). A byte-identical copy sits at 0x00AA80E0
//   (the non-const overload ?FindFirstSubChunkOfType@CChunkedFileSuperChunk@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@PBD@Z).
// Identity: FableWin.pdb ?FindFirstSubChunkOfType@CChunkedFileSuperChunk@@QBE?BV?$CCountedPointer@VCChunkedFileChunk@@@@PBD@Z
//   (engine/CChunkedFileSuperChunk.h); 40 landed callers bind it.
// Callees: each chunk's virtual IsUnknownType (slot 4); strcmp is the VC7.1 intrinsic; GetChunkType (FableWin, inline).
// Shape: indexed loop over the STLport SubChunks vector (+0x10/+0x14, 8-byte counted pointers); the size `(end-begin)>>3` is re-read every
//   iteration because the body calls out; the match is copied out into the result (count bumped) and its index stored in
//   CurrentChunkIndex (+0x28); a miss returns a null counted pointer.
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILECHUNK_GETCHUNKTYPE
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_SUBCHUNKS_TYPED
#include <string.h>
#pragma intrinsic(strcmp)
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CChunkedFileChunk.h"
#include "engine/CChunkedFileSuperChunk.h"

const CCountedPointer<CChunkedFileChunk> CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char* type) const
{
    for (unsigned long i = 0; i < SubChunks.size(); ++i) {
        const CCountedPointer<CChunkedFileChunk>& chunk = SubChunks[i];
        if (!chunk->IsUnknownType() && strcmp(type, chunk->GetChunkType()) == 0) {
            CurrentChunkIndex = i;
            return chunk;
        }
    }
    return CCountedPointer<CChunkedFileChunk>();
}
