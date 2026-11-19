// NUISystem::CComponent::GetTargetRenderPosition() const -- the target position, scaled by the parent zoom and offset by the parent's render position
// Leaf: ?GetTargetRenderPosition@CComponent@NUISystem@@UBE?AVC2DVector@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052EE00, 192 bytes, `ret 4` (C2DVector by hidden pointer). Callees: this->IsPositionIndependent
//   (virtual slot 102, +0x198, called twice) and the parent's GetTargetRenderPosition (virtual slot 112, +0x1c0, recursive).
// Shape: FableWin.pdb body 0x0229BB60. It copies TargetPosition with the user copy ctor, takes PPositionParent else PParent, and
//   returns early when there is no parent or the component is position independent. It re-tests IsPositionIndependent before
//   scaling by ParentZoom and adding the parent's render position (C2DVector::operator+=), then has one shared return.
//   Retail splits that shared return into two tails.
// Closed by the retail lib_coord C2DVector (FABLE_RETAIL_C2DVECTOR: packed class, user copy ctor, operator+=). The earlier park
//   (DIFFER 185v192, a local C2DVector model with three explicit returns) was the wrong class model, not a compiler gap.
#define FABLE_RETAIL_C2DVECTOR
#include "engine/NUISystem_CComponent.h"

namespace NUISystem {

C2DVector CComponent::GetTargetRenderPosition() const
{
    C2DVector target_position(TargetPosition);
    CComponent* pparent = PPositionParent ? PPositionParent : PParent;
    if (!pparent || IsPositionIndependent())
        return target_position;
    if (!IsPositionIndependent())
    {
        target_position.X *= ParentZoom.X;
        target_position.Y *= ParentZoom.Y;
        target_position += pparent->GetTargetRenderPosition();
    }
    return target_position;
}

}
