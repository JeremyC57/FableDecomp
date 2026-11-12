// Behaviour test for 007fef40: src/007fef40.cpp copied verbatim (agentNF24 respell onto engine/campfire_types.h), then a probe that
// drives CTCUserActionPointBase::SetActionPointAsFree(long) over a zeroed object image: the full path grows the vector through
// STLport _M_insert_overflow keeping the earlier indexes; the in-capacity path stores the index in place.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCUserActionPointBase::SetActionPointAsFree(long index) -- return an action point to the free list: push its index on FreeActionPointIndexes (+0x0c) through the inlined STLport push_back
// Leaf: ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FEF40, length 53 bytes, `ret 4`.
// Name fix (agentNF24): was the made-up CTCUserActionPointBase::FreeActionPoint (graded through the stand-in
//   CPointerListOwner_007fef40::Add over an invented CRetailVector). Ego_r.pdb 0x007EF000 + FableWin.pdb 0x02588640
//   ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z; every landed binder (0x00938A10 first, 0x00965640, 0x00966030 ...)
//   already binds this spelling. Ego_r member FreeActionPointIndexes (vector<long>) at +0x0c behind CTCBase.
// Callees: vector<long>::_M_insert_overflow(pos, x, __true_type, 1, true) @ 0x00440990 (instantiated in this TU, kept out of line).
// Shape: retail homes the empty __true_type tag on the live `index` argument slot (`lea edx,[esp+0Ch]` pushed twice); the
//   overflow body (stlport/fable_containers.h) is visible and VC7.1 keeps it out of line, as in the retail TU.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETACTIONID
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_SETCOMBATANIMATION
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#define FABLE_RETAIL_CTCCARRYING_SLOTACCESS
#define FABLE_RETAIL_CTCCARRYING_CARRYSLOT
#define FABLE_RETAIL_CCARRYABLEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_CARRYCRATEANDFOLLOW
#include "rebuild_abi.h"
#include "engine/ETCInterfaceType.h"
#include "engine/NCreatureMode.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCarryableDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/campfire_types.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CarryCrateAndFollow.h"
#include "engine/campfire_types.h"

void CTCUserActionPointBase::SetActionPointAsFree(long index)
{
    FreeActionPointIndexes.push_back(index);
}

// ---- probe ----
struct CActionPointProbe : CTCUserActionPointBase {
    static std::vector<long>& Free(CTCUserActionPointBase* p) { return ((CActionPointProbe*)p)->FreeActionPointIndexes; }
};
int main()
{
    static unsigned char image[sizeof(CTCUserActionPointBase)];
    memset(image, 0, sizeof(image));
    CTCUserActionPointBase* point = (CTCUserActionPointBase*)image;
    std::vector<long>& v = CActionPointProbe::Free(point);
    CHECK((unsigned char*)&v - image == 0x0c);
    CHECK(v.size() == 0);
    point->SetActionPointAsFree(7);                  // empty: grows through STLport _M_insert_overflow (capacity 1)
    CHECK(v.size() == 1 && v[0] == 7);
    point->SetActionPointAsFree(9);                  // full: grows again (capacity 2), the old index copied
    point->SetActionPointAsFree(11);                 // full: grows again (capacity 4)
    CHECK(v.size() == 3 && v[0] == 7 && v[1] == 9 && v[2] == 11);
    long* before = &v[0];
    point->SetActionPointAsFree(13);                 // room left: stored in place, no reallocation
    CHECK(v.size() == 4 && &v[0] == before && v[3] == 13);
    if (fails == 0) printf("007fef40_TEST PASS\n");
    return fails;
}
