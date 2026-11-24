// Leaf: ?IsBlockable@CCreatureAction_BlockCounter@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_BLOCKCOUNTER_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_BlockCounter.h"
bool CCreatureAction_BlockCounter::IsBlockable() const
{
    return false;
}
// Flatten route (flatten_pilot v5, NF329): CCreatureAction_BlockCounter_IsBlockable -> CCreatureAction_BlockCounter::IsBlockable, the RTTI-placed PDB member; class model: engine/CCreatureAction_BlockCounter.h opt-in FABLE_RETAIL_CCREATUREACTION_BLOCKCOUNTER_VIRTUAL_MODEL, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR.
