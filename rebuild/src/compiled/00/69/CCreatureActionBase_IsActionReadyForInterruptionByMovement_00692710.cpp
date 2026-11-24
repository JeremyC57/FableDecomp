// Leaf: ?IsActionReadyForInterruptionByMovement@CCreatureActionBase@@UBE_NABVC3DVector@@@Z
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureActionBase.h"
bool CCreatureActionBase::IsActionReadyForInterruptionByMovement(const class C3DVector&) const
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureActionBase_IsActionReadyForInterruptionByMovement -> CCreatureActionBase::IsActionReadyForInterruptionByMovement, the RTTI-placed PDB member; class model: engine/CCreatureActionBase.h under FABLE_RETAIL_CCREATUREACTIONBASE (preamble of CCreatureActionBase_PreventSneaking_00692820.cpp).
