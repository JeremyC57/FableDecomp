// Leaf: ?GetType@CAnimComponentCombatMisc@@UBE?AW4EAnimComponent@@XZ
#define FABLE_RETAIL_ANIMCOMPONENT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CAnimComponentCombatMisc.h"
EAnimComponent CAnimComponentCombatMisc::GetType() const
{
    return (EAnimComponent)6;
}
// Flatten route (flatten_pilot v4, NF324): CAnimComponentCombatMisc_GetType -> CAnimComponentCombatMisc::GetType, the RTTI-placed PDB member; class model: engine/CAnimComponentCombatMisc.h under FABLE_RETAIL_ANIMCOMPONENT (preamble of CAnimComponentCombatMisc_TransferBinaryIn_005dbcd0.cpp).
