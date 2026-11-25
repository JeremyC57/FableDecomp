// Leaf: ?GetChunkInfo@CChunkedFileChunk@@UBE?BVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CChunkedFileChunk.h (class-model switch FABLE_RETAIL_CCHUNKEDFILECHUNK; was ?GetChunkInfo@CChunkedFileChunk_Methods@@QBE?AVCCharString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#include "engine/CChunkedFileChunk.h"  // retyped onto the PDB layout; byte parity re-verified

const CCharString CChunkedFileChunk::GetChunkInfo() const
{
    return CCharString("", -1);
}