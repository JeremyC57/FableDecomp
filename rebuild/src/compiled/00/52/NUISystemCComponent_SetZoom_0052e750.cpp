// NUISystem::CComponent::SetZoom(const C2DVector& zoom) -- snap the zoom: target, initial and current zoom all take the argument
// Leaf: SetZoom
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E750, 38 bytes (`ret 4`), no callees.
// Label: the catalog "NUISystem::CSprite::vfunc_11" is the first vtable (0x0122F5D4 slot 11) listing this body;
// it is CComponent code inherited by every UI component. Ego_r.pdb names ?SetZoom@CComponent@NUISystem@@UAEXABVC2DVector@@@Z
// (slot 11 sits beside the zoom getter at slot 12, 0x0052E780, which reads +0x5c/+0x60).
// Layout: the retail view of the Ego_r CComponent (as the landed ChangePosition 0x0052E9C0): every member from
// Time on sits 8 bytes below its PDB offset, so Zoom +0x5c (PDB +0x64), TargetZoom +0x64, InitialZoom +0x6c.
// Shape: three memberwise C2DVector copies through edx, the source re-read for each one (the reference may alias
// the members), target first, then initial, then the current zoom.
struct C2DVector {
    float X;
    float Y;
};

namespace NUISystem {
struct CComponent {
    unsigned char _pad00[0x5c];
    C2DVector Zoom;                         // +0x5c
    C2DVector TargetZoom;                   // +0x64
    C2DVector InitialZoom;                  // +0x6c
    void SetZoom(const C2DVector& zoom);
};

void CComponent::SetZoom(const C2DVector& zoom)
{
    TargetZoom = zoom;
    InitialZoom = zoom;
    Zoom = zoom;
}
}