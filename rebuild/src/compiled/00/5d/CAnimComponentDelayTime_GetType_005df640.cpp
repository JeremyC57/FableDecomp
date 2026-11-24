// Leaf: ?GetType@CAnimComponentDelayTime@@UBE?AW4EAnimComponent@@XZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_ANIMCOMPONENT
#include "fable_string.h"
#include "engine/CAnimComponentDelayTime.h"
EAnimComponent CAnimComponentDelayTime::GetType() const
{
    return (EAnimComponent)2;
}
// Flatten route (flatten_pilot v4, NF324): CAnimComponentDelayTime_GetType -> CAnimComponentDelayTime::GetType, the RTTI-placed PDB member; class model: engine/CAnimComponentDelayTime.h under FABLE_RETAIL_ANIMCOMPONENT (preamble of CAnimComponentDelayTime_SetDefaultValue_005db220.cpp).
