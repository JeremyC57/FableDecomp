// Select the seat mode and animation, then perform the sit-down action.
// Leaf: ?SitInSeat@CAIStateGroup_UsesSeat@@IAE_NXZ
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMFILTER
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_HASPROPERTY
#define FABLE_RETAIL_CTHINGOBJECT_HASPROPERTY_MASK_PASS26
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ARROW_ASSERTED_BODY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CANIMATIONENTRY_SEAT_ASSIGN_PASS27
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_NOINLINE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_USESSEAT_VIRTUALS_PASS27
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTIONONCE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SITDOWN
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_PASS27
#include "fable_assert.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CAIStateGroup_UsesSeat.h"
#include "engine/CAIBrain.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterAnyGroup.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
#include "engine/CCreatureAction_SitDown.h"
#include "engine/fable_ai_action_enums.h"

namespace {
extern CCharString SIT_ON_CHAIR_ANIM_NAME;
extern CCharString SIT_ON_BENCH_ANIM_NAME;
const unsigned long THING_OBJECT_PROPERTY_CHAIR = 0x10000;
const unsigned long THING_OBJECT_PROPERTY_BENCH = 0x20000;
}
bool FABLE_FASTCALL GFWorld2DDistanceSmallerThan(const C3DVector&, const C3DVector&, float, bool);

bool CAIStateGroup_UsesSeat::SitInSeat()
{
    FABLE_RETAIL_ASSERT(GFWorld2DDistanceSmallerThan(GetCreature().GetPos(), SeatPos, 0.1f, false));
    CAnimationEntry anim_entry;
    CCountedPointer<NCreatureMode::CModeSitBase> psit_mode;
    if (PSeat->HasProperty(THING_OBJECT_PROPERTY_CHAIR)) {
        FABLE_RETAIL_ASSERT(!PSeat->HasProperty(THING_OBJECT_PROPERTY_BENCH));
        anim_entry = GetCreature().GetRandomAnimFromNameWithFilter(SIT_ON_CHAIR_ANIM_NAME, CAnimFilterAnyGroup());
        NCreatureMode::CModeSitBase* pmode = AllocChairSittingMode();
        FABLE_RETAIL_ASSERT(pmode);
        psit_mode.Reset(pmode);
        SittingOnBench = false;
    } else {
        FABLE_RETAIL_ASSERT(PSeat->HasProperty(THING_OBJECT_PROPERTY_BENCH));
        anim_entry = GetCreature().GetRandomAnimFromNameWithFilter(SIT_ON_BENCH_ANIM_NAME, CAnimFilterAnyGroup());
        psit_mode.Reset(new NCreatureMode::CModeSitBench(GetCreature()));
        SittingOnBench = true;
    }
    if (IsStopping())
        return false;
    Sitting = true;
    if (AIAction_PerformAction(CCreatureAction_SitDown(GetCreature(), anim_entry, psit_mode)) != AI_ACTION_FINISHED)
        return false;
    return true;
}