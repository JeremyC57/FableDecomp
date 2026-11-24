// Leaf: ?GetAnimDelayTime@CCreatureAction_Die@@UBEJXZ
#define FABLE_RETAIL_CCREATUREACTION_DIE_RTTI_DECLS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_DIE
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_Die.h"
long CCreatureAction_Die::GetAnimDelayTime() const
{
    return 0;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_Die_GetAnimDelayTime -> CCreatureAction_Die::GetAnimDelayTime, the RTTI-placed PDB member; class model: engine/CCreatureAction_Die.h opt-in FABLE_RETAIL_CCREATUREACTION_DIE, FABLE_RETAIL_CCREATUREACTION_DIE_RTTI_DECLS (preamble of CTCFlammable_FrameUpdate_00780d80.cpp).
