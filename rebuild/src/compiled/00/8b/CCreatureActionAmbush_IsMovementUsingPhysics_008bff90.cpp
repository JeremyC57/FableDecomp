// Leaf: ?IsMovementUsingPhysics@CCreatureAction_Ambush@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_AMBUSH_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_Ambush.h"
bool CCreatureAction_Ambush::IsMovementUsingPhysics() const
{
    return false;
}
// Flatten route (flatten_pilot v5, NF329): CCreatureAction_Ambush_IsMovementUsingPhysics -> CCreatureAction_Ambush::IsMovementUsingPhysics, the RTTI-placed PDB member; class model: engine/CCreatureAction_Ambush.h opt-in FABLE_RETAIL_CCREATUREACTION_AMBUSH_VIRTUAL_MODEL, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR.
