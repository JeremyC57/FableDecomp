// Behaviour test for NUISystem::CComponent::GetTargetRenderPosition (0x0052EE00), a portable model of the row. The render position is
// the target position when there is no parent (PPositionParent first, else PParent) or the component is position independent.
// Otherwise it is the target position scaled per axis by ParentZoom plus the parent's own render position (recursive).
// Dyadic values keep the arithmetic exact.
#include <stdio.h>
struct C2DVector { float X, Y; };
struct Comp {
    C2DVector TargetPosition, ParentZoom;
    Comp* PParent; Comp* PPositionParent; bool Independent; int calls;
    bool IsPositionIndependent() { ++calls; return Independent; }
    C2DVector GetTargetRenderPosition()
    {
        C2DVector target_position = TargetPosition;
        Comp* pparent = PPositionParent ? PPositionParent : PParent;
        if (!pparent || IsPositionIndependent())
            return target_position;
        if (!IsPositionIndependent())
        {
            target_position.X *= ParentZoom.X;
            target_position.Y *= ParentZoom.Y;
            C2DVector p = pparent->GetTargetRenderPosition();
            target_position.X += p.X;
            target_position.Y += p.Y;
        }
        return target_position;
    }
};
int main()
{
    int bad = 0;
    Comp root = { { 10.0f, 20.0f }, { 1.0f, 1.0f }, 0, 0, false, 0 };
    Comp other = { { 100.0f, 200.0f }, { 1.0f, 1.0f }, 0, 0, false, 0 };
    Comp child = { { 2.0f, 4.0f }, { 0.5f, 2.0f }, &other, &root, false, 0 };
    C2DVector r = root.GetTargetRenderPosition();
    if (r.X != 10.0f || r.Y != 20.0f || root.calls != 0) ++bad;            // no parent: no virtual call
    r = child.GetTargetRenderPosition();                                   // PPositionParent wins over PParent
    if (r.X != 2.0f * 0.5f + 10.0f || r.Y != 4.0f * 2.0f + 20.0f || child.calls != 2) ++bad;
    child.PPositionParent = 0; child.calls = 0;
    r = child.GetTargetRenderPosition();                                   // falls back to PParent
    if (r.X != 1.0f + 100.0f || r.Y != 8.0f + 200.0f) ++bad;
    child.Independent = true; child.calls = 0;
    r = child.GetTargetRenderPosition();
    if (r.X != 2.0f || r.Y != 4.0f || child.calls != 1) ++bad;            // independent: unscaled, parent not used
    printf(bad ? "0052ee00_TEST FAIL\n" : "0052ee00_TEST PASS\n");
    return bad ? 1 : 0;
}