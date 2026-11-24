// Leaf: ?GetType@CAnimComponentTransitionInTime@@UBE?AW4EAnimComponent@@XZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_ANIMCOMPONENT
#include "fable_string.h"
#include "engine/CAnimComponentTransitionInTime.h"
EAnimComponent CAnimComponentTransitionInTime::GetType() const
{
    return (EAnimComponent)1;
}
// Flatten route (flatten_pilot v4, NF324): CAnimComponentTransitionInTime_GetType -> CAnimComponentTransitionInTime::GetType, the RTTI-placed PDB member; class model: engine/CAnimComponentTransitionInTime.h under FABLE_RETAIL_ANIMCOMPONENT (preamble of CAnimComponentTransitionInTime_SetDefaultValue_005db1f0.cpp).
