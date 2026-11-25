// Behaviour test for 0x004E980E CShopDef::Copy (NF425): links the row object; CShopDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
// Ported (NF427 round 2, 2026-11-25): no class body is defined here; the classes come from the shared headers (the row's
//   prelude below, verbatim, plus engine/CShopDef.h). Removed local bodies: CShopDef. The members the test supplies (??4CShopDef@@QAEAAV0@ABV0@@Z) and the row's CShopDef::Copy are
//   reached through extern "C" __fastcall stand-ins mapped by /alternatename (ecx = this, edx unused, stack arguments
//   popped by the callee: the __thiscall frame); sizes and offsets are the retail literals of the original test.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#define FABLE_RETAIL_CTCSHOP_BUYITEM
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCSHOP_ADDSALEITEMPLACES
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_MORALITY
#define FABLE_RETAIL_CTCHEROSTATS_MORALITYMAX
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYBASE
#include "fable_string.h"
#include <math.h>
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CShopDef.h"
#include <stdio.h>

class CDefClassBase;

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
extern "C" void* __fastcall nf427_assign(void* self, int edx_unused, const void* other) { g_this = self; g_other = other; ++g_calls; return self; }   // CShopDef::operator=
#pragma comment(linker, "/alternatename:??4CShopDef@@QAEAAV0@ABV0@@Z=@nf427_assign@12")

static long g_dst[32];
static long g_src[32];

extern "C" void __fastcall nf427_copy(void* self, int edx_unused, const void* other);   // the row's CShopDef::Copy
#pragma comment(linker, "/alternatename:@nf427_copy@12=?Copy@CShopDef@@UAEXPBVCDefClassBase@@@Z")

int main(void)
{
    void* dst = g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    nf427_copy(dst, 0, src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_004e980e calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_004e980e\n");
    return 0;
}
