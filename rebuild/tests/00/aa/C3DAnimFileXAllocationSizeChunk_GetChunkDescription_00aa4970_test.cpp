// Behaviour test for 0x00AA4970 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "Allocation Size Helper Chunk", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// Leaf: ?GetChunkDescription@C3DAnimFileXAllocationSizeChunk@@UBE?BVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/C3DAnimFileXAllocationSizeChunk.h (class-model switch FABLE_RETAIL_C3DANIMFILEXALLOCATIONSIZECHUNK; was ?GetChunkDescription@C3DAnimFileXAllocationSizeChunk_Methods@@QBE?AVCCharString@@XZ);
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


static const char* g_text = 0;
static long g_len = 0;
static int g_calls = 0;
static const void* g_slot = 0;
static char g_storage[1024];
CCharString::CCharString(const char* s, long n) { g_text = s; g_len = n; ++g_calls; g_slot = this; }
CCharString::~CCharString() {}
int main(void)
{
    const void* where = 0;
    {
        CCharString r = ((C3DAnimFileXAllocationSizeChunk*)g_storage)->C3DAnimFileXAllocationSizeChunk::GetChunkDescription();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "Allocation Size Helper Chunk") != 0 || g_slot != where) {
            printf("FAIL_00aa4970 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("CHUNKDESC_OK\n");
    return 0;
}
