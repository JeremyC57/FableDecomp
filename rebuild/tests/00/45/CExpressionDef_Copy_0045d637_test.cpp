// Behaviour test for 0x0045D637 CExpressionDef::Copy (NF425): links the row object; CExpressionDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
// Ported (NF427 round 2, 2026-11-25): no class body is defined here; the classes come from the shared headers (the row's
//   prelude below, verbatim, plus engine/CExpressionDef.h). Removed local bodies: CExpressionDef. The members the test supplies (??4CExpressionDef@@QAEAAV0@ABV0@@Z) and the row's CExpressionDef::Copy are
//   reached through extern "C" __fastcall stand-ins mapped by /alternatename (ecx = this, edx unused, stack arguments
//   popped by the callee: the __thiscall frame); sizes and offsets are the retail literals of the original test.
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_NOTEQUAL
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS
#define FABLE_RETAIL_CTCTARGETINGBASE_OFTYPE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISPLAYERCHARACTER
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUEST
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_PLAYER_GETNUMBER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CExpressionDef.h"
#include <stdio.h>

class CDefClassBase;

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
extern "C" void* __fastcall nf427_assign(void* self, int edx_unused, const void* other) { g_this = self; g_other = other; ++g_calls; return self; }   // CExpressionDef::operator=
#pragma comment(linker, "/alternatename:??4CExpressionDef@@QAEAAV0@ABV0@@Z=@nf427_assign@12")

static long g_dst[32];
static long g_src[32];

extern "C" void __fastcall nf427_copy(void* self, int edx_unused, const void* other);   // the row's CExpressionDef::Copy
#pragma comment(linker, "/alternatename:@nf427_copy@12=?Copy@CExpressionDef@@UAEXPBVCDefClassBase@@@Z")

int main(void)
{
    void* dst = g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    nf427_copy(dst, 0, src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_0045d637 calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_0045d637\n");
    return 0;
}
