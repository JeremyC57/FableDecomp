// Leaf: ?IsThingInTargetingRangeForTargetType@NPlayerTargeting@@YI_NABVCThing@@0K@Z
// Flags:
#define FABLE_RETAIL_TARGET_RANGE_COMPONENT_MAP_PASS30
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#include "fable_string.h"
#include "engine/NPlayerTargeting.h"
#include "engine/CTCCarrying.h"
#include "engine/CTargetingDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/GFSqr.h"

namespace NPlayerTargeting {
bool FABLE_FASTCALL IsThingInTargetingRangeForTargetType(const CThing& targeter, const CThing& targeted, unsigned long targeting_type)
{
    const CThing* pcarried = 0;
    const CTCCarrying* ptc_carrying;
    if (targeter.PeekTC(&ptc_carrying))
        pcarried = ptc_carrying->GetThingInPrimarySlot();
    CDefPointer<const CTargetingDef> ptarg_def(NULL_DEF);
    // The original success assertion evaluates the sub-definition lookup in release.
    targeter.GetPBaseDef()->GetSubDef(&ptarg_def);
    return (targeted.GetPos() - targeter.GetPos()).GetSquaredMagnitude2D() <
           GFSqr(GetTargetingRangeForTargetType(targeting_type, *ptarg_def, targeted, pcarried));
}
}
