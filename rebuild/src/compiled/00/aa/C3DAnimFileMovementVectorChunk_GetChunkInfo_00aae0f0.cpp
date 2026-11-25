// Leaf: ?GetChunkInfo@C3DAnimFileMovementVectorChunk@@UBE?BVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/C3DAnimFileMovementVectorChunk.h (class-model switch FABLE_RETAIL_C3DANIMFILEMOVEMENTVECTORCHUNK; was ?GetChunkInfo@C3DAnimFileMovementVectorChunk_Methods@@QAE?AVCCharString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent C3DAnimation2_InitialiseSharedAnimation_00a4fe60.cpp.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_C3DANIMFILEBONEMASKCHUNK
#define FABLE_RETAIL_C3DANIMFILEHELPERSCHUNK
#define FABLE_RETAIL_C3DANIMFILEMOVEMENTVECTORCHUNK
#define FABLE_RETAIL_C3DANIMFILEXALLOCATIONSIZECHUNK
#define FABLE_RETAIL_C3DANIMFILEXSEQUENCECHUNK
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "stlport/fable_containers.h"
#include "engine/C3DAnimation2.h"
#include "engine/C3DAnimFile.h"
#include "engine/C3DAnimFileRootChunk.h"
#include "engine/C3DAnimFileHelpersChunk.h"
#include "engine/C3DAnimFileObjectChunk.h"
#include "engine/C3DAnimFileXAllocationSizeChunk.h"
#include "engine/C3DAnimFileXSequenceChunk.h"
#include "engine/C3DAnimFileBoneMaskChunk.h"
#include "engine/C3DAnimFileMovementVectorChunk.h"
#include "engine/C3DAnimFileSequenceChunk.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CMovableResourceMemoryPool.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CScopedLockFlagBool.h"

const CCharString C3DAnimFileMovementVectorChunk::GetChunkInfo() const
{
    return CCharString("", -1);
}
