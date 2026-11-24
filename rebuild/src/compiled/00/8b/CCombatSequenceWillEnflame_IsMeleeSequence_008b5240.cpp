// Leaf: ?IsMeleeSequence@CCombatSequence_WillEnflame@@UBE_NXZ
#define FABLE_RETAIL_CCOMBATSEQUENCE_WILLENFLAME_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_WillEnflame.h"
bool CCombatSequence_WillEnflame::IsMeleeSequence() const
{
    return true;
}
// Flatten route (flatten_pilot v5, NF329): CCombatSequence_WillEnflame_IsMeleeSequence -> CCombatSequence_WillEnflame::IsMeleeSequence, the RTTI-placed PDB member; class model: engine/CCombatSequence_WillEnflame.h opt-in FABLE_RETAIL_CCOMBATSEQUENCE_WILLENFLAME_VIRTUAL_MODEL, FABLE_RETAIL_CCOMBATSEQUENCEBASE.
