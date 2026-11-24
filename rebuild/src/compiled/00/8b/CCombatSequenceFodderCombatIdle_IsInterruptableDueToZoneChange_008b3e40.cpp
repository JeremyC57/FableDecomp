// Leaf: ?IsInterruptableDueToZoneChange@CCombatSequence_FodderCombatIdle@@UBE_NXZ
#define FABLE_RETAIL_CCOMBATSEQUENCE_FODDERCOMBATIDLE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_FodderCombatIdle.h"
bool CCombatSequence_FodderCombatIdle::IsInterruptableDueToZoneChange() const
{
    return true;
}
// Flatten route (flatten_pilot v5, NF329): CCombatSequence_FodderCombatIdle_IsInterruptableDueToZoneChange -> CCombatSequence_FodderCombatIdle::IsInterruptableDueToZoneChange, the RTTI-placed PDB member; class model: engine/CCombatSequence_FodderCombatIdle.h opt-in FABLE_RETAIL_CCOMBATSEQUENCE_FODDERCOMBATIDLE_VIRTUAL_MODEL, FABLE_RETAIL_CCOMBATSEQUENCEBASE.
