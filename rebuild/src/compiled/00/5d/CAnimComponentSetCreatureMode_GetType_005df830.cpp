// Leaf: ?GetType@CAnimComponentSetCreatureMode@@UBE?AW4EAnimComponent@@XZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_ANIMCOMPONENT
#include "engine/CAnimationEntry.h"
#include "engine/CAnimComponentSetCreatureMode.h"
EAnimComponent CAnimComponentSetCreatureMode::GetType() const
{
    return (EAnimComponent)10;
}
// Flatten route (flatten_pilot v4, NF324): CAnimComponentSetCreatureMode_GetType -> CAnimComponentSetCreatureMode::GetType, the RTTI-placed PDB member; class model: engine/CAnimComponentSetCreatureMode.h under FABLE_RETAIL_ANIMCOMPONENT (preamble of CAnimationEntry_StatRegisterClass_005dd670.cpp).
