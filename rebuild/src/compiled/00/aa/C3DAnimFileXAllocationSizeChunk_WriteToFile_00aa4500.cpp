// Leaf: ?WriteToFile@C3DAnimFileXAllocationSizeChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z
// Switch route (NF318, 2026-11-23): defined on the real class of engine/C3DAnimFileXAllocationSizeChunk.h (class-model switch FABLE_RETAIL_C3DANIMFILEXALLOCATIONSIZECHUNK; was ?WriteToFile@C3DAnimFileXAllocationSizeChunk_Methods@@QBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// C3DAnimFileXAllocationSizeChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver&) const -- version 1 then the allocation size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA4500, length 48 bytes.
// Callees: CDataOutputStream::WriteULONG @ 0x00993EA0 (x2).
// Modelled on the PDB engine header (AllocSize at +0x10 agrees with the bytes) through the usual _Methods
// derivation. The saver is unused; no super-chunk write (plain chunk base).
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

class CChunkedFileSaver;

class CDataOutputStream { public:
    void WriteULONG(unsigned long value);                          // 0x00993EA0
};

void C3DAnimFileXAllocationSizeChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& /*saver*/) const
{
    out.WriteULONG(1);
    out.WriteULONG(AllocSize);
}