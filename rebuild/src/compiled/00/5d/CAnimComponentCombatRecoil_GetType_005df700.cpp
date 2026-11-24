// Leaf: ?GetType@CAnimComponentCombatRecoil@@UBE?AW4EAnimComponent@@XZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_ANIMCOMPONENT
#include "fable_string.h"
#include "engine/CAnimComponentCombatRecoil.h"
EAnimComponent CAnimComponentCombatRecoil::GetType() const
{
    return (EAnimComponent)5;
}
// Flatten route (flatten_pilot v4, NF324): CAnimComponentCombatRecoil_GetType -> CAnimComponentCombatRecoil::GetType, the RTTI-placed PDB member; class model: engine/CAnimComponentCombatRecoil.h under FABLE_RETAIL_ANIMCOMPONENT (preamble of CAnimComponentCombatRecoil_SetDefaultValue_005db310.cpp).
