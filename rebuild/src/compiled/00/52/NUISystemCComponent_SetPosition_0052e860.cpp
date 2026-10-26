// NUISystem::CComponent::SetPosition(const C2DVector& position) -- snap the position: target, initial and current position all take the argument
// Leaf: SetPosition
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E860, 38 bytes (`ret 4`), no callees.
// Label: the catalog "NUISystem::CSprite::vfunc_15" is the first vtable (0x0122F5D4 slot 15) listing this body;
// it is CComponent code inherited by every UI component. Ego_r.pdb names ?SetPosition@CComponent@NUISystem@@UAEXABVC2DVector@@@Z
// (the timed variant is the landed CComponent::ChangePosition 0x0052E9C0, whose no-time arm does the same stores).
// Layout: retail CComponent (PDB offsets minus 8 from Time on): Position +0x34, TargetPosition +0x3c,
// InitialPosition +0x44.
// Shape: three memberwise C2DVector copies through edx, the source re-read for each one, target first, then
// initial, then the current position.
class C2DVector { public:
    float X;
    float Y;
};

namespace NUISystem {
class CComponent { public:
    unsigned char _pad00[0x34];
    C2DVector Position;                     // +0x34
    C2DVector TargetPosition;               // +0x3c
    C2DVector InitialPosition;              // +0x44
    void SetPosition(const C2DVector& position);
};

void CComponent::SetPosition(const C2DVector& position)
{
    TargetPosition = position;
    InitialPosition = position;
    Position = position;
}
}