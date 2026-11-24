// Leaf: ?IsPreventingQuickAccessItemUsage@CCreatureAction_Die@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_DIE_RTTI_DECLS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_DIE
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_Die.h"
bool CCreatureAction_Die::IsPreventingQuickAccessItemUsage() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_Die_IsPreventingQuickAccessItemUsage -> CCreatureAction_Die::IsPreventingQuickAccessItemUsage, the RTTI-placed PDB member; class model: engine/CCreatureAction_Die.h opt-in FABLE_RETAIL_CCREATUREACTION_DIE, FABLE_RETAIL_CCREATUREACTION_DIE_RTTI_DECLS (preamble of CTCFlammable_FrameUpdate_00780d80.cpp).
