// Leaf: ?IsAbleToBeAttacked@CCreatureAction_ReleasePhysicalShieldSpell@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_RELEASEPHYSICALSHIELDSPELL_RTTI_DECLS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_RELEASEPHYSICALSHIELDSPELL
#include "engine/CThing.h"
#include "engine/CCreatureAction_ReleasePhysicalShieldSpell.h"
bool CCreatureAction_ReleasePhysicalShieldSpell::IsAbleToBeAttacked() const
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_ReleasePhysicalShieldSpell_IsAbleToBeAttacked -> CCreatureAction_ReleasePhysicalShieldSpell::IsAbleToBeAttacked, the RTTI-placed PDB member; class model: engine/CCreatureAction_ReleasePhysicalShieldSpell.h opt-in FABLE_RETAIL_CCREATUREACTION_RELEASEPHYSICALSHIELDSPELL, FABLE_RETAIL_CCREATUREACTION_RELEASEPHYSICALSHIELDSPELL_RTTI_DECLS (preamble of CCreatureActionReleasePhysicalShieldSpell_FinishAction_008b93d0.cpp).
