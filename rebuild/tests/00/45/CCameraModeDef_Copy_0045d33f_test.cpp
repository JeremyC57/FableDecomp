// Behaviour test for 0x0045D33F CCameraModeDef::Copy (NF425): links the row object; CCameraModeDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
// Ported (NF427 round 2, 2026-11-25): no class body is defined here; the classes come from the shared headers (the row's
//   prelude below, verbatim, plus engine/CCameraModeDef.h). Removed local bodies: CCameraModeDef. The members the test supplies (??4CCameraModeDef@@QAEAAV0@ABV0@@Z) and the row's CCameraModeDef::Copy are
//   reached through extern "C" __fastcall stand-ins mapped by /alternatename (ecx = this, edx unused, stack arguments
//   popped by the callee: the __thiscall frame); sizes and offsets are the retail literals of the original test.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CCameraModeDef.h"
#include <stdio.h>

class CDefClassBase;

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
extern "C" void* __fastcall nf427_assign(void* self, int edx_unused, const void* other) { g_this = self; g_other = other; ++g_calls; return self; }   // CCameraModeDef::operator=
#pragma comment(linker, "/alternatename:??4CCameraModeDef@@QAEAAV0@ABV0@@Z=@nf427_assign@12")

static long g_dst[32];
static long g_src[32];

extern "C" void __fastcall nf427_copy(void* self, int edx_unused, const void* other);   // the row's CCameraModeDef::Copy
#pragma comment(linker, "/alternatename:@nf427_copy@12=?Copy@CCameraModeDef@@UAEXPBVCDefClassBase@@@Z")

int main(void)
{
    void* dst = g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    nf427_copy(dst, 0, src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_0045d33f calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_0045d33f\n");
    return 0;
}
