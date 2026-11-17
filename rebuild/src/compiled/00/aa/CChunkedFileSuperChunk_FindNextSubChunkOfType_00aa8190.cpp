// CChunkedFileSuperChunk::FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous) -- returns (by value) the next known sub-chunk after previous with the same four-character code, remembering its index
// Leaf: ?FindNextSubChunkOfType@CChunkedFileSuperChunk@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABV?$CCountedPointer@$$CBVCChunkedFileChunk@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA8190, 240 bytes (`ret 8`: hidden result pointer + previous). Byte-identical to 0x00AA7FF0 (a second emitted copy).
// Identity (agentNF57 name fix; was the stand-in ?FindNextSubChunkOfType@CChunkedFileSuperChunk@@QAE?AUCChunkPointer@@ABU2@@Z on a local
//   CChunkedFileChunk / CChunkPointer model): the non-const overload, Ego_r.pdb 0x00C1C7E0 / FableWin.pdb 0x0346AB50
//   ?FindNextSubChunkOfType@CChunkedFileSuperChunk@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABV?$CCountedPointer@$$CBVCChunkedFileChunk@@@@@Z.
//   Both PDBs list const First, const Next, non-const First, non-const Next, which is the retail order 0x00AA7F40 / 0x00AA7FF0 /
//   0x00AA80E0 / 0x00AA8190; the landed non-const callers (C3DAnimFileRootChunk::FindNextObject 0x00A99030 and the other FindNext* rows)
//   already bind this spelling.
// Callees: each chunk's virtual IsUnknownType (slot 4); strcmp is the VC7.1 intrinsic; GetChunkType (FableWin, inline).
// Shape: the const twin 0x00AA7FF0's body on the shared engine/CChunkedFileSuperChunk.h (opt-in FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_FINDNEXTSUBCHUNKOFTYPE
//   declares the non-const overload): the cached index is tried first, else a search for previous; then the scan for the next match.
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILECHUNK_GETCHUNKTYPE
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_SUBCHUNKS_TYPED
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK_FINDNEXTSUBCHUNKOFTYPE
#include <string.h>
#pragma intrinsic(strcmp)
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CChunkedFileChunk.h"
#include "engine/CChunkedFileSuperChunk.h"

CCountedPointer<CChunkedFileChunk> CChunkedFileSuperChunk::FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous)
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
