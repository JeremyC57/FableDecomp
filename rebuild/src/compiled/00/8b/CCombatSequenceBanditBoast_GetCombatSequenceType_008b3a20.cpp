// Leaf: ?GetCombatSequenceType@CCombatSequence_BanditBoast@@UBE?AW4ECombatSequenceType@@XZ
#define FABLE_RETAIL_CCOMBATSEQUENCE_BANDITBOAST_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_BanditBoast.h"
ECombatSequenceType CCombatSequence_BanditBoast::GetCombatSequenceType() const
{
    return (ECombatSequenceType)3;
}
// Flatten route (flatten_pilot v5, NF329): CCombatSequence_BanditBoast_GetCombatSequenceType -> CCombatSequence_BanditBoast::GetCombatSequenceType, the RTTI-placed PDB member; class model: engine/CCombatSequence_BanditBoast.h opt-in FABLE_RETAIL_CCOMBATSEQUENCE_BANDITBOAST_VIRTUAL_MODEL, FABLE_RETAIL_CCOMBATSEQUENCEBASE.
