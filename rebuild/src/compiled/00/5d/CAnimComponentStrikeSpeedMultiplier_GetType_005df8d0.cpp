// Leaf: ?GetType@CAnimComponentStrikeSpeedMultiplier@@UBE?AW4EAnimComponent@@XZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CAnimationEntry.h"
#include "engine/CAnimComponentStrikeSpeedMultiplier.h"
EAnimComponent CAnimComponentStrikeSpeedMultiplier::GetType() const
{
    return (EAnimComponent)9;
}
// Flatten route (flatten_pilot v4, NF324): CAnimComponentStrikeSpeedMultiplier_GetType -> CAnimComponentStrikeSpeedMultiplier::GetType, the RTTI-placed PDB member; class model: engine/CAnimComponentStrikeSpeedMultiplier.h under its default branch (preamble of CAnimationEntry_StatRegisterClass_005dd670.cpp).
