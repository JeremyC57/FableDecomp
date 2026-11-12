// C3DAnimationTransitionState::FindDominantAnimation @ 0x00710540
// Leaf: ?C3DAnimationTransitionState_FindDominantAnimation@@YIABVC3DAnimationState@@PAVC3DAnimationTransitionState@@@Z
#include "engine/C3DAnimationTransitionState.h"  // retyped onto the PDB layout; byte parity re-verified
class C3DAnimationState;

class CIVCountedPointeeBase { public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual const C3DAnimationState& FindDominant() = 0; // slot 3 (offset 0xc)
};



const C3DAnimationState& __fastcall C3DAnimationTransitionState_FindDominantAnimation(C3DAnimationTransitionState* self)
{
    if (self->Progress <= 0.5f)
        return self->StartState_Object->FindDominant();
    return self->EndState_Object->FindDominant();
}