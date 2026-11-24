// Leaf: ?GetType@CAnimComponentFlags@@UBE?AW4EAnimComponent@@XZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_ANIMCOMPONENT
#include "fable_string.h"
#include "engine/CAnimComponentFlags.h"
EAnimComponent CAnimComponentFlags::GetType() const
{
    return (EAnimComponent)0;
}
// Flatten route (flatten_pilot v4, NF324): CAnimComponentFlags_GetType -> CAnimComponentFlags::GetType, the RTTI-placed PDB member; class model: engine/CAnimComponentFlags.h under FABLE_RETAIL_ANIMCOMPONENT (preamble of CAnimComponentFlags_TransferBinaryOut_005db1e0.cpp).
