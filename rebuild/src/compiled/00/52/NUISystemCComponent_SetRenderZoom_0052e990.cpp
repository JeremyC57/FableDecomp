// NUISystem::CComponent::SetRenderZoom(C2DVector& zoom) -- store the zoom used for rendering
// Leaf: SetRenderZoom
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E990, 21 bytes (`ret 4`), no callees.
// Label: the catalog "NUISystem::CSprite::vfunc_29" is the first vtable (0x0122F5D4 slot 29) listing this body;
// it is CComponent code. Slot 28 (0x0052E970) stores +0x54 (RenderPosition), slot 29 +0x7c (RenderZoom), slot 30
// (landed 0x0052E9B0) +0x94 (RenderColour); Ego_r.pdb names SetRenderPosition / SetRenderZoom / SetRenderColour.
// Layout: retail CComponent (PDB offsets minus 8 from Time on): RenderZoom +0x7c (PDB +0x84).
// Ego_r.pdb mangles the parameter as a non-const reference (?SetRenderZoom@CComponent@NUISystem@@UAEXAAVC2DVector@@@Z).
// Shape: one memberwise C2DVector copy through edx/eax.
struct C2DVector {
    float X;
    float Y;
};

namespace NUISystem {
struct CComponent {
    unsigned char _pad00[0x7c];
    C2DVector RenderZoom;                   // +0x7c
    void SetRenderZoom(C2DVector& zoom);
};

void CComponent::SetRenderZoom(C2DVector& zoom)
{
    RenderZoom = zoom;
}
}