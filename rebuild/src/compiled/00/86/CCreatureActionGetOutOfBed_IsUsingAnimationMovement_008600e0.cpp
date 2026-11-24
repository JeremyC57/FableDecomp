// Leaf: ?IsUsingAnimationMovement@CCreatureAction_GetOutOfBed@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_GETOUTOFBED
#include "engine/CThing.h"
#include "engine/CCreatureAction_GetOutOfBed.h"
bool CCreatureAction_GetOutOfBed::IsUsingAnimationMovement() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_GetOutOfBed_IsUsingAnimationMovement -> CCreatureAction_GetOutOfBed::IsUsingAnimationMovement, the RTTI-placed PDB member; class model: engine/CCreatureAction_GetOutOfBed.h under FABLE_RETAIL_CCREATUREACTION_GETOUTOFBED (preamble of NCreatureMode_GetUpFromSleeping_0085f6f0.cpp).
