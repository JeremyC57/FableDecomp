// Leaf: ?GetChunkDescription@C3DMeshFileTrianglesChunk@@UBE?BVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/C3DMeshFileTrianglesChunk.h (class-model switch FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK; was ?GetChunkDescription@C3DMeshFileTrianglesChunk_Methods@@QBE?AVCCharString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent C3DMeshFileTrianglesChunk_GetCount_00aba370.cpp.
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK_ACCESSORS
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "engine/CArray.h"
#include "engine/C3DTriangle2.h"
#include "engine/C3DMeshFileTrianglesChunk.h"
const CCharString C3DMeshFileTrianglesChunk::GetChunkDescription() const
{
    return CCharString("Triangle List", -1);
}
