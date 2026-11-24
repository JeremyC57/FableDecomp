// Leaf: ?GetType@C3DAnimationTransitionState@@UBE?AW4EType@C3DAnimationStateBase@@XZ
#define FABLE_RETAIL_C3DANIMATIONTRANSITIONSTATE
#include "engine/C3DAnimationTransitionState.h"
C3DAnimationStateBase::EType C3DAnimationTransitionState::GetType() const
{
    return (C3DAnimationStateBase::EType)2;
}
// Flatten route (flatten_pilot v4, NF324): C3DAnimationTransitionState_GetType -> C3DAnimationTransitionState::GetType, the RTTI-placed PDB member; class model: engine/C3DAnimationTransitionState.h under FABLE_RETAIL_C3DANIMATIONTRANSITIONSTATE.
