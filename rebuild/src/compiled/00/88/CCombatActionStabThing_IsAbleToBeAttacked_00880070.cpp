// Leaf: ?IsAbleToBeAttacked@CCombatAction_StabThing@@UBE_NXZ
#define FABLE_RETAIL_CCOMBATACTION_STABTHING_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCombatAction_StabThing.h"
bool CCombatAction_StabThing::IsAbleToBeAttacked() const
{
    return false;
}
// Flatten route (flatten_pilot v5, NF329): CCombatAction_StabThing_IsAbleToBeAttacked -> CCombatAction_StabThing::IsAbleToBeAttacked, the RTTI-placed PDB member; class model: engine/CCombatAction_StabThing.h opt-in FABLE_RETAIL_CCOMBATACTION_STABTHING_VIRTUAL_MODEL, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR.
