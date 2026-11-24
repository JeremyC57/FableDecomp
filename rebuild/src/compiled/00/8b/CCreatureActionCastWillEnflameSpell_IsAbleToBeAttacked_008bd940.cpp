// Leaf: ?IsAbleToBeAttacked@CCreatureAction_CastWillEnflameSpell@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_CASTWILLENFLAMESPELL_RTTI_DECLS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_CASTWILLENFLAMESPELL_CLASS
#include "engine/CThing.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_CastWillEnflameSpell.h"
bool CCreatureAction_CastWillEnflameSpell::IsAbleToBeAttacked() const
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_CastWillEnflameSpell_IsAbleToBeAttacked -> CCreatureAction_CastWillEnflameSpell::IsAbleToBeAttacked, the RTTI-placed PDB member; class model: engine/CCreatureAction_CastWillEnflameSpell.h opt-in FABLE_RETAIL_CCREATUREACTION_CASTWILLENFLAMESPELL_CLASS, FABLE_RETAIL_CCREATUREACTION_CASTWILLENFLAMESPELL_RTTI_DECLS (preamble of CCombatActionWillEnflameInto_FinishAction_008c2a80.cpp).
