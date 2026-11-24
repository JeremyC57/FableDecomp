// Leaf: ?IsInterruptable@CCombatSequence_WillPhysicalShield@@UBE_NXZ
#define FABLE_RETAIL_CCOMBATSEQUENCE_WILLPHYSICALSHIELD_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_WillPhysicalShield.h"
bool CCombatSequence_WillPhysicalShield::IsInterruptable() const
{
    return false;
}
// Flatten route (flatten_pilot v5, NF329): CCombatSequence_WillPhysicalShield_IsInterruptable -> CCombatSequence_WillPhysicalShield::IsInterruptable, the RTTI-placed PDB member; class model: engine/CCombatSequence_WillPhysicalShield.h under FABLE_RETAIL_CCOMBATSEQUENCE_WILLPHYSICALSHIELD_VIRTUAL_MODEL, FABLE_RETAIL_CCOMBATSEQUENCEBASE.
