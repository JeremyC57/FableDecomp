// Leaf: ?GetType@CAnimComponentCombatComboChain@@UBE?AW4EAnimComponent@@XZ
#define FABLE_RETAIL_ANIMCOMPONENT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CAnimComponentCombatComboChain.h"
EAnimComponent CAnimComponentCombatComboChain::GetType() const
{
    return (EAnimComponent)4;
}
// Flatten route (flatten_pilot v4, NF324): CAnimComponentCombatComboChain_GetType -> CAnimComponentCombatComboChain::GetType, the RTTI-placed PDB member; class model: engine/CAnimComponentCombatComboChain.h under FABLE_RETAIL_ANIMCOMPONENT (preamble of CAnimComponentCombatComboChain_TransferBinaryIn_005dbb60.cpp).
