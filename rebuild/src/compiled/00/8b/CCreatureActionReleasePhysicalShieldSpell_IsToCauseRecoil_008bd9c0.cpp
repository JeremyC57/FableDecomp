// Leaf: ?IsToCauseRecoil@CCreatureAction_ReleasePhysicalShieldSpell@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_RELEASEPHYSICALSHIELDSPELL_RTTI_DECLS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_RELEASEPHYSICALSHIELDSPELL
#include "engine/CThing.h"
#include "engine/CCreatureAction_ReleasePhysicalShieldSpell.h"
bool CCreatureAction_ReleasePhysicalShieldSpell::IsToCauseRecoil() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_ReleasePhysicalShieldSpell_IsToCauseRecoil -> CCreatureAction_ReleasePhysicalShieldSpell::IsToCauseRecoil, the RTTI-placed PDB member; class model: engine/CCreatureAction_ReleasePhysicalShieldSpell.h opt-in FABLE_RETAIL_CCREATUREACTION_RELEASEPHYSICALSHIELDSPELL, FABLE_RETAIL_CCREATUREACTION_RELEASEPHYSICALSHIELDSPELL_RTTI_DECLS (preamble of CCreatureActionReleasePhysicalShieldSpell_FinishAction_008b93d0.cpp).
