// Leaf: ?IsAbleToBeAttacked@CCreatureAction_RemovePhysicalShieldSpell@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_REMOVEPHYSICALSHIELDSPELL_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_RemovePhysicalShieldSpell.h"
bool CCreatureAction_RemovePhysicalShieldSpell::IsAbleToBeAttacked() const
{
    return false;
}
// Flatten route (flatten_pilot v5, NF329): CCreatureAction_RemovePhysicalShieldSpell_IsAbleToBeAttacked -> CCreatureAction_RemovePhysicalShieldSpell::IsAbleToBeAttacked, the RTTI-placed PDB member; class model: engine/CCreatureAction_RemovePhysicalShieldSpell.h opt-in FABLE_RETAIL_CCREATUREACTION_REMOVEPHYSICALSHIELDSPELL_VIRTUAL_MODEL, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR.
