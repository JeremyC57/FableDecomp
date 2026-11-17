// CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char* type) -- returns (by value) the first known sub-chunk whose four-character code equals the type, remembering its index
// Leaf: ?FindFirstSubChunkOfType@CChunkedFileSuperChunk@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@PBD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA80E0, 176 bytes (`ret 8`: hidden result pointer + type). Byte-identical to 0x00AA7F40 (a second emitted copy).
// Identity (agentNF57 name fix; was the stand-in ?FindFirstSubChunkOfType@CChunkedFileSuperChunk@@QAE?AUCChunkPointer@@PBD@Z on a local
//   CChunkedFileChunk / CChunkPointer model): the non-const overload ?FindFirstSubChunkOfType@CChunkedFileSuperChunk@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@PBD@Z,
//   Ego_r.pdb 0x00C1C730 / FableWin.pdb 0x0346AA50. Both PDBs list const First, const Next, non-const First, non-const Next, which is the
//   retail order 0x00AA7F40 / 0x00AA7FF0 / 0x00AA80E0 / 0x00AA8190; the landed non-const callers (C3DAnimFileRootChunk::FindFirstObject
//   0x00A98FA0 and the other C3DMeshFile* / C3DAnimFile* FindFirst* rows) already bind this spelling.
// Callees: each chunk's virtual IsUnknownType (slot 4); strcmp is the VC7.1 intrinsic; GetChunkType (FableWin, inline).
// Shape: the const twin 0x00AA7F40's body on the shared engine/CChunkedFileSuperChunk.h (retail SubChunks +0x10, CurrentChunkIndex +0x28):
//   indexed loop with the size re-read each pass, the match copied out into the result and its index stored; a miss returns a null pointer.
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

CCountedPointer<CChunkedFileChunk> CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char* type)
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
