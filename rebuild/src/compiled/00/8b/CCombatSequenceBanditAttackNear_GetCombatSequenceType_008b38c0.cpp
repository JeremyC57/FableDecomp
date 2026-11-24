// Leaf: ?GetCombatSequenceType@CCombatSequence_BanditAttackNear@@UBE?AW4ECombatSequenceType@@XZ
#define FABLE_RETAIL_CCOMBATSEQUENCE_BANDITATTACKNEAR_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_BanditAttackNear.h"
ECombatSequenceType CCombatSequence_BanditAttackNear::GetCombatSequenceType() const
{
    return (ECombatSequenceType)1;
}
// Flatten route (flatten_pilot v5, NF329): CCombatSequence_BanditAttackNear_GetCombatSequenceType -> CCombatSequence_BanditAttackNear::GetCombatSequenceType, the RTTI-placed PDB member; class model: engine/CCombatSequence_BanditAttackNear.h opt-in FABLE_RETAIL_CCOMBATSEQUENCE_BANDITATTACKNEAR_VIRTUAL_MODEL, FABLE_RETAIL_CCOMBATSEQUENCEBASE.
