// Leaf: ?GetChunkInfo@C3DMeshFileClothChunk@@UBE?BVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/C3DMeshFileClothChunk.h (class-model switch FABLE_RETAIL_C3DMESHFILECLOTHCHUNK; was ?GetChunkInfo@C3DMeshFileClothChunk_Methods@@QAE?AVCCharString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the switches engine/C3DMeshFileClothChunk.h names (CChunkedFileChunk / CChunkedFileSuperChunk retail branches); a larger
//   precedent preamble (C3DPrimitive2_InitialiseFromMeshFileData_00a91640) added STLport /alternatename lines to .drectve.
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILECLOTHCHUNK
#include "fable_string.h"
#include "engine/C3DMeshFileClothChunk.h"

const CCharString C3DMeshFileClothChunk::GetChunkInfo() const
{
    return CCharString("", -1);
}
