// Leaf: ?GetChunkInfo@C3DMeshFileTransformationChunk@@UBE?BVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/C3DMeshFileTransformationChunk.h (class-model switch FABLE_RETAIL_C3DMESHFILETRANSFORMATIONCHUNK; was ?GetChunkInfo@C3DMeshFileTransformationChunk_Methods@@QBE?AVCCharString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent C3DMeshFileLoader_C3DMeshFileLoader_00a9bbe0.cpp.
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

const CCharString C3DMeshFileTransformationChunk::GetChunkInfo() const
{
    return CCharString("", -1);
}