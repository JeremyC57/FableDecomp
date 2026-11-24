// Leaf: ?IsVitalStrike@CCreatureAction_InterruptableFarAttack@@UBE_NXZ
#define FABLE_RETAIL_CCREATUREACTION_INTERRUPTABLEFARATTACK_RTTI_DECLS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_PRIORITYCTOR
#define FABLE_RETAIL_CCREATUREACTION_INTERRUPTABLEFARATTACK_PROTOTYPE
#include "engine/CCreatureAction_InterruptableFarAttack.h"
bool CCreatureAction_InterruptableFarAttack::IsVitalStrike() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_InterruptableFarAttack_IsVitalStrike -> CCreatureAction_InterruptableFarAttack::IsVitalStrike, the RTTI-placed PDB member; class model: engine/CCreatureAction_InterruptableFarAttack.h opt-in FABLE_RETAIL_CCREATUREACTION_INTERRUPTABLEFARATTACK_PROTOTYPE, FABLE_RETAIL_CCREATUREACTION_INTERRUPTABLEFARATTACK_RTTI_DECLS (preamble of CCreatureActionBase_StatRegisterClass_006f5c10.cpp).
