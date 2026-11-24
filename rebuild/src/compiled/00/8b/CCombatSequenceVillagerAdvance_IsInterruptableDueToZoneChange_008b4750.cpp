// Leaf: ?IsInterruptableDueToZoneChange@CCombatSequence_VillagerAdvance@@UBE_NXZ
#define FABLE_RETAIL_CCOMBATSEQUENCE_VILLAGERADVANCE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_VillagerAdvance.h"
bool CCombatSequence_VillagerAdvance::IsInterruptableDueToZoneChange() const
{
    return true;
}
// Flatten route (flatten_pilot v5, NF329): CCombatSequence_VillagerAdvance_IsInterruptableDueToZoneChange -> CCombatSequence_VillagerAdvance::IsInterruptableDueToZoneChange, the RTTI-placed PDB member; class model: engine/CCombatSequence_VillagerAdvance.h opt-in FABLE_RETAIL_CCOMBATSEQUENCE_VILLAGERADVANCE_VIRTUAL_MODEL, FABLE_RETAIL_CCOMBATSEQUENCEBASE.
