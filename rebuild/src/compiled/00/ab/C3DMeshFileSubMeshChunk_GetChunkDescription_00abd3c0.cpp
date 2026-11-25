// Leaf: ?GetChunkDescription@C3DMeshFileSubMeshChunk@@UBE?BVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/C3DMeshFileSubMeshChunk.h (class-model switch FABLE_RETAIL_C3DMESHFILESUBMESHCHUNK; was ?GetChunkDescription@C3DMeshFileSubMeshChunk_Methods@@QBE?AVCCharString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent C3DMesh2_InitialiseBones_00a8b990.cpp.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILESUBMESHCHUNK
#define FABLE_RETAIL_C3DMESHFILESUBMESHCHUNK_FINDBONE
#define FABLE_RETAIL_C3DMESHFILEBONECHUNK_GETBONE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_C3DMESH2_HELPERPOINTS
#define FABLE_RETAIL_C3DMESH2_INITIALISEBONES
#define FABLE_RETAIL_C3DBONE2_CTOR
#define FABLE_RETAIL_C3DMESH2_BONENAMEPOINTERS_DECL
#define FABLE_RETAIL_C3DMESHSTATS_PROTECTED_POOL
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CDATASTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITESTRING
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#include "engine/C3DBone2.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DMeshFileSubMeshChunk.h"
#include "engine/C3DMeshFileBoneChunk.h"
#include "engine/CMovableResourceMemoryPool.h"
#include "engine/CMemoryDataOutputStream.h"
const CCharString C3DMeshFileSubMeshChunk::GetChunkDescription() const
{
    return CCharString("Sub-Mesh", -1);
}
