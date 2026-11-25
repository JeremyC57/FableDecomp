// Behaviour test for 0x00ABEC90 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "Triangle List", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// Leaf: ?GetChunkDescription@C3DMeshFileTrianglesChunk@@UBE?BVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/C3DMeshFileTrianglesChunk.h (class-model switch FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK; was ?GetChunkDescription@C3DMeshFileTrianglesChunk_Methods@@QBE?AVCCharString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent C3DMeshFileTrianglesChunk_GetCount_00aba370.cpp.
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
        CCharString r = ((C3DMeshFileTrianglesChunk*)g_storage)->C3DMeshFileTrianglesChunk::GetChunkDescription();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "Triangle List") != 0 || g_slot != where) {
            printf("FAIL_00abec90 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("CHUNKDESC_OK\n");
    return 0;
}
