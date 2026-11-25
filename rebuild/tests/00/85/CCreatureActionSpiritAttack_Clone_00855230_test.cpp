// Behaviour test for 0x00855230 CCreatureAction_SpiritAttack::Clone (NF425): links the row object; operator new and the copy constructor are defined here and
// record their arguments: one allocation of 0xB8 bytes, one copy construction into that block from `*this`, the block returned;
// and 0 with no construction when the allocation fails.
// Ported (NF427 round 2, 2026-11-25): no class body is defined here; the classes come from the shared headers (the row's
//   prelude below, verbatim, plus engine/CCreatureAction_SpiritAttack.h). Removed local bodies: CCreatureAction_SpiritAttack. The members the test supplies (??0CCreatureAction_SpiritAttack@@QAE@ABV0@@Z) and the row's CCreatureAction_SpiritAttack::Clone are
//   reached through extern "C" __fastcall stand-ins mapped by /alternatename (ecx = this, edx unused, stack arguments
//   popped by the callee: the __thiscall frame); sizes and offsets are the retail literals of the original test.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_SpiritAttack.h"
#include <stdio.h>
#include <stddef.h>

class CCreatureActionBase;

static unsigned int g_size = 0;
static int g_news = 0, g_ctors = 0, g_fail = 0;
static const void* g_this = 0;
static const void* g_other = 0;
static long g_block[0xB8 / 4 + 4];
void* operator new(size_t n) { g_size = (unsigned int)n; ++g_news; return g_fail ? 0 : (void*)g_block; }
extern "C" void* __fastcall nf427_copy_ctor(void* self, int edx_unused, const void* other) { g_this = self; g_other = other; ++g_ctors; return self; }   // CCreatureAction_SpiritAttack::CCreatureAction_SpiritAttack
#pragma comment(linker, "/alternatename:??0CCreatureAction_SpiritAttack@@QAE@ABV0@@Z=@nf427_copy_ctor@12")

static long g_src[64];

extern "C" CCreatureActionBase* __fastcall nf427_clone(const void* self);   // the row's CCreatureAction_SpiritAttack::Clone
#pragma comment(linker, "/alternatename:@nf427_clone@4=?Clone@CCreatureAction_SpiritAttack@@UBEPAVCCreatureActionBase@@XZ")

int main(void)
{
    const void* src = g_src;
    CCreatureActionBase* r = nf427_clone(src);
    int ok = r == (CCreatureActionBase*)(void*)g_block && g_news == 1 && g_size == 0xB8 && g_ctors == 1 && g_this == (void*)g_block && g_other == (const void*)g_src;
    g_fail = 1;
    r = nf427_clone(src);
    ok = ok && r == 0 && g_news == 2 && g_ctors == 1;
    if (!ok) {
        printf("00855230_TEST FAIL news=%d ctors=%d size=%u\n", g_news, g_ctors, g_size);
        return 1;
    }
    printf("00855230_TEST PASS\n");
    return 0;
}
