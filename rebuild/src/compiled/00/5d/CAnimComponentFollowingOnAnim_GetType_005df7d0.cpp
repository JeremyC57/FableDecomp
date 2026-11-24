// Leaf: ?GetType@CAnimComponentFollowingOnAnim@@UBE?AW4EAnimComponent@@XZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_ANIMCOMPONENT
#include "engine/CAnimComponentFollowingOnAnim.h"
EAnimComponent CAnimComponentFollowingOnAnim::GetType() const
{
    return (EAnimComponent)8;
}
// Flatten route (flatten_pilot v4, NF324): CAnimComponentFollowingOnAnim_GetType -> CAnimComponentFollowingOnAnim::GetType, the RTTI-placed PDB member; class model: engine/CAnimComponentFollowingOnAnim.h under FABLE_RETAIL_ANIMCOMPONENT (preamble of CCombatActionGenericStrikeResponseKnockdownRecoverBase_Initialise_00865eb0.cpp).
