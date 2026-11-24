// Leaf: ?GetType@CAnimComponentCombatHandedness@@UBE?AW4EAnimComponent@@XZ
#define FABLE_RETAIL_ANIMCOMPONENT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CAnimComponentCombatHandedness.h"
EAnimComponent CAnimComponentCombatHandedness::GetType() const
{
    return (EAnimComponent)3;
}
// Flatten route (flatten_pilot v4, NF324): CAnimComponentCombatHandedness_GetType -> CAnimComponentCombatHandedness::GetType, the RTTI-placed PDB member; class model: engine/CAnimComponentCombatHandedness.h under FABLE_RETAIL_ANIMCOMPONENT (preamble of CAnimComponentCombatHandedness_TransferBinaryIn_005dba80.cpp).
