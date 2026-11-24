// Leaf: ?GetType@CAnimComponentStrikeResponseAnim@@UBE?AW4EAnimComponent@@XZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_ANIMCOMPONENT
#include "engine/CAnimationEntry.h"
#include "engine/CAnimComponentStrikeResponseAnim.h"
EAnimComponent CAnimComponentStrikeResponseAnim::GetType() const
{
    return (EAnimComponent)12;
}
// Flatten route (flatten_pilot v4, NF324): CAnimComponentStrikeResponseAnim_GetType -> CAnimComponentStrikeResponseAnim::GetType, the RTTI-placed PDB member; class model: engine/CAnimComponentStrikeResponseAnim.h under FABLE_RETAIL_ANIMCOMPONENT (preamble of CAnimationEntry_StatRegisterClass_005dd670.cpp).
