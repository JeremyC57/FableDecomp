// Leaf: ?IsInterruptableDueToZoneChange@CCombatSequence_VillagerStepIn@@UBE_NXZ
#define FABLE_RETAIL_CCOMBATSEQUENCE_VILLAGERSTEPIN_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_VillagerStepIn.h"
bool CCombatSequence_VillagerStepIn::IsInterruptableDueToZoneChange() const
{
    return true;
}
// Flatten route (flatten_pilot v5, NF329): CCombatSequence_VillagerStepIn_IsInterruptableDueToZoneChange -> CCombatSequence_VillagerStepIn::IsInterruptableDueToZoneChange, the RTTI-placed PDB member; class model: engine/CCombatSequence_VillagerStepIn.h opt-in FABLE_RETAIL_CCOMBATSEQUENCE_VILLAGERSTEPIN_VIRTUAL_MODEL, FABLE_RETAIL_CCOMBATSEQUENCEBASE.
