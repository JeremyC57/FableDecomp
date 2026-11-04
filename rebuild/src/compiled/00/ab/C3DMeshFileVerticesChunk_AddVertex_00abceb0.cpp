// C3DMeshFileVerticesChunk::AddVertex(const C3DVertex2& vertex) -- append one vertex to the mesh-file vertices chunk
// Leaf: ?AddVertex@C3DMeshFileVerticesChunk@@QAEXABVC3DVertex2@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABCEB0, 8 bytes: `add ecx,10h ; jmp 0x00ABED70` (VertexList.push_back(vertex), tail call).
// Identity (two sources): (1) Ego_r.pdb ?AddVertex@C3DMeshFileVerticesChunk@@QAEXABVC3DVertex2@@@Z with VertexList (CArray<C3DVertex2>) the
//   Ego_r member at +0x10, and the target 0x00ABED70 is a vector push_back of 0x20-byte elements (sizeof C3DVertex2: Position, Normal,
//   U, V); (2) its only caller, COptimisedPrimitive::CopyTo 0x00ACC6B3, assembles a 0x20-byte vertex (position, normal, uv) in its
//   frame and passes it here for each output vertex. It sits among the other mesh-file chunk bodies (C3DMeshFileGroupChunk::AddVertex
//   0x00ABCC20, C3DMeshFileTrianglesChunk::AddTriangle 0x00ABCEC0). The catalog's "_Cons_val<..CParticleMorphs..>" was propagated.
// Callees: ?push_back@?$vector@VC3DVertex2@@V?$allocator@VC3DVertex2@@@std@@@std@@QAEXABVC3DVertex2@@@Z 0x00ABED70 (STLport push_back,
//   out of line in this TU: FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE; the catalog's "CRetailVector_SRecord32::PushBack" stand-in).
// Header: engine/C3DMeshFileVerticesChunk.h (FABLE_RETAIL_C3DMESHFILEVERTICESCHUNK new), engine/CChunkedFileChunk.h
//   (FABLE_RETAIL_CCHUNKEDFILECHUNK), engine/CArray.h, engine/C3DVertex2.h, stlport/fable_containers.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_C3DMESHFILEVERTICESCHUNK
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "fable_string.h"
#include "engine/C3DMeshFileVerticesChunk.h"
void C3DMeshFileVerticesChunk::AddVertex(const C3DVertex2& vertex)
{
    VertexList.push_back(vertex);
}