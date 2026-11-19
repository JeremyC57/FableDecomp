// NUISystem::CComponent::GetTargetRenderPosition() const -- the target position, scaled by the parent zoom and offset by the parent's render position
// Leaf: ?GetTargetRenderPosition@CComponent@NUISystem@@UBE?AVC2DVector@@XZ
// Flags: none
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
