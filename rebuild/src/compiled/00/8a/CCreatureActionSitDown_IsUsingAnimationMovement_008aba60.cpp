// Leaf: ?IsUsingAnimationMovement@CCreatureAction_SitDown@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_SITDOWN_RTTI_DECLS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SITDOWN
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureAction_SitDown.h"
bool CCreatureAction_SitDown::IsUsingAnimationMovement() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_SitDown_IsUsingAnimationMovement -> CCreatureAction_SitDown::IsUsingAnimationMovement, the RTTI-placed PDB member; class model: engine/CCreatureAction_SitDown.h opt-in FABLE_RETAIL_CCREATUREACTION_SITDOWN, FABLE_RETAIL_CCREATUREACTION_SITDOWN_RTTI_DECLS (preamble of CCreatureActionSitDown_PerformAction_00848fd0.cpp).
