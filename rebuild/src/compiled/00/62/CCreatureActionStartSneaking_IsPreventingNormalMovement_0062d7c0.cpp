// Leaf: ?IsPreventingNormalMovement@CCreatureAction_StartSneaking@@UBE_NXZ
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_STARTSNEAKING_VIRTUAL_MODEL
#include "engine/CCreatureAction_StartSneaking.h"
bool CCreatureAction_StartSneaking::IsPreventingNormalMovement() const
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_StartSneaking_IsPreventingNormalMovement -> CCreatureAction_StartSneaking::IsPreventingNormalMovement, the RTTI-placed PDB member; class model: engine/CCreatureAction_StartSneaking.h under FABLE_RETAIL_CCREATUREACTION_STARTSNEAKING_VIRTUAL_MODEL (preamble of CCreatureActionStartSneaking_Clone_0062f230.cpp).
