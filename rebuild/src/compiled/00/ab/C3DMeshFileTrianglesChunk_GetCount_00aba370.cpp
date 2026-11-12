// C3DMeshFileTrianglesChunk::GetCount() const -- the number of triangles in the chunk (TriangleList.size(), 6-byte C3DTriangle2 records at +0x2C / +0x30)
// Leaf: ?GetCount@C3DMeshFileTrianglesChunk@@QBEKXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABA370: (TriangleList end +0x30 - begin +0x2C) / 6, `ret`.
// Name-fix 2026-11-12 (was the stand-in free function CWorldMap_DrawGetNoMaps(CWorldMap*), manifest CWorldMap::DrawGetNoMaps):
//   Ego_r.pdb 00c0ef50 + FableWin.pdb 033dc540 ?GetCount@C3DMeshFileTrianglesChunk@@QBEKXZ; engine/C3DMeshFileTrianglesChunk.h
//   (FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK_ACCESSORS) declares it with TriangleList (CArray<C3DTriangle2>) at +0x2C, and the landed
//   caller N3DPrimitive::CDeviceIndependent::AddTriangleList 0x00A90900 calls it at 0x00A9095B on its const chunk and binds this
//   address under that symbol. Include set of that caller.
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

unsigned long C3DMeshFileTrianglesChunk::GetCount() const
{
    return TriangleList.size();
}
