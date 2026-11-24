// Leaf: ?GetType@C3DAnimationState@@UBE?AW4EType@C3DAnimationStateBase@@XZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DANIMATIONSTATE
#define FABLE_RETAIL_C3DANIMATIONSTATEBASE_VIRTUALS
#include "engine/C3DAnimationState.h"
C3DAnimationStateBase::EType C3DAnimationState::GetType() const
{
    return (C3DAnimationStateBase::EType)0;
}
// Flatten route (flatten_pilot v4, NF324): C3DAnimationState_GetType -> C3DAnimationState::GetType, the RTTI-placed PDB member; class model: engine/C3DAnimationState.h under FABLE_RETAIL_C3DANIMATIONSTATE, FABLE_RETAIL_C3DANIMATIONSTATEBASE_VIRTUALS (preamble of C3DAnimationBlendState_CreateInterpolate_00a3bf10.cpp).
