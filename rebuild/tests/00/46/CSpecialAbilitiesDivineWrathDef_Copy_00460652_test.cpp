// Behaviour test for 0x00460652 CSpecialAbilitiesDivineWrathDef::Copy (NF425): links the row object; CSpecialAbilitiesDivineWrathDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
// Ported (NF427 round 2, 2026-11-25): no class body is defined here; the classes come from the shared headers (the row's
//   prelude below, verbatim). Removed local bodies: CSpecialAbilitiesDivineWrathDef. The members the test supplies (??4CSpecialAbilitiesDivineWrathDef@@QAEAAV0@ABV0@@Z) and the row's CSpecialAbilitiesDivineWrathDef::Copy are
//   reached through extern "C" __fastcall stand-ins mapped by /alternatename (ecx = this, edx unused, stack arguments
//   popped by the callee: the __thiscall frame); sizes and offsets are the retail literals of the original test.
//   engine/CSpecialAbilitiesDivineWrathDef.h is not included: it does not compile under the row's prelude (header debt, T3).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
#include <stdio.h>

class CDefClassBase;

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
extern "C" void* __fastcall nf427_assign(void* self, int edx_unused, const void* other) { g_this = self; g_other = other; ++g_calls; return self; }   // CSpecialAbilitiesDivineWrathDef::operator=
#pragma comment(linker, "/alternatename:??4CSpecialAbilitiesDivineWrathDef@@QAEAAV0@ABV0@@Z=@nf427_assign@12")

static long g_dst[32];
static long g_src[32];

extern "C" void __fastcall nf427_copy(void* self, int edx_unused, const void* other);   // the row's CSpecialAbilitiesDivineWrathDef::Copy
#pragma comment(linker, "/alternatename:@nf427_copy@12=?Copy@CSpecialAbilitiesDivineWrathDef@@UAEXPBVCDefClassBase@@@Z")

int main(void)
{
    void* dst = g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    nf427_copy(dst, 0, src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_00460652 calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_00460652\n");
    return 0;
}
