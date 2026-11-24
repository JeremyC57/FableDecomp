// Leaf: ?IsToCauseRecoil@CCreatureAction_ReleaseTimeSpell@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_RELEASETIMESPELL_RTTI_DECLS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_RELEASETIMESPELL
#include "engine/CThing.h"
#include "engine/CCreatureAction_ReleaseTimeSpell.h"
bool CCreatureAction_ReleaseTimeSpell::IsToCauseRecoil() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_ReleaseTimeSpell_IsToCauseRecoil -> CCreatureAction_ReleaseTimeSpell::IsToCauseRecoil, the RTTI-placed PDB member; class model: engine/CCreatureAction_ReleaseTimeSpell.h opt-in FABLE_RETAIL_CCREATUREACTION_RELEASETIMESPELL, FABLE_RETAIL_CCREATUREACTION_RELEASETIMESPELL_RTTI_DECLS (preamble of CCreatureActionReleaseTimeSpell_FinishAction_008cdd40.cpp).
