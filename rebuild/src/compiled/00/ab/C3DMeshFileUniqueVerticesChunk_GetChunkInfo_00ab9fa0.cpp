// Leaf: ?GetChunkInfo@C3DMeshFileUniqueVerticesChunk@@UBE?BVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/C3DMeshFileUniqueVerticesChunk.h (class-model switch FABLE_RETAIL_C3DMESHFILEUNIQUEVERTICESCHUNK; was ?GetChunkInfo@C3DMeshFileUniqueVerticesChunk_Methods@@QBE?AVCCharString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_C3DMESHFILEUNIQUEVERTICESCHUNK
#include "engine/C3DMeshFileUniqueVerticesChunk.h"  // retyped onto the PDB layout; byte parity re-verified

const CCharString C3DMeshFileUniqueVerticesChunk::GetChunkInfo() const
{
    return CCharString("", -1);
}