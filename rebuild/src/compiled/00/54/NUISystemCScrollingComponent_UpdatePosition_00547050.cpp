// NUISystem::CScrollingComponent::UpdatePosition(float deltaTime) -- advance the linear scroll from InitialPosition to TargetPosition over PositionTime (snapping at the end), then derive the render position (scaled by the parent zoom and offset by the parent position unless the component ignores its parent)
// Leaf: ?UpdatePosition@CScrollingComponent@NUISystem@@UAEXM@Z
// Flags: none
#define FABLE_RETAIL_C2DVECTOR
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00547050, length 259 bytes (`ret 4`), vtable 0x012482CC slot 34.
// FableWin.pdb: ?UpdatePosition@CScrollingComponent@NUISystem@@UAEXM@Z (the override of CComponent::UpdatePosition 0x0052FFD0).
// Callees: the component's virtual slot 101 (+0x194), IsIndependent() const (engine/NUISystem_CComponent.h vtable).
// Float pool: 0x0122DED8 = 1.0f.
// Layout: Ego_r.pdb CComponent members 8 bytes lower in retail: Position +0x34, TargetPosition +0x3c, InitialPosition +0x44,
// ParentPosition +0x4c, RenderPosition +0x54, ParentZoom +0x74, PositionTimeElapsed +0x98, PositionTime +0x9c.
// Fix (chunk 755): the parent transform through a `C2DVector& render` reference gives retail's `fld [render.X] ; fadd [parent.X]`.
// Shape: the elapsed time stays on the FPU (`fst`) for the interpolation and the end test; the velocity is a C2DVector local
// built from `(Target - Initial) / time` through the reciprocal and a frame temporary (user copy ctor); the position is
// `velocity * elapsed + Initial`; the snap is the chained `InitialPosition = Position = TargetPosition`; the parent
// transform is written per component (the operand order differs between the X and Y terms).
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CScrollingComponent : public CChangingStateComponent {
public:
    virtual void UpdatePosition(float deltaTime);
};

void CScrollingComponent::UpdatePosition(float deltaTime)
{
    if (PositionTimeElapsed < PositionTime) {
        PositionTimeElapsed += deltaTime;
        C2DVector velocity;
        velocity = (TargetPosition - InitialPosition) / PositionTime;
        Position = velocity * PositionTimeElapsed + InitialPosition;
        if (PositionTimeElapsed >= PositionTime)
            InitialPosition = Position = TargetPosition;
    }
    RenderPosition = Position;
    if (!IsIndependent()) {
        C2DVector& render = RenderPosition;
        render.X *= ParentZoom.X;
        render.Y *= ParentZoom.Y;
        render += ParentPosition;
    }
}
}