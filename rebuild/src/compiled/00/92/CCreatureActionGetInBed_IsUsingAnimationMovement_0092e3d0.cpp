// Leaf: ?IsUsingAnimationMovement@CCreatureAction_GetInBed@@UBE_NXZ
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
bool CCreatureAction_GetInBed::IsUsingAnimationMovement() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_GetInBed_IsUsingAnimationMovement -> CCreatureAction_GetInBed::IsUsingAnimationMovement, the RTTI-placed PDB member; class model: engine/CCreatureAction_GetInBed.h under FABLE_RETAIL_CCREATUREACTION_GETINBED (preamble of CAIStateGroupMinionSleep_IsInSleepTime_0092c520.cpp).
