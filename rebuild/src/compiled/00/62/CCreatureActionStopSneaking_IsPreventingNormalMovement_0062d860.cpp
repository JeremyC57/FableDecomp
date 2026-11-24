// Leaf: ?IsPreventingNormalMovement@CCreatureAction_StopSneaking@@UBE_NXZ
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_STOPSNEAKING_VIRTUAL_MODEL
#include "engine/CCreatureAction_StopSneaking.h"
bool CCreatureAction_StopSneaking::IsPreventingNormalMovement() const
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_StopSneaking_IsPreventingNormalMovement -> CCreatureAction_StopSneaking::IsPreventingNormalMovement, the RTTI-placed PDB member; class model: engine/CCreatureAction_StopSneaking.h under FABLE_RETAIL_CCREATUREACTION_STOPSNEAKING_VIRTUAL_MODEL (preamble of CCreatureActionStopSneaking_Clone_0062f260.cpp).
