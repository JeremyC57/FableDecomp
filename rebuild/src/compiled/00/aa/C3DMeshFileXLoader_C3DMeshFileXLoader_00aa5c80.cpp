// C3DMeshFileXLoader::C3DMeshFileXLoader(void) -- register the extended (X) chunk prototypes of the mesh file format.
// Leaf: ??0C3DMeshFileXLoader@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA5C80, 2144 bytes, no EH frame. Base ctor C3DMeshFileLoader 0x00A9BBE0, vtable 0x0129E450,
//   then each X chunk is built as a stack temporary ("XBDS" / "XBDB" / "XUVT" / "XMIF" / "XVBD" / "XTST" / "XTID" / "XAPR") and
//   handed to C3DMeshFileLoader::Register{Root,SubMesh,Primitive,Triangles,Map}SubChunkType (0x00A9B840 / 870 / 8A0 / 8D0 / A10);
//   the returned CCountedPointer<CChunkedFileChunk> is released through DeleteData 0x00A99FF0. The animated-primitive chunk then
//   gets XAPB, XAPB gets C3DMeshFileTrianglesChunk (0x00ABC860), Triangles gets XTST, via vtable slot 6 GetAsSuperChunk() and
//   CChunkedFileSuperChunk::RegisterSubChunkType 0x00AA87E0. Call targets paired one-to-one with the object relocations (callchk.py).
// Levers: (1) the X chunk classes have NO user destructor -- an inline `virtual ~X(){}` re-stores the derived vtable in the
//   temporary's teardown (2203 v 2144 bytes); the implicit one goes straight to the base dtor as retail does.
//   (2) CChunkedFileChunk declares the non-const GetAsSuperChunk() before the const one: VC7.1 lays overloaded virtuals out in
//   reverse declaration order, and retail calls the non-const at slot 6 ([edx+18h]; CChunkedFileSuperChunk::InitNewInstance
//   0x00AA8460 calls slot 6 non-const too).
//   (3) `CCountedPointer<CChunkedFileChunk> p; p = Register...();` (default-construct then assign) gives retail's copy shape.
// Header: engine/C3DMeshFileXLoader.h, C3DMeshFileLoader.h, CChunkedFileLoader.h, C3DMeshFileXAnimatedPrimitiveBlockChunk.h (new),
//   FABLE_RETAIL_<CLASS> branches of CChunkedFileChunk / CChunkedFileSuperChunk / the X chunk headers / C3DMeshFileTrianglesChunk
//   (overlay hdr/; default branch = the generated repo header).
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILEXBOUNDINGSPHERECHUNK
#define FABLE_RETAIL_C3DMESHFILEXBOUNDINGBOXCHUNK
#define FABLE_RETAIL_C3DMESHFILEXUVTANGENTCHUNK
#define FABLE_RETAIL_C3DMESHFILEXMAPPINGINFOCHUNK
#define FABLE_RETAIL_C3DMESHFILEXVERTEXDATACHUNK
#define FABLE_RETAIL_C3DMESHFILEXTRIANGLESTRIPCHUNK
#define FABLE_RETAIL_C3DMESHFILEXTEXTUREINDEXCHUNK
#define FABLE_RETAIL_C3DMESHFILEXANIMATEDPRIMITIVECHUNK
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK
#include "engine/C3DMeshFileXLoader.h"
#include "engine/C3DMeshFileXBoundingSphereChunk.h"
#include "engine/C3DMeshFileXBoundingBoxChunk.h"
#include "engine/C3DMeshFileXUVTangentChunk.h"
#include "engine/C3DMeshFileXMappingInfoChunk.h"
#include "engine/C3DMeshFileXVertexDataChunk.h"
#include "engine/C3DMeshFileXTriangleStripChunk.h"
#include "engine/C3DMeshFileXTextureIndexChunk.h"
#include "engine/C3DMeshFileXAnimatedPrimitiveChunk.h"
#include "engine/C3DMeshFileXAnimatedPrimitiveBlockChunk.h"
#include "engine/C3DMeshFileTrianglesChunk.h"

C3DMeshFileXLoader::C3DMeshFileXLoader()
{
    RegisterRootSubChunkType(C3DMeshFileXBoundingSphereChunk());
    RegisterRootSubChunkType(C3DMeshFileXBoundingBoxChunk());
    RegisterSubMeshSubChunkType(C3DMeshFileXBoundingSphereChunk());
    RegisterSubMeshSubChunkType(C3DMeshFileXBoundingBoxChunk());
    RegisterPrimitiveSubChunkType(C3DMeshFileXUVTangentChunk());
    RegisterPrimitiveSubChunkType(C3DMeshFileXMappingInfoChunk());
    RegisterPrimitiveSubChunkType(C3DMeshFileXBoundingSphereChunk());
    RegisterPrimitiveSubChunkType(C3DMeshFileXBoundingBoxChunk());
    RegisterPrimitiveSubChunkType(C3DMeshFileXVertexDataChunk());
    RegisterTrianglesSubChunkType(C3DMeshFileXTriangleStripChunk());
    RegisterMapSubChunkType(C3DMeshFileXTextureIndexChunk());
    CCountedPointer<CChunkedFileChunk> animated_primitive_chunk;
    animated_primitive_chunk = RegisterPrimitiveSubChunkType(C3DMeshFileXAnimatedPrimitiveChunk());
    CCountedPointer<CChunkedFileChunk> animated_primitive_block_chunk(new C3DMeshFileXAnimatedPrimitiveBlockChunk);
    animated_primitive_chunk->GetAsSuperChunk()->RegisterSubChunkType(animated_primitive_block_chunk);
    CCountedPointer<CChunkedFileChunk> triangles_chunk(new C3DMeshFileTrianglesChunk);
    animated_primitive_block_chunk->GetAsSuperChunk()->RegisterSubChunkType(triangles_chunk);
    CCountedPointer<CChunkedFileChunk> triangle_strip_chunk(new C3DMeshFileXTriangleStripChunk);
    triangles_chunk->GetAsSuperChunk()->RegisterSubChunkType(triangle_strip_chunk);
}