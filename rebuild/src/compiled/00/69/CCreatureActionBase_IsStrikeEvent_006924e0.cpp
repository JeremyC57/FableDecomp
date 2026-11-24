// Leaf: ?IsStrikeEvent@CCreatureActionBase@@UBE_NJ@Z
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureActionBase.h"
bool CCreatureActionBase::IsStrikeEvent(long) const
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureActionBase_IsStrikeEvent -> CCreatureActionBase::IsStrikeEvent, the RTTI-placed PDB member; class model: engine/CCreatureActionBase.h under FABLE_RETAIL_CCREATUREACTIONBASE (preamble of CCreatureActionBase_PreventSneaking_00692820.cpp).
