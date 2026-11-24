// Leaf: ?GetAnimExtraFlags@CCreatureAction_GetInBed@@UBEKXZ
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CCREATUREACTION_GETINBED
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureAction_GetInBed.h"
unsigned long CCreatureAction_GetInBed::GetAnimExtraFlags() const
{
    return 8;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_GetInBed_GetAnimExtraFlags -> CCreatureAction_GetInBed::GetAnimExtraFlags, the RTTI-placed PDB member; class model: engine/CCreatureAction_GetInBed.h under FABLE_RETAIL_CCREATUREACTION_GETINBED (preamble of CAIStateGroupMinionSleep_IsInSleepTime_0092c520.cpp).
