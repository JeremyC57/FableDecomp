// Leaf: ?IsBlockable@CCreatureAction_BanditKingAttack@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_BANDITKINGATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_BanditKingAttack.h"
bool CCreatureAction_BanditKingAttack::IsBlockable() const
{
    return false;
}
// Flatten route (flatten_pilot v5, NF329): CCreatureAction_BanditKingAttack_IsBlockable -> CCreatureAction_BanditKingAttack::IsBlockable, the RTTI-placed PDB member; class model: engine/CCreatureAction_BanditKingAttack.h opt-in FABLE_RETAIL_CCREATUREACTION_BANDITKINGATTACK_VIRTUAL_MODEL, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR.
