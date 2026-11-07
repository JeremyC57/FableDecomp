// C3DMeshFileLoader::C3DMeshFileLoader() -- register the 3DMF chunk prototypes: the root chunk, then each chunk type under its parent
// Leaf: ??0C3DMeshFileLoader@@QAE@XZ
// Flags: /Gr
// Retail: Fable.exe 0x00A9BBE0, 3281 bytes, no EH frame, aligned ebp frame (and esp,-8; sub esp,90h). Name: Ego_r.pdb + FableWin.pdb
//   ??0C3DMeshFileLoader@@QAE@XZ; the C3DMeshFileXLoader ctor 0x00AA5C80 (landed) calls it as its base. Rebase of the Codex pass 12
//   RELOCATION_MATCH (rebuild/candidates/manual/pending/2026-11-02_codex_pass12/verified_pending/00a9bbe0) onto our headers.
// Callees: CChunkedFileLoader("3DMF") 0x00AA7510, operator new 0x00BFEA1A + CChunkedFileSuperChunk("3DRT") 0x00AA8360 (inline root
//   ctor, vtable 0x0129E090), CCountedPointer<C3DMeshFileRootChunk>::Reset 0x00A9CED0, AddChunkType 0x00A9AD00 (out of line; each
//   Register*SubChunkType expands into it with the parent's slot-6 GetAsSuperChunk()), the out-of-line chunk ctors (SubMesh 0x00AB9A70,
//   MaterialList 0x00ABFB90, Helpers 0x00AA9650, Transformation 0x00AB9BE0, Bone 0x00AB9DA0, Primitive 0x00ABD080, Cloth 0x00AC66A0,
//   Triangles 0x00ABC860, SmoothingGroups 0x00ABC470, Vertices 0x00ABC780, UniqueVertices 0x00ABC560, Group 0x00ABC650, Material
//   0x00AC0AB0, EngineMaterial 0x00AC56F0, MaterialExt 0x00AC02B0, Map 0x00ABFE50, HelperPoint 0x00AA9B20, DummyObject 0x00AA9E10,
//   ConvexVolume 0x00AAAA10; RTTI vtable stores checked by Codex), the user dtors (SmoothingGroups 0x00ABC4A0, UniqueVertices 0x00ABC590,
//   Group 0x00ABC680, Vertices 0x00ABC7B0, Triangles 0x00ABC890), RegisterClothSubChunkType 0x00A9B930, ~CCharString 0x0099EAE0,
//   ~CChunkedFileSuperChunk 0x00AA8490, ~CChunkedFileChunk 0x00AA75C0, CChunkedFileChunk("MTEH") 0x00AA7590, free 0x00BFEA14,
//   operator delete 0x00BFE9BC (the inline CCountedPointer assignment release).
// Levers: (1) the chunk classes are the FABLE_RETAIL_<CLASS> branches of our generated headers (implicit destructors except the five
//   classes with a user one; the inline mirrored-texture ctor/dtor); (2) the Register* members expand inline here
//   (FABLE_RETAIL_C3DMESHFILELOADER_INLINE_REGISTER), AddChunkType's body above under auto_inline(off) as Codex had it; (3) the
//   EngineMaterial temporary's CIVCountedPointer release: the pointee's Release deletes itself
//   (FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES + FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER); the cached-copy form with the
//   Delete in the caller swaps `mov ecx,eax` above the count load (md2).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILELOADER_INLINE_REGISTER
#define FABLE_RETAIL_C3DMESHFILESUBMESHCHUNK
#define FABLE_RETAIL_C3DMESHFILEMATERIALLISTCHUNK
#define FABLE_RETAIL_C3DMESHFILEHELPERSCHUNK
#define FABLE_RETAIL_C3DMESHFILETRANSFORMATIONCHUNK
#define FABLE_RETAIL_C3DMESHFILEPRIMITIVECHUNK
#define FABLE_RETAIL_C3DMESHFILECLOTHCHUNK
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK
#define FABLE_RETAIL_C3DMESHFILESMOOTHINGGROUPSCHUNK
#define FABLE_RETAIL_C3DMESHFILEVERTICESCHUNK
#define FABLE_RETAIL_C3DMESHFILEVERTICESCHUNK_CTOR
#define FABLE_RETAIL_C3DMESHFILEUNIQUEVERTICESCHUNK
#define FABLE_RETAIL_C3DMESHFILEGROUPCHUNK
#define FABLE_RETAIL_C3DMESHFILEMATERIALCHUNK
#define FABLE_RETAIL_C3DMESHFILEENGINEMATERIALCHUNK
#define FABLE_RETAIL_C3DMESHFILEMATERIALEXTCHUNK
#define FABLE_RETAIL_C3DMESHFILEHELPERPOINTCHUNK
#define FABLE_RETAIL_C3DMESHFILEDUMMYOBJECTCHUNK
#define FABLE_RETAIL_CMIRROREDTEXTUREHIGHLIGHTSCHUNK
#include "engine/C3DMeshFileLoader.h"
#include "engine/C3DMeshFileSubMeshChunk.h"
#include "engine/C3DMeshFileMaterialListChunk.h"
#include "engine/C3DMeshFileHelpersChunk.h"
#include "engine/C3DMeshFileTransformationChunk.h"
#include "engine/C3DMeshFileBoneChunk.h"
#include "engine/C3DMeshFilePrimitiveChunk.h"
#include "engine/C3DMeshFileClothChunk.h"
#include "engine/C3DMeshFileTrianglesChunk.h"
#include "engine/C3DMeshFileSmoothingGroupsChunk.h"
#include "engine/C3DMeshFileVerticesChunk.h"
#include "engine/C3DMeshFileUniqueVerticesChunk.h"
#include "engine/C3DMeshFileGroupChunk.h"
#include "engine/C3DMeshFileMaterialChunk.h"
#include "engine/C3DMeshFileEngineMaterialChunk.h"
#include "engine/C3DMeshFileMaterialExtChunk.h"
#include "engine/C3DMeshFileMapChunk.h"
#include "engine/C3DMeshFileHelperPointChunk.h"
#include "engine/C3DMeshFileDummyObjectChunk.h"
#include "engine/C3DMeshFileConvexVolumeChunk.h"
#include "engine/CMirroredTextureHighlightsChunk.h"

#pragma auto_inline(off)
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::AddChunkType(const CChunkedFileChunk& chunk_type, CChunkedFileSuperChunk* parent)
{
    CCountedPointer<CChunkedFileChunk> chunk;
    chunk.Reset(chunk_type.CreateNewInstance());
    parent->RegisterSubChunkType(chunk);
    return chunk;
}
#pragma auto_inline(on)

C3DMeshFileLoader::C3DMeshFileLoader() : CChunkedFileLoader("3DMF")
{
    RootChunkType.Reset(new C3DMeshFileRootChunk);
    SubMeshChunkType = RegisterRootSubChunkType(C3DMeshFileSubMeshChunk());
    MaterialListChunkType = RegisterRootSubChunkType(C3DMeshFileMaterialListChunk());
    HelpersChunkType = RegisterRootSubChunkType(C3DMeshFileHelpersChunk());
    TransformationChunkType = RegisterSubMeshSubChunkType(C3DMeshFileTransformationChunk());
    BoneChunkType = RegisterSubMeshSubChunkType(C3DMeshFileBoneChunk());
    PrimitiveChunkType = RegisterSubMeshSubChunkType(C3DMeshFilePrimitiveChunk());
    SubMeshClothChunkType = RegisterSubMeshSubChunkType(C3DMeshFileClothChunk());
    TrianglesChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileTrianglesChunk());
    SmoothingGroupsChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileSmoothingGroupsChunk());
    VerticesChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileVerticesChunk());
    UniqueVerticesChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileUniqueVerticesChunk());
    GroupChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileGroupChunk());
    PrimitiveClothChunkType = RegisterPrimitiveSubChunkType(C3DMeshFileClothChunk());
    RegisterClothSubChunkType(C3DMeshFileGroupChunk());
    MaterialChunkType = RegisterMaterialListSubChunkType(C3DMeshFileMaterialChunk());
    EngineMaterialChunkType = RegisterMaterialListSubChunkType(C3DMeshFileEngineMaterialChunk());
    MaterialExtChunkType = RegisterMaterialSubChunkType(C3DMeshFileMaterialExtChunk());
    MapChunkType = RegisterMaterialSubChunkType(C3DMeshFileMapChunk());
    HelperPointChunkType = RegisterHelpersSubChunkType(C3DMeshFileHelperPointChunk());
    DummyObjectChunkType = RegisterHelpersSubChunkType(C3DMeshFileDummyObjectChunk());
    ConvexVolumeChunkType = RegisterHelpersSubChunkType(C3DMeshFileConvexVolumeChunk());
    MirroredTextureChunkType = RegisterPrimitiveSubChunkType(CMirroredTextureHighlightsChunk());
}