// C3DMeshFileLoader::C3DMeshFileLoader() -- Registers mesh file chunk types.
// Leaf: ??0C3DMeshFileLoader@@QAE@XZ
// Flags: /Gr
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_VENDOR_VECTOR_ORDINARY
#define FABLE_RETAIL_MESH_FILE_LOADER
#define FABLE_RETAIL_CARRAY_GEOMETRY
#define FABLE_RETAIL_DEF_CLASS_BASE
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
CCountedPointer<CChunkedFileChunk> C3DMeshFileLoader::AddChunkType(const CChunkedFileChunk&chunkType,CChunkedFileSuperChunk*parent)
{
 CCountedPointer<CChunkedFileChunk> chunk;
 chunk.Reset(chunkType.CreateNewInstance());
 parent->RegisterSubChunkType(chunk);
 return chunk;
}
#pragma auto_inline(on)

C3DMeshFileLoader::C3DMeshFileLoader():CChunkedFileLoader("3DMF")
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
