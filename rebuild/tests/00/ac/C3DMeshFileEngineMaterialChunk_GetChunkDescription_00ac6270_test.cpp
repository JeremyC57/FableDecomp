// Behaviour test for 0x00AC6270 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "Engine Material", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// Leaf: ?GetChunkDescription@C3DMeshFileEngineMaterialChunk@@UBE?BVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/C3DMeshFileEngineMaterialChunk.h (class-model switch FABLE_RETAIL_C3DMESHFILEENGINEMATERIALCHUNK; was ?GetChunkDescription@C3DMeshFileEngineMaterialChunk_Methods@@QBE?AVCCharString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// C3DMeshFileEngineMaterialChunk_Methods::GetChunkDescription
// retail 0x00ac6270 : const method returning CCharString by value.
// push -1 ; push <lit> ; ecx=retbuf ; call CCharString::ctor(char*, int) ; return retbuf

// Preamble: the define / include block of the landed precedent C3DMeshFileEngineMaterialChunk_ReadFromFile_00ac5770.cpp.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_C3DMESHFILEENGINEMATERIALCHUNK
#include "fable_string.h"
#include "engine/CDataInputStream.h"
#include "engine/C3DMeshFileEngineMaterialChunk.h"


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
        CCharString r = ((C3DMeshFileEngineMaterialChunk*)g_storage)->C3DMeshFileEngineMaterialChunk::GetChunkDescription();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "Engine Material") != 0 || g_slot != where) {
            printf("FAIL_00ac6270 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETCHUNKDESC_OK\n");
    return 0;
}
