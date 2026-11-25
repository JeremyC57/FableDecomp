// Behaviour test for 0x008800F0 CCombatAction_KickThingOnGround::Clone (NF425): links the row object; operator new and the copy constructor are defined here and
// record their arguments: one allocation of 0x130 bytes, one copy construction into that block from `*this`, the block returned;
// and 0 with no construction when the allocation fails.
// Ported (NF427 round 2, 2026-11-25): no class body is defined here; the classes come from the shared headers (the row's
//   prelude below, verbatim, plus engine/CCombatAction_KickThingOnGround.h). Removed local bodies: CCombatAction_KickThingOnGround. The members the test supplies (??0CCombatAction_KickThingOnGround@@QAE@ABV0@@Z) and the row's CCombatAction_KickThingOnGround::Clone are
//   reached through extern "C" __fastcall stand-ins mapped by /alternatename (ecx = this, edx unused, stack arguments
//   popped by the callee: the __thiscall frame); sizes and offsets are the retail literals of the original test.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCombatAction_KickThingOnGround.h"
#include <stdio.h>
#include <stddef.h>

class CCreatureActionBase;

static unsigned int g_size = 0;
static int g_news = 0, g_ctors = 0, g_fail = 0;
static const void* g_this = 0;
static const void* g_other = 0;
static long g_block[0x130 / 4 + 4];
void* operator new(size_t n) { g_size = (unsigned int)n; ++g_news; return g_fail ? 0 : (void*)g_block; }
extern "C" void* __fastcall nf427_copy_ctor(void* self, int edx_unused, const void* other) { g_this = self; g_other = other; ++g_ctors; return self; }   // CCombatAction_KickThingOnGround::CCombatAction_KickThingOnGround
#pragma comment(linker, "/alternatename:??0CCombatAction_KickThingOnGround@@QAE@ABV0@@Z=@nf427_copy_ctor@12")

static long g_src[64];

extern "C" CCreatureActionBase* __fastcall nf427_clone(const void* self);   // the row's CCombatAction_KickThingOnGround::Clone
#pragma comment(linker, "/alternatename:@nf427_clone@4=?Clone@CCombatAction_KickThingOnGround@@UBEPAVCCreatureActionBase@@XZ")

int main(void)
{
    const void* src = g_src;
    CCreatureActionBase* r = nf427_clone(src);
    int ok = r == (CCreatureActionBase*)(void*)g_block && g_news == 1 && g_size == 0x130 && g_ctors == 1 && g_this == (void*)g_block && g_other == (const void*)g_src;
    g_fail = 1;
    r = nf427_clone(src);
    ok = ok && r == 0 && g_news == 2 && g_ctors == 1;
    if (!ok) {
        printf("008800f0_TEST FAIL news=%d ctors=%d size=%u\n", g_news, g_ctors, g_size);
        return 1;
    }
    printf("008800f0_TEST PASS\n");
    return 0;
}
