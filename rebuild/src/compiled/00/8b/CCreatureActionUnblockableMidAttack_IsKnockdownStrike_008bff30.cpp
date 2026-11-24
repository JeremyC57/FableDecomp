// Leaf: ?IsKnockdownStrike@CCreatureAction_UnblockableMidAttack@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_UNBLOCKABLEMIDATTACK_RTTI_DECLS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_UNBLOCKABLEMIDATTACK_PROTOTYPE
#include "engine/CCreatureAction_UnblockableMidAttack.h"
bool CCreatureAction_UnblockableMidAttack::IsKnockdownStrike() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_UnblockableMidAttack_IsKnockdownStrike -> CCreatureAction_UnblockableMidAttack::IsKnockdownStrike, the RTTI-placed PDB member; class model: engine/CCreatureAction_UnblockableMidAttack.h opt-in FABLE_RETAIL_CCREATUREACTION_UNBLOCKABLEMIDATTACK_PROTOTYPE, FABLE_RETAIL_CCREATUREACTION_UNBLOCKABLEMIDATTACK_RTTI_DECLS (preamble of CCreatureActionBase_StatRegisterClass_006f5c10.cpp).
